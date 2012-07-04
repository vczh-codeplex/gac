using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace GaclibWebPage.Models
{
    public class DemoDescription
    {
        public string Name { get; set; }
        public string Title { get; set; }
    }

    public class DemoListModel : NavigationBarModel
    {
        public DemoDescription[] DemoDescriptions { get; set; }

        public DemoListModel()
        {
            this.DemoDescriptions = new DemoDescription[0];
        }
    }
}