#include "Test.h"

namespace vczh
{
	namespace whoknows
	{
		namespace xmlutility
		{
/***********************************************************************
ParserText
***********************************************************************/

const wchar_t parserTextBuffer[] = 
L"\r\n"L""
L"\r\n"L"class Node ambiguous(AmbiguousNode)"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class AmbiguousNode : Node"
L"\r\n"L"{"
L"\r\n"L"\tNode[] items;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Text : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken content (XmlUnescapeTextFragment);"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class CData : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken content (XmlUnescapeCData);"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Attribute : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken name;"
L"\r\n"L"\ttoken value (XmlUnescapeAttributeValue);"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Comment : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken content (XmlUnescapeComment);"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Element : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken name;"
L"\r\n"L"\ttoken closingName;"
L"\r\n"L"\tAttribute[] attributes;"
L"\r\n"L"\tNode[] subNodes (XmlMergeTextFragment);"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Instruction : Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken name;"
L"\r\n"L"\tAttribute[] attributes;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Document : Node"
L"\r\n"L"{"
L"\r\n"L"\tInstruction[] instructions;"
L"\r\n"L"\tComment[] comments;"
L"\r\n"L"\tElement rootElement;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"enum DirectEnum"
L"\r\n"L"{"
L"\r\n"L"\tA,B,C,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FirstClass"
L"\r\n"L"{"
L"\r\n"L"\tenum IndirectEnum"
L"\r\n"L"\t{"
L"\r\n"L"\t\tD,E,F,"
L"\r\n"L"\t}"
L"\r\n"L"\t"
L"\r\n"L"\tclass SecondClass"
L"\r\n"L"\t{"
L"\r\n"L"\t\tclass ThirdClass"
L"\r\n"L"\t\t{"
L"\r\n"L"\t\t\ttoken field1;"
L"\r\n"L"\t\t}"
L"\r\n"L"\t}"
L"\r\n"L""
L"\r\n"L"\tclass SecondClassChild : SecondClass"
L"\r\n"L"\t{"
L"\r\n"L"\t\ttoken field3;"
L"\r\n"L"\t}"
L"\r\n"L""
L"\r\n"L"\ttoken field4;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FirstClassChild : FirstClass"
L"\r\n"L"{"
L"\r\n"L"\ttoken field2;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ThirdClassChild : FirstClass.SecondClass.ThirdClass"
L"\r\n"L"{"
L"\r\n"L"\ttoken field5;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"token INSTRUCTION_OPEN = \"/</?\";"
L"\r\n"L"token INSTRUCTION_CLOSE = \"/?/>\";"
L"\r\n"L"token COMPLEX_ELEMENT_OPEN = \"/<//\";"
L"\r\n"L"token SINGLE_ELEMENT_CLOSE = \"///>\";"
L"\r\n"L"token ELEMENT_OPEN = \"/<\";"
L"\r\n"L"token ELEMENT_CLOSE = \"/>\";"
L"\r\n"L"token EQUAL = \"/=\";"
L"\r\n"L""
L"\r\n"L"token NAME = \"[a-zA-Z0-9:_/-]+\";"
L"\r\n"L"token ATTVALUE = \"\"\"([^\"\"&]|&/l+;)*\"\"\";"
L"\r\n"L"token COMMENT = \"/</!--[^>]*/>\";"
L"\r\n"L"token CDATA = \"/</!/[CDATA/[([^/]]|/][^/]]|/]/][^>])*/]/]/>\";"
L"\r\n"L"token TEXT = \"([^<>\"\"& /r/n/ta-zA-Z0-9:_/-]|&/l+;)+\";"
L"\r\n"L""
L"\r\n"L"discardtoken SPACE = \"/s+\";"
L"\r\n"L""
L"\r\n"L"rule Attribute XAttribute = NAME:name \"=\" ATTVALUE:value as Attribute;"
L"\r\n"L"rule Text XText = (NAME:content | EQUAL:content | TEXT:content) as Text;"
L"\r\n"L"rule CData XCData = CDATA:content as CData;"
L"\r\n"L"rule Comment XComment = COMMENT:content as Comment;"
L"\r\n"L"rule Element XElement = \"<\" NAME:name {XAttribute:attributes} (\"/>\" | \">\" {XSubNode:subNodes} \"</\" NAME:closingName \">\") as Element;"
L"\r\n"L"rule Node XSubNode = !XText | !XCData | !XComment | !XElement;"
L"\r\n"L"rule Instruction XInstruction = \"<?\" NAME:name {XAttribute:attributes} \"?>\" as Instruction;"
L"\r\n"L"rule Document XDocument = {XInstruction:instructions | XComment:comments} XElement:rootElement as Document;"
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
			extern void XmlUnescapeTextFragment(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

			class XmlTreeConverter : public vl::parsing::ParsingTreeConverter
			{
			public:
				using vl::parsing::ParsingTreeConverter::SetMember;

				bool SetMember(XmlDirectEnum& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
				{
					vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
					if(token)
					{
						if(token->GetValue()==L"A") { member=XmlDirectEnum::A; return true; }
						else if(token->GetValue()==L"B") { member=XmlDirectEnum::B; return true; }
						else if(token->GetValue()==L"C") { member=XmlDirectEnum::C; return true; }
						else { member=XmlDirectEnum::A; return false; }
					}
					member=XmlDirectEnum::A;
					return false;
				}

				bool SetMember(XmlFirstClass::XmlIndirectEnum& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
				{
					vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
					if(token)
					{
						if(token->GetValue()==L"D") { member=XmlFirstClass::XmlIndirectEnum::D; return true; }
						else if(token->GetValue()==L"E") { member=XmlFirstClass::XmlIndirectEnum::E; return true; }
						else if(token->GetValue()==L"F") { member=XmlFirstClass::XmlIndirectEnum::F; return true; }
						else { member=XmlFirstClass::XmlIndirectEnum::D; return false; }
					}
					member=XmlFirstClass::XmlIndirectEnum::D;
					return false;
				}

				void Fill(vl::Ptr<XmlNode> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
				}

				void Fill(vl::Ptr<XmlAmbiguousNode> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->items, obj->GetMember(L"items"), tokens);
				}

				void Fill(vl::Ptr<XmlText> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					if(SetMember(tree->content, obj->GetMember(L"content"), tokens))
					{
						XmlUnescapeTextFragment(tree->content, tokens);
					}
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
					SetMember(tree->instructions, obj->GetMember(L"instructions"), tokens);
					SetMember(tree->comments, obj->GetMember(L"comments"), tokens);
					SetMember(tree->rootElement, obj->GetMember(L"rootElement"), tokens);
				}

				void Fill(vl::Ptr<XmlFirstClass> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field4, obj->GetMember(L"field4"), tokens);
				}

				void Fill(vl::Ptr<XmlFirstClass::XmlSecondClass> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
				}

				void Fill(vl::Ptr<XmlFirstClass::XmlSecondClass::XmlThirdClass> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field1, obj->GetMember(L"field1"), tokens);
				}

				void Fill(vl::Ptr<XmlFirstClass::XmlSecondClassChild> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field3, obj->GetMember(L"field3"), tokens);
				}

				void Fill(vl::Ptr<XmlFirstClassChild> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field2, obj->GetMember(L"field2"), tokens);
				}

				void Fill(vl::Ptr<XmlThirdClassChild> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field5, obj->GetMember(L"field5"), tokens);
				}

				vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
				{
					if(obj->GetType()==L"AmbiguousNode")
					{
						vl::Ptr<XmlAmbiguousNode> tree = new XmlAmbiguousNode;
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
					else if(obj->GetType()==L"FirstClassChild")
					{
						vl::Ptr<XmlFirstClassChild> tree = new XmlFirstClassChild;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlFirstClass>(), obj, tokens);
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
					else if(obj->GetType()==L"ThirdClassChild")
					{
						vl::Ptr<XmlThirdClassChild> tree = new XmlThirdClassChild;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlFirstClass::XmlSecondClass::XmlThirdClass>(), obj, tokens);
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
					else if(obj->GetType()==L"Instruction")
					{
						vl::Ptr<XmlInstruction> tree = new XmlInstruction;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"FirstClass.SecondClassChild")
					{
						vl::Ptr<XmlFirstClass::XmlSecondClassChild> tree = new XmlFirstClass::XmlSecondClassChild;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlFirstClass::XmlSecondClass>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Text")
					{
						vl::Ptr<XmlText> tree = new XmlText;
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

			vl::Ptr<XmlAmbiguousNode> XmlAmbiguousNode::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlAmbiguousNode>();
			}

			vl::Ptr<XmlCData> XmlCData::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlCData>();
			}

			vl::Ptr<XmlComment> XmlComment::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlComment>();
			}

			vl::Ptr<XmlElement> XmlElement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlElement>();
			}

			vl::Ptr<XmlFirstClassChild> XmlFirstClassChild::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlFirstClassChild>();
			}

			vl::Ptr<XmlAttribute> XmlAttribute::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlAttribute>();
			}

			vl::Ptr<XmlThirdClassChild> XmlThirdClassChild::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlThirdClassChild>();
			}

			vl::Ptr<XmlDocument> XmlDocument::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlDocument>();
			}

			vl::Ptr<XmlInstruction> XmlInstruction::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlInstruction>();
			}

			vl::Ptr<XmlFirstClass::XmlSecondClassChild> XmlFirstClass::XmlSecondClassChild::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlFirstClass::XmlSecondClassChild>();
			}

			vl::Ptr<XmlText> XmlText::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlText>();
			}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

			void XmlAmbiguousNode::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

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

			void XmlFirstClass::XmlSecondClassChild::Accept(XmlFirstClass::XmlSecondClass::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlFirstClassChild::Accept(XmlFirstClass::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlThirdClassChild::Accept(XmlFirstClass::XmlSecondClass::XmlThirdClass::IVisitor* visitor)
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
namespace vl
{
	namespace reflection
	{
		namespace description
		{
#ifndef VCZH_DEBUG_NO_REFLECTION
			using namespace vczh::whoknows::xmlutility;

			IMPL_TYPE_INFO_RENAME(XmlAmbiguousNode, System::XmlAmbiguousNode)
			IMPL_TYPE_INFO_RENAME(XmlNode, System::XmlNode)
			IMPL_TYPE_INFO_RENAME(XmlCData, System::XmlCData)
			IMPL_TYPE_INFO_RENAME(XmlComment, System::XmlComment)
			IMPL_TYPE_INFO_RENAME(XmlElement, System::XmlElement)
			IMPL_TYPE_INFO_RENAME(XmlFirstClassChild, System::XmlFirstClassChild)
			IMPL_TYPE_INFO_RENAME(XmlAttribute, System::XmlAttribute)
			IMPL_TYPE_INFO_RENAME(XmlThirdClassChild, System::XmlThirdClassChild)
			IMPL_TYPE_INFO_RENAME(XmlDocument, System::XmlDocument)
			IMPL_TYPE_INFO_RENAME(XmlInstruction, System::XmlInstruction)
			IMPL_TYPE_INFO_RENAME(XmlFirstClass::XmlSecondClass::XmlThirdClass, System::XmlFirstClass::XmlSecondClass::XmlThirdClass)
			IMPL_TYPE_INFO_RENAME(XmlFirstClass, System::XmlFirstClass)
			IMPL_TYPE_INFO_RENAME(XmlFirstClass::XmlIndirectEnum, System::XmlFirstClass::XmlIndirectEnum)
			IMPL_TYPE_INFO_RENAME(XmlDirectEnum, System::XmlDirectEnum)
			IMPL_TYPE_INFO_RENAME(XmlFirstClass::XmlSecondClass, System::XmlFirstClass::XmlSecondClass)
			IMPL_TYPE_INFO_RENAME(XmlFirstClass::XmlSecondClassChild, System::XmlFirstClass::XmlSecondClassChild)
			IMPL_TYPE_INFO_RENAME(XmlText, System::XmlText)

			BEGIN_CLASS_MEMBER(XmlAmbiguousNode)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlAmbiguousNode>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(items)
			END_CLASS_MEMBER(XmlAmbiguousNode)

			BEGIN_CLASS_MEMBER(XmlNode)

			END_CLASS_MEMBER(XmlNode)

			BEGIN_CLASS_MEMBER(XmlCData)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlCData>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_content, NO_PARAMETER, vl::WString(XmlCData::*)(), [](XmlCData* node){ return node->content.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_content, {L"value"}, void(XmlCData::*)(const vl::WString&), [](XmlCData* node, const vl::WString& value){ node->content.value = value; })

				CLASS_MEMBER_PROPERTY(content, get_content, set_content)
			END_CLASS_MEMBER(XmlCData)

			BEGIN_CLASS_MEMBER(XmlComment)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlComment>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_content, NO_PARAMETER, vl::WString(XmlComment::*)(), [](XmlComment* node){ return node->content.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_content, {L"value"}, void(XmlComment::*)(const vl::WString&), [](XmlComment* node, const vl::WString& value){ node->content.value = value; })

				CLASS_MEMBER_PROPERTY(content, get_content, set_content)
			END_CLASS_MEMBER(XmlComment)

			BEGIN_CLASS_MEMBER(XmlElement)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlElement>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(XmlElement::*)(), [](XmlElement* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(XmlElement::*)(const vl::WString&), [](XmlElement* node, const vl::WString& value){ node->name.value = value; })
				CLASS_MEMBER_EXTERNALMETHOD(get_closingName, NO_PARAMETER, vl::WString(XmlElement::*)(), [](XmlElement* node){ return node->closingName.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_closingName, {L"value"}, void(XmlElement::*)(const vl::WString&), [](XmlElement* node, const vl::WString& value){ node->closingName.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_PROPERTY(closingName, get_closingName, set_closingName)
				CLASS_MEMBER_FIELD(attributes)
				CLASS_MEMBER_FIELD(subNodes)
			END_CLASS_MEMBER(XmlElement)

			BEGIN_CLASS_MEMBER(XmlFirstClassChild)
				CLASS_MEMBER_BASE(XmlFirstClass)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlFirstClassChild>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_field2, NO_PARAMETER, vl::WString(XmlFirstClassChild::*)(), [](XmlFirstClassChild* node){ return node->field2.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_field2, {L"value"}, void(XmlFirstClassChild::*)(const vl::WString&), [](XmlFirstClassChild* node, const vl::WString& value){ node->field2.value = value; })

				CLASS_MEMBER_PROPERTY(field2, get_field2, set_field2)
			END_CLASS_MEMBER(XmlFirstClassChild)

			BEGIN_CLASS_MEMBER(XmlAttribute)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlAttribute>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(XmlAttribute::*)(), [](XmlAttribute* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(XmlAttribute::*)(const vl::WString&), [](XmlAttribute* node, const vl::WString& value){ node->name.value = value; })
				CLASS_MEMBER_EXTERNALMETHOD(get_value, NO_PARAMETER, vl::WString(XmlAttribute::*)(), [](XmlAttribute* node){ return node->value.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_value, {L"value"}, void(XmlAttribute::*)(const vl::WString&), [](XmlAttribute* node, const vl::WString& value){ node->value.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_PROPERTY(value, get_value, set_value)
			END_CLASS_MEMBER(XmlAttribute)

			BEGIN_CLASS_MEMBER(XmlThirdClassChild)
				CLASS_MEMBER_BASE(XmlFirstClass::XmlSecondClass::XmlThirdClass)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlThirdClassChild>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_field5, NO_PARAMETER, vl::WString(XmlThirdClassChild::*)(), [](XmlThirdClassChild* node){ return node->field5.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_field5, {L"value"}, void(XmlThirdClassChild::*)(const vl::WString&), [](XmlThirdClassChild* node, const vl::WString& value){ node->field5.value = value; })

				CLASS_MEMBER_PROPERTY(field5, get_field5, set_field5)
			END_CLASS_MEMBER(XmlThirdClassChild)

			BEGIN_CLASS_MEMBER(XmlDocument)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlDocument>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(instructions)
				CLASS_MEMBER_FIELD(comments)
				CLASS_MEMBER_FIELD(rootElement)
			END_CLASS_MEMBER(XmlDocument)

			BEGIN_CLASS_MEMBER(XmlInstruction)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlInstruction>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(XmlInstruction::*)(), [](XmlInstruction* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(XmlInstruction::*)(const vl::WString&), [](XmlInstruction* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(attributes)
			END_CLASS_MEMBER(XmlInstruction)

			BEGIN_CLASS_MEMBER(XmlFirstClass::XmlSecondClass::XmlThirdClass)
				CLASS_MEMBER_EXTERNALMETHOD(get_field1, NO_PARAMETER, vl::WString(XmlFirstClass::XmlSecondClass::XmlThirdClass::*)(), [](XmlFirstClass::XmlSecondClass::XmlThirdClass* node){ return node->field1.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_field1, {L"value"}, void(XmlFirstClass::XmlSecondClass::XmlThirdClass::*)(const vl::WString&), [](XmlFirstClass::XmlSecondClass::XmlThirdClass* node, const vl::WString& value){ node->field1.value = value; })

				CLASS_MEMBER_PROPERTY(field1, get_field1, set_field1)
			END_CLASS_MEMBER(XmlFirstClass::XmlSecondClass::XmlThirdClass)

			BEGIN_CLASS_MEMBER(XmlFirstClass)
				CLASS_MEMBER_EXTERNALMETHOD(get_field4, NO_PARAMETER, vl::WString(XmlFirstClass::*)(), [](XmlFirstClass* node){ return node->field4.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_field4, {L"value"}, void(XmlFirstClass::*)(const vl::WString&), [](XmlFirstClass* node, const vl::WString& value){ node->field4.value = value; })

				CLASS_MEMBER_PROPERTY(field4, get_field4, set_field4)
			END_CLASS_MEMBER(XmlFirstClass)

			BEGIN_ENUM_ITEM(XmlFirstClass::XmlIndirectEnum)
				ENUM_ITEM_NAMESPACE(XmlFirstClass::XmlIndirectEnum)
				ENUM_NAMESPACE_ITEM(D)
				ENUM_NAMESPACE_ITEM(E)
				ENUM_NAMESPACE_ITEM(F)
			END_ENUM_ITEM(XmlFirstClass::XmlIndirectEnum)

			BEGIN_ENUM_ITEM(XmlDirectEnum)
				ENUM_ITEM_NAMESPACE(XmlDirectEnum)
				ENUM_NAMESPACE_ITEM(A)
				ENUM_NAMESPACE_ITEM(B)
				ENUM_NAMESPACE_ITEM(C)
			END_ENUM_ITEM(XmlDirectEnum)

			BEGIN_CLASS_MEMBER(XmlFirstClass::XmlSecondClass)

			END_CLASS_MEMBER(XmlFirstClass::XmlSecondClass)

			BEGIN_CLASS_MEMBER(XmlFirstClass::XmlSecondClassChild)
				CLASS_MEMBER_BASE(XmlFirstClass::XmlSecondClass)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlFirstClass::XmlSecondClassChild>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_field3, NO_PARAMETER, vl::WString(XmlFirstClass::XmlSecondClassChild::*)(), [](XmlFirstClass::XmlSecondClassChild* node){ return node->field3.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_field3, {L"value"}, void(XmlFirstClass::XmlSecondClassChild::*)(const vl::WString&), [](XmlFirstClass::XmlSecondClassChild* node, const vl::WString& value){ node->field3.value = value; })

				CLASS_MEMBER_PROPERTY(field3, get_field3, set_field3)
			END_CLASS_MEMBER(XmlFirstClass::XmlSecondClassChild)

			BEGIN_CLASS_MEMBER(XmlText)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlText>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_content, NO_PARAMETER, vl::WString(XmlText::*)(), [](XmlText* node){ return node->content.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_content, {L"value"}, void(XmlText::*)(const vl::WString&), [](XmlText* node, const vl::WString& value){ node->content.value = value; })

				CLASS_MEMBER_PROPERTY(content, get_content, set_content)
			END_CLASS_MEMBER(XmlText)

			class XmlTypeLoader : public vl::Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlAmbiguousNode)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlNode)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlCData)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlComment)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlElement)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClassChild)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlAttribute)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlThirdClassChild)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlDocument)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlInstruction)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass::XmlSecondClass::XmlThirdClass)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass::XmlIndirectEnum)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlDirectEnum)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass::XmlSecondClass)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass::XmlSecondClassChild)
					ADD_TYPE_INFO(vczh::whoknows::xmlutility::XmlText)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};
#endif

			bool XmlLoadTypes()
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new XmlTypeLoader;
					return manager->AddTypeLoader(loader);
				}
#endif
				return false;
			}
		}
	}
}
