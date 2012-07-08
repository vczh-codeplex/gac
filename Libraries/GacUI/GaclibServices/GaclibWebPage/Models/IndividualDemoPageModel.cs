using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace GaclibWebPage.Models
{
    public class IndividualDemoPageModel : NavigationBarModel
    {
        public string Title { get; set; }
        public string Name { get; set; }
        public string[] Images { get; set; }
        public DateTime UpdateDate { get; set; }
        public string Description { get; set; }
        public Dictionary<string, string> CppCodes { get; set; }

        public IndividualDemoPageModel()
        {
            this.Title = "";
            this.Images = new string[0];
            this.CppCodes = new Dictionary<string, string>();
        }
    }
}