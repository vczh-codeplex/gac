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
namespace vl
{
	namespace reflection
	{
		namespace description
		{
#ifndef VCZH_DEBUG_NO_REFLECTION
			using namespace vl::parsing::xml;

			IMPL_TYPE_INFO_RENAME(XmlNode, system::XmlNode)
			IMPL_TYPE_INFO_RENAME(XmlText, system::XmlText)
			IMPL_TYPE_INFO_RENAME(XmlCData, system::XmlCData)
			IMPL_TYPE_INFO_RENAME(XmlAttribute, system::XmlAttribute)
			IMPL_TYPE_INFO_RENAME(XmlComment, system::XmlComment)
			IMPL_TYPE_INFO_RENAME(XmlElement, system::XmlElement)
			IMPL_TYPE_INFO_RENAME(XmlInstruction, system::XmlInstruction)
			IMPL_TYPE_INFO_RENAME(XmlDocument, system::XmlDocument)

			BEGIN_CLASS_MEMBER(XmlNode)

			END_CLASS_MEMBER(XmlNode)

			BEGIN_CLASS_MEMBER(XmlText)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlText>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_content, NO_PARAMETER, vl::WString(XmlText::*)(), [](XmlText* node){ return node->content.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_content, {L"value"}, void(XmlText::*)(const vl::WString&), [](XmlText* node, const vl::WString& value){ node->content.value = value; })

				CLASS_MEMBER_PROPERTY(content, get_content, set_content)
			END_CLASS_MEMBER(XmlText)

			BEGIN_CLASS_MEMBER(XmlCData)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlCData>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_content, NO_PARAMETER, vl::WString(XmlCData::*)(), [](XmlCData* node){ return node->content.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_content, {L"value"}, void(XmlCData::*)(const vl::WString&), [](XmlCData* node, const vl::WString& value){ node->content.value = value; })

				CLASS_MEMBER_PROPERTY(content, get_content, set_content)
			END_CLASS_MEMBER(XmlCData)

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

			BEGIN_CLASS_MEMBER(XmlInstruction)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlInstruction>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(XmlInstruction::*)(), [](XmlInstruction* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(XmlInstruction::*)(const vl::WString&), [](XmlInstruction* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(attributes)
			END_CLASS_MEMBER(XmlInstruction)

			BEGIN_CLASS_MEMBER(XmlDocument)
				CLASS_MEMBER_BASE(XmlNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<XmlDocument>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(prologs)
				CLASS_MEMBER_FIELD(rootElement)
			END_CLASS_MEMBER(XmlDocument)

			class XmlTypeLoader : public vl::Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vl::parsing::xml::XmlNode)
					ADD_TYPE_INFO(vl::parsing::xml::XmlText)
					ADD_TYPE_INFO(vl::parsing::xml::XmlCData)
					ADD_TYPE_INFO(vl::parsing::xml::XmlAttribute)
					ADD_TYPE_INFO(vl::parsing::xml::XmlComment)
					ADD_TYPE_INFO(vl::parsing::xml::XmlElement)
					ADD_TYPE_INFO(vl::parsing::xml::XmlInstruction)
					ADD_TYPE_INFO(vl::parsing::xml::XmlDocument)
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
