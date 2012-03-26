using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace GenDocIndex
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length >= 2)
            {
                string outputIndexFile = Path.GetFullPath(args[0]);
                string[] inputFiles = args.Skip(1)
                    .SelectMany(d => Directory.GetFiles(d))
                    .Select(f => Path.GetFullPath(f).ToUpper())
                    .Where(f => f.EndsWith(".DOCITEM.TXT"))
                    .Distinct()
                    .ToArray();

                DocItem[] docItems = inputFiles
                    .Select(f =>
                        {
                            try
                            {
                                List<string> errorLines = new List<string>();
                                DocItem result = DocItemParser.LoadDocItem(f, errorLines);
                                if (errorLines.Count > 0)
                                {
                                    Console.WriteLine("[{0}]:", f);
                                    foreach (var error in errorLines)
                                    {
                                        Console.WriteLine(error);
                                    }
                                }
                                return result;
                            }
                            catch (ArgumentException ex)
                            {
                                Console.WriteLine("[{0}]: {1}", f, ex.Message);
                                return null;
                            }
                        }
                    )
                    .Where(i => i != null)
                    .ToArray();
                var mappedDocItems = DocItemSorter.MapItems(docItems);
                var rootItems = DocItemSorter.SortAndGetRootItems(mappedDocItems);
            }
            else
            {
                Console.WriteLine("GenDocIndex.exe <output-index-file> <input-.docitem.txt-folder> ...");
            }
        }
    }
}
