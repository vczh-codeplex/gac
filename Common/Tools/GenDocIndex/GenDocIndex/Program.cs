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
                    .Select(f => Path.GetFullPath(f))
                    .Distinct()
                    .ToArray();
            }
            else
            {
                Console.WriteLine("GenDocIndex.exe <output-index-file> <input-.docitem.txt-folder> ...");
            }
        }
    }
}
