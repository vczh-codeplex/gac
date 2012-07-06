using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AzureStorageFacadeLibrary;

namespace AzureStorageFacadeLibraryTest
{
    // wad-control-container

    [TestClass]
    public class BlobTest
    {
        private AzureBlobServer blobServer = null;

        [TestInitialize]
        public void PrepareBlob()
        {
            this.blobServer = AzureStorageFacade.ConnectBlobServer(AzureStorageFacade.DevelopmentStorageConnectionString);
            foreach (var blobContainer in this.blobServer.Containers)
            {
                blobContainer.DeleteContainer().Sync();
            }
        }

        [TestMethod]
        public void TestCreateDeleteContainer()
        {
            Assert.AreEqual(0, this.blobServer.Containers.Count());
            for (int i = 0; i < 10; i++)
            {
                this.blobServer["container-" + i.ToString()].CreateContainer().Sync();
            }

            var blobContainers = this.blobServer.Containers.ToArray();
            Assert.AreEqual(10, blobContainers.Length);
            for (int i = 0; i < 10; i++)
            {
                Assert.AreEqual("container-" + i.ToString(), blobContainers[i].Name);
            }

            for (int i = 0; i < 5; i++)
            {
                this.blobServer["container-" + i.ToString()].DeleteContainer().Sync();
            }
            for (int i = 0; i < 10; i++)
            {
                Assert.AreEqual(i >= 5, this.blobServer["container-" + i.ToString()].IsContainerExists().Sync());
            }
            for (int i = 0; i < 10; i++)
            {
                Assert.AreEqual(i < 5, this.blobServer["container-" + i.ToString()].CreateContainerIfNotExist().Sync());
            }
        }
    }
}
