﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace SimpleHttpServer
{
    public static class SimpleHttpServerHost
    {
        class RouteMethodRecord
        {
            public RouteAttribute RouteProperties { get; set; }
            public MethodInfo RouteMethod { get; set; }
            public Regex RoutePathRegex { get; set; }

            private string[] Parameters { get; set; }
            private int[] ParameterIndexMapping { get; set; }

            private static Regex regexRoutePathParser = new Regex(@"\{(?<name>\w+)\}");

            public RouteMethodRecord(RouteAttribute routeProperties, string prefix, MethodInfo routeMethod)
            {
                string routePath = routeProperties.RoutePath.Trim().TrimEnd('/');
                var matches = regexRoutePathParser.Matches(routePath);
                {
                    this.RouteProperties = routeProperties;
                    this.RouteMethod = routeMethod;
                }
                {
                    List<string> regexFragments = new List<string>();
                    int index = 0;
                    foreach (Match match in matches)
                    {
                        regexFragments.Add(Regex.Escape(routePath.Substring(index, match.Index - index)));
                        regexFragments.Add(string.Format(@"(?<{0}>[^/]*?)", match.Groups["name"].Value));
                        index = match.Index + match.Length;
                    }
                    regexFragments.Add(Regex.Escape(routePath.Substring(index, routePath.Length - index)));

                    string regex = regexFragments.Aggregate((a, b) => a + b);
                    this.RoutePathRegex = new Regex(string.Format("^{0}{1}$", Regex.Escape(prefix), regex));
                }
                {
                    this.Parameters = matches
                        .Cast<Match>()
                        .Select(m => m.Groups["name"].Value)
                        .ToArray();
                }
                {
                    var parameterInfoNames = this.RouteMethod
                        .GetParameters()
                        .Select(p => p.Name)
                        .ToArray();

                    var parameterIndices = Enumerable
                        .Range(0, parameterInfoNames.Length)
                        .ToDictionary(i => parameterInfoNames[i], i => i);

                    this.ParameterIndexMapping = this.Parameters
                        .Select(p => parameterIndices[p])
                        .ToArray();
                }
            }

            public string Execute(Match match, object serverObject)
            {
                var parameters = this.Parameters
                    .Select(p => match.Groups[p].Value)
                    .Cast<object>()
                    .ToArray();
                var parametersInOrder = this.ParameterIndexMapping
                    .Select(i => parameters[i])
                    .ToArray();
                return this.RouteMethod.Invoke(serverObject, parametersInOrder).ToString();
            }
        }

        class RouteContext
        {
            private object serverObject = null;
            private RouteMethodRecord[] methods = null;

            public RouteContext(Type serverType, string prefix)
            {
                this.serverObject = serverType
                    .GetConstructor(new Type[] { })
                    .Invoke(new object[] { });

                methods = serverType
                    .GetMethods()
                    .Select(m => Tuple.Create(
                        m.GetCustomAttributes().Where(a => a is RouteAttribute).FirstOrDefault(),
                        m)
                        )
                    .Where(t => t.Item1 != null)
                    .Select(t => new RouteMethodRecord(t.Item1 as RouteAttribute, prefix, t.Item2))
                    .ToArray();
            }

            public string Route(string url, HttpListenerContext context)
            {
                foreach (var method in this.methods)
                {
                    var match = method.RoutePathRegex.Match(url);
                    if (match.Success)
                    {
                        if (method.RouteProperties.Method == context.Request.HttpMethod)
                        {
                            return method.Execute(match, this.serverObject);
                        }
                    }
                }
                return null;
            }
        }

        private static void DriveHttpListener(HttpListener httpListener, RouteContext routeContext)
        {
            httpListener.BeginGetContext((ar) =>
            {
                var context = httpListener.EndGetContext(ar);
                DriveHttpListener(httpListener, routeContext);

                string path = context.Request.Url.AbsolutePath.Trim().TrimEnd('/');
                var result = routeContext.Route(path, context);
                if (result == null)
                {
                    context.Response.StatusCode = 404;
                    context.Response.ContentLength64 = 0;
                }
                else
                {
                    context.Response.ContentEncoding = Encoding.UTF8;
                    byte[] buffer = Encoding.UTF8.GetBytes(result);
                    context.Response.ContentLength64 = buffer.Length;
                    context.Response.OutputStream.Write(buffer, 0, buffer.Length);
                }
                context.Response.Close();
            }, null);
        }

        public static void Run(Type serverType, string host, int port, string prefix)
        {
            HttpListener httpListener = new HttpListener();
            httpListener.Prefixes.Add(string.Format("http://{0}:{1}{2}/", host, port, prefix));
            httpListener.Start();
            DriveHttpListener(httpListener, new RouteContext(serverType, prefix));
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public abstract class RouteAttribute : Attribute
    {
        public string RoutePath { get; set; }
        public string Method { get; set; }

        public RouteAttribute(string method, string routePath)
        {
            this.Method = method;
            this.RoutePath = routePath;
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class GetAttribute : RouteAttribute
    {
        public GetAttribute(string routePath)
            : base("GET", routePath)
        {
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class PostAttribute : RouteAttribute
    {
        public PostAttribute(string routePath)
            : base("POST", routePath)
        {
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class PutAttribute : RouteAttribute
    {
        public PutAttribute(string routePath)
            : base("PUT", routePath)
        {
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class DeleteAttribute : RouteAttribute
    {
        public DeleteAttribute(string routePath)
            : base("DELETE", routePath)
        {
        }
    }
}
