using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace GaclibWebPage.Models
{
    public class IndexPageModel : NavigationBarModel
    {
        public IndexPageMirrorSite[] MirrorSites { get; set; }
        public IndividualDemoPageModel LatestUpdateDemo { get; set; }

        public IndexPageModel(string xmlIndexPath)
        {
            this.MirrorSites = new IndexPageMirrorSite[]
            {
                new IndexPageMirrorSite(){ Url = "http://www.gaclib.net", Description = "Hosted in China Mainland" },
                new IndexPageMirrorSite(){ Url = "http://asia.gaclib.net", Description = "Hosted in Hong Kong" },
                new IndexPageMirrorSite(){ Url = "http://us.gaclib.net", Description = "Hosted in West US" },
            };
            this.LatestUpdateDemo = new DemoPageModel(xmlIndexPath)
                .DemoPages
                .OrderBy(d => d.UpdateDate)
                .Last();
        }
    }

    public class IndexPageMirrorSite
    {
        public string Url { get; set; }
        public string Description { get; set; }
    }
}