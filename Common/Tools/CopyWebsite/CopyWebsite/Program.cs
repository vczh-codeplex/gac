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
        static void SearchForUrls(string text, List<string> urls)
        {
            const int PLANTEXT = 0;
            const int WAITFORTAGNAME = 1;
            const int TAGNAME = 2;
            const int WAITFORATTNAME = 3;
            const int ATTNAME = 4;
            const int WAITFORQUOT = 5;
            const int ATTVALUE = 6;
            /*
             * abc< link    href="x.html"   alt=hi>text</a>
             * 00011222233334444566666663333444566000001120
             */
            int state = 0;
            foreach (var c in text)
            {
                switch (state)
                {
                    case PLANTEXT:
                        break;
                    case WAITFORTAGNAME:
                        break;
                    case TAGNAME:
                        break;
                    case WAITFORATTNAME:
                        break;
                    case ATTNAME:
                        break;
                    case WAITFORQUOT:
                        break;
                    case ATTVALUE:
                        break;
                }
            }
        }

        static void Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("Codepack.exe <url> <rootDefaultFileName> <path>");
                return;
            }

            List<string> relativeUrls = new List<string>() { "/" };
            HashSet<string> writtenFiles = new HashSet<string>();
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

                string file = Path.GetFullPath(outputPath + url.Replace('/', '\\'));
                if (file == outputPath + "\\")
                {
                    file += defaultFileName;
                }

                if (writtenFiles.Add(file.ToUpper()))
                {
                    Console.WriteLine("Downloading \"{0}\"...", url);
                    HttpWebRequest request = (HttpWebRequest)WebRequest.Create(rootUrl + url);
                    request.Method = "GET";
                    HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                    if (response.StatusCode != HttpStatusCode.OK)
                    {
                        Console.WriteLine("This web page cannot be found.");
                    }
                    else
                    {
                        if (!file.StartsWith(outputPath))
                        {
                            Console.WriteLine("This file cannot be downloaded into the specified output directory.");
                        }
                        else
                        {
                            Console.WriteLine("Writing to \"{0}\"...", file.Substring(outputPath.Length));

                            using (StreamReader reader = new StreamReader(response.GetResponseStream()))
                            using (StreamWriter writer = new StreamWriter(file, false, Encoding.UTF8))
                            {
                                string text = reader.ReadToEnd();
                                writer.Write(text);
                                SearchForUrls(text, relativeUrls);
                            }
                        }
                    }
                    response.Close();
                }
            }
        }
    }
}
