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

        [TestMethod]
        public void TestUploadText()
        {
            var container = this.blobServer["azure-storage-facade-library-test"];
            Assert.IsFalse(container.IsContainerExists().Sync());
            container.CreateContainer().Sync();
            Assert.IsTrue(container.IsContainerExists().Sync());

            Assert.AreEqual(0, container.Directories.Count());
            Assert.AreEqual(0, container.Blobs.Count());
            Assert.AreEqual(0, container.FlatBlobs.Count());

            var blob = container.GetBlob("vczh");
            Assert.IsFalse(blob.IsBlobExists().Sync());
            blob.AsString = "genius";
            Assert.IsTrue(blob.IsBlobExists().Sync());
            Assert.AreEqual("genius", blob.AsString);

            Assert.AreEqual(0, container.Directories.Count());
            Assert.AreEqual(1, container.Blobs.Count());
            Assert.AreEqual(1, container.FlatBlobs.Count());
            Assert.AreEqual("vczh", container.Blobs.First().Name);
            Assert.AreEqual("vczh", container.FlatBlobs.First().Name);

            blob.Delete().Sync();
            Assert.IsFalse(blob.DeleteIfExists().Sync());

            Assert.AreEqual(0, container.Directories.Count());
            Assert.AreEqual(0, container.Blobs.Count());
            Assert.AreEqual(0, container.FlatBlobs.Count());
        }
    }
}
