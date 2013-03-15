#include "ParsingBootstrap_Parser.h"

namespace vl
{
	namespace parsing
	{
		namespace bootstrap
		{
/***********************************************************************
Parser Function
***********************************************************************/

			vl::Ptr<vl::parsing::ParsingTreeNode> BootstrapParseDeclarationAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"ParserDecl");
				vl::Ptr<vl::parsing::tabling::ParsingStrictParser> parser=new vl::parsing::tabling::ParsingStrictParser;
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				return node;
			}

/***********************************************************************
Table Generation
***********************************************************************/

			vl::Ptr<vl::parsing::tabling::ParsingTable> BootstrapLoadTable()
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(25-vl::parsing::tabling::ParsingTable::UserTokenStart, 1, 105, 14);
				table->SetTokenInfo(0, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
				table->SetTokenInfo(1, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
				table->SetTokenInfo(2, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
				table->SetTokenInfo(3, vl::parsing::tabling::ParsingTable::TokenInfo(L"CLASS", L"class"));
				table->SetTokenInfo(4, vl::parsing::tabling::ParsingTable::TokenInfo(L"ENUM", L"enum"));
				table->SetTokenInfo(5, vl::parsing::tabling::ParsingTable::TokenInfo(L"TOKEN", L"token"));
				table->SetTokenInfo(6, vl::parsing::tabling::ParsingTable::TokenInfo(L"DISCARDTOKEN", L"discardtoken"));
				table->SetTokenInfo(7, vl::parsing::tabling::ParsingTable::TokenInfo(L"RULE", L"rule"));
				table->SetTokenInfo(8, vl::parsing::tabling::ParsingTable::TokenInfo(L"AS", L"as"));
				table->SetTokenInfo(9, vl::parsing::tabling::ParsingTable::TokenInfo(L"WITH", L"with"));
				table->SetTokenInfo(10, vl::parsing::tabling::ParsingTable::TokenInfo(L"OPEN", L"/{"));
				table->SetTokenInfo(11, vl::parsing::tabling::ParsingTable::TokenInfo(L"CLOSE", L"/}"));
				table->SetTokenInfo(12, vl::parsing::tabling::ParsingTable::TokenInfo(L"SEMICOLON", L";"));
				table->SetTokenInfo(13, vl::parsing::tabling::ParsingTable::TokenInfo(L"COLON", L":"));
				table->SetTokenInfo(14, vl::parsing::tabling::ParsingTable::TokenInfo(L"COMMA", L","));
				table->SetTokenInfo(15, vl::parsing::tabling::ParsingTable::TokenInfo(L"DOT", L"."));
				table->SetTokenInfo(16, vl::parsing::tabling::ParsingTable::TokenInfo(L"ASSIGN", L"/="));
				table->SetTokenInfo(17, vl::parsing::tabling::ParsingTable::TokenInfo(L"USING", L"/!"));
				table->SetTokenInfo(18, vl::parsing::tabling::ParsingTable::TokenInfo(L"OR", L"/|"));
				table->SetTokenInfo(19, vl::parsing::tabling::ParsingTable::TokenInfo(L"OPTOPEN", L"/["));
				table->SetTokenInfo(20, vl::parsing::tabling::ParsingTable::TokenInfo(L"OPTCLOSE", L"/]"));
				table->SetTokenInfo(21, vl::parsing::tabling::ParsingTable::TokenInfo(L"PREOPEN", L"/("));
				table->SetTokenInfo(22, vl::parsing::tabling::ParsingTable::TokenInfo(L"PRECLOSE", L"/)"));
				table->SetTokenInfo(23, vl::parsing::tabling::ParsingTable::TokenInfo(L"NAME", L"[a-zA-Z_]/w*"));
				table->SetTokenInfo(24, vl::parsing::tabling::ParsingTable::TokenInfo(L"STRING", L"\"([^\"]|\"\")*\""));
				table->SetDiscardTokenInfo(0, vl::parsing::tabling::ParsingTable::TokenInfo(L"SPACE", L"/s+"));
				table->SetStateInfo(0, vl::parsing::tabling::ParsingTable::StateInfo(L"SequenceGrammar", L"SequenceGrammar.RootStart", L"¡ñ $<SequenceGrammar>"));
				table->SetStateInfo(1, vl::parsing::tabling::ParsingTable::StateInfo(L"SequenceGrammar", L"SequenceGrammar.Start", L"¡¤ <SequenceGrammar>"));
				table->SetStateInfo(2, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.10", L"<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\" NAME : memberName as AssignGrammarDef¡ñ\r\n<PrimitiveGrammar>: NAME : name as PrimitiveGrammarDef¡ñ\r\n<PrimitiveGrammar>: STRING : text as TextGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"{\" Grammar : grammar \"}\" as LoopGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"(\" !Grammar \")\"¡ñ\r\n<PrimitiveGrammar>: \"[\" Grammar : grammar \"]\" as OptionalGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"!\" PrimitiveGrammar : grammar as UseGrammarDef¡ñ"));
				table->SetStateInfo(3, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.2", L"<PrimitiveGrammar>: \"!\"¡ñ PrimitiveGrammar : grammar as UseGrammarDef"));
				table->SetStateInfo(4, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.3", L"<PrimitiveGrammar>: \"[\"¡ñ Grammar : grammar \"]\" as OptionalGrammarDef"));
				table->SetStateInfo(5, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.4", L"<PrimitiveGrammar>: \"{\"¡ñ Grammar : grammar \"}\" as LoopGrammarDef"));
				table->SetStateInfo(6, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.5", L"<PrimitiveGrammar>: \"(\"¡ñ !Grammar \")\""));
				table->SetStateInfo(7, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.RootEnd", L"$<PrimitiveGrammar> ¡ñ"));
				table->SetStateInfo(8, vl::parsing::tabling::ParsingTable::StateInfo(L"SequenceGrammar", L"SequenceGrammar.RootEnd", L"$<SequenceGrammar> ¡ñ"));
				table->SetStateInfo(9, vl::parsing::tabling::ParsingTable::StateInfo(L"AlternativeGrammar", L"AlternativeGrammar.3", L"<AlternativeGrammar>: AlternativeGrammar : first \"|\"¡ñ SequenceGrammar : second as AlternativeGrammarDef"));
				table->SetStateInfo(10, vl::parsing::tabling::ParsingTable::StateInfo(L"AlternativeGrammar", L"AlternativeGrammar.RootEnd", L"$<AlternativeGrammar> ¡ñ"));
				table->SetStateInfo(11, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.4", L"<Grammar>: Grammar : grammar \"as\"¡ñ Type : type as CreateGrammarDef"));
				table->SetStateInfo(12, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.5", L"<Grammar>: Grammar : grammar \"with\"¡ñ \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef"));
				table->SetStateInfo(13, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.RootEnd", L"$<Grammar> ¡ñ"));
				table->SetStateInfo(14, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleFragmentDecl", L"RuleFragmentDecl.3", L"<RuleFragmentDecl>: \"=\" !Grammar \";\"¡ñ"));
				table->SetStateInfo(15, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleDecl", L"RuleDecl.5", L"<RuleDecl>: \"rule\" Type : type NAME : name { \"=\"¡ñ Grammar : grammars } \";\" as RuleDef"));
				table->SetStateInfo(16, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleDecl", L"RuleDecl.4", L"<RuleDecl>: \"rule\" Type : type NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef¡ñ"));
				table->SetStateInfo(17, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.6", L"<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\"¡ñ NAME : memberName as AssignGrammarDef"));
				table->SetStateInfo(18, vl::parsing::tabling::ParsingTable::StateInfo(L"Type", L"Type.1", L"<Type>: \"token\" as TokenTypeObj¡ñ\r\n<Type>: NAME : name as PrimitiveTypeObj¡ñ\r\n<Type>: Type : parentType \".\" NAME : name as SubTypeObj¡ñ\r\n<Type>: Type : elementType \"[\" \"]\" as ArrayTypeObj¡ñ"));
				table->SetStateInfo(19, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.6", L"<Grammar>: Grammar : grammar \"with\" \"{\"¡ñ NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef"));
				table->SetStateInfo(20, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleFragmentDecl", L"RuleFragmentDecl.RootEnd", L"$<RuleFragmentDecl> ¡ñ"));
				table->SetStateInfo(21, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleDecl", L"RuleDecl.RootEnd", L"$<RuleDecl> ¡ñ"));
				table->SetStateInfo(22, vl::parsing::tabling::ParsingTable::StateInfo(L"ParserDecl", L"ParserDecl.RootEnd", L"$<ParserDecl> ¡ñ"));
				table->SetStateInfo(23, vl::parsing::tabling::ParsingTable::StateInfo(L"Enum", L"Enum.1", L"<Enum>: \"enum\"¡ñ NAME : name \"{\" { EnumMember : members } \"}\" as EnumTypeDef"));
				table->SetStateInfo(24, vl::parsing::tabling::ParsingTable::StateInfo(L"Class", L"Class.1", L"<Class>: \"class\"¡ñ NAME : name [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef"));
				table->SetStateInfo(25, vl::parsing::tabling::ParsingTable::StateInfo(L"TokenDecl", L"TokenDecl.1", L"<TokenDecl>: \"token\"¡ñ NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\"¡ñ NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }"));
				table->SetStateInfo(26, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleDecl", L"RuleDecl.1", L"<RuleDecl>: \"rule\"¡ñ Type : type NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef"));
				table->SetStateInfo(27, vl::parsing::tabling::ParsingTable::StateInfo(L"Type", L"Type.RootEnd", L"$<Type> ¡ñ"));
				table->SetStateInfo(28, vl::parsing::tabling::ParsingTable::StateInfo(L"ClassMember", L"ClassMember.2", L"<ClassMember>: Type : type NAME : name¡ñ [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef"));
				table->SetStateInfo(29, vl::parsing::tabling::ParsingTable::StateInfo(L"Class", L"Class.4", L"<Class>: \"class\" NAME : name [ \":\" Type : parentType ] \"{\"¡ñ { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef\r\n<Class>: \"class\" NAME : name [ \":\" Type : parentType ] \"{\" ¡ñ{ ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef"));
				table->SetStateInfo(30, vl::parsing::tabling::ParsingTable::StateInfo(L"Type", L"Type.4", L"<Type>: Type : parentType \".\"¡ñ NAME : name as SubTypeObj"));
				table->SetStateInfo(31, vl::parsing::tabling::ParsingTable::StateInfo(L"Type", L"Type.5", L"<Type>: Type : elementType \"[\"¡ñ \"]\" as ArrayTypeObj"));
				table->SetStateInfo(32, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleDecl", L"RuleDecl.3", L"<RuleDecl>: \"rule\" Type : type NAME : name¡ñ { \"=\" Grammar : grammars } \";\" as RuleDef\r\n<RuleDecl>: \"rule\" Type : type NAME : name ¡ñ{ \"=\" Grammar : grammars } \";\" as RuleDef"));
				table->SetStateInfo(33, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.7", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName¡ñ \"=\" STRING : value \"}\" as SetterGrammarDef"));
				table->SetStateInfo(34, vl::parsing::tabling::ParsingTable::StateInfo(L"Enum", L"Enum.2", L"<Enum>: \"enum\" NAME : name¡ñ \"{\" { EnumMember : members } \"}\" as EnumTypeDef"));
				table->SetStateInfo(35, vl::parsing::tabling::ParsingTable::StateInfo(L"Class", L"Class.2", L"<Class>: \"class\" NAME : name¡ñ [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef"));
				table->SetStateInfo(36, vl::parsing::tabling::ParsingTable::StateInfo(L"TokenDecl", L"TokenDecl.2", L"<TokenDecl>: \"token\" NAME : name¡ñ \"=\" STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name¡ñ \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }"));
				table->SetStateInfo(37, vl::parsing::tabling::ParsingTable::StateInfo(L"ClassMember", L"ClassMember.3", L"<ClassMember>: Type : type NAME : name [ \"(\"¡ñ NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef"));
				table->SetStateInfo(38, vl::parsing::tabling::ParsingTable::StateInfo(L"ClassMember", L"ClassMember.4", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef¡ñ"));
				table->SetStateInfo(39, vl::parsing::tabling::ParsingTable::StateInfo(L"Class", L"Class.6", L"<Class>: \"class\" NAME : name [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef¡ñ"));
				table->SetStateInfo(40, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.8", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\"¡ñ STRING : value \"}\" as SetterGrammarDef"));
				table->SetStateInfo(41, vl::parsing::tabling::ParsingTable::StateInfo(L"Enum", L"Enum.3", L"<Enum>: \"enum\" NAME : name \"{\"¡ñ { EnumMember : members } \"}\" as EnumTypeDef\r\n<Enum>: \"enum\" NAME : name \"{\" ¡ñ{ EnumMember : members } \"}\" as EnumTypeDef"));
				table->SetStateInfo(42, vl::parsing::tabling::ParsingTable::StateInfo(L"Class", L"Class.3", L"<Class>: \"class\" NAME : name [ \":\"¡ñ Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef"));
				table->SetStateInfo(43, vl::parsing::tabling::ParsingTable::StateInfo(L"TokenDecl", L"TokenDecl.3", L"<TokenDecl>: \"token\" NAME : name \"=\"¡ñ STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\"¡ñ STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }"));
				table->SetStateInfo(44, vl::parsing::tabling::ParsingTable::StateInfo(L"ClassMember", L"ClassMember.5", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction¡ñ \")\" ] \";\" as ClassMemberDef"));
				table->SetStateInfo(45, vl::parsing::tabling::ParsingTable::StateInfo(L"ClassMember", L"ClassMember.RootEnd", L"$<ClassMember> ¡ñ"));
				table->SetStateInfo(46, vl::parsing::tabling::ParsingTable::StateInfo(L"Class", L"Class.RootEnd", L"$<Class> ¡ñ"));
				table->SetStateInfo(47, vl::parsing::tabling::ParsingTable::StateInfo(L"TypeDecl", L"TypeDecl.RootEnd", L"$<TypeDecl> ¡ñ"));
				table->SetStateInfo(48, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.9", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value¡ñ \"}\" as SetterGrammarDef"));
				table->SetStateInfo(49, vl::parsing::tabling::ParsingTable::StateInfo(L"Enum", L"Enum.4", L"<Enum>: \"enum\" NAME : name \"{\" { EnumMember : members } \"}\" as EnumTypeDef¡ñ"));
				table->SetStateInfo(50, vl::parsing::tabling::ParsingTable::StateInfo(L"EnumMember", L"EnumMember.1", L"<EnumMember>: NAME : name¡ñ \",\" as EnumMemberDef"));
				table->SetStateInfo(51, vl::parsing::tabling::ParsingTable::StateInfo(L"TokenDecl", L"TokenDecl.4", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex¡ñ \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex¡ñ \";\" as TokenDef with { discard = \"DiscardToken\" }"));
				table->SetStateInfo(52, vl::parsing::tabling::ParsingTable::StateInfo(L"ClassMember", L"ClassMember.6", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ]¡ñ \";\" as ClassMemberDef"));
				table->SetStateInfo(53, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.1", L"<Grammar>: !AlternativeGrammar¡ñ\r\n<Grammar>: Grammar : grammar \"as\" Type : type as CreateGrammarDef¡ñ\r\n<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef¡ñ"));
				table->SetStateInfo(54, vl::parsing::tabling::ParsingTable::StateInfo(L"Enum", L"Enum.RootEnd", L"$<Enum> ¡ñ"));
				table->SetStateInfo(55, vl::parsing::tabling::ParsingTable::StateInfo(L"EnumMember", L"EnumMember.2", L"<EnumMember>: NAME : name \",\" as EnumMemberDef¡ñ"));
				table->SetStateInfo(56, vl::parsing::tabling::ParsingTable::StateInfo(L"TokenDecl", L"TokenDecl.5", L"<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }¡ñ\r\n<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }¡ñ"));
				table->SetStateInfo(57, vl::parsing::tabling::ParsingTable::StateInfo(L"EnumMember", L"EnumMember.RootEnd", L"$<EnumMember> ¡ñ"));
				table->SetStateInfo(58, vl::parsing::tabling::ParsingTable::StateInfo(L"TokenDecl", L"TokenDecl.RootEnd", L"$<TokenDecl> ¡ñ"));
				table->SetStateInfo(59, vl::parsing::tabling::ParsingTable::StateInfo(L"EnumMember", L"EnumMember.RootStart", L"¡ñ $<EnumMember>"));
				table->SetStateInfo(60, vl::parsing::tabling::ParsingTable::StateInfo(L"EnumMember", L"EnumMember.Start", L"¡¤ <EnumMember>"));
				table->SetStateInfo(61, vl::parsing::tabling::ParsingTable::StateInfo(L"Enum", L"Enum.RootStart", L"¡ñ $<Enum>"));
				table->SetStateInfo(62, vl::parsing::tabling::ParsingTable::StateInfo(L"Enum", L"Enum.Start", L"¡¤ <Enum>"));
				table->SetStateInfo(63, vl::parsing::tabling::ParsingTable::StateInfo(L"ClassMember", L"ClassMember.RootStart", L"¡ñ $<ClassMember>"));
				table->SetStateInfo(64, vl::parsing::tabling::ParsingTable::StateInfo(L"ClassMember", L"ClassMember.Start", L"¡¤ <ClassMember>"));
				table->SetStateInfo(65, vl::parsing::tabling::ParsingTable::StateInfo(L"Class", L"Class.RootStart", L"¡ñ $<Class>"));
				table->SetStateInfo(66, vl::parsing::tabling::ParsingTable::StateInfo(L"Class", L"Class.Start", L"¡¤ <Class>"));
				table->SetStateInfo(67, vl::parsing::tabling::ParsingTable::StateInfo(L"Type", L"Type.RootStart", L"¡ñ $<Type>"));
				table->SetStateInfo(68, vl::parsing::tabling::ParsingTable::StateInfo(L"Type", L"Type.Start", L"¡¤ <Type>"));
				table->SetStateInfo(69, vl::parsing::tabling::ParsingTable::StateInfo(L"TypeDecl", L"TypeDecl.RootStart", L"¡ñ $<TypeDecl>"));
				table->SetStateInfo(70, vl::parsing::tabling::ParsingTable::StateInfo(L"TypeDecl", L"TypeDecl.Start", L"¡¤ <TypeDecl>"));
				table->SetStateInfo(71, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.RootStart", L"¡ñ $<PrimitiveGrammar>"));
				table->SetStateInfo(72, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.Start", L"¡¤ <PrimitiveGrammar>"));
				table->SetStateInfo(73, vl::parsing::tabling::ParsingTable::StateInfo(L"ParserDecl", L"ParserDecl.RootStart", L"¡ñ $<ParserDecl>"));
				table->SetStateInfo(74, vl::parsing::tabling::ParsingTable::StateInfo(L"ParserDecl", L"ParserDecl.Start", L"¡¤ <ParserDecl>"));
				table->SetStateInfo(75, vl::parsing::tabling::ParsingTable::StateInfo(L"AlternativeGrammar", L"AlternativeGrammar.RootStart", L"¡ñ $<AlternativeGrammar>"));
				table->SetStateInfo(76, vl::parsing::tabling::ParsingTable::StateInfo(L"AlternativeGrammar", L"AlternativeGrammar.Start", L"¡¤ <AlternativeGrammar>"));
				table->SetStateInfo(77, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.RootStart", L"¡ñ $<Grammar>"));
				table->SetStateInfo(78, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.Start", L"¡¤ <Grammar>"));
				table->SetStateInfo(79, vl::parsing::tabling::ParsingTable::StateInfo(L"TokenDecl", L"TokenDecl.RootStart", L"¡ñ $<TokenDecl>"));
				table->SetStateInfo(80, vl::parsing::tabling::ParsingTable::StateInfo(L"TokenDecl", L"TokenDecl.Start", L"¡¤ <TokenDecl>"));
				table->SetStateInfo(81, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleFragmentDecl", L"RuleFragmentDecl.RootStart", L"¡ñ $<RuleFragmentDecl>"));
				table->SetStateInfo(82, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleFragmentDecl", L"RuleFragmentDecl.Start", L"¡¤ <RuleFragmentDecl>"));
				table->SetStateInfo(83, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleFragmentDecl", L"RuleFragmentDecl.1", L"<RuleFragmentDecl>: \"=\"¡ñ !Grammar \";\""));
				table->SetStateInfo(84, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleDecl", L"RuleDecl.RootStart", L"¡ñ $<RuleDecl>"));
				table->SetStateInfo(85, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleDecl", L"RuleDecl.Start", L"¡¤ <RuleDecl>"));
				table->SetStateInfo(86, vl::parsing::tabling::ParsingTable::StateInfo(L"SequenceGrammar", L"SequenceGrammar.1", L"<SequenceGrammar>: !PrimitiveGrammar¡ñ\r\n<SequenceGrammar>: SequenceGrammar : first PrimitiveGrammar : second as SequenceGrammarDef¡ñ"));
				table->SetStateInfo(87, vl::parsing::tabling::ParsingTable::StateInfo(L"SequenceGrammar", L"SequenceGrammar.2", L"<SequenceGrammar>: SequenceGrammar : first¡ñ PrimitiveGrammar : second as SequenceGrammarDef"));
				table->SetStateInfo(88, vl::parsing::tabling::ParsingTable::StateInfo(L"ClassMember", L"ClassMember.1", L"<ClassMember>: Type : type¡ñ NAME : name [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef"));
				table->SetStateInfo(89, vl::parsing::tabling::ParsingTable::StateInfo(L"Class", L"Class.5", L"<Class>: \"class\" NAME : name [ \":\" Type : parentType ]¡ñ \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef"));
				table->SetStateInfo(90, vl::parsing::tabling::ParsingTable::StateInfo(L"Type", L"Type.2", L"<Type>: Type : parentType¡ñ \".\" NAME : name as SubTypeObj"));
				table->SetStateInfo(91, vl::parsing::tabling::ParsingTable::StateInfo(L"Type", L"Type.3", L"<Type>: Type : elementType¡ñ \"[\" \"]\" as ArrayTypeObj"));
				table->SetStateInfo(92, vl::parsing::tabling::ParsingTable::StateInfo(L"TypeDecl", L"TypeDecl.1", L"<TypeDecl>: !Enum | !Class¡ñ"));
				table->SetStateInfo(93, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.1", L"<PrimitiveGrammar>: PrimitiveGrammar : grammar¡ñ \":\" NAME : memberName as AssignGrammarDef"));
				table->SetStateInfo(94, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.7", L"<PrimitiveGrammar>: \"[\" Grammar : grammar¡ñ \"]\" as OptionalGrammarDef"));
				table->SetStateInfo(95, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.8", L"<PrimitiveGrammar>: \"{\" Grammar : grammar¡ñ \"}\" as LoopGrammarDef"));
				table->SetStateInfo(96, vl::parsing::tabling::ParsingTable::StateInfo(L"PrimitiveGrammar", L"PrimitiveGrammar.9", L"<PrimitiveGrammar>: \"(\" !Grammar¡ñ \")\""));
				table->SetStateInfo(97, vl::parsing::tabling::ParsingTable::StateInfo(L"ParserDecl", L"ParserDecl.1", L"<ParserDecl>: { TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef¡ñ"));
				table->SetStateInfo(98, vl::parsing::tabling::ParsingTable::StateInfo(L"ParserDecl", L"ParserDecl.2", L"<ParserDecl>: ¡ñ{ TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef"));
				table->SetStateInfo(99, vl::parsing::tabling::ParsingTable::StateInfo(L"AlternativeGrammar", L"AlternativeGrammar.1", L"<AlternativeGrammar>: !SequenceGrammar¡ñ\r\n<AlternativeGrammar>: AlternativeGrammar : first \"|\" SequenceGrammar : second as AlternativeGrammarDef¡ñ"));
				table->SetStateInfo(100, vl::parsing::tabling::ParsingTable::StateInfo(L"AlternativeGrammar", L"AlternativeGrammar.2", L"<AlternativeGrammar>: AlternativeGrammar : first¡ñ \"|\" SequenceGrammar : second as AlternativeGrammarDef"));
				table->SetStateInfo(101, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.2", L"<Grammar>: Grammar : grammar¡ñ \"as\" Type : type as CreateGrammarDef"));
				table->SetStateInfo(102, vl::parsing::tabling::ParsingTable::StateInfo(L"Grammar", L"Grammar.3", L"<Grammar>: Grammar : grammar¡ñ \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef"));
				table->SetStateInfo(103, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleFragmentDecl", L"RuleFragmentDecl.2", L"<RuleFragmentDecl>: \"=\" !Grammar¡ñ \";\""));
				table->SetStateInfo(104, vl::parsing::tabling::ParsingTable::StateInfo(L"RuleDecl", L"RuleDecl.2", L"<RuleDecl>: \"rule\" Type : type¡ñ NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef"));
				table->SetRuleInfo(0, vl::parsing::tabling::ParsingTable::RuleInfo(L"SequenceGrammar", L"GrammarDef", 0));
				table->SetRuleInfo(1, vl::parsing::tabling::ParsingTable::RuleInfo(L"EnumMember", L"EnumMemberDef", 59));
				table->SetRuleInfo(2, vl::parsing::tabling::ParsingTable::RuleInfo(L"Enum", L"EnumTypeDef", 61));
				table->SetRuleInfo(3, vl::parsing::tabling::ParsingTable::RuleInfo(L"ClassMember", L"ClassMemberDef", 63));
				table->SetRuleInfo(4, vl::parsing::tabling::ParsingTable::RuleInfo(L"Class", L"ClassTypeDef", 65));
				table->SetRuleInfo(5, vl::parsing::tabling::ParsingTable::RuleInfo(L"Type", L"TypeObj", 67));
				table->SetRuleInfo(6, vl::parsing::tabling::ParsingTable::RuleInfo(L"TypeDecl", L"TypeDef", 69));
				table->SetRuleInfo(7, vl::parsing::tabling::ParsingTable::RuleInfo(L"PrimitiveGrammar", L"GrammarDef", 71));
				table->SetRuleInfo(8, vl::parsing::tabling::ParsingTable::RuleInfo(L"ParserDecl", L"ParserDef", 73));
				table->SetRuleInfo(9, vl::parsing::tabling::ParsingTable::RuleInfo(L"AlternativeGrammar", L"GrammarDef", 75));
				table->SetRuleInfo(10, vl::parsing::tabling::ParsingTable::RuleInfo(L"Grammar", L"GrammarDef", 77));
				table->SetRuleInfo(11, vl::parsing::tabling::ParsingTable::RuleInfo(L"TokenDecl", L"TokenDef", 79));
				table->SetRuleInfo(12, vl::parsing::tabling::ParsingTable::RuleInfo(L"RuleFragmentDecl", L"GrammarDef", 81));
				table->SetRuleInfo(13, vl::parsing::tabling::ParsingTable::RuleInfo(L"RuleDecl", L"RuleDef", 84));
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 1);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(0, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 7);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 13);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 10);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 13);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 10);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 8);
						item->stackPattern.Add(87);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 13);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 10);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 13);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 10);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 8);
						item->stackPattern.Add(1);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(2, 2);
						item->stackPattern.Add(3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 3, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"UseGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 2, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 11);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 11);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 11);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 11);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 12);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 12);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 12);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 12);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->stackPattern.Add(87);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->stackPattern.Add(1);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 2);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"LoopGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 2);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"LoopGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 2);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"LoopGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 2);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"LoopGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 14);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(83);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 16);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"RuleDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 14);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(83);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 16);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"RuleDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 14);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(83);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 16);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"RuleDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 14);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(83);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 16);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"RuleDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 17);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 72, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 15);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 15);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 15);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 15);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 16, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->stackPattern.Add(87);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->stackPattern.Add(1);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(18, 9);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(18, 9);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(18, 9);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(18, 9);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 18, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->stackPattern.Add(87);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->stackPattern.Add(1);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(20, 2);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"OptionalGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(20, 2);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"OptionalGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(20, 2);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"OptionalGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(20, 2);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"OptionalGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 20, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->stackPattern.Add(87);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->stackPattern.Add(1);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(22, 2);
						item->stackPattern.Add(87);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(22, 2);
						item->stackPattern.Add(87);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(22, 2);
						item->stackPattern.Add(1);
						item->stackPattern.Add(76);
						item->stackPattern.Add(78);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(22, 2);
						item->stackPattern.Add(1);
						item->stackPattern.Add(9);
						item->stackPattern.Add(78);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AlternativeGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 22, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->stackPattern.Add(87);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->stackPattern.Add(1);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->stackPattern.Add(87);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SequenceGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"second", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->stackPattern.Add(1);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"first", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 87, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 3, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(3, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 3, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(3, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 3, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(3, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 3, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(3, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 3, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(3, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 3, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(3, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(9, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(9, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(9, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(9, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(9, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 9, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(9, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 19);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 20);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(14, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 21);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 22);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ParserDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 22);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ParserDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"rules", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"AssignGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"memberName", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(17, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 27);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 13);
						item->stackPattern.Add(11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CreateGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 11);
						item->stackPattern.Add(11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CreateGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 12);
						item->stackPattern.Add(11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CreateGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 29);
						item->stackPattern.Add(42);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 42, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"parentType", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 2);
						item->stackPattern.Add(11);
						item->stackPattern.Add(5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CreateGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"LoopGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 14);
						item->stackPattern.Add(11);
						item->stackPattern.Add(83);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CreateGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 16);
						item->stackPattern.Add(11);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CreateGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"RuleDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(15, 30);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 68, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"parentType", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 15, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 15);
						item->stackPattern.Add(11);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CreateGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 16, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 31);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 68, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"elementType", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(20, 2);
						item->stackPattern.Add(11);
						item->stackPattern.Add(4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CreateGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"OptionalGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 20, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(22, 2);
						item->stackPattern.Add(11);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 11, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CreateGrammarDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 22, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 28);
						item->stackPattern.Add(64);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 32);
						item->stackPattern.Add(26);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 26, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"type", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 33);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"memberName", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(19, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 34);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(23, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 35);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 36);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(25, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 26, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(26, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 26, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(26, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 38);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ClassMemberDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(28, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 37);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(28, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(29, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(29, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(29, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 39);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ClassTypeDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(29, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(29, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SubTypeObj", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(30, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(20, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ArrayTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(31, 20, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 16);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"RuleDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(32, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 15);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(32, 16, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 40);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(33, 16, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 41);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(34, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 29);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(35, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 42);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(35, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 43);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(36, 16, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 44);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"unescapingFunction", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(37, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 45);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(38, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"members", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(38, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"members", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(38, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 18);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"members", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(38, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 39);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"members", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ClassTypeDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(38, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"members", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(38, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 46);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 22);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ParserDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 22);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ParserDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 47);
						item->stackPattern.Add(70);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(39, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(39, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(39, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 18);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(39, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(39, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(39, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 39);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ClassTypeDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(39, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(39, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 48);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(40, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 49);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"EnumTypeDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(41, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 50);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(41, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 42, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(42, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 42, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(42, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 51);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"regex", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(43, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(22, 52);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(44, 22, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 53);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"SetterGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(48, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 54);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 22);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ParserDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 22);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ParserDef", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 47);
						item->stackPattern.Add(70);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(49, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(49, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(49, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 18);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(49, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(49, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						item->stackPattern.Add(70);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						item->stackPattern.Add(70);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"types", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(49, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 39);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ClassTypeDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(49, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->stackPattern.Add(70);
						item->stackPattern.Add(29);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 70, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subTypes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 29, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(49, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(14, 55);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"EnumMemberDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(50, 14, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 56);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(51, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 38);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ClassMemberDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(52, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 13);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(53, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(53, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 12);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(53, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 2);
						item->stackPattern.Add(5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"LoopGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(53, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 14);
						item->stackPattern.Add(83);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 16);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"RuleDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(53, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 15);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"grammars", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(53, 16, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(20, 2);
						item->stackPattern.Add(4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 4, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"grammar", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"OptionalGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(53, 20, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(22, 2);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(53, 22, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 57);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(55, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 49);
						item->stackPattern.Add(41);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"members", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"EnumTypeDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(55, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 50);
						item->stackPattern.Add(41);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"members", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(55, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 58);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 22);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ParserDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 22);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ParserDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(56, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(56, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(56, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(56, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(56, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						item->stackPattern.Add(98);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 98, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						item->stackPattern.Add(74);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"tokens", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 98, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(56, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 60);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(59, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 50);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(60, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 62);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(61, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(62, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 64);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(63, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(64, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 64, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(64, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 66);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(65, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(66, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 68);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(67, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(68, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveTypeObj", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(68, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 70);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(69, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(70, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(70, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 72);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(71, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(72, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(72, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(72, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(72, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(72, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(72, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 74);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(73, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 24);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(74, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 23);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 70, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(74, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(74, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 74, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(74, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 74, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(74, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 76);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(75, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(76, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(76, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(76, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(76, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(76, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(76, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 78);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(77, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(78, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(78, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(78, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(78, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(78, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(78, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 80);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(79, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 25);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"KeepToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(80, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 25);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TokenDef", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"discard", L"DiscardToken"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(80, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 82);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(81, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(16, 83);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(82, 16, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(83, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(17, 3);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(83, 17, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(19, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(83, 19, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(21, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(83, 21, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(23, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"PrimitiveGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(83, 23, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(24, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 83, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 78, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 76, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 1, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"text", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"TextGrammarDef", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(83, 24, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 85);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(84, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 26);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(85, 7, bag);
				}
				table->Initialize();
				return table;
			}

		}
	}
}
