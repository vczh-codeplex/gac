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
        public CloudBlobClient RawQueueClient { get; protected set; }

        protected internal AzureBlobServer(CloudStorageAccount account)
        {
            this.RawQueueClient = new CloudBlobClient(account.BlobEndpoint.AbsoluteUri, account.Credentials);
        }
    }
}
