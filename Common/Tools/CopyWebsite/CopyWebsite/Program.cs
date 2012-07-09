using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;

namespace CopyWebsite
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("Codepack.exe <url> <rootDefaultFileName> <path>");
                return;
            }

            List<string> relativeUrls = new List<string>() { "/" };
            HashSet<string> queriedRelativeUrls = new HashSet<string>();
            int currentUrlIndex = 0;

            string rootUrl = args[0];
            string defaultFileName = args[1];
            string outputPath = Path.GetFullPath(args[2]);
            if (outputPath.Last() == '\\')
            {
                outputPath.Substring(0, outputPath.Length - 1);
            }
            Directory.CreateDirectory(outputPath);

            Console.WriteLine("Root Url: {0}", rootUrl);
            Console.WriteLine("Default File Name: {0}", defaultFileName);
            Console.WriteLine("Output Directory: {0}", outputPath);

            while (currentUrlIndex < relativeUrls.Count)
            {
                string url = relativeUrls[currentUrlIndex++];
                if (url.Length == 0 || url[0] != '/')
                {
                    url = '/' + url;
                }
                if (queriedRelativeUrls.Add(url.ToUpper()))
                {
                    Console.WriteLine("Downloading \"{0}\"...", url);
                    string file = Path.GetFullPath(outputPath + url.Replace('/', '\\'));
                    if (!file.StartsWith(outputPath))
                    {
                        Console.WriteLine("This file cannot be downloaded into the specified output directory.");
                    }
                    else
                    {
                        if (file == outputPath + "\\")
                        {
                            file += defaultFileName;
                        }
                        Console.WriteLine("Writing to \"{0}\"...", file.Substring(outputPath.Length));
                    }
                }
            }
        }
    }
}
