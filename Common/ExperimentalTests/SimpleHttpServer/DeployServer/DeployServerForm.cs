using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;
using DeployLibrary;
using SimpleHttpServer;

namespace DeployServer
{
    public partial class DeployServerForm : Form
    {
        private DeployDatabase deployDatabase;
        private Deployment deployment;
        private Dictionary<string, DeployServerCallbackProxy> proxies = new Dictionary<string, DeployServerCallbackProxy>();
        private XDocument serviceConfiguration = null;
        private bool autoRestartService = true;

        private void EnableControls()
        {
            this.Cursor = Cursors.Default;
            foreach (Control control in this.Controls)
            {
                control.Enabled = true;
            }
            timerUpdate.Enabled = true;
        }

        private void DisableControls()
        {
            this.Cursor = Cursors.WaitCursor;
            foreach (Control control in this.Controls)
            {
                control.Enabled = false;
            }
            timerUpdate.Enabled = false;
        }

        private void Async(Action action)
        {
            ThreadPool.QueueUserWorkItem(_ => action());
        }

        private void Sync(Action action)
        {
            this.Invoke(action);
        }

        private void AsyncUpdate(Action action)
        {
            DisableControls();
            Async(() =>
            {
                try
                {
                    action();
                    Sync(() =>
                    {
                        EnableControls();
                        UpdateData();
                    });
                }
                catch (Exception ex)
                {
                    Sync(() =>
                    {
                        EnableControls();
                        MessageBox.Show(ex.Message, this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    });
                }
            });
        }

        private void DisplayDeploymentProperty(string name, string value)
        {
            var item = listViewDeployment
                .Items
                .Cast<ListViewItem>()
                .Where(i => i.Text == name)
                .FirstOrDefault();
            if (item == null)
            {
                item = new ListViewItem(name);
                item.SubItems.Add("");
                listViewDeployment.Items.Add(item);
            }
            item.SubItems[1].Text = value;
        }

        private void UpdateData()
        {
            labelDeploymentDirectory.Text = this.deployDatabase.DeployDirectory;
            DisplayDeploymentProperty("Service Name", this.deployDatabase.ServerName);
            DisplayDeploymentProperty("Deployment Name", this.deployment.DeploymenName);
            DisplayDeploymentProperty("Version", this.deployment.Version);
            DisplayDeploymentProperty("Heart Beats", this.deployment.HeartBeats);
            DisplayDeploymentProperty("Status", this.deployment.Status.ToString());

            listViewServices.Items.Clear();
            foreach (var key in this.proxies.Keys.OrderBy(s => s).ToArray())
            {
                var proxy = this.proxies[key];

                ListViewItem item = new ListViewItem(proxy.Name);
                item.SubItems.Add(proxy.Running ? "Running" : "Stopped");
                item.SubItems.Add(proxy.Url);
                item.SubItems.Add(proxy.Error);
            }
        }

        private void StopServices()
        {
            this.autoRestartService = false;
            foreach (DeployServerCallbackProxy proxy in this.proxies.Values)
            {
                if (proxy.Running)
                {
                    proxy.Stop();
                }
            }

            while (proxies.Values.Cast<DeployServerCallbackProxy>().Any(p => p.Running))
            {
                Thread.Sleep(1000);
            }
        }

        private void StartServices()
        {
            foreach (DeployServerCallbackProxy proxy in this.proxies.Values)
            {
                if (!proxy.Running)
                {
                    proxy.Start();
                }
            }

            while (!proxies.Values.Cast<DeployServerCallbackProxy>().All(p => p.Running))
            {
                Thread.Sleep(1000);
            }
            this.autoRestartService = true;
        }

        private string ServiceConfigurationFile
        {
            get
            {
                return Path.GetFullPath(@".\ServiceFolder\ServiceConfiguration.xml");
            }
        }

        private void ReloadServiceConfiguration()
        {
            this.serviceConfiguration = XDocument.Load(this.ServiceConfigurationFile);
            this.proxies.Clear();

            foreach (var name in this.serviceConfiguration
                .Root
                .Elements("service")
                .Select(s => s.Element("name").Value)
                .ToArray()
                )
            {
                this.proxies.Add(name, new DeployServerCallbackProxy(name, this.ServiceConfigurationFile, this.serviceConfiguration));
            }
        }

        public DeployServerForm()
        {
            InitializeComponent();
            ReloadServiceConfiguration();
            DisableControls();
            Async(() =>
            {
                try
                {
                    this.deployDatabase = new DeployDatabase(false);
                    this.deployment = this.deployDatabase.GetDeployment();
                    this.deployment.InitializeIfNotExists();
                    Sync(() =>
                    {
                        EnableControls();
                        UpdateData();
                        timerUpdate.Enabled = true;
                    });
                }
                catch (Exception ex)
                {
                    Sync(() =>
                    {
                        MessageBox.Show(ex.Message, this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    });
                }
            });
        }

        private void timerUpdate_Tick(object sender, EventArgs e)
        {
            this.deployment.HeartBeats = DateTime.Now.ToString();
            if (this.deployment.NeedDownload)
            {
                AsyncUpdate(() =>
                {
                    StopServices();
                    this.deployment.Download();
                    ReloadServiceConfiguration();
                    StartServices();
                });
            }
            UpdateData();

            if (this.autoRestartService)
            {
                foreach (DeployServerCallbackProxy proxy in this.proxies.Values)
                {
                    if (!proxy.Running)
                    {
                        proxy.Start();
                    }
                }
            }
        }

        private void buttonOpenDeploymentDirectory_Click(object sender, EventArgs e)
        {
            Process.Start("explorer", "\"" + labelDeploymentDirectory.Text + "\"");
        }

        private void buttonStartServices_Click(object sender, EventArgs e)
        {
            AsyncUpdate(() =>
            {
                StartServices();
                buttonStartServices.Enabled = false;
                buttonStopServices.Enabled = true;
            });
        }

        private void buttonStopServices_Click(object sender, EventArgs e)
        {
            AsyncUpdate(() =>
            {
                StopServices();
                buttonStartServices.Enabled = true;
                buttonStopServices.Enabled = false;
            });
        }
    }
}
