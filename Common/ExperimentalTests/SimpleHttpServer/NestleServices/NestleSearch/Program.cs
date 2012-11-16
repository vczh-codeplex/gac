using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using SimpleHttpServer;

namespace NestleSearch
{
    class Program : IDeployServerCallback
    {
        [Get("")]
        public string Default()
        {
            return "Welcome to Nestle Search!";
        }

        public void Stop()
        {
            stopEvent.Set();
        }

        static ManualResetEvent stopEvent = null;

        static void Main(string[] args)
        {
            if (SimpleHttpServerHost.StartService<Program>("NestleSearch"))
            {
                stopEvent = new ManualResetEvent(false);
                stopEvent.WaitOne();
            }
            else
            {
                Console.WriteLine("Press [ENTER] to exit.");
                Console.ReadLine();
            }
        }
    }
}
