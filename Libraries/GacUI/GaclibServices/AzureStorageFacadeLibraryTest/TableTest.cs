using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AzureStorageFacadeLibrary;

namespace AzureStorageFacadeLibraryTest
{
    [TestClass]
    public class TableTest
    {
        private AzureTableServer tableServer = null;

        [TestInitialize]
        public void PrepareTable()
        {
            this.tableServer = AzureStorageFacade.ConnectTableServer(AzureStorageFacade.DevelopmentStorageConnectionString);
            foreach (var blobContainer in this.tableServer.Tables)
            {
                blobContainer.DeleteTable().Sync();
            }
        }

        [TestMethod]
        public void TestCreateDeleteTable()
        {
            Assert.AreEqual(0, this.tableServer.Tables.Count());
            for (int i = 0; i < 10; i++)
            {
                this.tableServer["Table" + i.ToString()].CreateTable().Sync();
            }

            var tables = this.tableServer.Tables.ToArray();
            Assert.AreEqual(10, tables.Length);
            for (int i = 0; i < 10; i++)
            {
                Assert.AreEqual("Table" + i.ToString(), tables[i].Name);
            }

            for (int i = 0; i < 5; i++)
            {
                this.tableServer["Table" + i.ToString()].DeleteTable().Sync();
            }
            for (int i = 0; i < 10; i++)
            {
                Assert.AreEqual(i >= 5, this.tableServer["Table" + i.ToString()].DoesTableExist().Sync());
            }
            for (int i = 0; i < 10; i++)
            {
                Assert.AreEqual(i < 5, this.tableServer["Table" + i.ToString()].CreateTableIfNotExist().Sync());
            }

            for (int i = 0; i < 5; i++)
            {
                this.tableServer["Table" + i.ToString()].DeleteTable().Sync();
            }
            for (int i = 0; i < 10; i++)
            {
                Assert.AreEqual(i >= 5, this.tableServer["Table" + i.ToString()].DeleteTableIfExist().Sync());
            }
            for (int i = 0; i < 10; i++)
            {
                Assert.AreEqual(false, this.tableServer["Table" + i.ToString()].DoesTableExist().Sync());
            }
        }
    }
}
