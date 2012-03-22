using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace GenXmlDocRef
{
    class DocItemWriterContext
    {
        public Dictionary<string, DocItem> UniqueIdItemMap { get; set; }
        public Dictionary<string, DocItem> MemberIdItemMap { get; set; }
    }

    abstract class DocItemWriter
    {
        protected abstract void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context);

        public void Write(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            writer.WriteLine("/title:{0}", docItem.Title);
            writer.WriteLine("/uniqueId:{0}", docItem.UniqueId);
            writer.WriteLine("/parentUniqueId:{0}", docItem.Parent.UniqueId);
            WriteContent(docItem, writer, context);
        }

        public static DocItemWriter CreateWriter(string name)
        {
            switch (name)
            {
                case "namespace": return new NamespaceDocItemWriter();
                case "enum": return new EnumDocItemWriter();
                case "type": return new TypeDocItemWriter();
                case "functionGroup": return new FunctionGroupDocItemWriter();
                case "function": return new FunctionDocItemWriter();
                default: throw new ArgumentException();
            }
        }
    }

    class NamespaceDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            writer.Write(docItem.Content);
        }
    }

    class EnumDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            writer.Write(docItem.Content);
        }
    }

    class TypeDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            writer.Write(docItem.Content);
        }
    }

    class FunctionGroupDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            writer.Write(docItem.Content);
        }
    }

    class FunctionDocItemWriter : DocItemWriter
    {
        protected override void WriteContent(DocItem docItem, TextWriter writer, DocItemWriterContext context)
        {
            writer.Write(docItem.Content);
        }
    }
}
