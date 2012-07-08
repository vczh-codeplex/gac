using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using GaclibWebPage.Models;

namespace GaclibWebPage.Controllers
{
    public class DemoPageController : Controller
    {
        public ActionResult ShowDemo()
        {
            string name = this.RouteData.Values["demoId"].ToString();
            IndividualDemoPageModel model = DemoPageModel.LoadPage(Server.MapPath(DemoPageModel.DefaultXmlIndexPath), name);
            model.ActiveButton = NavigationButtons.Demos;
            return View(model);
        }
    }
}
