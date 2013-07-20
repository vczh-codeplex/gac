/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Parser::ParsingBootstrap_Parser

���ļ�ʹ��Vczh Parsing Generator�����Զ�����
***********************************************************************/

#ifndef VCZH_PARSING_BOOTSTRAP_PARSINGBOOTSTRAP_PARSER
#define VCZH_PARSING_BOOTSTRAP_PARSINGBOOTSTRAP_PARSER

#include "..\Parsing.h"

namespace vl
{
	namespace parsing
	{
		namespace bootstrap
		{
			extern vl::WString BootstrapGetParserTextBuffer();
			extern vl::Ptr<vl::parsing::tabling::ParsingTable> BootstrapLoadTable();

			extern vl::Ptr<vl::parsing::ParsingTreeNode> BootstrapParseDeclarationAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

		}
	}
}
#endif