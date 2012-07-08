using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Xml.Linq;
using System.IO;

namespace GaclibWebPage.Models
{
    public class DemoPageModel : NavigationBarModel
    {
        public const string DefaultXmlIndexPath = "~/Content/Demos/Index.xml";

        public static IndividualDemoPageModel[] LoadPages(string xmlIndexPath)
        {
            XDocument xmlIndex = XDocument.Load(xmlIndexPath);
            List<IndividualDemoPageModel> pages = new List<IndividualDemoPageModel>();
            foreach (var xmlCategory in xmlIndex.Root.Elements("category"))
            {
                string category = xmlCategory.Attribute("name").Value;
                foreach (var xmlDemo in xmlCategory.Elements("demo"))
                {
                    string title = xmlDemo.Attribute("name").Value;
                    IndividualDemoPageModel page = new IndividualDemoPageModel
                    {
                        Title = title,
                        Name = category + "." + title,
                        Images = xmlDemo.Elements("image").Select(e => e.Attribute("src").Value).ToArray(),
                        UpdateDate = DateTime.Parse(xmlDemo.Element("date").Value),
                        Description = xmlDemo.Element("description").Value,
                        CppCodes = xmlDemo.Elements("cpp").Select(e => e.Attribute("src").Value).ToDictionary(s => s, s => ""),
                    };
                    pages.Add(page);
                }
            }
            return pages.ToArray();
        }

        public static IndividualDemoPageModel LoadPage(string xmlIndexPath, string name)
        {
            IndividualDemoPageModel page = LoadPages(xmlIndexPath)
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

        public IndividualDemoPageModel[] DemoPages { get; set; }

        public DemoPageModel(string xmlIndexPath)
        {
            this.DemoPages = LoadPages(xmlIndexPath);
        }
    }
}