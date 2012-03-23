using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Linq;
using System.Text.RegularExpressions;

namespace GenXmlDocRef
{
    class DocItemWriterContext
    {
        public Dictionary<string, DocItem> UniqueIdItemMap { get; set; }
        public Dictionary<string, DocItem> MemberIdItemMap { get; set; }
    }

    abstract class DocItemWriter
    {
        public DocItem AssociatedDocItem { get; private set; }

        protected abstract void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context);

        public void Write(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            writer.WriteLine("/title:{0}", docItem.Title);
            writer.WriteLine("/uniqueId:{0}", docItem.UniqueId);
            writer.WriteLine("/parentUniqueId:{0}", docItem.Parent.UniqueId);
            writer.WriteLine("/content");
            writer.WriteLine("/+h1/{0}/-h1//para/", docItem.Title);
            WriteContent(docItem, writer, context);
            if (docItem.SubItems.Count > 0)
            {
                writer.WriteLine("/+b/Contents under this category:/-b//para/");
                foreach (var item in docItem.SubItems
                    .OrderBy(x => x.UniqueId.StartsWith("functionGroup:") ? "function:" + x.UniqueIdBody : x.UniqueId)
                    )
                {
                    writer.WriteLine("/nop/    /+linkid:{0}/{1}/-linkid//crlf/", item.UniqueId, item.Title);
                }
            }
        }

        public static DocItemWriter CreateWriter(string name, DocItem docItem)
        {
            switch (name)
            {
                case "namespace": return new NamespaceDocItemWriter() { AssociatedDocItem = docItem };
                case "enum": return new EnumDocItemWriter() { AssociatedDocItem = docItem };
                case "type": return new TypeDocItemWriter() { AssociatedDocItem = docItem };
                case "functionGroup": return new FunctionGroupDocItemWriter() { AssociatedDocItem = docItem };
                case "function": return new FunctionDocItemWriter() { AssociatedDocItem = docItem };
                default: throw new ArgumentException(string.Format("Cannot create DocItemWriter for {0}.", name));
            }
        }

        private static Regex symbolRegex = new Regex(@"\[(?<cref>[A-Z]+:[a-zA-Z0-9_.]+)\]");

        protected void WriteHyperlinkEnabledText(string text, TextWriter writer, DocItemWriterContext context)
        {
            int index = 0;
            while (true)
            {
                Match match = symbolRegex.Match(text, index);
                if (match.Success)
                {
                    writer.Write(text.Substring(index, match.Index - index));
                    index = match.Index + match.Value.Length;
                    string cref = match.Groups["cref"].Value;
                    WriteHyperlinkSymbol(cref, "", writer, context);
                }
                else
                {
                    break;
                }
            }
            writer.Write(text.Substring(index));
        }

        protected void WriteNode(XNode node, TextWriter writer, DocItemWriterContext context)
        {
            if (node is XText || node is XCData)
            {
                WriteHyperlinkEnabledText(node.ToString(), writer, context);
                return;
            }

            XElement element = node as XElement;
            if (element != null)
            {
                switch (element.Name.ToString())
                {
                    case "see":
                        {
                            string cref = element.Attribute("cref").Value;
                            string text = element.Value;
                            WriteHyperlinkSymbol(cref, text, writer, context);
                        }
                        return;
                }
            }

            throw new ArgumentException(string.Format("[{0}]: Don't know how to print {1}.", this.AssociatedDocItem.Name, node.GetType().Name));
        }

        protected void WriteNodes(IEnumerable<XNode> nodes, TextWriter writer, DocItemWriterContext context)
        {
            foreach (var node in nodes)
            {
                WriteNode(node, writer, context);
            }
        }

        protected void WriteSummary(XElement memberElement, TextWriter writer, DocItemWriterContext context)
        {
            XElement summary = memberElement.Element("summary");
            if (summary != null)
            {
                WriteNodes(summary.Nodes(), writer, context);
            }
        }

        protected XElement WriteSummaryIfDocumentExists(XElement containerElement, TextWriter writer, DocItemWriterContext context)
        {
            XElement documentElement = containerElement.Element("document");
            if (documentElement != null)
            {
                XElement memberElement = documentElement.Element("member");
                if (memberElement != null)
                {
                    WriteSummary(memberElement, writer, context);
                    return memberElement;
                }
            }
            return null;
        }

        protected XElement WriteSummaryInDocItem(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            if (docItem.Document != null)
            {
                XElement member = docItem.Document.Element("member");
                if (member != null)
                {
                    WriteSummary(member, writer, context);
                    writer.WriteLine();
                    return member;
                }
            }
            return null;
        }

        protected void WriteHyperlinkType(string type, TextWriter writer, DocItemWriterContext context)
        {
            var docItem = context
                .MemberIdItemMap
                .Where(p => p.Key.StartsWith("T:"))
                .Where(p => p.Value.Content.Attribute("fullName").Value == type)
                .Select(p => p.Value)
                .FirstOrDefault();
            if (docItem == null)
            {
                writer.Write(type);
            }
            else
            {
                writer.Write("/+linkid:{0}/{1}/-linkid/", docItem.UniqueId, GetReadableName(docItem));
            }
        }

        protected void WriteHyperlinkSymbol(string cref, string text, TextWriter writer, DocItemWriterContext context)
        {
            if (text != "")
            {
                writer.Write("/+linksymbol:{0}/{1}/-linksymbol/", cref, text);
            }
            else
            {
                DocItem docItem = null;
                if (!context.MemberIdItemMap.TryGetValue(cref, out docItem))
                {
                    if (cref.StartsWith("M:"))
                    {
                        var docItems = context
                            .MemberIdItemMap
                            .Where(p => p.Key.StartsWith(cref + "("))
                            .Select(p => p.Value)
                            .ToArray();
                        if (docItems.Length == 1)
                        {
                            docItem = docItems[0];
                        }
                    }
                }

                if (docItem != null)
                {
                    writer.Write("/+linksymbol:{0}/{1}/-linksymbol/", cref, GetReadableName(docItem));
                }
                else
                {
                    throw new ArgumentException(string.Format("[{0}]: Cannot generate hyperlink text for symbol {1}", this.AssociatedDocItem.Name, cref));
                }
            }
        }

        protected string GetReadableName(DocItem item)
        {
            string name = item.Content.Attribute("name").Value;
            if (item.UniqueId.StartsWith("enum:") && name == "Type" && item.Parent.UniqueId.StartsWith("namespace:"))
            {
                name = item.Parent.Content.Attribute("name").Value + "::" + name;
            }
            else
            {
                while (true)
                {
                    item = item.Parent;
                    if (item == null || !item.UniqueId.StartsWith("type:"))
                    {
                        break;
                    }
                    else
                    {
                        name = item.Content.Attribute("name").Value + "::" + name;
                    }
                }
            }
            return name;
        }
    }

    class NamespaceDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
        }
    }

    class EnumDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            WriteSummaryInDocItem(docItem, writer, context);
            writer.WriteLine("/para/");
            writer.WriteLine("/+b/Members:/-b//crlf/");
            writer.WriteLine("/+table/");
            writer.WriteLine("    /+rowheader//+col/Name/-col//+col/Description/-col//-rowheader/");
            foreach (var fieldElement in docItem.Content.Elements("field"))
            {
                writer.WriteLine("    /+row/");
                string name = fieldElement.Attribute("name").Value;
                writer.WriteLine("        /+col/{0}/-col/", name);
                writer.Write("        /+col/");
                WriteSummaryIfDocumentExists(fieldElement, writer, context);
                writer.WriteLine("/-col/");
                writer.WriteLine("    /-row/");
            }
            writer.WriteLine("/-table//para/");
        }
    }

    class TypeDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            WriteSummaryInDocItem(docItem, writer, context);
            writer.WriteLine("/para/");
            var baseTypes = docItem.Content.Elements("baseType").ToArray();
            if (baseTypes.Length > 0)
            {
                writer.WriteLine("/+b/Base Types:/-b//crlf/");
                foreach (var baseType in baseTypes)
                {
                    writer.Write("/nop/    ");
                    WriteHyperlinkType(baseType.Attribute("fullName").Value, writer, context);
                    writer.WriteLine("/crlf/");
                }
            }
        }
    }

    class FunctionGroupDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
        }
    }

    class FunctionDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            XElement member = WriteSummaryInDocItem(docItem, writer, context);
            writer.WriteLine("/para/");
            string returnType = docItem.Content.Element("returnType").Value;
            XElement returnElement = member.Element("returns");
            var parameterTypes = docItem.Content.Elements("parameterType").Select(x => x.Value).ToArray();
            var parameters = docItem.Content.Elements("parameterPair")
                .ToDictionary(x => x.Attribute("name").Value, x => x.Value);
            var parameterElements = member.Elements("param").ToArray();
            foreach (var name in parameterElements.Select((x, i) => Tuple.Create(x.Attribute("name").Value, i)))
            {
                if (!parameters.ContainsKey(name.Item1))
                {
                    parameters.Add(name.Item1, parameterTypes[name.Item2]);
                }
            }

            writer.WriteLine("/+b/Signature:/-b//crlf/");
            writer.WriteLine("/+table/");
            writer.WriteLine("    /+rowheader//+col/Parameter or Return Value/-col//+col/Type/-col//+col/Description/-col//-rowheader/");
            {
                writer.WriteLine("    /+row/");
                writer.WriteLine("        /+col/return value/-col/");
                writer.Write("        /+col/");
                WriteHyperlinkType(returnType, writer, context);
                writer.WriteLine("/-col/");
                writer.Write("        /+col/");
                if (returnElement != null)
                {
                    WriteNodes(returnElement.Nodes(), writer, context);
                }
                writer.WriteLine("/-col/");
                writer.WriteLine("    /-row/");
            }
            foreach (var p in parameterElements)
            {
                string name = p.Attribute("name").Value;
                string type = parameters[name];
                var description = p.Nodes();

                writer.WriteLine("    /+row/");
                writer.WriteLine("        /+col/{0}/-col/", name);
                writer.Write("        /+col/");
                WriteHyperlinkType(type, writer, context);
                writer.WriteLine("/-col/");
                writer.Write("        /+col/");
                WriteNodes(description, writer, context);
                writer.WriteLine("/-col/");
                writer.WriteLine("    /-row/");
            }
            writer.WriteLine("/-table//para/");
        }
    }
}
