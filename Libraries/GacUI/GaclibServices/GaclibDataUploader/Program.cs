using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Microsoft.WindowsAzure;
using Microsoft.WindowsAzure.StorageClient;
using System.Windows.Forms;

namespace GaclibDataUploader
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            string metaDocFolder = @"..\..\..\..\Public\Temp\StaticMetaDoc";
            string[] files = Directory.GetFiles(metaDocFolder).OrderBy(s => s.ToUpper()).ToArray();

            string key = Clipboard.GetText();
            Console.WriteLine("Your key is: " + key);
            Console.Write("Press enter to continue:");
            Console.ReadLine();
            var account = new CloudStorageAccount(new StorageCredentialsAccountAndKey("gaclibservicesasia", key), true);
            var blobClient = new CloudBlobClient(account.BlobEndpoint.AbsoluteUri, account.Credentials);

            Console.WriteLine("Creating container...");
            var container = blobClient.GetContainerReference("gaclib-meta-doc");
            container.CreateIfNotExist();

            Console.WriteLine("Writing data...");
            Console.WriteLine("Done!");
        }
    }
}
