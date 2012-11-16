﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using NestleDatabase;

namespace NestleSpider
{
    public class Spider
    {
        private const string Username = "geniusvczh_apptest";
        private const string Password = "fuckkula";
        private const string ApiKey = "e1424eb84b6d4169a10c03fb1e73e140";
        private const string ApiSecret = "9814021f20054b558105fca1df6559a7";
        private static NestleDatabaseServer nestleServer = null;

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
            if (nestleServer == null)
            {
                nestleServer = new NestleDatabaseServer();
            }

            Console.WriteLine("*********************************************");
            Console.WriteLine("Downloading");
            Console.WriteLine("*********************************************");

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
                break;
            }

            Console.WriteLine("*********************************************");
            Console.WriteLine("Analyzing");
            Console.WriteLine("*********************************************");

            var authorTopics = fullTopics
                .GroupBy(t => t.Author)
                .ToDictionary(g => g.Key, g => g.ToArray());

            var authorComments = fullTopics
                .SelectMany(t => t.Comments)
                .GroupBy(c => c.Author)
                .ToDictionary(g => g.Key, g => g.ToArray());

            Console.WriteLine("*********************************************");
            Console.WriteLine("Clearing");
            Console.WriteLine("*********************************************");

            nestleServer.Clear();

            Console.WriteLine("*********************************************");
            Console.WriteLine("Recording");
            Console.WriteLine("*********************************************");

            int counter = 0;

            // query before insert to determine old row key
            // calculate body hash to determine the necessarity of inserting into blob container
            // only update the delta

            Console.WriteLine("Topics");
            counter = 0;
            foreach (var topic in fullTopics)
            {
                NestleTopic nestleTopic = new NestleTopic()
                {
                    PartitionKey = "",
                    RowKey = topic.RowKey,
                    id = topic.id,
                    Title = topic.Title,
                    Description = topic.Description,
                    Author = topic.Author,
                    CreateDateTime = topic.CreateDateTime
                };
                nestleServer.Topics.AddEntity(nestleTopic);
                nestleServer.Topics.Server.SaveChanges().Sync();
                Console.WriteLine("{0}/{1}", ++counter, fullTopics.Count);
            }

            Console.WriteLine("Comments");
            counter = 0;
            var fullComments = fullTopics.SelectMany(t => t.Comments).ToArray();
            foreach (var comment in fullComments)
            {
                NestleComment nestleComment = new NestleComment()
                {
                    PartitionKey = "",
                    RowKey = comment.RowKey,
                    TopicKey = comment.Topic.RowKey,
                    id = comment.id,
                    Author = comment.Author,
                    CreateDateTime = comment.CreateDateTime
                };
                nestleServer.Comments.AddEntity(nestleComment);
                nestleServer.Topics.Server.SaveChanges().Sync();
                Console.WriteLine("{0}/{1}", ++counter, fullComments.Length);
            }

            Console.WriteLine("AuthorTopics");
            counter = 0;
            foreach (var authorTopic in authorTopics)
            {
                foreach (var topic in authorTopic.Value)
                {
                    NestleAuthorTopic nestleAuthorTopic = new NestleAuthorTopic()
                    {
                        PartitionKey = "",
                        RowKey = Guid.NewGuid().ToString(),
                        Author = authorTopic.Key,
                        TopicKey = topic.RowKey,
                    };
                    nestleServer.AuthorTopics.AddEntity(nestleAuthorTopic);
                    nestleServer.Topics.Server.SaveChanges().Sync();
                }
                Console.WriteLine("{0}/{1}", ++counter, authorTopics.Count);
            }

            Console.WriteLine("AuthorComments");
            counter = 0;
            foreach (var authorComment in authorComments)
            {
                foreach (var comment in authorComment.Value)
                {
                    NestleAuthorComment nestleAuthorComment = new NestleAuthorComment()
                    {
                        PartitionKey = "",
                        RowKey = Guid.NewGuid().ToString(),
                        Author = authorComment.Key,
                        CommentKey = comment.RowKey,
                    };
                    nestleServer.AuthorComments.AddEntity(nestleAuthorComment);
                    nestleServer.Topics.Server.SaveChanges().Sync();
                }
                Console.WriteLine("{0}/{1}", ++counter, authorComments.Count);
            }

            Console.WriteLine("Bodies");
            counter = 0;
            foreach (var topic in fullTopics)
            {
                nestleServer.Bodies.GetBlob(topic.RowKey).AsString = topic.Body;
                Console.WriteLine("{0}/{1}", ++counter, fullTopics.Count + fullComments.Length);
            }
            foreach (var comment in fullComments)
            {
                nestleServer.Bodies.GetBlob(comment.RowKey).AsString = comment.Body;
                Console.WriteLine("{0}/{1}", ++counter, fullTopics.Count + fullComments.Length);
            }

            Console.WriteLine("*********************************************");
            Console.WriteLine("Finished");
            Console.WriteLine("*********************************************");
        }
    }

    class Comment
    {
        public string RowKey { get; set; }
        public Topic Topic { get; set; }
        public int id { get; set; }
        public string Author { get; set; }
        public string Body { get; set; }
        public DateTime CreateDateTime { get; set; }

        public Comment(XElement xml, Topic topic)
        {
            this.RowKey = Guid.NewGuid().ToString();
            this.Topic = topic;
            this.id = int.Parse(xml.Element("id").Value);
            this.Author = xml.Element("author").Value;
            this.Body = xml.Element("body").Value;
            this.CreateDateTime = DateTime.Parse(xml.Element("created").Value);
        }
    }

    class Topic
    {
        public string RowKey { get; set; }
        public int id { get; set; }
        public string Title { get; set; }
        public string Description { get; set; }
        public string Author { get; set; }
        public string Body { get; set; }
        public DateTime CreateDateTime { get; set; }
        public Comment[] Comments { get; set; }

        public Topic(XElement xml)
        {
            this.RowKey = Guid.NewGuid().ToString();
            this.id = int.Parse(xml.Element("topic").Element("id").Value);
            this.Title = xml.Element("topic").Element("title").Value;
            this.Description = xml.Element("topic").Element("desc").Value;
            this.Author = xml.Element("topic").Element("author").Value;
            this.Body = xml.Element("topic").Element("body").Value;
            this.CreateDateTime = DateTime.Parse(xml.Element("topic").Element("created").Value);
            this.Comments = xml
                .Element("comments")
                .Elements("comment")
                .Select(c => new Comment(c, this))
                .ToArray();
        }
    }
}
