using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Xml.Linq;
using System.IO;

namespace GaclibWebPage.Models
{
    public class DemoListModel : NavigationBarModel
    {
        public static DemoPageModel[] LoadPages(string xmlIndexPath)
        {
            XDocument xmlIndex = XDocument.Load(xmlIndexPath);
            List<DemoPageModel> pages = new List<DemoPageModel>();
            foreach (var xmlCategory in xmlIndex.Root.Elements("category"))
            {
                string category = xmlCategory.Attribute("name").Value;
                foreach (var xmlDemo in xmlCategory.Elements("demo"))
                {
                    string title = xmlDemo.Attribute("name").Value;
                    DemoPageModel page = new DemoPageModel
                    {
                        Title = title,
                        Name = category + "." + title,
                        Images = xmlDemo.Elements("image").Select(e => e.Attribute("src").Value).ToArray(),
                        CppCodes = xmlDemo.Elements("cpp").Select(e => e.Attribute("src").Value).ToDictionary(s => s, s => ""),
                    };
                    pages.Add(page);
                }
            }
            return pages.ToArray();
        }

        public static DemoPageModel LoadPage(string xmlIndexPath, string name)
        {
            DemoPageModel page = LoadPages(xmlIndexPath)
                .Where(d => d.Name == name)
                .First();
            string[] cppFiles = page.CppCodes.Keys.ToArray();
            page.CppCodes = cppFiles
                .ToDictionary(
                    s => s,
                    s => File.ReadAllText(string.Format("{0}/{1}/{2}", Path.GetDirectoryName(xmlIndexPath), page.Name, s))
                    );
            return page;
        }

        public DemoPageModel[] DemoPages { get; set; }

        public DemoListModel(string xmlIndexPath)
        {
            this.DemoPages = LoadPages(xmlIndexPath);
        }
    }
}