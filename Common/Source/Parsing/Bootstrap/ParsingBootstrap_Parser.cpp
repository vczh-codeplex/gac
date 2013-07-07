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
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				return node;
			}

/***********************************************************************
Table Generation
***********************************************************************/

			vl::Ptr<vl::parsing::tabling::ParsingTable> BootstrapLoadTable()
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(26-vl::parsing::tabling::ParsingTable::UserTokenStart, 1, 120, 14);
				#define SET_TOKEN_INFO(INDEX, NAME, REGEX) table->SetTokenInfo(INDEX, vl::parsing::tabling::ParsingTable::TokenInfo(NAME, REGEX));
				#define SET_DISCARD_TOKEN_INFO(INDEX, NAME, REGEX) table->SetDiscardTokenInfo(INDEX, vl::parsing::tabling::ParsingTable::TokenInfo(NAME, REGEX));
				#define SET_STATE_INFO(INDEX, RULE, STATE, EXPR) table->SetStateInfo(INDEX, vl::parsing::tabling::ParsingTable::StateInfo(RULE, STATE, EXPR));
				#define SET_RULE_INFO(INDEX, NAME, TYPE, STARTSTATE) table->SetRuleInfo(INDEX, vl::parsing::tabling::ParsingTable::RuleInfo(NAME, TYPE, L"", STARTSTATE));
				#define SET_AMBIGUOUS_RULE_INFO(INDEX, NAME, TYPE, AMBIGUOUSTYPE, STARTSTATE) table->SetRuleInfo(INDEX, vl::parsing::tabling::ParsingTable::RuleInfo(NAME, TYPE, AMBIGUOUSTYPE, STARTSTATE));
				#define BEGIN_TRANSITION_BAG(STATE, TOKEN) {vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag; table->SetTransitionBag(STATE, TOKEN, bag);
				#define BEGIN_TRANSITION_ITEM(TOKEN, TARGETSTATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(TOKEN, TARGETSTATE); bag->transitionItems.Add(item);
				#define END_TRANSITION_ITEM }
				#define END_TRANSITION_BAG }
				#define ITEM_STACK_PATTERN(STATE) item->stackPattern.Add(STATE);
				#define ITEM_INSTRUCTION(TYPE, STATE, NAME, VALUE, RULE) item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::TYPE, STATE, NAME, VALUE, RULE));
				#define BEGIN_LOOK_AHEAD(STATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo> lookAheadInfo=new vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo; item->lookAheads.Add(lookAheadInfo); lookAheadInfo->state=STATE;
				#define LOOK_AHEAD(TOKEN) lookAheadInfo->tokens.Add(TOKEN);
				#define END_LOOK_AHEAD }

				SET_TOKEN_INFO(0, L"", L"")
				SET_TOKEN_INFO(1, L"", L"")
				SET_TOKEN_INFO(2, L"", L"")
				SET_TOKEN_INFO(3, L"CLASS", L"class")
				SET_TOKEN_INFO(4, L"AMBIGUOUS", L"ambiguous")
				SET_TOKEN_INFO(5, L"ENUM", L"enum")
				SET_TOKEN_INFO(6, L"TOKEN", L"token")
				SET_TOKEN_INFO(7, L"DISCARDTOKEN", L"discardtoken")
				SET_TOKEN_INFO(8, L"RULE", L"rule")
				SET_TOKEN_INFO(9, L"AS", L"as")
				SET_TOKEN_INFO(10, L"WITH", L"with")
				SET_TOKEN_INFO(11, L"OPEN", L"/{")
				SET_TOKEN_INFO(12, L"CLOSE", L"/}")
				SET_TOKEN_INFO(13, L"SEMICOLON", L";")
				SET_TOKEN_INFO(14, L"COLON", L":")
				SET_TOKEN_INFO(15, L"COMMA", L",")
				SET_TOKEN_INFO(16, L"DOT", L".")
				SET_TOKEN_INFO(17, L"ASSIGN", L"/=")
				SET_TOKEN_INFO(18, L"USING", L"/!")
				SET_TOKEN_INFO(19, L"OR", L"/|")
				SET_TOKEN_INFO(20, L"OPTOPEN", L"/[")
				SET_TOKEN_INFO(21, L"OPTCLOSE", L"/]")
				SET_TOKEN_INFO(22, L"PREOPEN", L"/(")
				SET_TOKEN_INFO(23, L"PRECLOSE", L"/)")
				SET_TOKEN_INFO(24, L"NAME", L"[a-zA-Z_]/w*")
				SET_TOKEN_INFO(25, L"STRING", L"\"([^\"]|\"\")*\"")

				SET_DISCARD_TOKEN_INFO(0, L"SPACE", L"/s+")

				SET_STATE_INFO(0, L"Type", L"Type.RootStart", L"¡ñ $<Type>")
				SET_STATE_INFO(1, L"Type", L"Type.Start", L"¡¤ <Type>")
				SET_STATE_INFO(2, L"Type", L"Type.1", L"<Type>: NAME : name as PrimitiveTypeObj¡ñ\r\n<Type>: \"token\" as TokenTypeObj¡ñ\r\n<Type>: Type : parentType \".\" NAME : name as SubTypeObj¡ñ\r\n<Type>: Type : elementType \"[\" \"]\" as ArrayTypeObj¡ñ")
				SET_STATE_INFO(3, L"Type", L"Type.5", L"<Type>: Type : parentType \".\"¡ñ NAME : name as SubTypeObj")
				SET_STATE_INFO(4, L"Type", L"Type.6", L"<Type>: Type : elementType \"[\"¡ñ \"]\" as ArrayTypeObj")
				SET_STATE_INFO(5, L"Type", L"Type.RootEnd", L"$<Type> ¡ñ")
				SET_STATE_INFO(6, L"ClassMember", L"ClassMember.2", L"<ClassMember>: Type : type NAME : name¡ñ [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef")
				SET_STATE_INFO(7, L"Class", L"Class.5", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\"¡ñ { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef\r\n<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" ¡ñ{ ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(8, L"RuleDecl", L"RuleDecl.3", L"<RuleDecl>: \"rule\" Type : type NAME : name¡ñ { \"=\" Grammar : grammars } \";\" as RuleDef\r\n<RuleDecl>: \"rule\" Type : type NAME : name ¡ñ{ \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(9, L"PrimitiveGrammar", L"PrimitiveGrammar.1", L"<PrimitiveGrammar>: NAME : name as PrimitiveGrammarDef¡ñ\r\n<PrimitiveGrammar>: STRING : text as TextGrammarDef¡ñ\r\n<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\" NAME : memberName as AssignGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"!\" PrimitiveGrammar : grammar as UseGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"[\" Grammar : grammar \"]\" as OptionalGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"{\" Grammar : grammar \"}\" as LoopGrammarDef¡ñ")
				SET_STATE_INFO(10, L"PrimitiveGrammar", L"PrimitiveGrammar.11", L"<PrimitiveGrammar>: \"(\" !Grammar \")\"¡ñ\r\n<PrimitiveGrammar>: NAME : name as PrimitiveGrammarDef¡ñ\r\n<PrimitiveGrammar>: STRING : text as TextGrammarDef¡ñ\r\n<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\" NAME : memberName as AssignGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"!\" PrimitiveGrammar : grammar as UseGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"[\" Grammar : grammar \"]\" as OptionalGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"{\" Grammar : grammar \"}\" as LoopGrammarDef¡ñ")
				SET_STATE_INFO(11, L"RuleDecl", L"RuleDecl.4", L"<RuleDecl>: \"rule\" Type : type NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef¡ñ")
				SET_STATE_INFO(12, L"RuleDecl", L"RuleDecl.5", L"<RuleDecl>: \"rule\" Type : type NAME : name { \"=\"¡ñ Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(13, L"Grammar", L"Grammar.4", L"<Grammar>: Grammar : grammar \"as\"¡ñ Type : type as CreateGrammarDef")
				SET_STATE_INFO(14, L"Grammar", L"Grammar.5", L"<Grammar>: Grammar : grammar \"with\"¡ñ \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(15, L"Grammar", L"Grammar.RootEnd", L"$<Grammar> ¡ñ")
				SET_STATE_INFO(16, L"RuleFragmentDecl", L"RuleFragmentDecl.3", L"<RuleFragmentDecl>: \"=\" !Grammar \";\"¡ñ")
				SET_STATE_INFO(17, L"ClassMember", L"ClassMember.4", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef¡ñ")
				SET_STATE_INFO(18, L"ClassMember", L"ClassMember.3", L"<ClassMember>: Type : type NAME : name [ \"(\"¡ñ NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef")
				SET_STATE_INFO(19, L"Class", L"Class.8", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef¡ñ")
				SET_STATE_INFO(20, L"Enum", L"Enum.1", L"<Enum>: \"enum\"¡ñ NAME : name \"{\" { EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(21, L"Class", L"Class.1", L"<Class>: \"class\"¡ñ NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(22, L"PrimitiveGrammar", L"PrimitiveGrammar.7", L"<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\"¡ñ NAME : memberName as AssignGrammarDef")
				SET_STATE_INFO(23, L"PrimitiveGrammar", L"PrimitiveGrammar.RootEnd", L"$<PrimitiveGrammar> ¡ñ")
				SET_STATE_INFO(24, L"PrimitiveGrammar", L"PrimitiveGrammar.3", L"<PrimitiveGrammar>: \"!\"¡ñ PrimitiveGrammar : grammar as UseGrammarDef")
				SET_STATE_INFO(25, L"PrimitiveGrammar", L"PrimitiveGrammar.4", L"<PrimitiveGrammar>: \"[\"¡ñ Grammar : grammar \"]\" as OptionalGrammarDef")
				SET_STATE_INFO(26, L"PrimitiveGrammar", L"PrimitiveGrammar.5", L"<PrimitiveGrammar>: \"{\"¡ñ Grammar : grammar \"}\" as LoopGrammarDef")
				SET_STATE_INFO(27, L"PrimitiveGrammar", L"PrimitiveGrammar.6", L"<PrimitiveGrammar>: \"(\"¡ñ !Grammar \")\"")
				SET_STATE_INFO(28, L"SequenceGrammar", L"SequenceGrammar.RootEnd", L"$<SequenceGrammar> ¡ñ")
				SET_STATE_INFO(29, L"AlternativeGrammar", L"AlternativeGrammar.3", L"<AlternativeGrammar>: AlternativeGrammar : first \"|\"¡ñ SequenceGrammar : second as AlternativeGrammarDef")
				SET_STATE_INFO(30, L"AlternativeGrammar", L"AlternativeGrammar.RootEnd", L"$<AlternativeGrammar> ¡ñ")
				SET_STATE_INFO(31, L"RuleDecl", L"RuleDecl.RootEnd", L"$<RuleDecl> ¡ñ")
				SET_STATE_INFO(32, L"ParserDecl", L"ParserDecl.RootEnd", L"$<ParserDecl> ¡ñ")
				SET_STATE_INFO(33, L"RuleDecl", L"RuleDecl.1", L"<RuleDecl>: \"rule\"¡ñ Type : type NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(34, L"TokenDecl", L"TokenDecl.1", L"<TokenDecl>: \"token\"¡ñ NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\"¡ñ NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(35, L"Grammar", L"Grammar.7", L"<Grammar>: Grammar : grammar \"with\" \"{\"¡ñ NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(36, L"RuleFragmentDecl", L"RuleFragmentDecl.RootEnd", L"$<RuleFragmentDecl> ¡ñ")
				SET_STATE_INFO(37, L"ClassMember", L"ClassMember.RootEnd", L"$<ClassMember> ¡ñ")
				SET_STATE_INFO(38, L"ClassMember", L"ClassMember.5", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction¡ñ \")\" ] \";\" as ClassMemberDef")
				SET_STATE_INFO(39, L"Class", L"Class.RootEnd", L"$<Class> ¡ñ")
				SET_STATE_INFO(40, L"TypeDecl", L"TypeDecl.RootEnd", L"$<TypeDecl> ¡ñ")
				SET_STATE_INFO(41, L"Enum", L"Enum.2", L"<Enum>: \"enum\" NAME : name¡ñ \"{\" { EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(42, L"Class", L"Class.2", L"<Class>: \"class\" NAME : name¡ñ [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(43, L"TokenDecl", L"TokenDecl.2", L"<TokenDecl>: \"token\" NAME : name¡ñ \"=\" STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name¡ñ \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(44, L"Grammar", L"Grammar.8", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName¡ñ \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(45, L"ClassMember", L"ClassMember.7", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ]¡ñ \";\" as ClassMemberDef")
				SET_STATE_INFO(46, L"Enum", L"Enum.3", L"<Enum>: \"enum\" NAME : name \"{\"¡ñ { EnumMember : members } \"}\" as EnumTypeDef\r\n<Enum>: \"enum\" NAME : name \"{\" ¡ñ{ EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(47, L"Class", L"Class.3", L"<Class>: \"class\" NAME : name [ \"ambiguous\"¡ñ \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(48, L"Class", L"Class.4", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\"¡ñ Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(49, L"TokenDecl", L"TokenDecl.3", L"<TokenDecl>: \"token\" NAME : name \"=\"¡ñ STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\"¡ñ STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(50, L"Grammar", L"Grammar.9", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\"¡ñ STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(51, L"Enum", L"Enum.4", L"<Enum>: \"enum\" NAME : name \"{\" { EnumMember : members } \"}\" as EnumTypeDef¡ñ")
				SET_STATE_INFO(52, L"EnumMember", L"EnumMember.1", L"<EnumMember>: NAME : name¡ñ \",\" as EnumMemberDef")
				SET_STATE_INFO(53, L"Class", L"Class.6", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\"¡ñ NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(54, L"TokenDecl", L"TokenDecl.4", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex¡ñ \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex¡ñ \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(55, L"Grammar", L"Grammar.10", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value¡ñ \"}\" as SetterGrammarDef")
				SET_STATE_INFO(56, L"Enum", L"Enum.RootEnd", L"$<Enum> ¡ñ")
				SET_STATE_INFO(57, L"EnumMember", L"EnumMember.2", L"<EnumMember>: NAME : name \",\" as EnumMemberDef¡ñ")
				SET_STATE_INFO(58, L"Class", L"Class.9", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType¡ñ \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(59, L"TokenDecl", L"TokenDecl.5", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex \";\" as TokenDef¡ñ with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex \";\" as TokenDef¡ñ with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(60, L"Grammar", L"Grammar.6", L"<Grammar>: Grammar : grammar \"as\" Type : type as CreateGrammarDef¡ñ\r\n<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef¡ñ")
				SET_STATE_INFO(61, L"EnumMember", L"EnumMember.RootEnd", L"$<EnumMember> ¡ñ")
				SET_STATE_INFO(62, L"Class", L"Class.11", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ]¡ñ [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(63, L"TokenDecl", L"TokenDecl.RootEnd", L"$<TokenDecl> ¡ñ")
				SET_STATE_INFO(64, L"EnumMember", L"EnumMember.RootStart", L"¡ñ $<EnumMember>")
				SET_STATE_INFO(65, L"EnumMember", L"EnumMember.Start", L"¡¤ <EnumMember>")
				SET_STATE_INFO(66, L"Enum", L"Enum.RootStart", L"¡ñ $<Enum>")
				SET_STATE_INFO(67, L"Enum", L"Enum.Start", L"¡¤ <Enum>")
				SET_STATE_INFO(68, L"ClassMember", L"ClassMember.RootStart", L"¡ñ $<ClassMember>")
				SET_STATE_INFO(69, L"ClassMember", L"ClassMember.Start", L"¡¤ <ClassMember>")
				SET_STATE_INFO(70, L"Class", L"Class.RootStart", L"¡ñ $<Class>")
				SET_STATE_INFO(71, L"Class", L"Class.Start", L"¡¤ <Class>")
				SET_STATE_INFO(72, L"TypeDecl", L"TypeDecl.RootStart", L"¡ñ $<TypeDecl>")
				SET_STATE_INFO(73, L"TypeDecl", L"TypeDecl.Start", L"¡¤ <TypeDecl>")
				SET_STATE_INFO(74, L"PrimitiveGrammar", L"PrimitiveGrammar.RootStart", L"¡ñ $<PrimitiveGrammar>")
				SET_STATE_INFO(75, L"PrimitiveGrammar", L"PrimitiveGrammar.Start", L"¡¤ <PrimitiveGrammar>")
				SET_STATE_INFO(76, L"SequenceGrammar", L"SequenceGrammar.RootStart", L"¡ñ $<SequenceGrammar>")
				SET_STATE_INFO(77, L"SequenceGrammar", L"SequenceGrammar.Start", L"¡¤ <SequenceGrammar>")
				SET_STATE_INFO(78, L"AlternativeGrammar", L"AlternativeGrammar.RootStart", L"¡ñ $<AlternativeGrammar>")
				SET_STATE_INFO(79, L"AlternativeGrammar", L"AlternativeGrammar.Start", L"¡¤ <AlternativeGrammar>")
				SET_STATE_INFO(80, L"TokenDecl", L"TokenDecl.RootStart", L"¡ñ $<TokenDecl>")
				SET_STATE_INFO(81, L"TokenDecl", L"TokenDecl.Start", L"¡¤ <TokenDecl>")
				SET_STATE_INFO(82, L"RuleDecl", L"RuleDecl.RootStart", L"¡ñ $<RuleDecl>")
				SET_STATE_INFO(83, L"RuleDecl", L"RuleDecl.Start", L"¡¤ <RuleDecl>")
				SET_STATE_INFO(84, L"Grammar", L"Grammar.RootStart", L"¡ñ $<Grammar>")
				SET_STATE_INFO(85, L"Grammar", L"Grammar.Start", L"¡¤ <Grammar>")
				SET_STATE_INFO(86, L"RuleFragmentDecl", L"RuleFragmentDecl.RootStart", L"¡ñ $<RuleFragmentDecl>")
				SET_STATE_INFO(87, L"RuleFragmentDecl", L"RuleFragmentDecl.Start", L"¡¤ <RuleFragmentDecl>")
				SET_STATE_INFO(88, L"RuleFragmentDecl", L"RuleFragmentDecl.1", L"<RuleFragmentDecl>: \"=\"¡ñ !Grammar \";\"")
				SET_STATE_INFO(89, L"ParserDecl", L"ParserDecl.RootStart", L"¡ñ $<ParserDecl>")
				SET_STATE_INFO(90, L"ParserDecl", L"ParserDecl.Start", L"¡¤ <ParserDecl>")
				SET_STATE_INFO(91, L"Type", L"Type.2", L"<Type>: Type : parentType¡ñ \".\" NAME : name as SubTypeObj")
				SET_STATE_INFO(92, L"Type", L"Type.3", L"<Type>: Type : elementType¡ñ \"[\" \"]\" as ArrayTypeObj")
				SET_STATE_INFO(93, L"Type", L"Type.4", L"<Type>: NAME : name as PrimitiveTypeObj¡ñ\r\n<Type>: \"token\" as TokenTypeObj¡ñ\r\n<Type>: Type : parentType \".\" NAME : name as SubTypeObj¡ñ\r\n<Type>: Type : elementType \"[\" \"]\" as ArrayTypeObj¡ñ")
				SET_STATE_INFO(94, L"EnumMember", L"EnumMember.3", L"<EnumMember>: NAME : name \",\" as EnumMemberDef¡ñ")
				SET_STATE_INFO(95, L"Enum", L"Enum.5", L"<Enum>: \"enum\" NAME : name \"{\" { EnumMember : members } \"}\" as EnumTypeDef¡ñ")
				SET_STATE_INFO(96, L"ClassMember", L"ClassMember.1", L"<ClassMember>: Type : type¡ñ NAME : name [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef")
				SET_STATE_INFO(97, L"ClassMember", L"ClassMember.6", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef¡ñ")
				SET_STATE_INFO(98, L"Class", L"Class.7", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ]¡ñ \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(99, L"Class", L"Class.10", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef¡ñ")
				SET_STATE_INFO(100, L"TypeDecl", L"TypeDecl.1", L"<TypeDecl>: !Enum | !Class¡ñ")
				SET_STATE_INFO(101, L"PrimitiveGrammar", L"PrimitiveGrammar.2", L"<PrimitiveGrammar>: PrimitiveGrammar : grammar¡ñ \":\" NAME : memberName as AssignGrammarDef")
				SET_STATE_INFO(102, L"PrimitiveGrammar", L"PrimitiveGrammar.8", L"<PrimitiveGrammar>: \"[\" Grammar : grammar¡ñ \"]\" as OptionalGrammarDef")
				SET_STATE_INFO(103, L"PrimitiveGrammar", L"PrimitiveGrammar.9", L"<PrimitiveGrammar>: \"{\" Grammar : grammar¡ñ \"}\" as LoopGrammarDef")
				SET_STATE_INFO(104, L"PrimitiveGrammar", L"PrimitiveGrammar.10", L"<PrimitiveGrammar>: \"(\" !Grammar¡ñ \")\"")
				SET_STATE_INFO(105, L"SequenceGrammar", L"SequenceGrammar.1", L"<SequenceGrammar>: !PrimitiveGrammar¡ñ\r\n<SequenceGrammar>: SequenceGrammar : first PrimitiveGrammar : second as SequenceGrammarDef¡ñ")
				SET_STATE_INFO(106, L"SequenceGrammar", L"SequenceGrammar.2", L"<SequenceGrammar>: SequenceGrammar : first¡ñ PrimitiveGrammar : second as SequenceGrammarDef")
				SET_STATE_INFO(107, L"SequenceGrammar", L"SequenceGrammar.3", L"<SequenceGrammar>: SequenceGrammar : first PrimitiveGrammar : second as SequenceGrammarDef¡ñ")
				SET_STATE_INFO(108, L"AlternativeGrammar", L"AlternativeGrammar.1", L"<AlternativeGrammar>: !SequenceGrammar¡ñ\r\n<AlternativeGrammar>: AlternativeGrammar : first \"|\" SequenceGrammar : second as AlternativeGrammarDef¡ñ")
				SET_STATE_INFO(109, L"AlternativeGrammar", L"AlternativeGrammar.2", L"<AlternativeGrammar>: AlternativeGrammar : first¡ñ \"|\" SequenceGrammar : second as AlternativeGrammarDef")
				SET_STATE_INFO(110, L"AlternativeGrammar", L"AlternativeGrammar.4", L"<AlternativeGrammar>: AlternativeGrammar : first \"|\" SequenceGrammar : second as AlternativeGrammarDef¡ñ")
				SET_STATE_INFO(111, L"TokenDecl", L"TokenDecl.6", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }¡ñ\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }¡ñ")
				SET_STATE_INFO(112, L"RuleDecl", L"RuleDecl.2", L"<RuleDecl>: \"rule\" Type : type¡ñ NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(113, L"RuleDecl", L"RuleDecl.6", L"<RuleDecl>: \"rule\" Type : type NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef¡ñ")
				SET_STATE_INFO(114, L"Grammar", L"Grammar.1", L"<Grammar>: !AlternativeGrammar¡ñ\r\n<Grammar>: Grammar : grammar \"as\" Type : type as CreateGrammarDef¡ñ\r\n<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef¡ñ")
				SET_STATE_INFO(115, L"Grammar", L"Grammar.2", L"<Grammar>: Grammar : grammar¡ñ \"as\" Type : type as CreateGrammarDef")
				SET_STATE_INFO(116, L"Grammar", L"Grammar.3", L"<Grammar>: Grammar : grammar¡ñ \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(117, L"RuleFragmentDecl", L"RuleFragmentDecl.2", L"<RuleFragmentDecl>: \"=\" !Grammar¡ñ \";\"")
				SET_STATE_INFO(118, L"ParserDecl", L"ParserDecl.1", L"<ParserDecl>: { TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef¡ñ\r\n<ParserDecl>: ¡ñ{ TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef")
				SET_STATE_INFO(119, L"ParserDecl", L"ParserDecl.2", L"<ParserDecl>: { TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef¡ñ")

				SET_RULE_INFO(0, L"Type", L"TypeObj", 0)
				SET_RULE_INFO(1, L"EnumMember", L"EnumMemberDef", 64)
				SET_RULE_INFO(2, L"Enum", L"EnumTypeDef", 66)
				SET_RULE_INFO(3, L"ClassMember", L"ClassMemberDef", 68)
				SET_RULE_INFO(4, L"Class", L"ClassTypeDef", 70)
				SET_RULE_INFO(5, L"TypeDecl", L"TypeDef", 72)
				SET_RULE_INFO(6, L"PrimitiveGrammar", L"GrammarDef", 74)
				SET_RULE_INFO(7, L"SequenceGrammar", L"GrammarDef", 76)
				SET_RULE_INFO(8, L"AlternativeGrammar", L"GrammarDef", 78)
				SET_RULE_INFO(9, L"TokenDecl", L"TokenDef", 80)
				SET_RULE_INFO(10, L"RuleDecl", L"RuleDef", 82)
				SET_RULE_INFO(11, L"Grammar", L"GrammarDef", 84)
				SET_RULE_INFO(12, L"RuleFragmentDecl", L"GrammarDef", 86)
				SET_RULE_INFO(13, L"ParserDecl", L"ParserDef", 89)

				BEGIN_TRANSITION_BAG(0, 0)

					BEGIN_TRANSITION_ITEM(0, 1)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 1)

					BEGIN_TRANSITION_ITEM(1, 5)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(13)
					ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 9)

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_STACK_PATTERN(13)
					ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 10)

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_STACK_PATTERN(13)
					ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 11)

					BEGIN_TRANSITION_ITEM(11, 7)
					ITEM_STACK_PATTERN(48)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"parentType", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 12)

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(13)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 13)

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(13)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(13)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 16)

					BEGIN_TRANSITION_ITEM(16, 3)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 1, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"parentType", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 17)

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(13)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 20)

					BEGIN_TRANSITION_ITEM(20, 4)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 1, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"elementType", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 21)

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(13)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 23)

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(13)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 24)

					BEGIN_TRANSITION_ITEM(24, 6)
					ITEM_STACK_PATTERN(69)
					ITEM_INSTRUCTION(Reduce, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_STACK_PATTERN(33)
					ITEM_INSTRUCTION(Reduce, 33, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(3, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SubTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(4, 21)

					BEGIN_TRANSITION_ITEM(21, 2)
					ITEM_INSTRUCTION(Create, 0, L"ArrayTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 13)

					BEGIN_TRANSITION_ITEM(13, 17)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 22)

					BEGIN_TRANSITION_ITEM(22, 18)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 3)

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 5)

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 12)

					BEGIN_TRANSITION_ITEM(12, 19)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 13)

					BEGIN_TRANSITION_ITEM(13, 11)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 17)

					BEGIN_TRANSITION_ITEM(17, 12)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 1)

					BEGIN_TRANSITION_ITEM(1, 23)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 28)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 28)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 2)

					BEGIN_TRANSITION_ITEM(2, 9)
					ITEM_STACK_PATTERN(24)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 9)

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 10)

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 12)

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 13)

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 14)

					BEGIN_TRANSITION_ITEM(14, 22)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 75, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 17)

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 19)

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 21)

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 23)

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 1)

					BEGIN_TRANSITION_ITEM(1, 23)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 28)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 28)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 2)

					BEGIN_TRANSITION_ITEM(2, 10)
					ITEM_STACK_PATTERN(24)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 9)

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 10)

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 12)

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 13)

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 14)

					BEGIN_TRANSITION_ITEM(14, 22)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 75, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 17)

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 19)

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 21)

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 23)

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(85)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 1)

					BEGIN_TRANSITION_ITEM(1, 31)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 32)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 32)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 3)

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 5)

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 6)

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 8)

					BEGIN_TRANSITION_ITEM(8, 33)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 33)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 13, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 11)

					BEGIN_TRANSITION_ITEM(11, 35)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 1)

					BEGIN_TRANSITION_ITEM(1, 36)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 1)

					BEGIN_TRANSITION_ITEM(1, 37)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 3)

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 5)

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 12)

					BEGIN_TRANSITION_ITEM(12, 19)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(18, 24)

					BEGIN_TRANSITION_ITEM(24, 38)
					ITEM_INSTRUCTION(Assign, 0, L"unescapingFunction", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 1)

					BEGIN_TRANSITION_ITEM(1, 39)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 32)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 32)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 40)
					ITEM_STACK_PATTERN(73)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 3)

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 5)

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 6)

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 8)

					BEGIN_TRANSITION_ITEM(8, 33)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 33)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 12)

					BEGIN_TRANSITION_ITEM(12, 19)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(20, 24)

					BEGIN_TRANSITION_ITEM(24, 41)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(21, 24)

					BEGIN_TRANSITION_ITEM(24, 42)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(22, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Assign, 0, L"memberName", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AssignGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 29, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(33, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 33, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(33, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 33, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(34, 24)

					BEGIN_TRANSITION_ITEM(24, 43)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(35, 24)

					BEGIN_TRANSITION_ITEM(24, 44)
					ITEM_INSTRUCTION(Assign, 0, L"memberName", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 23)

					BEGIN_TRANSITION_ITEM(23, 45)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(41, 11)

					BEGIN_TRANSITION_ITEM(11, 46)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(42, 4)

					BEGIN_TRANSITION_ITEM(4, 47)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(42, 11)

					BEGIN_TRANSITION_ITEM(11, 7)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(42, 14)

					BEGIN_TRANSITION_ITEM(14, 48)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(43, 17)

					BEGIN_TRANSITION_ITEM(17, 49)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(44, 17)

					BEGIN_TRANSITION_ITEM(17, 50)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(45, 13)

					BEGIN_TRANSITION_ITEM(13, 17)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(46, 12)

					BEGIN_TRANSITION_ITEM(12, 51)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(46, 24)

					BEGIN_TRANSITION_ITEM(24, 52)
					ITEM_INSTRUCTION(Shift, 46, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 22)

					BEGIN_TRANSITION_ITEM(22, 53)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(48, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(48, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(49, 25)

					BEGIN_TRANSITION_ITEM(25, 54)
					ITEM_INSTRUCTION(Assign, 0, L"regex", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(50, 25)

					BEGIN_TRANSITION_ITEM(25, 55)
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 1)

					BEGIN_TRANSITION_ITEM(1, 56)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 32)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 32)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 40)
					ITEM_STACK_PATTERN(73)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 3)

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 5)

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 6)

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 8)

					BEGIN_TRANSITION_ITEM(8, 33)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 33)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 12)

					BEGIN_TRANSITION_ITEM(12, 19)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(52, 15)

					BEGIN_TRANSITION_ITEM(15, 57)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(53, 24)

					BEGIN_TRANSITION_ITEM(24, 58)
					ITEM_INSTRUCTION(Assign, 0, L"ambiguousType", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(54, 13)

					BEGIN_TRANSITION_ITEM(13, 59)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(55, 12)

					BEGIN_TRANSITION_ITEM(12, 60)
					ITEM_INSTRUCTION(Create, 0, L"SetterGrammarDef", L"", L"Grammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(57, 1)

					BEGIN_TRANSITION_ITEM(1, 61)
					ITEM_INSTRUCTION(Create, 0, L"EnumMemberDef", L"", L"EnumMember");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(57, 12)

					BEGIN_TRANSITION_ITEM(12, 51)
					ITEM_STACK_PATTERN(46)
					ITEM_INSTRUCTION(Create, 0, L"EnumMemberDef", L"", L"EnumMember");
					ITEM_INSTRUCTION(Reduce, 46, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(57, 24)

					BEGIN_TRANSITION_ITEM(24, 52)
					ITEM_STACK_PATTERN(46)
					ITEM_INSTRUCTION(Create, 0, L"EnumMemberDef", L"", L"EnumMember");
					ITEM_INSTRUCTION(Reduce, 46, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 46, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(58, 23)

					BEGIN_TRANSITION_ITEM(23, 62)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 1)

					BEGIN_TRANSITION_ITEM(1, 63)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 32)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 32)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 3)

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 5)

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 6)

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 8)

					BEGIN_TRANSITION_ITEM(8, 33)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 33)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 1)

					BEGIN_TRANSITION_ITEM(1, 15)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 9)

					BEGIN_TRANSITION_ITEM(9, 13)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 10)

					BEGIN_TRANSITION_ITEM(10, 14)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 12)

					BEGIN_TRANSITION_ITEM(12, 9)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 13)

					BEGIN_TRANSITION_ITEM(13, 16)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 17)

					BEGIN_TRANSITION_ITEM(17, 12)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 21)

					BEGIN_TRANSITION_ITEM(21, 9)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 25, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 23)

					BEGIN_TRANSITION_ITEM(23, 10)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 27, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(62, 11)

					BEGIN_TRANSITION_ITEM(11, 7)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(62, 14)

					BEGIN_TRANSITION_ITEM(14, 48)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(64, 0)

					BEGIN_TRANSITION_ITEM(0, 65)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(65, 24)

					BEGIN_TRANSITION_ITEM(24, 52)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(66, 0)

					BEGIN_TRANSITION_ITEM(0, 67)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(67, 5)

					BEGIN_TRANSITION_ITEM(5, 20)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(68, 0)

					BEGIN_TRANSITION_ITEM(0, 69)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(69, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(69, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 69, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(70, 0)

					BEGIN_TRANSITION_ITEM(0, 71)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(71, 3)

					BEGIN_TRANSITION_ITEM(3, 21)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(72, 0)

					BEGIN_TRANSITION_ITEM(0, 73)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(73, 3)

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(73, 5)

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(74, 0)

					BEGIN_TRANSITION_ITEM(0, 75)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(75, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(75, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(75, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(75, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(75, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(75, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(76, 0)

					BEGIN_TRANSITION_ITEM(0, 77)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(78, 0)

					BEGIN_TRANSITION_ITEM(0, 79)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 0)

					BEGIN_TRANSITION_ITEM(0, 81)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(81, 6)

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(81, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(82, 0)

					BEGIN_TRANSITION_ITEM(0, 83)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(83, 8)

					BEGIN_TRANSITION_ITEM(8, 33)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(84, 0)

					BEGIN_TRANSITION_ITEM(0, 85)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(85, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(85, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(85, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(85, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(85, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(85, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(86, 0)

					BEGIN_TRANSITION_ITEM(0, 87)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(87, 17)

					BEGIN_TRANSITION_ITEM(17, 88)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(88, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(88, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(88, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(88, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(88, 24)

					BEGIN_TRANSITION_ITEM(24, 9)
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(88, 25)

					BEGIN_TRANSITION_ITEM(25, 9)
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 85, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(89, 0)

					BEGIN_TRANSITION_ITEM(0, 90)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 3)

					BEGIN_TRANSITION_ITEM(3, 21)
					ITEM_INSTRUCTION(Shift, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 5)

					BEGIN_TRANSITION_ITEM(5, 20)
					ITEM_INSTRUCTION(Shift, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 6)

					BEGIN_TRANSITION_ITEM(6, 34)
					ITEM_INSTRUCTION(Shift, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_INSTRUCTION(Shift, 90, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 8)

					BEGIN_TRANSITION_ITEM(8, 33)
					ITEM_INSTRUCTION(Shift, 90, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				table->Initialize();
				return table;

				#undef SET_TOKEN_INFO
				#undef SET_DISCARD_TOKEN_INFO
				#undef SET_STATE_INFO
				#undef SET_RULE_INFO
				#undef SET_AMBIGUOUS_RULE_INFO
				#undef BEGIN_TRANSITION_BAG
				#undef BEGIN_TRANSITION_ITEM
				#undef END_TRANSITION_ITEM
				#undef END_TRANSITION_BAG
				#undef ITEM_STACK_PATTERN
				#undef ITEM_INSTRUCTION
				#undef BEGIN_LOOK_AHEAD
				#undef LOOK_AHEAD
				#undef END_LOOK_AHEAD
			}

		}
	}
}
