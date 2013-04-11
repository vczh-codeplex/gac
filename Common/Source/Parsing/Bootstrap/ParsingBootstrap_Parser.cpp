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
				#define ITEM_INSTRUCTION(TYPE, STATE, NAME, VALUE) item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::TYPE, STATE, NAME, VALUE));
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
				SET_STATE_INFO(6, L"Class", L"Class.5", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\"¡ñ { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef\r\n<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" ¡ñ{ ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(7, L"ClassMember", L"ClassMember.2", L"<ClassMember>: Type : type NAME : name¡ñ [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef")
				SET_STATE_INFO(8, L"PrimitiveGrammar", L"PrimitiveGrammar.1", L"<PrimitiveGrammar>: NAME : name as PrimitiveGrammarDef¡ñ\r\n<PrimitiveGrammar>: STRING : text as TextGrammarDef¡ñ\r\n<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\" NAME : memberName as AssignGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"!\" PrimitiveGrammar : grammar as UseGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"[\" Grammar : grammar \"]\" as OptionalGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"{\" Grammar : grammar \"}\" as LoopGrammarDef¡ñ")
				SET_STATE_INFO(9, L"PrimitiveGrammar", L"PrimitiveGrammar.7", L"<PrimitiveGrammar>: NAME : name as PrimitiveGrammarDef¡ñ\r\n<PrimitiveGrammar>: STRING : text as TextGrammarDef¡ñ\r\n<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\" NAME : memberName as AssignGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"!\" PrimitiveGrammar : grammar as UseGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"[\" Grammar : grammar \"]\" as OptionalGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"{\" Grammar : grammar \"}\" as LoopGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"(\" !Grammar \")\"¡ñ")
				SET_STATE_INFO(10, L"Grammar", L"Grammar.4", L"<Grammar>: Grammar : grammar \"as\"¡ñ Type : type as CreateGrammarDef")
				SET_STATE_INFO(11, L"Grammar", L"Grammar.5", L"<Grammar>: Grammar : grammar \"with\"¡ñ \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(12, L"Grammar", L"Grammar.RootEnd", L"$<Grammar> ¡ñ")
				SET_STATE_INFO(13, L"RuleFragmentDecl", L"RuleFragmentDecl.3", L"<RuleFragmentDecl>: \"=\" !Grammar \";\"¡ñ")
				SET_STATE_INFO(14, L"RuleDecl", L"RuleDecl.4", L"<RuleDecl>: \"rule\" Type : type NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef¡ñ")
				SET_STATE_INFO(15, L"RuleDecl", L"RuleDecl.5", L"<RuleDecl>: \"rule\" Type : type NAME : name { \"=\"¡ñ Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(16, L"RuleDecl", L"RuleDecl.3", L"<RuleDecl>: \"rule\" Type : type NAME : name¡ñ { \"=\" Grammar : grammars } \";\" as RuleDef\r\n<RuleDecl>: \"rule\" Type : type NAME : name ¡ñ{ \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(17, L"Class", L"Class.8", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef¡ñ")
				SET_STATE_INFO(18, L"Enum", L"Enum.1", L"<Enum>: \"enum\"¡ñ NAME : name \"{\" { EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(19, L"Class", L"Class.1", L"<Class>: \"class\"¡ñ NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(20, L"ClassMember", L"ClassMember.4", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef¡ñ")
				SET_STATE_INFO(21, L"ClassMember", L"ClassMember.3", L"<ClassMember>: Type : type NAME : name [ \"(\"¡ñ NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef")
				SET_STATE_INFO(22, L"PrimitiveGrammar", L"PrimitiveGrammar.8", L"<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\"¡ñ NAME : memberName as AssignGrammarDef")
				SET_STATE_INFO(23, L"PrimitiveGrammar", L"PrimitiveGrammar.RootEnd", L"$<PrimitiveGrammar> ¡ñ")
				SET_STATE_INFO(24, L"PrimitiveGrammar", L"PrimitiveGrammar.3", L"<PrimitiveGrammar>: \"!\"¡ñ PrimitiveGrammar : grammar as UseGrammarDef")
				SET_STATE_INFO(25, L"PrimitiveGrammar", L"PrimitiveGrammar.4", L"<PrimitiveGrammar>: \"[\"¡ñ Grammar : grammar \"]\" as OptionalGrammarDef")
				SET_STATE_INFO(26, L"PrimitiveGrammar", L"PrimitiveGrammar.5", L"<PrimitiveGrammar>: \"{\"¡ñ Grammar : grammar \"}\" as LoopGrammarDef")
				SET_STATE_INFO(27, L"PrimitiveGrammar", L"PrimitiveGrammar.6", L"<PrimitiveGrammar>: \"(\"¡ñ !Grammar \")\"")
				SET_STATE_INFO(28, L"SequenceGrammar", L"SequenceGrammar.RootEnd", L"$<SequenceGrammar> ¡ñ")
				SET_STATE_INFO(29, L"AlternativeGrammar", L"AlternativeGrammar.3", L"<AlternativeGrammar>: AlternativeGrammar : first \"|\"¡ñ SequenceGrammar : second as AlternativeGrammarDef")
				SET_STATE_INFO(30, L"AlternativeGrammar", L"AlternativeGrammar.RootEnd", L"$<AlternativeGrammar> ¡ñ")
				SET_STATE_INFO(31, L"Grammar", L"Grammar.7", L"<Grammar>: Grammar : grammar \"with\" \"{\"¡ñ NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(32, L"RuleFragmentDecl", L"RuleFragmentDecl.RootEnd", L"$<RuleFragmentDecl> ¡ñ")
				SET_STATE_INFO(33, L"RuleDecl", L"RuleDecl.RootEnd", L"$<RuleDecl> ¡ñ")
				SET_STATE_INFO(34, L"ParserDecl", L"ParserDecl.RootEnd", L"$<ParserDecl> ¡ñ")
				SET_STATE_INFO(35, L"RuleDecl", L"RuleDecl.1", L"<RuleDecl>: \"rule\"¡ñ Type : type NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(36, L"TokenDecl", L"TokenDecl.1", L"<TokenDecl>: \"token\"¡ñ NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\"¡ñ NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(37, L"Class", L"Class.RootEnd", L"$<Class> ¡ñ")
				SET_STATE_INFO(38, L"TypeDecl", L"TypeDecl.RootEnd", L"$<TypeDecl> ¡ñ")
				SET_STATE_INFO(39, L"Enum", L"Enum.2", L"<Enum>: \"enum\" NAME : name¡ñ \"{\" { EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(40, L"Class", L"Class.2", L"<Class>: \"class\" NAME : name¡ñ [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(41, L"ClassMember", L"ClassMember.RootEnd", L"$<ClassMember> ¡ñ")
				SET_STATE_INFO(42, L"ClassMember", L"ClassMember.5", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction¡ñ \")\" ] \";\" as ClassMemberDef")
				SET_STATE_INFO(43, L"Grammar", L"Grammar.8", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName¡ñ \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(44, L"TokenDecl", L"TokenDecl.2", L"<TokenDecl>: \"token\" NAME : name¡ñ \"=\" STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name¡ñ \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(45, L"Enum", L"Enum.3", L"<Enum>: \"enum\" NAME : name \"{\"¡ñ { EnumMember : members } \"}\" as EnumTypeDef\r\n<Enum>: \"enum\" NAME : name \"{\" ¡ñ{ EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(46, L"Class", L"Class.3", L"<Class>: \"class\" NAME : name [ \"ambiguous\"¡ñ \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(47, L"Class", L"Class.4", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\"¡ñ Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(48, L"ClassMember", L"ClassMember.7", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ]¡ñ \";\" as ClassMemberDef")
				SET_STATE_INFO(49, L"Grammar", L"Grammar.9", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\"¡ñ STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(50, L"TokenDecl", L"TokenDecl.3", L"<TokenDecl>: \"token\" NAME : name \"=\"¡ñ STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\"¡ñ STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(51, L"Enum", L"Enum.4", L"<Enum>: \"enum\" NAME : name \"{\" { EnumMember : members } \"}\" as EnumTypeDef¡ñ")
				SET_STATE_INFO(52, L"EnumMember", L"EnumMember.1", L"<EnumMember>: NAME : name¡ñ \",\" as EnumMemberDef")
				SET_STATE_INFO(53, L"Class", L"Class.6", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\"¡ñ NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(54, L"Grammar", L"Grammar.10", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value¡ñ \"}\" as SetterGrammarDef")
				SET_STATE_INFO(55, L"TokenDecl", L"TokenDecl.4", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex¡ñ \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex¡ñ \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(56, L"Enum", L"Enum.RootEnd", L"$<Enum> ¡ñ")
				SET_STATE_INFO(57, L"EnumMember", L"EnumMember.2", L"<EnumMember>: NAME : name \",\" as EnumMemberDef¡ñ")
				SET_STATE_INFO(58, L"Class", L"Class.9", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType¡ñ \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(59, L"Grammar", L"Grammar.6", L"<Grammar>: Grammar : grammar \"as\" Type : type as CreateGrammarDef¡ñ\r\n<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef¡ñ")
				SET_STATE_INFO(60, L"TokenDecl", L"TokenDecl.5", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex \";\" as TokenDef¡ñ with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex \";\" as TokenDef¡ñ with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(61, L"EnumMember", L"EnumMember.RootEnd", L"$<EnumMember> ¡ñ")
				SET_STATE_INFO(62, L"Class", L"Class.11", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ]¡ñ [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(63, L"TokenDecl", L"TokenDecl.RootEnd", L"$<TokenDecl> ¡ñ")
				SET_STATE_INFO(64, L"EnumMember", L"EnumMember.RootStart", L"¡ñ $<EnumMember>")
				SET_STATE_INFO(65, L"EnumMember", L"EnumMember.Start", L"¡¤ <EnumMember>")
				SET_STATE_INFO(66, L"Class", L"Class.RootStart", L"¡ñ $<Class>")
				SET_STATE_INFO(67, L"Class", L"Class.Start", L"¡¤ <Class>")
				SET_STATE_INFO(68, L"Enum", L"Enum.RootStart", L"¡ñ $<Enum>")
				SET_STATE_INFO(69, L"Enum", L"Enum.Start", L"¡¤ <Enum>")
				SET_STATE_INFO(70, L"ClassMember", L"ClassMember.RootStart", L"¡ñ $<ClassMember>")
				SET_STATE_INFO(71, L"ClassMember", L"ClassMember.Start", L"¡¤ <ClassMember>")
				SET_STATE_INFO(72, L"TypeDecl", L"TypeDecl.RootStart", L"¡ñ $<TypeDecl>")
				SET_STATE_INFO(73, L"TypeDecl", L"TypeDecl.Start", L"¡¤ <TypeDecl>")
				SET_STATE_INFO(74, L"PrimitiveGrammar", L"PrimitiveGrammar.RootStart", L"¡ñ $<PrimitiveGrammar>")
				SET_STATE_INFO(75, L"PrimitiveGrammar", L"PrimitiveGrammar.Start", L"¡¤ <PrimitiveGrammar>")
				SET_STATE_INFO(76, L"SequenceGrammar", L"SequenceGrammar.RootStart", L"¡ñ $<SequenceGrammar>")
				SET_STATE_INFO(77, L"SequenceGrammar", L"SequenceGrammar.Start", L"¡¤ <SequenceGrammar>")
				SET_STATE_INFO(78, L"AlternativeGrammar", L"AlternativeGrammar.RootStart", L"¡ñ $<AlternativeGrammar>")
				SET_STATE_INFO(79, L"AlternativeGrammar", L"AlternativeGrammar.Start", L"¡¤ <AlternativeGrammar>")
				SET_STATE_INFO(80, L"Grammar", L"Grammar.RootStart", L"¡ñ $<Grammar>")
				SET_STATE_INFO(81, L"Grammar", L"Grammar.Start", L"¡¤ <Grammar>")
				SET_STATE_INFO(82, L"TokenDecl", L"TokenDecl.RootStart", L"¡ñ $<TokenDecl>")
				SET_STATE_INFO(83, L"TokenDecl", L"TokenDecl.Start", L"¡¤ <TokenDecl>")
				SET_STATE_INFO(84, L"RuleFragmentDecl", L"RuleFragmentDecl.RootStart", L"¡ñ $<RuleFragmentDecl>")
				SET_STATE_INFO(85, L"RuleFragmentDecl", L"RuleFragmentDecl.Start", L"¡¤ <RuleFragmentDecl>")
				SET_STATE_INFO(86, L"RuleFragmentDecl", L"RuleFragmentDecl.1", L"<RuleFragmentDecl>: \"=\"¡ñ !Grammar \";\"")
				SET_STATE_INFO(87, L"RuleDecl", L"RuleDecl.RootStart", L"¡ñ $<RuleDecl>")
				SET_STATE_INFO(88, L"RuleDecl", L"RuleDecl.Start", L"¡¤ <RuleDecl>")
				SET_STATE_INFO(89, L"ParserDecl", L"ParserDecl.RootStart", L"¡ñ $<ParserDecl>")
				SET_STATE_INFO(90, L"ParserDecl", L"ParserDecl.Start", L"¡¤ <ParserDecl>")
				SET_STATE_INFO(91, L"Type", L"Type.2", L"<Type>: Type : parentType¡ñ \".\" NAME : name as SubTypeObj")
				SET_STATE_INFO(92, L"Type", L"Type.3", L"<Type>: Type : elementType¡ñ \"[\" \"]\" as ArrayTypeObj")
				SET_STATE_INFO(93, L"Type", L"Type.4", L"<Type>: NAME : name as PrimitiveTypeObj¡ñ\r\n<Type>: \"token\" as TokenTypeObj¡ñ\r\n<Type>: Type : parentType \".\" NAME : name as SubTypeObj¡ñ\r\n<Type>: Type : elementType \"[\" \"]\" as ArrayTypeObj¡ñ")
				SET_STATE_INFO(94, L"EnumMember", L"EnumMember.3", L"<EnumMember>: NAME : name \",\" as EnumMemberDef¡ñ")
				SET_STATE_INFO(95, L"Class", L"Class.7", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ]¡ñ \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(96, L"Class", L"Class.10", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef¡ñ")
				SET_STATE_INFO(97, L"Enum", L"Enum.5", L"<Enum>: \"enum\" NAME : name \"{\" { EnumMember : members } \"}\" as EnumTypeDef¡ñ")
				SET_STATE_INFO(98, L"ClassMember", L"ClassMember.1", L"<ClassMember>: Type : type¡ñ NAME : name [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef")
				SET_STATE_INFO(99, L"ClassMember", L"ClassMember.6", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] \";\" as ClassMemberDef¡ñ")
				SET_STATE_INFO(100, L"TypeDecl", L"TypeDecl.1", L"<TypeDecl>: !Enum | !Class¡ñ")
				SET_STATE_INFO(101, L"PrimitiveGrammar", L"PrimitiveGrammar.2", L"<PrimitiveGrammar>: PrimitiveGrammar : grammar¡ñ \":\" NAME : memberName as AssignGrammarDef")
				SET_STATE_INFO(102, L"PrimitiveGrammar", L"PrimitiveGrammar.9", L"<PrimitiveGrammar>: \"[\" Grammar : grammar¡ñ \"]\" as OptionalGrammarDef")
				SET_STATE_INFO(103, L"PrimitiveGrammar", L"PrimitiveGrammar.10", L"<PrimitiveGrammar>: \"{\" Grammar : grammar¡ñ \"}\" as LoopGrammarDef")
				SET_STATE_INFO(104, L"PrimitiveGrammar", L"PrimitiveGrammar.11", L"<PrimitiveGrammar>: \"(\" !Grammar¡ñ \")\"")
				SET_STATE_INFO(105, L"SequenceGrammar", L"SequenceGrammar.1", L"<SequenceGrammar>: !PrimitiveGrammar¡ñ\r\n<SequenceGrammar>: SequenceGrammar : first PrimitiveGrammar : second as SequenceGrammarDef¡ñ")
				SET_STATE_INFO(106, L"SequenceGrammar", L"SequenceGrammar.2", L"<SequenceGrammar>: SequenceGrammar : first¡ñ PrimitiveGrammar : second as SequenceGrammarDef")
				SET_STATE_INFO(107, L"SequenceGrammar", L"SequenceGrammar.3", L"<SequenceGrammar>: SequenceGrammar : first PrimitiveGrammar : second as SequenceGrammarDef¡ñ")
				SET_STATE_INFO(108, L"AlternativeGrammar", L"AlternativeGrammar.1", L"<AlternativeGrammar>: !SequenceGrammar¡ñ\r\n<AlternativeGrammar>: AlternativeGrammar : first \"|\" SequenceGrammar : second as AlternativeGrammarDef¡ñ")
				SET_STATE_INFO(109, L"AlternativeGrammar", L"AlternativeGrammar.2", L"<AlternativeGrammar>: AlternativeGrammar : first¡ñ \"|\" SequenceGrammar : second as AlternativeGrammarDef")
				SET_STATE_INFO(110, L"AlternativeGrammar", L"AlternativeGrammar.4", L"<AlternativeGrammar>: AlternativeGrammar : first \"|\" SequenceGrammar : second as AlternativeGrammarDef¡ñ")
				SET_STATE_INFO(111, L"Grammar", L"Grammar.1", L"<Grammar>: !AlternativeGrammar¡ñ\r\n<Grammar>: Grammar : grammar \"as\" Type : type as CreateGrammarDef¡ñ\r\n<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef¡ñ")
				SET_STATE_INFO(112, L"Grammar", L"Grammar.2", L"<Grammar>: Grammar : grammar¡ñ \"as\" Type : type as CreateGrammarDef")
				SET_STATE_INFO(113, L"Grammar", L"Grammar.3", L"<Grammar>: Grammar : grammar¡ñ \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(114, L"TokenDecl", L"TokenDecl.6", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"KeepToken\" }¡ñ\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }¡ñ")
				SET_STATE_INFO(115, L"RuleFragmentDecl", L"RuleFragmentDecl.2", L"<RuleFragmentDecl>: \"=\" !Grammar¡ñ \";\"")
				SET_STATE_INFO(116, L"RuleDecl", L"RuleDecl.2", L"<RuleDecl>: \"rule\" Type : type¡ñ NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(117, L"RuleDecl", L"RuleDecl.6", L"<RuleDecl>: \"rule\" Type : type NAME : name { \"=\" Grammar : grammars } \";\" as RuleDef¡ñ")
				SET_STATE_INFO(118, L"ParserDecl", L"ParserDecl.1", L"<ParserDecl>: { TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef¡ñ\r\n<ParserDecl>: ¡ñ{ TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef")
				SET_STATE_INFO(119, L"ParserDecl", L"ParserDecl.2", L"<ParserDecl>: { TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef¡ñ")

				SET_RULE_INFO(0, L"Type", L"TypeObj", 0)
				SET_RULE_INFO(1, L"EnumMember", L"EnumMemberDef", 64)
				SET_RULE_INFO(2, L"Class", L"ClassTypeDef", 66)
				SET_RULE_INFO(3, L"Enum", L"EnumTypeDef", 68)
				SET_RULE_INFO(4, L"ClassMember", L"ClassMemberDef", 70)
				SET_RULE_INFO(5, L"TypeDecl", L"TypeDef", 72)
				SET_RULE_INFO(6, L"PrimitiveGrammar", L"GrammarDef", 74)
				SET_RULE_INFO(7, L"SequenceGrammar", L"GrammarDef", 76)
				SET_RULE_INFO(8, L"AlternativeGrammar", L"GrammarDef", 78)
				SET_RULE_INFO(9, L"Grammar", L"GrammarDef", 80)
				SET_RULE_INFO(10, L"TokenDecl", L"TokenDef", 82)
				SET_RULE_INFO(11, L"RuleFragmentDecl", L"GrammarDef", 84)
				SET_RULE_INFO(12, L"RuleDecl", L"RuleDef", 87)
				SET_RULE_INFO(13, L"ParserDecl", L"ParserDef", 89)

				BEGIN_TRANSITION_BAG(0, 0)

					BEGIN_TRANSITION_ITEM(0, 1)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 1)

					BEGIN_TRANSITION_ITEM(1, 5)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 12)
					ITEM_STACK_PATTERN(10)
					ITEM_INSTRUCTION(Reduce, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(10)
					ITEM_INSTRUCTION(Reduce, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 10)

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_STACK_PATTERN(10)
					ITEM_INSTRUCTION(Reduce, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 11)

					BEGIN_TRANSITION_ITEM(11, 6)
					ITEM_STACK_PATTERN(47)
					ITEM_INSTRUCTION(Reduce, 47, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"parentType", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 12)

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(10)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 13)

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(10)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(10)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 16)

					BEGIN_TRANSITION_ITEM(16, 3)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 1, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"parentType", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 17)

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(10)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 20)

					BEGIN_TRANSITION_ITEM(20, 4)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 1, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"elementType", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 21)

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(10)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 23)

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(10)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 24)

					BEGIN_TRANSITION_ITEM(24, 7)
					ITEM_STACK_PATTERN(71)
					ITEM_INSTRUCTION(Reduce, 71, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(24, 16)
					ITEM_STACK_PATTERN(35)
					ITEM_INSTRUCTION(Reduce, 35, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(3, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"SubTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(4, 21)

					BEGIN_TRANSITION_ITEM(21, 2)
					ITEM_INSTRUCTION(Create, 0, L"ArrayTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 3)

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 5)

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 12)

					BEGIN_TRANSITION_ITEM(12, 17)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 13)

					BEGIN_TRANSITION_ITEM(13, 20)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 22)

					BEGIN_TRANSITION_ITEM(22, 21)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 1)

					BEGIN_TRANSITION_ITEM(1, 23)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 12)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 12)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 28)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 12)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 12)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 28)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 2)

					BEGIN_TRANSITION_ITEM(2, 8)
					ITEM_STACK_PATTERN(24)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 10)

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 12)

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 13)

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 14)

					BEGIN_TRANSITION_ITEM(14, 22)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 75, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 17)

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 19)

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 21)

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 23)

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 1)

					BEGIN_TRANSITION_ITEM(1, 23)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 12)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 12)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 28)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 12)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 12)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 30)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 28)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 2)

					BEGIN_TRANSITION_ITEM(2, 9)
					ITEM_STACK_PATTERN(24)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 10)

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 12)

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 13)

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 14)

					BEGIN_TRANSITION_ITEM(14, 22)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 75, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 17)

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 19)

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 29)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 79, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 21)

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 23)

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(106)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(79)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(77)
					ITEM_STACK_PATTERN(29)
					ITEM_STACK_PATTERN(81)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 29, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"");
					ITEM_INSTRUCTION(Reduce, 81, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_STACK_PATTERN(106)
					ITEM_INSTRUCTION(Reduce, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_STACK_PATTERN(77)
					ITEM_INSTRUCTION(Reduce, 77, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"");
					ITEM_INSTRUCTION(Shift, 106, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 10, L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 10, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 11)

					BEGIN_TRANSITION_ITEM(11, 31)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 1)

					BEGIN_TRANSITION_ITEM(1, 32)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 1)

					BEGIN_TRANSITION_ITEM(1, 33)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 34)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 34)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 3)

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 5)

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 6)

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 7)

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 8)

					BEGIN_TRANSITION_ITEM(8, 35)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 35)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 15, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 15, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 15, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 15, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 15, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 15, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 13)

					BEGIN_TRANSITION_ITEM(13, 14)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 17)

					BEGIN_TRANSITION_ITEM(17, 15)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 1)

					BEGIN_TRANSITION_ITEM(1, 37)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 38)
					ITEM_STACK_PATTERN(73)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 3)

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 5)

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 6)

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 7)

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 8)

					BEGIN_TRANSITION_ITEM(8, 35)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 35)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 12)

					BEGIN_TRANSITION_ITEM(12, 17)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(18, 24)

					BEGIN_TRANSITION_ITEM(24, 39)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 24)

					BEGIN_TRANSITION_ITEM(24, 40)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(20, 1)

					BEGIN_TRANSITION_ITEM(1, 41)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(20, 3)

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(20, 5)

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(20, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(20, 12)

					BEGIN_TRANSITION_ITEM(12, 17)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(20, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(21, 24)

					BEGIN_TRANSITION_ITEM(24, 42)
					ITEM_INSTRUCTION(Assign, 0, L"unescapingFunction", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(22, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Assign, 0, L"memberName", L"");
					ITEM_INSTRUCTION(Create, 0, L"AssignGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 24, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 24, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 24, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 24, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 24, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 24, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 25, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 25, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 25, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 25, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 25, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 25, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 26, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 26, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 26, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 26, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 26, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 26, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 27, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 27, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 27, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 27, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 27, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 27, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 29, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 29, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 29, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 29, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 29, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 29, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(31, 24)

					BEGIN_TRANSITION_ITEM(24, 43)
					ITEM_INSTRUCTION(Assign, 0, L"memberName", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(35, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 35, L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(35, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 35, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 24)

					BEGIN_TRANSITION_ITEM(24, 44)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(39, 11)

					BEGIN_TRANSITION_ITEM(11, 45)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(40, 4)

					BEGIN_TRANSITION_ITEM(4, 46)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(40, 11)

					BEGIN_TRANSITION_ITEM(11, 6)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(40, 14)

					BEGIN_TRANSITION_ITEM(14, 47)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(42, 23)

					BEGIN_TRANSITION_ITEM(23, 48)
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

				BEGIN_TRANSITION_BAG(45, 12)

					BEGIN_TRANSITION_ITEM(12, 51)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(45, 24)

					BEGIN_TRANSITION_ITEM(24, 52)
					ITEM_INSTRUCTION(Shift, 45, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(46, 22)

					BEGIN_TRANSITION_ITEM(22, 53)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 47, L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 47, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(48, 13)

					BEGIN_TRANSITION_ITEM(13, 20)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(49, 25)

					BEGIN_TRANSITION_ITEM(25, 54)
					ITEM_INSTRUCTION(Assign, 0, L"value", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(50, 25)

					BEGIN_TRANSITION_ITEM(25, 55)
					ITEM_INSTRUCTION(Assign, 0, L"regex", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 1)

					BEGIN_TRANSITION_ITEM(1, 56)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 34)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 38)
					ITEM_STACK_PATTERN(73)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 3)

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 5)

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 6)

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 7)

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 8)

					BEGIN_TRANSITION_ITEM(8, 35)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 35)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 12)

					BEGIN_TRANSITION_ITEM(12, 17)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_STACK_PATTERN(73)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"");
					ITEM_INSTRUCTION(Shift, 6, L"", L"");
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(52, 15)

					BEGIN_TRANSITION_ITEM(15, 57)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(53, 24)

					BEGIN_TRANSITION_ITEM(24, 58)
					ITEM_INSTRUCTION(Assign, 0, L"ambiguousType", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(54, 12)

					BEGIN_TRANSITION_ITEM(12, 59)
					ITEM_INSTRUCTION(Create, 0, L"SetterGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(55, 13)

					BEGIN_TRANSITION_ITEM(13, 60)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(57, 1)

					BEGIN_TRANSITION_ITEM(1, 61)
					ITEM_INSTRUCTION(Create, 0, L"EnumMemberDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(57, 12)

					BEGIN_TRANSITION_ITEM(12, 51)
					ITEM_STACK_PATTERN(45)
					ITEM_INSTRUCTION(Create, 0, L"EnumMemberDef", L"");
					ITEM_INSTRUCTION(Reduce, 45, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(57, 24)

					BEGIN_TRANSITION_ITEM(24, 52)
					ITEM_STACK_PATTERN(45)
					ITEM_INSTRUCTION(Create, 0, L"EnumMemberDef", L"");
					ITEM_INSTRUCTION(Reduce, 45, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"");
					ITEM_INSTRUCTION(Shift, 45, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(58, 23)

					BEGIN_TRANSITION_ITEM(23, 62)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 1)

					BEGIN_TRANSITION_ITEM(1, 12)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 10)

					BEGIN_TRANSITION_ITEM(10, 11)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 81, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 12)

					BEGIN_TRANSITION_ITEM(12, 8)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Reduce, 26, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 13)

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(86)
					ITEM_INSTRUCTION(Reduce, 86, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 14)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 17)

					BEGIN_TRANSITION_ITEM(17, 15)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 15, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 21)

					BEGIN_TRANSITION_ITEM(21, 8)
					ITEM_STACK_PATTERN(25)
					ITEM_INSTRUCTION(Reduce, 25, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(59, 23)

					BEGIN_TRANSITION_ITEM(23, 9)
					ITEM_STACK_PATTERN(27)
					ITEM_INSTRUCTION(Reduce, 27, L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 1)

					BEGIN_TRANSITION_ITEM(1, 63)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 34)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 34)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 3)

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 5)

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 6)

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 7)

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(60, 8)

					BEGIN_TRANSITION_ITEM(8, 35)
					ITEM_STACK_PATTERN(118)
					ITEM_INSTRUCTION(Reduce, 118, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 35)
					ITEM_STACK_PATTERN(90)
					ITEM_INSTRUCTION(Reduce, 90, L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"");
					ITEM_INSTRUCTION(Shift, 118, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(62, 11)

					BEGIN_TRANSITION_ITEM(11, 6)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(62, 14)

					BEGIN_TRANSITION_ITEM(14, 47)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(64, 0)

					BEGIN_TRANSITION_ITEM(0, 65)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(65, 24)

					BEGIN_TRANSITION_ITEM(24, 52)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(66, 0)

					BEGIN_TRANSITION_ITEM(0, 67)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(67, 3)

					BEGIN_TRANSITION_ITEM(3, 19)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(68, 0)

					BEGIN_TRANSITION_ITEM(0, 69)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(69, 5)

					BEGIN_TRANSITION_ITEM(5, 18)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(70, 0)

					BEGIN_TRANSITION_ITEM(0, 71)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(71, 6)

					BEGIN_TRANSITION_ITEM(6, 2)
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(71, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 71, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(72, 0)

					BEGIN_TRANSITION_ITEM(0, 73)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(73, 3)

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(73, 5)

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
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

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(75, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(76, 0)

					BEGIN_TRANSITION_ITEM(0, 77)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(78, 0)

					BEGIN_TRANSITION_ITEM(0, 79)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 0)

					BEGIN_TRANSITION_ITEM(0, 81)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(81, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(81, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(81, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(81, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(81, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(81, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(82, 0)

					BEGIN_TRANSITION_ITEM(0, 83)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(83, 6)

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(83, 7)

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(84, 0)

					BEGIN_TRANSITION_ITEM(0, 85)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(85, 17)

					BEGIN_TRANSITION_ITEM(17, 86)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(86, 11)

					BEGIN_TRANSITION_ITEM(11, 26)
					ITEM_INSTRUCTION(Shift, 86, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(86, 18)

					BEGIN_TRANSITION_ITEM(18, 24)
					ITEM_INSTRUCTION(Shift, 86, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(86, 20)

					BEGIN_TRANSITION_ITEM(20, 25)
					ITEM_INSTRUCTION(Shift, 86, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(86, 22)

					BEGIN_TRANSITION_ITEM(22, 27)
					ITEM_INSTRUCTION(Shift, 86, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(86, 24)

					BEGIN_TRANSITION_ITEM(24, 8)
					ITEM_INSTRUCTION(Shift, 86, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(86, 25)

					BEGIN_TRANSITION_ITEM(25, 8)
					ITEM_INSTRUCTION(Shift, 86, L"", L"");
					ITEM_INSTRUCTION(Shift, 81, L"", L"");
					ITEM_INSTRUCTION(Shift, 79, L"", L"");
					ITEM_INSTRUCTION(Shift, 77, L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(87, 0)

					BEGIN_TRANSITION_ITEM(0, 88)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(88, 8)

					BEGIN_TRANSITION_ITEM(8, 35)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(89, 0)

					BEGIN_TRANSITION_ITEM(0, 90)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 3)

					BEGIN_TRANSITION_ITEM(3, 19)
					ITEM_INSTRUCTION(Shift, 90, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 5)

					BEGIN_TRANSITION_ITEM(5, 18)
					ITEM_INSTRUCTION(Shift, 90, L"", L"");
					ITEM_INSTRUCTION(Shift, 73, L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 6)

					BEGIN_TRANSITION_ITEM(6, 36)
					ITEM_INSTRUCTION(Shift, 90, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 7)

					BEGIN_TRANSITION_ITEM(7, 36)
					ITEM_INSTRUCTION(Shift, 90, L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 8)

					BEGIN_TRANSITION_ITEM(8, 35)
					ITEM_INSTRUCTION(Shift, 90, L"", L"");
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
