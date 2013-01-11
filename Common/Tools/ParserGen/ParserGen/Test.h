/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::Test

本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/

#include "..\..\..\Source\Parsing\Parsing.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
			class Node
			{
			};

			class Text : public Node
			{
				vl::parsing::ParsingToken content;
			};

			class CData : public Node
			{
				vl::parsing::ParsingToken content;
			};

			class Attribute : public Node
			{
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken value;
			};

			class Comment : public Node
			{
				vl::parsing::ParsingToken content;
			};

			class Element : public Node
			{
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken closingName;
				vl::collections::List<vl::Ptr<Attribute>> attributes;
				vl::collections::List<vl::Ptr<Node>> subNodes;
			};

			class Instruction : public Node
			{
				vl::parsing::ParsingToken name;
				vl::collections::List<vl::Ptr<Attribute>> attributes;
			};

			class Document : public Node
			{
				vl::collections::List<vl::Ptr<Instruction>> instructions;
				vl::collections::List<vl::Ptr<Comment>> comments;
				vl::Ptr<Element> rootElement;
			};


		}
	}
}
