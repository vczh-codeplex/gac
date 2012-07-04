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

        public ActionResult Demos()
        {
            DemoListModel model = new DemoListModel(Server.MapPath("~/Content/Demos/Index.xml"));
            model.ActiveButton = NavigationButtons.Demos;
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
