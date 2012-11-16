using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml.Linq;
using AzureStorageFacadeLibrary;

namespace DeployLibrary
{
    public class DeployDatabase
    {
        private AzureBlobServer blobServer;
        private AzureBlobContainer deployContainer;

        public AzureBlobDirectory ServiceFolder
        {
            get
            {
                return this.deployContainer.GetDirectory("ServiceFolder");
            }
        }

        public string ServiceMetadataVersion
        {
            get
            {
                return this.deployContainer.GetDirectory("ServiceMetadata").GetBlob("Version").AsString;
            }
            set
            {
                this.deployContainer.GetDirectory("ServiceMetadata").GetBlob("Version").AsString = value;
            }
        }

        public string LocalDirectory { get; private set; }
        public string ServerName { get; private set; }

        public AzureBlobDirectory GetDeploymentDirectory(string deploynameName)
        {
            return this.deployContainer
                .GetDirectory("Deployments")
                .GetDirectory("deployment-" + deploynameName);
        }

        public string[] GetDeploymentNames()
        {
            return this.deployContainer
                .GetDirectory("Deployments")
                .Directories
                .Where(d => d.Name.StartsWith("deployment-"))
                .Select(d => d.Name.Substring("deployment-".Length))
                .ToArray();
        }

        public Deployment GetDeployment(string deploymentName)
        {
            if (deploymentName == null)
            {
                deploymentName = Environment.MachineName;
            }
            return new Deployment(this, deploymentName);
        }

        public Deployment[] Deployments
        {
            get
            {
                return GetDeploymentNames()
                    .Select(GetDeployment)
                    .ToArray();
            }
        }

        public DeployDatabase()
        {
            XDocument configuration = XDocument.Load("StorageConfiguration.xml");
            string account = configuration.Root.Element("account").Value;
            string key = configuration.Root.Element("key").Value;
            string container = configuration.Root.Element("container").Value;
            this.blobServer = AzureStorageFacade.ConnectBlobServer(AzureStorageFacade.CreateConnectionString(account, key));

            this.LocalDirectory = Path.GetFullPath(configuration.Root.Element("localDirectory").Value);
            this.ServerName = account;

            this.deployContainer = this.blobServer[container];
            if (this.deployContainer.CreateContainerIfNotExist().Sync())
            {
                this.ServiceFolder.GetBlob("StorageConfiguration.xml").AsString = configuration.ToString();
                this.ServiceMetadataVersion = "0";
            }
        }
    }

    public enum DeploymentStatus
    {
        Starting,
        Stopping,
        Running,
        Deploying,
    }

    public class Deployment
    {
        public AzureBlobDirectory Directory { get; private set; }
        public DeployDatabase Database { get; private set; }
        public string DeploymenName { get; private set; }

        public Deployment(DeployDatabase database, string deploymentName)
        {
            this.Database = database;
            this.DeploymenName = deploymentName;
            this.Directory = database.GetDeploymentDirectory(deploymentName);
        }

        public bool Exists
        {
            get
            {
                return this.Directory.GetBlob("Status").DoesBlobExist().Sync();
            }
        }

        public void InitializeIfNotExists()
        {
            if (!this.Exists)
            {
                this.Version = "NO DEPLOYMENT";
                this.Status = DeploymentStatus.Running;
            }
        }

        public string Version
        {
            get
            {
                return this.Directory.GetBlob("Version").AsString;
            }
            set
            {
                this.Directory.GetBlob("Version").AsString = value;
            }
        }

        public DeploymentStatus Status
        {
            get
            {
                string value = this.Directory.GetBlob("Status").AsString;
                return (DeploymentStatus)
                    (
                        typeof(DeploymentStatus)
                            .GetField(value, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.Static)
                            .GetValue(null)
                    );
            }
            set
            {
                this.Directory.GetBlob("Sttaus").AsString = value.ToString();
            }
        }
    }
}
