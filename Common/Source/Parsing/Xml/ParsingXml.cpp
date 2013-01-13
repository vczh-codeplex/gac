#include "ParsingXml.h"
#include "..\..\Stream\MemoryStream.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
			using namespace stream;
			using namespace collections;
			using namespace regex;

/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

			void XmlMergeTextFragment(vl::collections::List<vl::Ptr<XmlNode>>& value, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				vint begin=-1;
				vint end=-1;
				for(vint i=value.Count()-1;i>=-1;i--)
				{
					if(i==-1)
					{
						if(end!=-1) begin=0;
					}
					else if(value[i].Cast<XmlText>())
					{
						if(end==-1) end=i;
					}
					else
					{
						if(end!=-1) begin=i+1;
					}
					if(begin!=-1 && end!=-1)
					{
						vint tokenBegin=value[begin].Cast<XmlText>()->content.tokenIndex;
						vint tokenEnd=value[end].Cast<XmlText>()->content.tokenIndex;
						while(tokenBegin>0)
						{
							if(tokens.Get(tokenBegin).token==XmlParserTokenIndex::SPACE || tokens.Get(tokenBegin).token==-1)
							{
								tokenBegin--;
							}
							else
							{
								break;
							}
						}
						while(tokenEnd<tokens.Count()-1)
						{
							if(tokens.Get(tokenEnd).token==XmlParserTokenIndex::SPACE || tokens.Get(tokenEnd).token==-1)
							{
								tokenEnd++;
							}
							else
							{
								break;
							}
						}

						const RegexToken& beginToken=tokens.Get(tokenBegin);
						const RegexToken& endToken=tokens.Get(tokenEnd);
						const wchar_t* textBegin=beginToken.reading;
						const wchar_t* textEnd=endToken.reading+endToken.length;
						WString text(textBegin, textEnd-textBegin);
						ParsingTextRange range(&beginToken, &endToken);

						Ptr<XmlText> xmlText=new XmlText;
						xmlText->codeRange=range;
						xmlText->content.codeRange=range;
						xmlText->content.value=XmlUnescapeValue(text);

						value.RemoveRange(begin, end-begin+1);
						value.Insert(begin, xmlText);

						begin=-1;
						end=-1;
					}
				}
			}

			void XmlUnescapeAttributeValue(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				value.value=XmlUnescapeValue(value.value.Sub(1, value.value.Length()-2));
			}

			void XmlUnescapeCData(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				value.value=XmlUnescapeCData(value.value);
			}

			void XmlUnescapeComment(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				value.value=XmlUnescapeComment(value.value);
			}

/***********************************************************************
JsonPrintVisitor
***********************************************************************/

			class XmlPrintVisitor : public Object, public XmlNode::IVisitor
			{
			public:
				TextWriter&					writer;

				XmlPrintVisitor(TextWriter& _writer)
					:writer(_writer)
				{
				}

				void Visit(XmlText* node)
				{
					XmlEscapeValue(node->content.value);
				}

				void Visit(XmlCData* node)
				{
					XmlEscapeCData(node->content.value);
				}

				void Visit(XmlAttribute* node)
				{
					writer.WriteString(node->name.value);
					writer.WriteString(L"=\"");
					XmlEscapeValue(node->value.value);
					writer.WriteString(L"\"");
				}

				void Visit(XmlComment* node)
				{
					XmlEscapeComment(node->content.value);
				}

				void Visit(XmlElement* node)
				{
					writer.WriteChar(L'<');
					writer.WriteString(node->name.value);
					FOREACH(Ptr<XmlAttribute>, att, node->attributes)
					{
						writer.WriteChar(L' ');
						att->Accept(this);
					}
					if(node->subNodes.Count()==0)
					{
						writer.WriteString(L"/>");
					}
					else
					{
						writer.WriteChar(L'>');
						FOREACH(Ptr<XmlNode>, subNode, node->subNodes)
						{
							subNode->Accept(this);
						}
						writer.WriteString(L"</");
						writer.WriteString(node->name.value);
						writer.WriteChar(L'>');
					}
				}

				void Visit(XmlInstruction* node)
				{
					writer.WriteString(L"<?");
					writer.WriteString(node->name.value);
					FOREACH(Ptr<XmlAttribute>, att, node->attributes)
					{
						writer.WriteChar(L' ');
						att->Accept(this);
					}
					writer.WriteString(L"?>");
				}

				void Visit(XmlDocument* node)
				{
					FOREACH(Ptr<XmlInstruction>, ins, node->instructions)
					{
						ins->Accept(this);
					}
					FOREACH(Ptr<XmlComment>, comment, node->comments)
					{
						comment->Accept(this);
					}
					node->rootElement->Accept(this);
				}
			};

/***********************************************************************
API
***********************************************************************/

			void XmlEscapeValue(const WString& value, stream::TextWriter& writer)
			{
				const wchar_t* reading=value.Buffer();
				while(wchar_t c=*reading++)
				{
					switch(c)
					{
					case L'<':
						writer.WriteString(L"&lt;");
						break;
					case L'>':
						writer.WriteString(L"&gt;");
						break;
					case L'&':
						writer.WriteString(L"&amp;");
						break;
					case L'\'':
						writer.WriteString(L"&apos;");
						break;
					case L'\"':
						writer.WriteString(L"&quot;");
						break;
					default:
						writer.WriteChar(c);
					}
				}
			}

			void XmlUnescapeValue(const WString& value, stream::TextWriter& writer)
			{
				const wchar_t* reading=value.Buffer();
				while(*reading)
				{
					if(wcsncmp(reading, L"&lt;", 4)==0)
					{
						writer.WriteChar(L'<');
						reading+=4;
					}
					else if(wcsncmp(reading, L"&gt;", 4)==0)
					{
						writer.WriteChar(L'>');
						reading+=4;
					}
					else if(wcsncmp(reading, L"&amp;", 5)==0)
					{
						writer.WriteChar(L'&');
						reading+=5;
					}
					else if(wcsncmp(reading, L"&apos;", 6)==0)
					{
						writer.WriteChar(L'\'');
						reading+=6;
					}
					else if(wcsncmp(reading, L"&quot;", 6)==0)
					{
						writer.WriteChar(L'\"');
						reading+=6;
					}
					else
					{
						writer.WriteChar(*reading++);
					}
				}
			}

			WString XmlEscapeValue(const WString& value)
			{
				WString result;
				const wchar_t* reading=value.Buffer();
				while(wchar_t c=*reading++)
				{
					switch(c)
					{
					case L'<':
						result+=L"&lt;";
						break;
					case L'>':
						result+=L"&gt;";
						break;
					case L'&':
						result+=L"&amp;";
						break;
					case L'\'':
						result+=L"&apos;";
						break;
					case L'\"':
						result+=L"&quot;";
						break;
					default:
						result+=c;
					}
				}
				return result;
			}

			WString XmlUnescapeValue(const WString& value)
			{
				WString result;
				const wchar_t* reading=value.Buffer();
				while(*reading)
				{
					if(wcsncmp(reading, L"&lt;", 4)==0)
					{
						result+=L'<';
						reading+=4;
					}
					else if(wcsncmp(reading, L"&gt;", 4)==0)
					{
						result+=L'>';
						reading+=4;
					}
					else if(wcsncmp(reading, L"&amp;", 5)==0)
					{
						result+=L'&';
						reading+=5;
					}
					else if(wcsncmp(reading, L"&apos;", 6)==0)
					{
						result+=L'\'';
						reading+=6;
					}
					else if(wcsncmp(reading, L"&quot;", 6)==0)
					{
						result+=L'\"';
						reading+=6;
					}
					else
					{
						result+=*reading++;
					}
				}
				return result;
			}

			WString XmlEscapeCData(const WString& value)
			{
				return L"<![CDATA["+value+L"]]>";
			}

			WString XmlUnescapeCData(const WString& value)
			{
				return value.Sub(9, value.Length()-12);
			}

			WString XmlEscapeComment(const WString& value)
			{
				return L"<!--"+value+L"-->";
			}

			WString XmlUnescapeComment(const WString& value)
			{
				return value.Sub(4, value.Length()-7);
			}

			void XmlPrint(Ptr<XmlNode> node, stream::TextWriter& writer)
			{
				XmlPrintVisitor visitor(writer);
				node->Accept(&visitor);
			}

			WString XmlToString(Ptr<XmlNode> node)
			{
				MemoryStream stream;
				{
					StreamWriter writer(stream);
					XmlPrint(node, writer);
				}
				stream.SeekFromBegin(0);
				{
					StreamReader reader(stream);
					return reader.ReadToEnd();
				}
			}
		}
	}
}
