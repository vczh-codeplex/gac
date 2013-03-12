using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace GaclibWebPage.Models
{
    public class DownloadPageModel : NavigationBarModel
    {
        public DownloadVersionUrls[] Versions { get; set; }

        public DownloadPageModel()
        {
            this.Versions = new DownloadVersionUrls[] { 
                new DownloadVersionUrls{ Version="0.5.0.0", Url="http://gac.codeplex.com/releases/view/103284" },
                new DownloadVersionUrls{ Version="0.4.3.0", Url="http://gac.codeplex.com/releases/view/100843" },
                new DownloadVersionUrls{ Version="0.4.2.0", Url="http://gac.codeplex.com/releases/view/100843" },
                new DownloadVersionUrls{ Version="0.4.1.0", Url="http://gac.codeplex.com/releases/view/98192" },
                new DownloadVersionUrls{ Version="0.4.0.0", Url="http://gac.codeplex.com/releases/view/96473" },
                new DownloadVersionUrls{ Version="0.3.0.0", Url="http://gac.codeplex.com/releases/view/92292" },
                new DownloadVersionUrls{ Version="0.2.5.0", Url="http://gac.codeplex.com/releases/view/91058" },
                new DownloadVersionUrls{ Version="0.2.0.0", Url="http://gac.codeplex.com/releases/view/89642" },
                new DownloadVersionUrls{ Version="0.1.0.0", Url="http://gac.codeplex.com/releases/view/88533" },
            };
        }
    }

    public class DownloadVersionUrls
    {
        public string Version { get; set; }
        public string Url { get; set; }
    }
}