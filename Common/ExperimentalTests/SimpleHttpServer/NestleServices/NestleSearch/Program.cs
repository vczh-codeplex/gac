using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using NestleDatabase;
using SimpleHttpServer;

namespace NestleSearch
{
    class Program : IDeployServerCallback
    {
        const string html = "<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\"/></head><body>{0}</body></html>";

        [Get("")]
        public string Default()
        {
            string body = "<p>Welcome to Nestle Search!</p><p>Try word tokenizer for <a href=\"/NestleSearch/Tokenize/今天我在肯德基吃了一只鸡腿\">今天我在肯德基吃了一只鸡腿</a>.</p>";
            return string.Format(html, body);
        }

        [Get("/Tokenize/{input}")]
        public string Tokenize(string input)
        {
            input = Uri.UnescapeDataString(input);
            string[] words = WordTokenizer.Tokenize(input);
            string body = words.Select(w => w + "<br/>").Aggregate("", (a, b) => a + b);
            return string.Format(html, body);
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
