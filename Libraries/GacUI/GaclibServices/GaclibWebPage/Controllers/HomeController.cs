using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using GaclibWebPage.Models;

namespace GaclibWebPage.Controllers
{
    public class HomeController : Controller
    {
        private static string[] demoControls = new string[]
        {
            "Button.CheckAndRadio",
            "Button.EnableDisable",
            "Label.Hyperlink",

            "Tab.TextBoxPage",
            "TextBox.Editor",
            "TextBox.Colorizer",

            "ListBox.NameEditor",
            "ListBox.NameSelector",
            "ListBox.VirtualMode",

            "ListView.ViewSwitching",
            "ListView.VirtualMode",
            "ListView.SortingAndFiltering",

            "TreeView.FileExplorer",
        };

        public ActionResult Index(NavigationBarModel model)
        {
            model.ActiveButton = NavigationButtons.Home;
            return View(model);
        }

        public ActionResult GettingStart(NavigationBarModel model)
        {
            model.ActiveButton = NavigationButtons.GettingStart;
            return View(model);
        }

        public ActionResult Demos(DemoListModel model)
        {
            model.ActiveButton = NavigationButtons.Demos;
            model.DemoDescriptions =
                demoControls
                .Select(name => new DemoDescription
                {
                    Name = "Controls." + name,
                    Title = name,
                })
                .ToArray();
            return View(model);
        }

        public ActionResult Download(NavigationBarModel model)
        {
            model.ActiveButton = NavigationButtons.Download;
            return View(model);
        }

        public ActionResult Contact(NavigationBarModel model)
        {
            model.ActiveButton = NavigationButtons.Contact;
            return View(model);
        }
    }
}
