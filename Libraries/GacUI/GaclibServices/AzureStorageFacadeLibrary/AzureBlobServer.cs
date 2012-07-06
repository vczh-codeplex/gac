using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.WindowsAzure;
using Microsoft.WindowsAzure.StorageClient;

namespace AzureStorageFacadeLibrary
{
    public class AzureBlobServer
    {
        public CloudBlobClient RawContainerClient { get; protected set; }

        protected internal AzureBlobServer(CloudStorageAccount account)
        {
            this.RawContainerClient = new CloudBlobClient(account.BlobEndpoint.AbsoluteUri, account.Credentials);
        }

        public IEnumerable<AzureBlobContainer> Containers
        {
            get
            {
                return this.RawContainerClient.ListContainers().Select(c => new AzureBlobContainer(this, c));
            }
        }

        public AzureBlobContainer this[string name]
        {
            get
            {
                return new AzureBlobContainer(this, this.RawContainerClient.GetContainerReference(name));
            }
        }
    }

    public class AzureBlobContainer
    {
        public AzureBlobServer Server { get; private set; }
        public CloudBlobContainer RawContainer { get; private set; }

        internal AzureBlobContainer(AzureBlobServer server, CloudBlobContainer rawContainer)
        {
            this.Server = server;
            this.RawContainer = rawContainer;
        }

        public string Name
        {
            get
            {
                return this.RawContainer.Name;
            }
        }

        public AzureLazy CreateContainer()
        {
            return new AzureLazy(this.RawContainer.Create, this.RawContainer.BeginCreate, this.RawContainer.EndCreate);
        }

        public AzureLazy<bool> CreateContainerIfNotExist()
        {
            return new AzureLazy<bool>(this.RawContainer.CreateIfNotExist, this.RawContainer.BeginCreateIfNotExist, this.RawContainer.EndCreateIfNotExist);
        }

        public AzureLazy DeleteContainer()
        {
            return new AzureLazy(this.RawContainer.Delete, this.RawContainer.BeginDelete, this.RawContainer.EndDelete);
        }

        public AzureLazy<bool> IsContainerExists()
        {
            return new AzureLazy<bool>(
                () =>
                {
                    try
                    {
                        this.RawContainer.FetchAttributes();
                        return true;
                    }
                    catch (StorageClientException e)
                    {
                        if (e.ErrorCode == StorageErrorCode.ResourceNotFound)
                        {
                            return false;
                        }
                        else
                        {
                            throw;
                        }
                    }
                }
                );
        }
    }
}
