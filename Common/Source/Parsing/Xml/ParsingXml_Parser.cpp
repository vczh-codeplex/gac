#include "ParsingXml_Parser.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
/***********************************************************************
ParserText
***********************************************************************/

const wchar_t parserTextBuffer[] = 
L"\r\n"L""
L"\r\n"L"class Node"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Text : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken content;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class CData : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken content (XmlUnescapeCData);"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Attribute : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken name\t\t\t\t\t\t\t\t\t@Color(\"AttName\");"
L"\r\n"L"\ttoken value (XmlUnescapeAttributeValue)\t\t@Color(\"AttValue\");"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Comment : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken content (XmlUnescapeComment);"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Element : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken name\t\t\t\t\t\t\t\t\t@Color(\"TagName\");"
L"\r\n"L"\ttoken closingName\t\t\t\t\t\t\t@Color(\"TagName\");"
L"\r\n"L"\tAttribute[] attributes;"
L"\r\n"L"\tNode[] subNodes (XmlMergeTextFragment);"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Instruction : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken name\t\t\t\t\t\t\t\t\t@Color(\"TagName\");"
L"\r\n"L"\tAttribute[] attributes;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Document : Node"
L"\r\n"L"{"
L"\r\n"L"\tNode[] prologs;"
L"\r\n"L"\tElement rootElement;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"token INSTRUCTION_OPEN = \"/</?\"\t\t\t@Color(\"Boundary\");"
L"\r\n"L"token INSTRUCTION_CLOSE = \"/?/>\"\t\t@Color(\"Boundary\");"
L"\r\n"L"token COMPLEX_ELEMENT_OPEN = \"/<//\"\t\t@Color(\"Boundary\");"
L"\r\n"L"token SINGLE_ELEMENT_CLOSE = \"///>\"\t\t@Color(\"Boundary\");"
L"\r\n"L"token ELEMENT_OPEN = \"/<\"\t\t\t\t@Color(\"Boundary\");"
L"\r\n"L"token ELEMENT_CLOSE = \"/>\"\t\t\t\t@Color(\"Boundary\");"
L"\r\n"L"token EQUAL = \"/=\";"
L"\r\n"L""
L"\r\n"L"token NAME = \"[a-zA-Z0-9:._/-]+\"\t\t\t\t\t\t\t\t@ContextColor();"
L"\r\n"L"token ATTVALUE = \"\"\"[^<>\"\"]*\"\"|\'[^<>\']*\'\"\t\t\t\t\t\t@ContextColor();"
L"\r\n"L"token COMMENT = \"/</!--([^/->]|-[^/->]|--[^>])*--/>\"\t\t\t@Color(\"Comment\");"
L"\r\n"L"token CDATA = \"/</!/[CDATA/[([^/]]|/][^/]]|/]/][^>])*/]/]/>\";"
L"\r\n"L"token TEXT = \"([^<>=\"\"\' /r/n/ta-zA-Z0-9:._/-])+|\"\"|\'\";"
L"\r\n"L""
L"\r\n"L"discardtoken SPACE = \"/s+\";"
L"\r\n"L""
L"\r\n"L"rule Attribute XAttribute = NAME:name \"=\" ATTVALUE:value as Attribute;"
L"\r\n"L"rule Text XText = (NAME:content | EQUAL:content | ATTVALUE:content | TEXT:content) as Text;"
L"\r\n"L"rule CData XCData = CDATA:content as CData;"
L"\r\n"L"rule Comment XComment = COMMENT:content as Comment;"
L"\r\n"L"rule Element XElement = \"<\" NAME:name {XAttribute:attributes} (\"/>\" | \">\" {XSubNode:subNodes} \"</\" NAME:closingName \">\") as Element;"
L"\r\n"L"rule Node XSubNode = !XText | !XCData | !XComment | !XElement;"
L"\r\n"L"rule Instruction XInstruction = \"<?\" NAME:name {XAttribute:attributes} \"?>\" as Instruction;"
L"\r\n"L"rule Document XDocument = {XInstruction:prologs | XComment:prologs} XElement:rootElement as Document;"
;

			vl::WString XmlGetParserTextBuffer()
			{
				return parserTextBuffer;
			}

/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

			extern void XmlMergeTextFragment(vl::collections::List<vl::Ptr<XmlNode>>& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeAttributeValue(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeCData(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeComment(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

			class XmlTreeConverter : public vl::parsing::ParsingTreeConverter
			{
			public:
				using vl::parsing::ParsingTreeConverter::SetMember;

				void Fill(vl::Ptr<XmlNode> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
				}

				void Fill(vl::Ptr<XmlText> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->content, obj->GetMember(L"content"), tokens);
				}

				void Fill(vl::Ptr<XmlCData> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					if(SetMember(tree->content, obj->GetMember(L"content"), tokens))
					{
						XmlUnescapeCData(tree->content, tokens);
					}
				}

				void Fill(vl::Ptr<XmlAttribute> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->name, obj->GetMember(L"name"), tokens);
					if(SetMember(tree->value, obj->GetMember(L"value"), tokens))
					{
						XmlUnescapeAttributeValue(tree->value, tokens);
					}
				}

				void Fill(vl::Ptr<XmlComment> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					if(SetMember(tree->content, obj->GetMember(L"content"), tokens))
					{
						XmlUnescapeComment(tree->content, tokens);
					}
				}

				void Fill(vl::Ptr<XmlElement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->name, obj->GetMember(L"name"), tokens);
					SetMember(tree->closingName, obj->GetMember(L"closingName"), tokens);
					SetMember(tree->attributes, obj->GetMember(L"attributes"), tokens);
					if(SetMember(tree->subNodes, obj->GetMember(L"subNodes"), tokens))
					{
						XmlMergeTextFragment(tree->subNodes, tokens);
					}
				}

				void Fill(vl::Ptr<XmlInstruction> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->name, obj->GetMember(L"name"), tokens);
					SetMember(tree->attributes, obj->GetMember(L"attributes"), tokens);
				}

				void Fill(vl::Ptr<XmlDocument> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->prologs, obj->GetMember(L"prologs"), tokens);
					SetMember(tree->rootElement, obj->GetMember(L"rootElement"), tokens);
				}

				vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
				{
					if(obj->GetType()==L"Text")
					{
						vl::Ptr<XmlText> tree = new XmlText;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"CData")
					{
						vl::Ptr<XmlCData> tree = new XmlCData;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Attribute")
					{
						vl::Ptr<XmlAttribute> tree = new XmlAttribute;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Comment")
					{
						vl::Ptr<XmlComment> tree = new XmlComment;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Element")
					{
						vl::Ptr<XmlElement> tree = new XmlElement;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Instruction")
					{
						vl::Ptr<XmlInstruction> tree = new XmlInstruction;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Document")
					{
						vl::Ptr<XmlDocument> tree = new XmlDocument;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else 
						return 0;
				}
			};

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> XmlConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				XmlTreeConverter converter;
				vl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;
				converter.SetMember(tree, node, tokens);
				return tree;
			}

/***********************************************************************
Parsing Tree Conversion Implementation
***********************************************************************/

			vl::Ptr<XmlText> XmlText::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlText>();
			}

			vl::Ptr<XmlCData> XmlCData::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlCData>();
			}

			vl::Ptr<XmlAttribute> XmlAttribute::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlAttribute>();
			}

			vl::Ptr<XmlComment> XmlComment::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlComment>();
			}

			vl::Ptr<XmlElement> XmlElement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlElement>();
			}

			vl::Ptr<XmlInstruction> XmlInstruction::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlInstruction>();
			}

			vl::Ptr<XmlDocument> XmlDocument::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlDocument>();
			}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

			void XmlText::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlCData::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlAttribute::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlComment::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlElement::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlInstruction::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlDocument::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

/***********************************************************************
Parser Function
***********************************************************************/

			vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XDocument");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				return node;
			}

			vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				return XmlParseDocumentAsParsingTreeNode(input, table, errors);
			}

			vl::Ptr<XmlDocument> XmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XDocument");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				if(node && errors.Count()==0)
				{
					return XmlConvertParsingTreeNode(node, state.GetTokens()).Cast<XmlDocument>();
				}
				return 0;
			}

			vl::Ptr<XmlDocument> XmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				return XmlParseDocument(input, table, errors);
			}

			vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XElement");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				return node;
			}

			vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				return XmlParseElementAsParsingTreeNode(input, table, errors);
			}

			vl::Ptr<XmlElement> XmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XElement");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				if(node && errors.Count()==0)
				{
					return XmlConvertParsingTreeNode(node, state.GetTokens()).Cast<XmlElement>();
				}
				return 0;
			}

			vl::Ptr<XmlElement> XmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				return XmlParseElement(input, table, errors);
			}

/***********************************************************************
Table Generation
***********************************************************************/

			vl::Ptr<vl::parsing::tabling::ParsingTable> XmlLoadTable()
			{
			    vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateBootstrapStrictParser();
			    vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			    vl::Ptr<vl::parsing::ParsingTreeNode> definitionNode=parser->Parse(parserTextBuffer, L"ParserDecl", errors);
			    vl::Ptr<vl::parsing::definitions::ParsingDefinition> definition=vl::parsing::definitions::DeserializeDefinition(definitionNode);
			    vl::Ptr<vl::parsing::tabling::ParsingTable> table=vl::parsing::analyzing::GenerateTable(definition, false, errors);
			    return table;
			}

		}
	}
}
