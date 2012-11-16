using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using AzureStorageFacadeLibrary;
using Microsoft.WindowsAzure.StorageClient;

namespace NestleDatabase
{
    public class NestleDatabaseServer
    {
        private AzureTableServer tableServer = null;

        public AzureTable Topics { get; set; }
        public AzureTable Comments { get; set; }
        public AzureTable AuthorTopics { get; set; }
        public AzureTable AuthorComments { get; set; }

        public NestleDatabaseServer()
        {
            XDocument configuration = XDocument.Load("StorageConfiguration.xml");
            string account = configuration.Root.Element("account").Value;
            string key = configuration.Root.Element("key").Value;
            string container = configuration.Root.Element("container").Value;
            this.tableServer = AzureStorageFacade.ConnectTableServer(AzureStorageFacade.CreateConnectionString(account, key));

            this.Topics = this.tableServer["NestleTopics"];
            this.Comments = this.tableServer["NestleComments"];
            this.AuthorTopics = this.tableServer["NestleAuthorTopics"];
            this.AuthorComments = this.tableServer["NestleAuthorComments"];

            this.Topics.CreateTableIfNotExist().Sync();
            this.Comments.CreateTableIfNotExist().Sync();
            this.AuthorTopics.CreateTableIfNotExist().Sync();
            this.AuthorComments.CreateTableIfNotExist().Sync();
        }

        public void Clear()
        {
            foreach (var entity in this.Topics.Query<NestleTopic>().Execute())
            {
                this.Topics.DeleteEntity(entity);
            }
            foreach (var entity in this.Comments.Query<NestleComment>().Execute())
            {
                this.Comments.DeleteEntity(entity);
            }
            foreach (var entity in this.AuthorTopics.Query<NestleAuthorTopic>().Execute())
            {
                this.AuthorTopics.DeleteEntity(entity);
            }
            foreach (var entity in this.AuthorComments.Query<NestleAuthorComment>().Execute())
            {
                this.AuthorComments.DeleteEntity(entity);
            }
            this.tableServer.SaveChanges().Sync();
        }
    }

    public class NestleTopic : TableServiceEntity
    {
        public int id { get; set; }
        public string Title { get; set; }
        public string Description { get; set; }
        public string Author { get; set; }
        public string Body { get; set; }
        public DateTime CreateDateTime { get; set; }
    }

    public class NestleComment : TableServiceEntity
    {
        public string TopicKey { get; set; }
        public int id { get; set; }
        public string Author { get; set; }
        public string Body { get; set; }
        public DateTime CreateDateTime { get; set; }
    }

    public class NestleAuthorTopic : TableServiceEntity
    {
        public string Author { get; set; }
        public string TopicKey { get; set; }
    }

    public class NestleAuthorComment : TableServiceEntity
    {
        public string Author { get; set; }
        public string CommentKey { get; set; }
    }
}
