using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using GaclibWebPage.Models;

namespace GaclibWebPage.Controllers
{
    public class DocumentPageController : Controller
    {
        public ActionResult ShowDocument()
        {
            DocumentPageModel model = new DocumentPageModel();
            model.Title = this.RouteData.Values["uniqueId"].ToString();
            return View(model);
        }
    }
}
