using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace GaclibWebPage.Models
{
    public class DemoPageModel : NavigationBarModel
    {
        public string Title { get; set; }
        public string Name { get; set; }
        public string[] Images { get; set; }
        public Dictionary<string, string> CppCodes { get; set; }

        public DemoPageModel()
        {
            this.Title = "";
            this.Images = new string[0];
            this.CppCodes = new Dictionary<string, string>();
        }
    }
}