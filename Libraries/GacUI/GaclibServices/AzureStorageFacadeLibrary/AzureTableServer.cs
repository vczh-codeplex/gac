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

        public IEnumerable<AzureTable> Tables
        {
            get
            {
                return this.RawQueueClient.ListTables().Select(n => new AzureTable(this, n));
            }
        }

        public AzureTable this[string name]
        {
            get
            {
                return new AzureTable(this, name);
            }
        }
    }

    public class AzureTable
    {
        public AzureTableServer Server { get; private set; }
        public string Name { get; set; }

        internal AzureTable(AzureTableServer server, string name)
        {
            this.Server = server;
            this.Name = name;
        }

        public AzureLazy CreateTable()
        {
            return new AzureLazy(
                () => this.Server.RawQueueClient.CreateTable(this.Name),
                (ac, st) => this.Server.RawQueueClient.BeginCreateTable(this.Name, ac, st),
                this.Server.RawQueueClient.EndCreateTable
                );
        }

        public AzureLazy<bool> CreateTableIfNotExist()
        {
            return new AzureLazy<bool>(
                () => this.Server.RawQueueClient.CreateTableIfNotExist(this.Name),
                (ac, st) => this.Server.RawQueueClient.BeginCreateTableIfNotExist(this.Name, ac, st),
                this.Server.RawQueueClient.EndCreateTableIfNotExist
                );
        }

        public AzureLazy DeleteTable()
        {
            return new AzureLazy(
                () => this.Server.RawQueueClient.DeleteTable(this.Name),
                (ac, st) => this.Server.RawQueueClient.BeginDeleteTable(this.Name, ac, st),
                this.Server.RawQueueClient.EndDeleteTable
                );
        }

        public AzureLazy<bool> DeleteTableIfExist()
        {
            return new AzureLazy<bool>(
                () => this.Server.RawQueueClient.DeleteTableIfExist(this.Name),
                (ac, st) => this.Server.RawQueueClient.BeginDeleteTableIfExist(this.Name, ac, st),
                this.Server.RawQueueClient.EndDeleteTableIfExist
                );
        }

        public AzureLazy<bool> DoesTableExist()
        {
            return new AzureLazy<bool>(
                () => this.Server.RawQueueClient.DoesTableExist(this.Name),
                (ac, st) => this.Server.RawQueueClient.BeginDoesTableExist(this.Name, ac, st),
                this.Server.RawQueueClient.EndDoesTableExist
                );
        }
    }
}
