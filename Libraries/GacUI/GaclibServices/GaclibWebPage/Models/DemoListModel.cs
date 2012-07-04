using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace GaclibWebPage.Models
{
    public class DemoListModel : NavigationBarModel
    {
        private class DemoMeta
        {
            public string Title { get; set; }
            public int MinImage { get; set; }
            public int MaxImage { get; set; }
        }

        private static DemoMeta[] demoControls = new DemoMeta[]
        {
            new DemoMeta { Title = "Label.Hyperlink",               MinImage = 27, MaxImage = 27},
            new DemoMeta { Title = "Button.EnableDisable",          MinImage = 28, MaxImage = 29},
            new DemoMeta { Title = "Button.CheckAndRadio",          MinImage = 30, MaxImage = 31},
            new DemoMeta { Title = "Tab.TextBoxPage",               MinImage = 32, MaxImage = 33},
            new DemoMeta { Title = "TextBox.Editor",                MinImage = 34, MaxImage = 34},
            new DemoMeta { Title = "TextBox.Colorizer",             MinImage = 35, MaxImage = 37},
            new DemoMeta { Title = "ListBox.NameEditor",            MinImage = 38, MaxImage = 38},
            new DemoMeta { Title = "ListBox.NameSelector",          MinImage = 39, MaxImage = 39},
            new DemoMeta { Title = "ListBox.VirtualMode",           MinImage = 40, MaxImage = 40},
            new DemoMeta { Title = "ListView.ViewSwitching",        MinImage = 41, MaxImage = 46},
            new DemoMeta { Title = "ListView.VirtualMode",          MinImage = 47, MaxImage = 47},
            new DemoMeta { Title = "ListView.SortingAndFiltering",  MinImage = 48, MaxImage = 50},
            new DemoMeta { Title = "TreeView.FileExplorer",         MinImage = 51, MaxImage = 52},
        };

        private static string[] demoCategories = new string[]
        {
            "Controls",
        };

        private static Dictionary<string, DemoPageModel[]> categoryIndexedDemos = null;
        private static Dictionary<string, DemoPageModel> nameIndexedDemos = null;

        static DemoListModel()
        {
            categoryIndexedDemos = new Dictionary<string, DemoPageModel[]>();
            foreach (var category in demoCategories)
            {
                var categorizedDemos = (DemoMeta[])typeof(DemoListModel)
                    .GetField("demo" + category, System.Reflection.BindingFlags.Static | System.Reflection.BindingFlags.NonPublic)
                    .GetValue(null);
                categoryIndexedDemos.Add(
                    category,
                    categorizedDemos
                        .Select(name => new DemoPageModel
                        {
                            Name = "Controls." + name.Title,
                            Title = name.Title,
                            Images = Enumerable
                                .Range(name.MinImage, name.MaxImage - name.MinImage + 1)
                                .Select(i => string.Format("DXGUI_{0}.jpg", i))
                                .ToArray(),
                        })
                        .ToArray()
                    );
            }

            nameIndexedDemos = categoryIndexedDemos
                .Values
                .SelectMany(v => v)
                .ToDictionary(d => d.Name, d => d);
        }

        public DemoPageModel[] DemoPages { get; set; }

        public DemoListModel()
        {
            this.DemoPages = demoCategories.SelectMany(c => categoryIndexedDemos[c]).ToArray();
        }

        public static DemoPageModel GetPage(string name)
        {
            return nameIndexedDemos[name];
        }
    }
}