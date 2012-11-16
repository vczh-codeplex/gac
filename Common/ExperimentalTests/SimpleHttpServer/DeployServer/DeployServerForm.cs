using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using DeployLibrary;

namespace DeployServer
{
    public partial class DeployServerForm : Form
    {
        private DeployDatabase deployDatabase;
        private Deployment deployment;

        private void EnableControls()
        {
            this.Cursor = Cursors.Default;
            foreach (Control control in this.Controls)
            {
                control.Enabled = true;
            }
        }

        private void DisableControls()
        {
            this.Cursor = Cursors.WaitCursor;
            foreach (Control control in this.Controls)
            {
                control.Enabled = false;
            }
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
                        timerUpdate.Enabled = true;
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
        }

        public DeployServerForm()
        {
            InitializeComponent();
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
                AsyncUpdate(() => this.deployment.Download());
            }
            UpdateData();
        }

        private void buttonOpenDeploymentDirectory_Click(object sender, EventArgs e)
        {
            Process.Start("explorer", "\"" + labelDeploymentDirectory.Text + "\"");
        }
    }
}
