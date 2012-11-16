using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace NestleSpider
{
    public class Spider
    {
        private const string Username = "geniusvczh_apptest";
        private const string Password = "fuckkula";
        private const string ApiKey = "e1424eb84b6d4169a10c03fb1e73e140";
        private const string ApiSecret = "9814021f20054b558105fca1df6559a7";

        private Cookie GetSession()
        {
            string body = string.Format("api_key={0}&api_secret={1}&username={2}&password={3}", ApiKey, ApiSecret, Username, Password);
            string url = "https://www.niaowo.me/account/token/";
            byte[] bodyBytes = Encoding.UTF8.GetBytes(body);

            HttpWebRequest request = HttpWebRequest.CreateHttp(url);
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.ContentLength = bodyBytes.Length;
            request.GetRequestStream().Write(bodyBytes, 0, bodyBytes.Length);
            request.CookieContainer = new CookieContainer();

            using (HttpWebResponse response = (HttpWebResponse)request.GetResponse())
            {
                return response.Cookies[0];
            }
        }

        private XDocument GetXml(string path, Cookie cookie)
        {
            string url = string.Format("https://www.niaowo.me{0}", path);

            HttpWebRequest request = HttpWebRequest.CreateHttp(url);
            request.Method = "GET";
            request.CookieContainer = new CookieContainer();
            request.CookieContainer.Add(cookie);

            using (HttpWebResponse response = (HttpWebResponse)request.GetResponse())
            {
                return XDocument.Load(response.GetResponseStream());
            }
        }

        public void Run()
        {
            Cookie cookie = GetSession();
            var firstPage = GetXml("/topics.xml", cookie);
            int totalPages = int.Parse(firstPage.Root.Element("page").Element("total-pages").Value);
            List<Topic> fullTopics = new List<Topic>();

            for (int i = 1; i <= totalPages; i++)
            {
                var topicPage = firstPage;
                if (i > 1)
                {
                    topicPage = GetXml(string.Format("/topics/page/{0}.xml", i), cookie);
                }
                Console.WriteLine("Downloading Page: {0}/{1}", i, totalPages);

                var topicIds = topicPage
                    .Root
                    .Element("topics")
                    .Elements("topic")
                    .Select(e => int.Parse(e.Element("id").Value))
                    .ToArray();

                for (int j = 0; j < topicIds.Length; j++)
                {
                    int id = topicIds[j];
                    var topic = GetXml(string.Format("/topics/{0}.xml?output=markdown", id), cookie);
                    fullTopics.Add(new Topic(topic.Root));
                    Console.WriteLine("Downloading Topic: {0}/{1} - {2}/{3}", i, totalPages, j + 1, topicIds.Length);
                }
            }
        }
    }

    class Comment
    {
        public int id { get; set; }
        public string Author { get; set; }
        public string Body { get; set; }
        public DateTime CreateDateTime { get; set; }

        public Comment(XElement xml)
        {
            this.id = int.Parse(xml.Element("id").Value);
            this.Author = xml.Element("author").Value;
            this.Body = xml.Element("body").Value;
            this.CreateDateTime = DateTime.Parse(xml.Element("created").Value);
        }
    }

    class Topic
    {
        public int id { get; set; }
        public string Title { get; set; }
        public string Description { get; set; }
        public string Author { get; set; }
        public string Body { get; set; }
        public DateTime CreateDateTime { get; set; }
        public Comment[] Comments { get; set; }

        public Topic(XElement xml)
        {
            this.id = int.Parse(xml.Element("topic").Element("id").Value);
            this.Title = xml.Element("topic").Element("title").Value;
            this.Description = xml.Element("topic").Element("desc").Value;
            this.Author = xml.Element("topic").Element("author").Value;
            this.Body = xml.Element("topic").Element("body").Value;
            this.CreateDateTime = DateTime.Parse(xml.Element("topic").Element("created").Value);
            this.Comments = xml
                .Element("comments")
                .Elements("comment")
                .Select(c => new Comment(c))
                .ToArray();
        }
    }
}
