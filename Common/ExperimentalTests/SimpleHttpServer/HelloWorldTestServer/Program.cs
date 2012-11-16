using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SimpleHttpServer;

namespace HelloWorldTestServer
{
    class Program
    {
        [Get("")]
        public string Default()
        {
            return string.Format(
                "<p>Hello, world!</p><p>The current machine name is: {0}</p><p>{1}</p>",
                Environment.MachineName,
                new string[] { "Genius", "Vczh" }
                    .Select(s => string.Format("<a href=\"/HelloWorld/Person/{0}/\">{1}</a><br/>", s, s))
                    .Aggregate((a, b) => a + b)
                    );
        }

        [Get("/Person/{name}")]
        public string Person(string name)
        {
            return string.Format(
                "<p>Hi, {0}.</p><p><a href=\"/HelloWorld/\">Home Page</a></p>",
                name);
        }

        static void Main(string[] args)
        {
            SimpleHttpServerHost.Run(typeof(Program), "+", 8080, "/HelloWorld");
            Console.WriteLine("Press [ENTER] to exit.");
            Console.ReadLine();
        }
    }
}
