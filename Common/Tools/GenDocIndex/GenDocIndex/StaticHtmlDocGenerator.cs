using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace GenDocIndex
{
    static class StaticHtmlDocGenerator
    {
        static string GetFileName(DocItem docItem, string outputFolder = "")
        {
            return outputFolder + docItem.UniqueId.Replace(':', '_').Replace('.', '_') + ".html";
        }

        static void GenerateText(TextWriter writer, string content)
        {
            writer.Write(content);
        }

        static void GenerateContent(TextWriter writer, DocItem docItem, DocItemValidationResult validationResult)
        {
            string currentColor = "000000";
            writer.Write("<p><font color=\"{0}\">", currentColor);
            foreach (var docEntity in docItem.Content.Entities)
            {
                switch (docEntity.Type)
                {
                    case DocEntityType.Bold:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<b>");
                                break;
                            case DocEntityState.Close:
                                writer.Write("</b>");
                                break;
                        }
                        break;
                    case DocEntityType.Header1:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<h1>");
                                break;
                            case DocEntityState.Close:
                                writer.Write("</h1>");
                                break;
                        }
                        break;
                    case DocEntityType.Header2:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<h2>");
                                break;
                            case DocEntityState.Close:
                                writer.Write("</h2>");
                                break;
                        }
                        break;
                    case DocEntityType.Header3:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<h3>");
                                break;
                            case DocEntityState.Close:
                                writer.Write("</h3>");
                                break;
                        }
                        break;
                    case DocEntityType.LinkId:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<a href=\"{0}\">", Path.GetFileName(GetFileName(validationResult.UniqueIdItemMap[docEntity.Argument])));
                                break;
                            case DocEntityState.Close:
                                writer.Write("</a>");
                                break;
                        }
                        break;
                    case DocEntityType.LinkSymbol:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<a href=\"{0}\">", Path.GetFileName(GetFileName(validationResult.MemberIdItemMap[docEntity.Argument])));
                                break;
                            case DocEntityState.Close:
                                writer.Write("</a>");
                                break;
                        }
                        break;
                    case DocEntityType.Table:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<table>");
                                break;
                            case DocEntityState.Close:
                                writer.Write("</table>");
                                break;
                        }
                        break;
                    case DocEntityType.RowHeader:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<tr>");
                                break;
                            case DocEntityState.Close:
                                writer.Write("</tr>");
                                break;
                        }
                        break;
                    case DocEntityType.Row:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<tr>");
                                break;
                            case DocEntityState.Close:
                                writer.Write("</tr>");
                                break;
                        }
                        break;
                    case DocEntityType.Col:
                        switch (docEntity.State)
                        {
                            case DocEntityState.Open:
                                writer.Write("<td>");
                                break;
                            case DocEntityState.Close:
                                writer.Write("</td>");
                                break;
                        }
                        break;
                    case DocEntityType.Code:
                        writer.Write("<table><tr><td>");
                        GenerateText(writer, docEntity.Argument);
                        writer.Write("</td></tr></table>");
                        break;
                    case DocEntityType.Slash:
                        GenerateText(writer, "/");
                        break;
                    case DocEntityType.Crlf:
                        writer.Write("<br/>");
                        break;
                    case DocEntityType.Para:
                        writer.Write("</font></p><p><font color=\"{0}\">", currentColor);
                        break;
                    case DocEntityType.Nop:
                        break;
                    case DocEntityType.Img:
                        writer.Write("<img src=\"{0}\"/>", docEntity.Argument);
                        break;
                    case DocEntityType.Color:
                        currentColor = docEntity.Argument;
                        writer.Write("</font><font color=\"{0}\">", currentColor);
                        break;
                    case DocEntityType.Text:
                        GenerateText(writer, docEntity.Argument);
                        break;
                }
            }
            writer.Write("</font></p>");
        }

        static void GenerateFile(DocItem docItem, string outputFolder, DocItemValidationResult validationResult)
        {
            string fileName = GetFileName(docItem, outputFolder);
            using (StreamWriter writer = new StreamWriter(new FileStream(fileName, FileMode.Create, FileAccess.Write), Encoding.UTF8))
            {
                writer.WriteLine("<html>");
                writer.WriteLine("<head><title>{0}</title></head>", docItem.Title);
                writer.WriteLine("<body>");
                GenerateContent(writer, docItem, validationResult);
                writer.WriteLine("</body>");
                writer.WriteLine("</html>");
            }
            GenerateFiles(docItem.SubItems, outputFolder, validationResult);
        }

        static void GenerateFiles(IEnumerable<DocItem> docItems, string outputFolder, DocItemValidationResult validationResult)
        {
            foreach (var item in docItems.OrderBy(i => i.Title))
            {
                GenerateFile(item, outputFolder, validationResult);
            }
        }

        public static void GenerateStaticHtmlDocument(DocItem[] rootItems, string outputFolder, DocItemValidationResult validationResult)
        {
            GenerateFiles(rootItems, outputFolder, validationResult);
        }
    }
}
