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
            string[] files = Directory.GetFiles(metaDocFolder).OrderBy(s => s.ToUpper()).Select(p => Path.GetFullPath(p)).ToArray();

            string key = Clipboard.GetText();
            Console.WriteLine("Your key is: " + key);
            Console.Write("Press enter to continue:");
            Console.ReadLine();
            var account = new CloudStorageAccount(new StorageCredentialsAccountAndKey("gaclibservicesasia", key), true);
            var blobClient = new CloudBlobClient(account.BlobEndpoint.AbsoluteUri, account.Credentials);

            Console.WriteLine("Creating container...");
            var container = blobClient.GetContainerReference("gaclib-meta-doc");
            container.CreateIfNotExist();

            Console.WriteLine("Clearning container...");
            var blobs = container.ListBlobs().ToArray();
            foreach (var blob in blobs)
            {
                var blobRef = container.GetBlobReference(blob.Uri.ToString());
                blobRef.Delete();
            }

            Console.WriteLine("Uploading files...");
            int index = 0;
            foreach (var file in files)
            {
                string blobKey = Path.GetFileNameWithoutExtension(file);
                var blobRef = container.GetBlobReference(blobKey);
                blobRef.UploadFile(file);
                Console.WriteLine("{0}/{1}", ++index, files.Length);
            }

            Console.WriteLine("Checking files...");
            string[] urls = container.ListBlobs().Select(b => b.Uri.ToString()).ToArray();

            Console.WriteLine("Writing data...");
            Console.WriteLine("Done!");
        }
    }
}
