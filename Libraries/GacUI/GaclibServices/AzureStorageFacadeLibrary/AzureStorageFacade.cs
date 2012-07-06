using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.WindowsAzure;

namespace AzureStorageFacadeLibrary
{
    public static class AzureStorageFacade
    {
        public const string DevelopmentStorageConnectionString = "UseDevelopmentStorage=true";
        public const string ProductionStorageConnectionString = "DefaultEndpointsProtocol=https;AccountName={0};AccountKey={1}";

        public static string CreateConnectionString(string account, string key)
        {
            return string.Format(ProductionStorageConnectionString, account, key);
        }

        public static AzureQueueServer ConnectQueueServer(string connectionString)
        {
            return new AzureQueueServer(CloudStorageAccount.Parse(connectionString));
        }

        public static AzureBlobServer ConnectBlobServer(string connectionString)
        {
            return new AzureBlobServer(CloudStorageAccount.Parse(connectionString));
        }

        public static AzureTableServer ConnectTableServer(string connectionString)
        {
            return new AzureTableServer(CloudStorageAccount.Parse(connectionString));
        }
    }

    public class AzureLazy
    {
        public Action SyncAction { get; private set; }
        public Func<AsyncCallback, object, IAsyncResult> AsyncBegin { get; private set; }
        public Action<IAsyncResult> AsyncEnd { get; private set; }

        public AzureLazy(Action syncAction, Func<AsyncCallback, object, IAsyncResult> asyncBegin, Action<IAsyncResult> asyncEnd)
        {
            this.SyncAction = syncAction;
            this.AsyncBegin = asyncBegin;
            this.AsyncEnd = asyncEnd;
        }

        public void Sync()
        {
            this.SyncAction();
        }

        public void Async(Action continuation = null)
        {
            this.AsyncBegin(ar =>
            {
                this.AsyncEnd(ar);
                if (continuation != null)
                {
                    continuation();
                }
            }, null);
        }
    }

    public class AzureLazy<T>
    {
        public Func<T> SyncAction { get; private set; }
        public Func<AsyncCallback, object, IAsyncResult> AsyncBegin { get; private set; }
        public Func<IAsyncResult, T> AsyncEnd { get; private set; }

        public AzureLazy(Func<T> syncAction, Func<AsyncCallback, object, IAsyncResult> asyncBegin, Func<IAsyncResult, T> asyncEnd)
        {
            this.SyncAction = syncAction;
            this.AsyncBegin = asyncBegin;
            this.AsyncEnd = asyncEnd;
        }

        public T Sync()
        {
            return this.SyncAction();
        }

        public void Async(Action<T> continuation = null)
        {
            this.AsyncBegin(ar =>
            {
                T result = this.AsyncEnd(ar);
                if (continuation != null)
                {
                    continuation(result);
                }
            }, null);
        }
    }
}
