using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.WindowsAzure;
using Microsoft.WindowsAzure.StorageClient;

namespace AzureStorageFacadeLibrary
{
    public class AzureTableServer
    {
        public CloudTableClient RawQueueClient { get; protected set; }

        protected internal AzureTableServer(CloudStorageAccount account)
        {
            this.RawQueueClient = new CloudTableClient(account.TableEndpoint.AbsoluteUri, account.Credentials);
        }
    }
}
