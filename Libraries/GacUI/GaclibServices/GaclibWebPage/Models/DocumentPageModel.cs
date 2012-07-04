using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Microsoft.WindowsAzure;
using Microsoft.WindowsAzure.StorageClient;
using System.Xml.Linq;
using System.IO;

namespace GaclibWebPage.Models
{
    public class DocumentPageModel
    {
        public class IndexNode
        {
            public string Title { get; set; }
            public int Level { get; set; }
            public string UniqueId { get; set; }
            public bool Link { get; set; }
        };

        public string Title { get; set; }
        public IndexNode[] IndexTree { get; set; }
        public string Content { get; set; }

        public DocumentPageModel(string uniqueId)
        {
            string serviceName = "gaclibservicesasia";
            string accessKey = "";
            var account = new CloudStorageAccount(new StorageCredentialsAccountAndKey(serviceName, accessKey), true);
            var blobClient = new CloudBlobClient(account.BlobEndpoint.AbsoluteUri, account.Credentials);
            var container = blobClient.GetContainerReference("gaclib-meta-doc");
            var blobRef = container.GetBlobReference(uniqueId);
            string xmlText = blobRef.DownloadText();
            xmlText = xmlText.Substring(xmlText.IndexOf("<metaDoc>"));
            XDocument xmlDocument = XDocument.Parse(xmlText);

            var indexNodes = xmlDocument.Root.Element("indexTree").Elements("indexNode").ToArray();
            this.IndexTree = indexNodes
                .Select(e => new IndexNode()
                {
                    Title = e.Element("title").Value,
                    Level = int.Parse(e.Element("level").Value),
                    UniqueId = e.Element("uniqueId").Value,
                    Link = e.Element("link").Value == "true",
                })
                .ToArray();

            this.Content = xmlDocument.Root.Element("content").Value;
        }
    }
}