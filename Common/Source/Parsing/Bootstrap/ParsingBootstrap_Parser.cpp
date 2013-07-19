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
				vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(27-vl::parsing::tabling::ParsingTable::UserTokenStart, 1, 147, 15);
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
				#define BEGIN_LOOK_AHEAD(STATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo> lookAheadInfo=new vl::parsing::tabling::ParsingTable::LookAheadInfo; item->lookAheads.Add(lookAheadInfo); lookAheadInfo->state=STATE;
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
				SET_TOKEN_INFO(24, L"ATT", L"@")
				SET_TOKEN_INFO(25, L"NAME", L"[a-zA-Z_]/w*")
				SET_TOKEN_INFO(26, L"STRING", L"\"([^\"]|\"\")*\"")

				SET_DISCARD_TOKEN_INFO(0, L"SPACE", L"/s+")

				SET_STATE_INFO(0, L"Attribute", L"Attribute.RootStart", L"¡ñ $<Attribute>")
				SET_STATE_INFO(1, L"Attribute", L"Attribute.Start", L"¡¤ <Attribute>")
				SET_STATE_INFO(2, L"Attribute", L"Attribute.1", L"<Attribute>: \"@\"¡ñ NAME : name \"(\" [ STRING : arguments { \",\" STRING : arguments } ] \")\" as AttributeDef")
				SET_STATE_INFO(3, L"Attribute", L"Attribute.2", L"<Attribute>: \"@\" NAME : name¡ñ \"(\" [ STRING : arguments { \",\" STRING : arguments } ] \")\" as AttributeDef")
				SET_STATE_INFO(4, L"Attribute", L"Attribute.3", L"<Attribute>: \"@\" NAME : name \"(\"¡ñ [ STRING : arguments { \",\" STRING : arguments } ] \")\" as AttributeDef")
				SET_STATE_INFO(5, L"Attribute", L"Attribute.4", L"<Attribute>: \"@\" NAME : name \"(\" [ STRING : arguments¡ñ { \",\" STRING : arguments } ] \")\" as AttributeDef\r\n<Attribute>: \"@\" NAME : name \"(\" [ STRING : arguments ¡ñ{ \",\" STRING : arguments } ] \")\" as AttributeDef")
				SET_STATE_INFO(6, L"Attribute", L"Attribute.5", L"<Attribute>: \"@\" NAME : name \"(\" [ STRING : arguments { \",\" STRING : arguments } ] \")\" as AttributeDef¡ñ")
				SET_STATE_INFO(7, L"Attribute", L"Attribute.6", L"<Attribute>: \"@\" NAME : name \"(\" [ STRING : arguments { \",\"¡ñ STRING : arguments } ] \")\" as AttributeDef")
				SET_STATE_INFO(8, L"Attribute", L"Attribute.RootEnd", L"$<Attribute> ¡ñ")
				SET_STATE_INFO(9, L"EnumMember", L"EnumMember.3", L"<EnumMember>: NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] \",\" as EnumMemberDef¡ñ")
				SET_STATE_INFO(10, L"EnumMember", L"EnumMember.4", L"<EnumMember>: NAME : name [ Attribute : attributes { \",\"¡ñ Attribute : attributes } ] \",\" as EnumMemberDef")
				SET_STATE_INFO(11, L"Enum", L"Enum.4", L"<Enum>: \"enum\" NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\"¡ñ { EnumMember : members } \"}\" as EnumTypeDef\r\n<Enum>: \"enum\" NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" ¡ñ{ EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(12, L"Enum", L"Enum.5", L"<Enum>: \"enum\" NAME : name [ Attribute : attributes { \",\"¡ñ Attribute : attributes } ] \"{\" { EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(13, L"ClassMember", L"ClassMember.5", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as ClassMemberDef¡ñ")
				SET_STATE_INFO(14, L"ClassMember", L"ClassMember.7", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] [ Attribute : attributes { \",\"¡ñ Attribute : attributes } ] \";\" as ClassMemberDef")
				SET_STATE_INFO(15, L"Class", L"Class.6", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\"¡ñ { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef\r\n<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" ¡ñ{ ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(16, L"Class", L"Class.9", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\"¡ñ Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(17, L"TokenDecl", L"TokenDecl.10", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as TokenDef¡ñ with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex \";\" as TokenDef¡ñ with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(18, L"TokenDecl", L"TokenDecl.11", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex [ Attribute : attributes { \",\"¡ñ Attribute : attributes } ] \";\" as TokenDef with { discard = \"KeepToken\" }")
				SET_STATE_INFO(19, L"RuleDecl", L"RuleDecl.5", L"<RuleDecl>: \"rule\" Type : type NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] { \"=\" Grammar : grammars } \";\" as RuleDef¡ñ")
				SET_STATE_INFO(20, L"RuleDecl", L"RuleDecl.7", L"<RuleDecl>: \"rule\" Type : type NAME : name [ Attribute : attributes { \",\"¡ñ Attribute : attributes } ] { \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(21, L"RuleDecl", L"RuleDecl.6", L"<RuleDecl>: \"rule\" Type : type NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] { \"=\"¡ñ Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(22, L"EnumMember", L"EnumMember.RootEnd", L"$<EnumMember> ¡ñ")
				SET_STATE_INFO(23, L"EnumMember", L"EnumMember.1", L"<EnumMember>: NAME : name¡ñ [ Attribute : attributes { \",\" Attribute : attributes } ] \",\" as EnumMemberDef")
				SET_STATE_INFO(24, L"Enum", L"Enum.6", L"<Enum>: \"enum\" NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { EnumMember : members } \"}\" as EnumTypeDef¡ñ")
				SET_STATE_INFO(25, L"ClassMember", L"ClassMember.RootEnd", L"$<ClassMember> ¡ñ")
				SET_STATE_INFO(26, L"Type", L"Type.1", L"<Type>: NAME : name as PrimitiveTypeObj¡ñ\r\n<Type>: \"token\" as TokenTypeObj¡ñ\r\n<Type>: Type : parentType \".\" NAME : name as SubTypeObj¡ñ\r\n<Type>: Type : elementType \"[\" \"]\" as ArrayTypeObj¡ñ")
				SET_STATE_INFO(27, L"Enum", L"Enum.1", L"<Enum>: \"enum\"¡ñ NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(28, L"Class", L"Class.1", L"<Class>: \"class\"¡ñ NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(29, L"Class", L"Class.10", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef¡ñ")
				SET_STATE_INFO(30, L"TokenDecl", L"TokenDecl.RootEnd", L"$<TokenDecl> ¡ñ")
				SET_STATE_INFO(31, L"ParserDecl", L"ParserDecl.RootEnd", L"$<ParserDecl> ¡ñ")
				SET_STATE_INFO(32, L"RuleDecl", L"RuleDecl.1", L"<RuleDecl>: \"rule\"¡ñ Type : type NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] { \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(33, L"TokenDecl", L"TokenDecl.1", L"<TokenDecl>: \"token\"¡ñ NAME : name \"=\" STRING : regex [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as TokenDef with { discard = \"KeepToken\" }")
				SET_STATE_INFO(34, L"TokenDecl", L"TokenDecl.2", L"<TokenDecl>: \"discardtoken\"¡ñ NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(35, L"RuleDecl", L"RuleDecl.RootEnd", L"$<RuleDecl> ¡ñ")
				SET_STATE_INFO(36, L"PrimitiveGrammar", L"PrimitiveGrammar.10", L"<PrimitiveGrammar>: \"[\" Grammar : grammar \"]\" as OptionalGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"{\" Grammar : grammar \"}\" as LoopGrammarDef¡ñ\r\n<PrimitiveGrammar>: NAME : name as PrimitiveGrammarDef¡ñ\r\n<PrimitiveGrammar>: STRING : text as TextGrammarDef¡ñ\r\n<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\" NAME : memberName as AssignGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"!\" PrimitiveGrammar : grammar as UseGrammarDef¡ñ")
				SET_STATE_INFO(37, L"PrimitiveGrammar", L"PrimitiveGrammar.2", L"<PrimitiveGrammar>: \"!\"¡ñ PrimitiveGrammar : grammar as UseGrammarDef")
				SET_STATE_INFO(38, L"PrimitiveGrammar", L"PrimitiveGrammar.3", L"<PrimitiveGrammar>: \"[\"¡ñ Grammar : grammar \"]\" as OptionalGrammarDef")
				SET_STATE_INFO(39, L"PrimitiveGrammar", L"PrimitiveGrammar.4", L"<PrimitiveGrammar>: \"{\"¡ñ Grammar : grammar \"}\" as LoopGrammarDef")
				SET_STATE_INFO(40, L"PrimitiveGrammar", L"PrimitiveGrammar.5", L"<PrimitiveGrammar>: \"(\"¡ñ !Grammar \")\"")
				SET_STATE_INFO(41, L"Enum", L"Enum.RootEnd", L"$<Enum> ¡ñ")
				SET_STATE_INFO(42, L"TypeDecl", L"TypeDecl.RootEnd", L"$<TypeDecl> ¡ñ")
				SET_STATE_INFO(43, L"Type", L"Type.5", L"<Type>: Type : parentType \".\"¡ñ NAME : name as SubTypeObj")
				SET_STATE_INFO(44, L"Type", L"Type.6", L"<Type>: Type : elementType \"[\"¡ñ \"]\" as ArrayTypeObj")
				SET_STATE_INFO(45, L"Type", L"Type.RootEnd", L"$<Type> ¡ñ")
				SET_STATE_INFO(46, L"ClassMember", L"ClassMember.2", L"<ClassMember>: Type : type NAME : name¡ñ [ \"(\" NAME : unescapingFunction \")\" ] [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as ClassMemberDef")
				SET_STATE_INFO(47, L"PrimitiveGrammar", L"PrimitiveGrammar.11", L"<PrimitiveGrammar>: \"{\" Grammar : grammar \"}\" as LoopGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"(\" !Grammar \")\"¡ñ\r\n<PrimitiveGrammar>: NAME : name as PrimitiveGrammarDef¡ñ\r\n<PrimitiveGrammar>: STRING : text as TextGrammarDef¡ñ\r\n<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\" NAME : memberName as AssignGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"!\" PrimitiveGrammar : grammar as UseGrammarDef¡ñ\r\n<PrimitiveGrammar>: \"[\" Grammar : grammar \"]\" as OptionalGrammarDef¡ñ")
				SET_STATE_INFO(48, L"Grammar", L"Grammar.4", L"<Grammar>: Grammar : grammar \"as\"¡ñ Type : type as CreateGrammarDef")
				SET_STATE_INFO(49, L"Grammar", L"Grammar.5", L"<Grammar>: Grammar : grammar \"with\"¡ñ \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(50, L"Grammar", L"Grammar.RootEnd", L"$<Grammar> ¡ñ")
				SET_STATE_INFO(51, L"RuleFragmentDecl", L"RuleFragmentDecl.3", L"<RuleFragmentDecl>: \"=\" !Grammar \";\"¡ñ")
				SET_STATE_INFO(52, L"RuleDecl", L"RuleDecl.3", L"<RuleDecl>: \"rule\" Type : type NAME : name¡ñ [ Attribute : attributes { \",\" Attribute : attributes } ] { \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(53, L"Enum", L"Enum.2", L"<Enum>: \"enum\" NAME : name¡ñ [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(54, L"Class", L"Class.2", L"<Class>: \"class\" NAME : name¡ñ [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(55, L"Class", L"Class.RootEnd", L"$<Class> ¡ñ")
				SET_STATE_INFO(56, L"TokenDecl", L"TokenDecl.3", L"<TokenDecl>: \"token\" NAME : name¡ñ \"=\" STRING : regex [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as TokenDef with { discard = \"KeepToken\" }")
				SET_STATE_INFO(57, L"TokenDecl", L"TokenDecl.4", L"<TokenDecl>: \"discardtoken\" NAME : name¡ñ \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(58, L"PrimitiveGrammar", L"PrimitiveGrammar.6", L"<PrimitiveGrammar>: PrimitiveGrammar : grammar \":\"¡ñ NAME : memberName as AssignGrammarDef")
				SET_STATE_INFO(59, L"PrimitiveGrammar", L"PrimitiveGrammar.RootEnd", L"$<PrimitiveGrammar> ¡ñ")
				SET_STATE_INFO(60, L"SequenceGrammar", L"SequenceGrammar.RootEnd", L"$<SequenceGrammar> ¡ñ")
				SET_STATE_INFO(61, L"AlternativeGrammar", L"AlternativeGrammar.3", L"<AlternativeGrammar>: AlternativeGrammar : first \"|\"¡ñ SequenceGrammar : second as AlternativeGrammarDef")
				SET_STATE_INFO(62, L"AlternativeGrammar", L"AlternativeGrammar.RootEnd", L"$<AlternativeGrammar> ¡ñ")
				SET_STATE_INFO(63, L"ClassMember", L"ClassMember.3", L"<ClassMember>: Type : type NAME : name [ \"(\"¡ñ NAME : unescapingFunction \")\" ] [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as ClassMemberDef")
				SET_STATE_INFO(64, L"Grammar", L"Grammar.7", L"<Grammar>: Grammar : grammar \"with\" \"{\"¡ñ NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(65, L"RuleFragmentDecl", L"RuleFragmentDecl.RootEnd", L"$<RuleFragmentDecl> ¡ñ")
				SET_STATE_INFO(66, L"Class", L"Class.3", L"<Class>: \"class\" NAME : name [ \"ambiguous\"¡ñ \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(67, L"Class", L"Class.4", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\"¡ñ Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(68, L"TokenDecl", L"TokenDecl.5", L"<TokenDecl>: \"token\" NAME : name \"=\"¡ñ STRING : regex [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as TokenDef with { discard = \"KeepToken\" }")
				SET_STATE_INFO(69, L"TokenDecl", L"TokenDecl.6", L"<TokenDecl>: \"discardtoken\" NAME : name \"=\"¡ñ STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(70, L"ClassMember", L"ClassMember.6", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction¡ñ \")\" ] [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as ClassMemberDef")
				SET_STATE_INFO(71, L"Grammar", L"Grammar.8", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName¡ñ \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(72, L"Class", L"Class.7", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\"¡ñ NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(73, L"TokenDecl", L"TokenDecl.7", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex¡ñ [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as TokenDef with { discard = \"KeepToken\" }")
				SET_STATE_INFO(74, L"TokenDecl", L"TokenDecl.8", L"<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex¡ñ \";\" as TokenDef with { discard = \"DiscardToken\" }")
				SET_STATE_INFO(75, L"ClassMember", L"ClassMember.9", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ]¡ñ [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as ClassMemberDef")
				SET_STATE_INFO(76, L"Grammar", L"Grammar.9", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\"¡ñ STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(77, L"Class", L"Class.11", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType¡ñ \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(78, L"Grammar", L"Grammar.10", L"<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value¡ñ \"}\" as SetterGrammarDef")
				SET_STATE_INFO(79, L"Class", L"Class.13", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ]¡ñ [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(80, L"Grammar", L"Grammar.6", L"<Grammar>: Grammar : grammar \"as\" Type : type as CreateGrammarDef¡ñ\r\n<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef¡ñ")
				SET_STATE_INFO(81, L"Type", L"Type.RootStart", L"¡ñ $<Type>")
				SET_STATE_INFO(82, L"Type", L"Type.Start", L"¡¤ <Type>")
				SET_STATE_INFO(83, L"EnumMember", L"EnumMember.RootStart", L"¡ñ $<EnumMember>")
				SET_STATE_INFO(84, L"EnumMember", L"EnumMember.Start", L"¡¤ <EnumMember>")
				SET_STATE_INFO(85, L"Enum", L"Enum.RootStart", L"¡ñ $<Enum>")
				SET_STATE_INFO(86, L"Enum", L"Enum.Start", L"¡¤ <Enum>")
				SET_STATE_INFO(87, L"ClassMember", L"ClassMember.RootStart", L"¡ñ $<ClassMember>")
				SET_STATE_INFO(88, L"ClassMember", L"ClassMember.Start", L"¡¤ <ClassMember>")
				SET_STATE_INFO(89, L"Class", L"Class.RootStart", L"¡ñ $<Class>")
				SET_STATE_INFO(90, L"Class", L"Class.Start", L"¡¤ <Class>")
				SET_STATE_INFO(91, L"TypeDecl", L"TypeDecl.RootStart", L"¡ñ $<TypeDecl>")
				SET_STATE_INFO(92, L"TypeDecl", L"TypeDecl.Start", L"¡¤ <TypeDecl>")
				SET_STATE_INFO(93, L"PrimitiveGrammar", L"PrimitiveGrammar.RootStart", L"¡ñ $<PrimitiveGrammar>")
				SET_STATE_INFO(94, L"PrimitiveGrammar", L"PrimitiveGrammar.Start", L"¡¤ <PrimitiveGrammar>")
				SET_STATE_INFO(95, L"SequenceGrammar", L"SequenceGrammar.RootStart", L"¡ñ $<SequenceGrammar>")
				SET_STATE_INFO(96, L"SequenceGrammar", L"SequenceGrammar.Start", L"¡¤ <SequenceGrammar>")
				SET_STATE_INFO(97, L"AlternativeGrammar", L"AlternativeGrammar.RootStart", L"¡ñ $<AlternativeGrammar>")
				SET_STATE_INFO(98, L"AlternativeGrammar", L"AlternativeGrammar.Start", L"¡¤ <AlternativeGrammar>")
				SET_STATE_INFO(99, L"Grammar", L"Grammar.RootStart", L"¡ñ $<Grammar>")
				SET_STATE_INFO(100, L"Grammar", L"Grammar.Start", L"¡¤ <Grammar>")
				SET_STATE_INFO(101, L"TokenDecl", L"TokenDecl.RootStart", L"¡ñ $<TokenDecl>")
				SET_STATE_INFO(102, L"TokenDecl", L"TokenDecl.Start", L"¡¤ <TokenDecl>")
				SET_STATE_INFO(103, L"RuleFragmentDecl", L"RuleFragmentDecl.RootStart", L"¡ñ $<RuleFragmentDecl>")
				SET_STATE_INFO(104, L"RuleFragmentDecl", L"RuleFragmentDecl.Start", L"¡¤ <RuleFragmentDecl>")
				SET_STATE_INFO(105, L"RuleFragmentDecl", L"RuleFragmentDecl.1", L"<RuleFragmentDecl>: \"=\"¡ñ !Grammar \";\"")
				SET_STATE_INFO(106, L"RuleDecl", L"RuleDecl.RootStart", L"¡ñ $<RuleDecl>")
				SET_STATE_INFO(107, L"RuleDecl", L"RuleDecl.Start", L"¡¤ <RuleDecl>")
				SET_STATE_INFO(108, L"ParserDecl", L"ParserDecl.RootStart", L"¡ñ $<ParserDecl>")
				SET_STATE_INFO(109, L"ParserDecl", L"ParserDecl.Start", L"¡¤ <ParserDecl>")
				SET_STATE_INFO(110, L"Attribute", L"Attribute.7", L"<Attribute>: \"@\" NAME : name \"(\" [ STRING : arguments { \",\" STRING : arguments } ] \")\" as AttributeDef¡ñ")
				SET_STATE_INFO(111, L"Type", L"Type.2", L"<Type>: Type : parentType¡ñ \".\" NAME : name as SubTypeObj")
				SET_STATE_INFO(112, L"Type", L"Type.3", L"<Type>: Type : elementType¡ñ \"[\" \"]\" as ArrayTypeObj")
				SET_STATE_INFO(113, L"Type", L"Type.4", L"<Type>: NAME : name as PrimitiveTypeObj¡ñ\r\n<Type>: \"token\" as TokenTypeObj¡ñ\r\n<Type>: Type : parentType \".\" NAME : name as SubTypeObj¡ñ\r\n<Type>: Type : elementType \"[\" \"]\" as ArrayTypeObj¡ñ")
				SET_STATE_INFO(114, L"EnumMember", L"EnumMember.2", L"<EnumMember>: NAME : name [ Attribute : attributes¡ñ { \",\" Attribute : attributes } ] \",\" as EnumMemberDef\r\n<EnumMember>: NAME : name [ Attribute : attributes ¡ñ{ \",\" Attribute : attributes } ] \",\" as EnumMemberDef")
				SET_STATE_INFO(115, L"EnumMember", L"EnumMember.5", L"<EnumMember>: NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] \",\" as EnumMemberDef¡ñ")
				SET_STATE_INFO(116, L"Enum", L"Enum.3", L"<Enum>: \"enum\" NAME : name [ Attribute : attributes¡ñ { \",\" Attribute : attributes } ] \"{\" { EnumMember : members } \"}\" as EnumTypeDef\r\n<Enum>: \"enum\" NAME : name [ Attribute : attributes ¡ñ{ \",\" Attribute : attributes } ] \"{\" { EnumMember : members } \"}\" as EnumTypeDef")
				SET_STATE_INFO(117, L"Enum", L"Enum.7", L"<Enum>: \"enum\" NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { EnumMember : members } \"}\" as EnumTypeDef¡ñ")
				SET_STATE_INFO(118, L"ClassMember", L"ClassMember.1", L"<ClassMember>: Type : type¡ñ NAME : name [ \"(\" NAME : unescapingFunction \")\" ] [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as ClassMemberDef")
				SET_STATE_INFO(119, L"ClassMember", L"ClassMember.4", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] [ Attribute : attributes¡ñ { \",\" Attribute : attributes } ] \";\" as ClassMemberDef\r\n<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] [ Attribute : attributes ¡ñ{ \",\" Attribute : attributes } ] \";\" as ClassMemberDef")
				SET_STATE_INFO(120, L"ClassMember", L"ClassMember.8", L"<ClassMember>: Type : type NAME : name [ \"(\" NAME : unescapingFunction \")\" ] [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as ClassMemberDef¡ñ")
				SET_STATE_INFO(121, L"Class", L"Class.5", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes¡ñ { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef\r\n<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes ¡ñ{ \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(122, L"Class", L"Class.8", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ]¡ñ [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef")
				SET_STATE_INFO(123, L"Class", L"Class.12", L"<Class>: \"class\" NAME : name [ \"ambiguous\" \"(\" NAME : ambiguousType \")\" ] [ \":\" Type : parentType ] [ Attribute : attributes { \",\" Attribute : attributes } ] \"{\" { ClassMember : members | TypeDecl : subTypes } \"}\" as ClassTypeDef¡ñ")
				SET_STATE_INFO(124, L"TypeDecl", L"TypeDecl.1", L"<TypeDecl>: !Enum | !Class¡ñ")
				SET_STATE_INFO(125, L"PrimitiveGrammar", L"PrimitiveGrammar.1", L"<PrimitiveGrammar>: PrimitiveGrammar : grammar¡ñ \":\" NAME : memberName as AssignGrammarDef")
				SET_STATE_INFO(126, L"PrimitiveGrammar", L"PrimitiveGrammar.7", L"<PrimitiveGrammar>: \"[\" Grammar : grammar¡ñ \"]\" as OptionalGrammarDef")
				SET_STATE_INFO(127, L"PrimitiveGrammar", L"PrimitiveGrammar.8", L"<PrimitiveGrammar>: \"{\" Grammar : grammar¡ñ \"}\" as LoopGrammarDef")
				SET_STATE_INFO(128, L"PrimitiveGrammar", L"PrimitiveGrammar.9", L"<PrimitiveGrammar>: \"(\" !Grammar¡ñ \")\"")
				SET_STATE_INFO(129, L"SequenceGrammar", L"SequenceGrammar.1", L"<SequenceGrammar>: !PrimitiveGrammar¡ñ\r\n<SequenceGrammar>: SequenceGrammar : first PrimitiveGrammar : second as SequenceGrammarDef¡ñ")
				SET_STATE_INFO(130, L"SequenceGrammar", L"SequenceGrammar.2", L"<SequenceGrammar>: SequenceGrammar : first¡ñ PrimitiveGrammar : second as SequenceGrammarDef")
				SET_STATE_INFO(131, L"SequenceGrammar", L"SequenceGrammar.3", L"<SequenceGrammar>: SequenceGrammar : first PrimitiveGrammar : second as SequenceGrammarDef¡ñ")
				SET_STATE_INFO(132, L"AlternativeGrammar", L"AlternativeGrammar.1", L"<AlternativeGrammar>: !SequenceGrammar¡ñ\r\n<AlternativeGrammar>: AlternativeGrammar : first \"|\" SequenceGrammar : second as AlternativeGrammarDef¡ñ")
				SET_STATE_INFO(133, L"AlternativeGrammar", L"AlternativeGrammar.2", L"<AlternativeGrammar>: AlternativeGrammar : first¡ñ \"|\" SequenceGrammar : second as AlternativeGrammarDef")
				SET_STATE_INFO(134, L"AlternativeGrammar", L"AlternativeGrammar.4", L"<AlternativeGrammar>: AlternativeGrammar : first \"|\" SequenceGrammar : second as AlternativeGrammarDef¡ñ")
				SET_STATE_INFO(135, L"Grammar", L"Grammar.1", L"<Grammar>: !AlternativeGrammar¡ñ\r\n<Grammar>: Grammar : grammar \"as\" Type : type as CreateGrammarDef¡ñ\r\n<Grammar>: Grammar : grammar \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef¡ñ")
				SET_STATE_INFO(136, L"Grammar", L"Grammar.2", L"<Grammar>: Grammar : grammar¡ñ \"as\" Type : type as CreateGrammarDef")
				SET_STATE_INFO(137, L"Grammar", L"Grammar.3", L"<Grammar>: Grammar : grammar¡ñ \"with\" \"{\" NAME : memberName \"=\" STRING : value \"}\" as SetterGrammarDef")
				SET_STATE_INFO(138, L"TokenDecl", L"TokenDecl.9", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex [ Attribute : attributes¡ñ { \",\" Attribute : attributes } ] \";\" as TokenDef with { discard = \"KeepToken\" }\r\n<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex [ Attribute : attributes ¡ñ{ \",\" Attribute : attributes } ] \";\" as TokenDef with { discard = \"KeepToken\" }")
				SET_STATE_INFO(139, L"TokenDecl", L"TokenDecl.12", L"<TokenDecl>: \"token\" NAME : name \"=\" STRING : regex [ Attribute : attributes { \",\" Attribute : attributes } ] \";\" as TokenDef with { discard = \"KeepToken\" }¡ñ\r\n<TokenDecl>: \"discardtoken\" NAME : name \"=\" STRING : regex \";\" as TokenDef with { discard = \"DiscardToken\" }¡ñ")
				SET_STATE_INFO(140, L"RuleFragmentDecl", L"RuleFragmentDecl.2", L"<RuleFragmentDecl>: \"=\" !Grammar¡ñ \";\"")
				SET_STATE_INFO(141, L"RuleDecl", L"RuleDecl.2", L"<RuleDecl>: \"rule\" Type : type¡ñ NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] { \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(142, L"RuleDecl", L"RuleDecl.4", L"<RuleDecl>: \"rule\" Type : type NAME : name [ Attribute : attributes¡ñ { \",\" Attribute : attributes } ] { \"=\" Grammar : grammars } \";\" as RuleDef\r\n<RuleDecl>: \"rule\" Type : type NAME : name [ Attribute : attributes ¡ñ{ \",\" Attribute : attributes } ] { \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(143, L"RuleDecl", L"RuleDecl.8", L"<RuleDecl>: \"rule\" Type : type NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] { \"=\" Grammar : grammars } \";\" as RuleDef¡ñ")
				SET_STATE_INFO(144, L"RuleDecl", L"RuleDecl.9", L"<RuleDecl>: \"rule\" Type : type NAME : name [ Attribute : attributes { \",\" Attribute : attributes } ] ¡ñ{ \"=\" Grammar : grammars } \";\" as RuleDef")
				SET_STATE_INFO(145, L"ParserDecl", L"ParserDecl.1", L"<ParserDecl>: { TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef¡ñ\r\n<ParserDecl>: ¡ñ{ TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef")
				SET_STATE_INFO(146, L"ParserDecl", L"ParserDecl.2", L"<ParserDecl>: { TypeDecl : types | TokenDecl : tokens | RuleDecl : rules } ( TypeDecl : types | TokenDecl : tokens | RuleDecl : rules ) as ParserDef¡ñ")

				SET_RULE_INFO(0, L"Attribute", L"AttributeDef", 0)
				SET_RULE_INFO(1, L"Type", L"TypeObj", 81)
				SET_RULE_INFO(2, L"EnumMember", L"EnumMemberDef", 83)
				SET_RULE_INFO(3, L"Enum", L"EnumTypeDef", 85)
				SET_RULE_INFO(4, L"ClassMember", L"ClassMemberDef", 87)
				SET_RULE_INFO(5, L"Class", L"ClassTypeDef", 89)
				SET_RULE_INFO(6, L"TypeDecl", L"TypeDef", 91)
				SET_RULE_INFO(7, L"PrimitiveGrammar", L"GrammarDef", 93)
				SET_RULE_INFO(8, L"SequenceGrammar", L"GrammarDef", 95)
				SET_RULE_INFO(9, L"AlternativeGrammar", L"GrammarDef", 97)
				SET_RULE_INFO(10, L"Grammar", L"GrammarDef", 99)
				SET_RULE_INFO(11, L"TokenDecl", L"TokenDef", 101)
				SET_RULE_INFO(12, L"RuleFragmentDecl", L"GrammarDef", 103)
				SET_RULE_INFO(13, L"RuleDecl", L"RuleDef", 106)
				SET_RULE_INFO(14, L"ParserDecl", L"ParserDef", 108)

				BEGIN_TRANSITION_BAG(0, 0)

					BEGIN_TRANSITION_ITEM(0, 1)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 25)

					BEGIN_TRANSITION_ITEM(25, 3)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(3, 22)

					BEGIN_TRANSITION_ITEM(22, 4)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(4, 23)

					BEGIN_TRANSITION_ITEM(23, 6)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(4, 26)

					BEGIN_TRANSITION_ITEM(26, 5)
					ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(5, 15)

					BEGIN_TRANSITION_ITEM(15, 7)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(5, 23)

					BEGIN_TRANSITION_ITEM(23, 6)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 1)

					BEGIN_TRANSITION_ITEM(1, 8)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 11)

					BEGIN_TRANSITION_ITEM(11, 15)
					ITEM_STACK_PATTERN(122)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 122, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 15)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 15)
					ITEM_STACK_PATTERN(54)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 54, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 11)
					ITEM_STACK_PATTERN(53)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 53, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 15)
					ITEM_STACK_PATTERN(16)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 11)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 13)

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(75)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 75, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 17)
					ITEM_STACK_PATTERN(73)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(52)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 52, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(46)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 46, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(20)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 20, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 17)
					ITEM_STACK_PATTERN(18)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 18, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 13)
					ITEM_STACK_PATTERN(14)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 14, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 15)

					BEGIN_TRANSITION_ITEM(15, 16)
					ITEM_STACK_PATTERN(122)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 122, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 16)
					ITEM_STACK_PATTERN(79)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 79, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 14)
					ITEM_STACK_PATTERN(75)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 75, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 18)
					ITEM_STACK_PATTERN(73)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 73, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 16)
					ITEM_STACK_PATTERN(54)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 54, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 12)
					ITEM_STACK_PATTERN(53)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 53, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 20)
					ITEM_STACK_PATTERN(52)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 52, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 14)
					ITEM_STACK_PATTERN(46)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 46, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 10)
					BEGIN_LOOK_AHEAD(2) LOOK_AHEAD(24) END_LOOK_AHEAD
					ITEM_STACK_PATTERN(23)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 9)
					BEGIN_LOOK_AHEAD(22) LOOK_AHEAD(1) END_LOOK_AHEAD
					BEGIN_LOOK_AHEAD(24) LOOK_AHEAD(12) END_LOOK_AHEAD
					BEGIN_LOOK_AHEAD(23) LOOK_AHEAD(25) END_LOOK_AHEAD
					ITEM_STACK_PATTERN(23)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 20)
					ITEM_STACK_PATTERN(20)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 20, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 18)
					ITEM_STACK_PATTERN(18)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 18, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 16)
					ITEM_STACK_PATTERN(16)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 14)
					ITEM_STACK_PATTERN(14)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 14, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 12)
					ITEM_STACK_PATTERN(12)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 12, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 10)
					BEGIN_LOOK_AHEAD(2) LOOK_AHEAD(24) END_LOOK_AHEAD
					ITEM_STACK_PATTERN(10)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(15, 9)
					BEGIN_LOOK_AHEAD(22) LOOK_AHEAD(1) END_LOOK_AHEAD
					BEGIN_LOOK_AHEAD(24) LOOK_AHEAD(12) END_LOOK_AHEAD
					BEGIN_LOOK_AHEAD(23) LOOK_AHEAD(25) END_LOOK_AHEAD
					ITEM_STACK_PATTERN(10)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 17)

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(52)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 52, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(20)
					ITEM_INSTRUCTION(Create, 0, L"AttributeDef", L"", L"Attribute");
					ITEM_INSTRUCTION(Reduce, 20, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 26)

					BEGIN_TRANSITION_ITEM(26, 5)
					ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 1)

					BEGIN_TRANSITION_ITEM(1, 22)
					ITEM_INSTRUCTION(Create, 0, L"EnumMemberDef", L"", L"EnumMember");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 12)

					BEGIN_TRANSITION_ITEM(12, 24)
					ITEM_STACK_PATTERN(11)
					ITEM_INSTRUCTION(Create, 0, L"EnumMemberDef", L"", L"EnumMember");
					ITEM_INSTRUCTION(Reduce, 11, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(9, 25)

					BEGIN_TRANSITION_ITEM(25, 23)
					ITEM_STACK_PATTERN(11)
					ITEM_INSTRUCTION(Create, 0, L"EnumMemberDef", L"", L"EnumMember");
					ITEM_INSTRUCTION(Reduce, 11, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 11, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 10, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 12)

					BEGIN_TRANSITION_ITEM(12, 24)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 25)

					BEGIN_TRANSITION_ITEM(25, 23)
					ITEM_INSTRUCTION(Shift, 11, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 12, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 1)

					BEGIN_TRANSITION_ITEM(1, 25)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 3)

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 5)

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 6)

					BEGIN_TRANSITION_ITEM(6, 26)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 12)

					BEGIN_TRANSITION_ITEM(12, 29)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassMemberDef", L"", L"ClassMember");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"members", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 14, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 3)

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 5)

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 6)

					BEGIN_TRANSITION_ITEM(6, 26)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 12)

					BEGIN_TRANSITION_ITEM(12, 29)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 16, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 1)

					BEGIN_TRANSITION_ITEM(1, 30)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 31)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 31)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 3)

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 5)

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 6)

					BEGIN_TRANSITION_ITEM(6, 33)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 33)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 8)

					BEGIN_TRANSITION_ITEM(8, 32)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 32)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"tokens", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(18, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 18, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 1)

					BEGIN_TRANSITION_ITEM(1, 35)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 31)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 31)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 3)

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 5)

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 6)

					BEGIN_TRANSITION_ITEM(6, 33)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 33)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 8)

					BEGIN_TRANSITION_ITEM(8, 32)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 32)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"RuleDef", L"", L"RuleDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"rules", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(20, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 20, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(21, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(21, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(21, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(21, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(21, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(21, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(23, 15)

					BEGIN_TRANSITION_ITEM(15, 9)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(23, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 1)

					BEGIN_TRANSITION_ITEM(1, 41)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 31)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 31)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 42)
					ITEM_STACK_PATTERN(92)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 3)

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 5)

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 6)

					BEGIN_TRANSITION_ITEM(6, 33)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 33)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 26)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 8)

					BEGIN_TRANSITION_ITEM(8, 32)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 32)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 12)

					BEGIN_TRANSITION_ITEM(12, 29)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"EnumTypeDef", L"", L"Enum");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 1)

					BEGIN_TRANSITION_ITEM(1, 45)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 50)
					ITEM_STACK_PATTERN(48)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 9)

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_STACK_PATTERN(48)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 10)

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_STACK_PATTERN(48)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 11)

					BEGIN_TRANSITION_ITEM(11, 15)
					ITEM_STACK_PATTERN(67)
					ITEM_INSTRUCTION(Reduce, 67, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"parentType", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 12)

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(48)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 13)

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(48)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(48)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 16)

					BEGIN_TRANSITION_ITEM(16, 43)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 82, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"parentType", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 17)

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(48)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 20)

					BEGIN_TRANSITION_ITEM(20, 44)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 82, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"elementType", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 21)

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(48)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 23)

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(48)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"CreateGrammarDef", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_STACK_PATTERN(67)
					ITEM_INSTRUCTION(Reduce, 67, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"parentType", L"", L"");
					ITEM_INSTRUCTION(Shift, 122, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 25)

					BEGIN_TRANSITION_ITEM(25, 46)
					ITEM_STACK_PATTERN(88)
					ITEM_INSTRUCTION(Reduce, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(25, 52)
					ITEM_STACK_PATTERN(32)
					ITEM_INSTRUCTION(Reduce, 32, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"type", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 25)

					BEGIN_TRANSITION_ITEM(25, 53)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(28, 25)

					BEGIN_TRANSITION_ITEM(25, 54)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 1)

					BEGIN_TRANSITION_ITEM(1, 55)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 31)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 31)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ParserDef", L"", L"ParserDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 42)
					ITEM_STACK_PATTERN(92)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 3)

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 5)

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 6)

					BEGIN_TRANSITION_ITEM(6, 33)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 33)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(6, 26)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 8)

					BEGIN_TRANSITION_ITEM(8, 32)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(145)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 145, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(8, 32)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(109)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"types", L"", L"");
					ITEM_INSTRUCTION(Shift, 145, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 12)

					BEGIN_TRANSITION_ITEM(12, 29)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_STACK_PATTERN(92)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"ClassTypeDef", L"", L"Class");
					ITEM_INSTRUCTION(Reduce, 92, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"TypeDecl");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subTypes", L"", L"");
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(32, 6)

					BEGIN_TRANSITION_ITEM(6, 26)
					ITEM_INSTRUCTION(Shift, 32, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(32, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_INSTRUCTION(Shift, 32, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(33, 25)

					BEGIN_TRANSITION_ITEM(25, 56)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(34, 25)

					BEGIN_TRANSITION_ITEM(25, 57)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 1)

					BEGIN_TRANSITION_ITEM(1, 59)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 50)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 62)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 50)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 62)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 60)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 50)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 62)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 50)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 62)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 60)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 2)

					BEGIN_TRANSITION_ITEM(2, 36)
					ITEM_STACK_PATTERN(37)
					ITEM_INSTRUCTION(Reduce, 37, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 9)

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 10)

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 12)

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 13)

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 14)

					BEGIN_TRANSITION_ITEM(14, 58)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 94, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 17)

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 19)

					BEGIN_TRANSITION_ITEM(19, 61)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 61)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 61)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 61)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 21)

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 23)

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(37, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 37, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(37, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 37, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(37, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 37, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(37, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 37, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(37, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 37, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(37, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 37, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(39, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(39, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(39, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(39, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(39, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(39, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(40, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(40, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(40, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(40, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(40, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(40, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(43, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SubTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(44, 21)

					BEGIN_TRANSITION_ITEM(21, 26)
					ITEM_INSTRUCTION(Create, 0, L"ArrayTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(46, 13)

					BEGIN_TRANSITION_ITEM(13, 13)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(46, 22)

					BEGIN_TRANSITION_ITEM(22, 63)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(46, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 46, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 1)

					BEGIN_TRANSITION_ITEM(1, 59)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 50)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 62)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 50)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 62)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 60)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 50)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 62)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 50)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 62)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 60)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 2)

					BEGIN_TRANSITION_ITEM(2, 47)
					ITEM_STACK_PATTERN(37)
					ITEM_INSTRUCTION(Reduce, 37, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 9)

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 10)

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 12)

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 13)

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 14)

					BEGIN_TRANSITION_ITEM(14, 58)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 94, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 17)

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 19)

					BEGIN_TRANSITION_ITEM(19, 61)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 61)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 61)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(19, 61)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 21)

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 23)

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(37)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 37, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(37)
					ITEM_STACK_PATTERN(130)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 37, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(37)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(98)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 37, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(37)
					ITEM_STACK_PATTERN(96)
					ITEM_STACK_PATTERN(61)
					ITEM_STACK_PATTERN(100)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 37, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"UseGrammarDef", L"", L"PrimitiveGrammar");
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(Reduce, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AlternativeGrammarDef", L"", L"AlternativeGrammar");
					ITEM_INSTRUCTION(Reduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"Grammar");
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(47, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_STACK_PATTERN(130)
					ITEM_INSTRUCTION(Reduce, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"second", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"SequenceGrammarDef", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_STACK_PATTERN(96)
					ITEM_INSTRUCTION(Reduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"SequenceGrammar");
					ITEM_INSTRUCTION(LeftRecursiveReduce, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"first", L"", L"");
					ITEM_INSTRUCTION(Shift, 130, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(48, 6)

					BEGIN_TRANSITION_ITEM(6, 26)
					ITEM_INSTRUCTION(Shift, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(48, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_INSTRUCTION(Shift, 48, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(49, 11)

					BEGIN_TRANSITION_ITEM(11, 64)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(51, 1)

					BEGIN_TRANSITION_ITEM(1, 65)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(52, 13)

					BEGIN_TRANSITION_ITEM(13, 19)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(52, 17)

					BEGIN_TRANSITION_ITEM(17, 21)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(52, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 52, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(53, 11)

					BEGIN_TRANSITION_ITEM(11, 11)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(53, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 53, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(54, 4)

					BEGIN_TRANSITION_ITEM(4, 66)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(54, 11)

					BEGIN_TRANSITION_ITEM(11, 15)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(54, 14)

					BEGIN_TRANSITION_ITEM(14, 67)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(54, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 54, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(56, 17)

					BEGIN_TRANSITION_ITEM(17, 68)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(57, 17)

					BEGIN_TRANSITION_ITEM(17, 69)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(58, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Assign, 0, L"memberName", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"AssignGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(61, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(61, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(61, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(61, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(61, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(61, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 61, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(63, 25)

					BEGIN_TRANSITION_ITEM(25, 70)
					ITEM_INSTRUCTION(Assign, 0, L"unescapingFunction", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(64, 25)

					BEGIN_TRANSITION_ITEM(25, 71)
					ITEM_INSTRUCTION(Assign, 0, L"memberName", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(66, 22)

					BEGIN_TRANSITION_ITEM(22, 72)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(67, 6)

					BEGIN_TRANSITION_ITEM(6, 26)
					ITEM_INSTRUCTION(Shift, 67, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(67, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_INSTRUCTION(Shift, 67, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(68, 26)

					BEGIN_TRANSITION_ITEM(26, 73)
					ITEM_INSTRUCTION(Assign, 0, L"regex", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(69, 26)

					BEGIN_TRANSITION_ITEM(26, 74)
					ITEM_INSTRUCTION(Assign, 0, L"regex", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(70, 23)

					BEGIN_TRANSITION_ITEM(23, 75)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(71, 17)

					BEGIN_TRANSITION_ITEM(17, 76)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(72, 25)

					BEGIN_TRANSITION_ITEM(25, 77)
					ITEM_INSTRUCTION(Assign, 0, L"ambiguousType", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(73, 13)

					BEGIN_TRANSITION_ITEM(13, 17)
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"KeepToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(73, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 73, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(74, 13)

					BEGIN_TRANSITION_ITEM(13, 17)
					ITEM_INSTRUCTION(Setter, 0, L"discard", L"DiscardToken", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenDef", L"", L"TokenDecl");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(75, 13)

					BEGIN_TRANSITION_ITEM(13, 13)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(75, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 75, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(76, 26)

					BEGIN_TRANSITION_ITEM(26, 78)
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(77, 23)

					BEGIN_TRANSITION_ITEM(23, 79)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(78, 12)

					BEGIN_TRANSITION_ITEM(12, 80)
					ITEM_INSTRUCTION(Create, 0, L"SetterGrammarDef", L"", L"Grammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 11)

					BEGIN_TRANSITION_ITEM(11, 15)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 14)

					BEGIN_TRANSITION_ITEM(14, 67)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(79, 24)

					BEGIN_TRANSITION_ITEM(24, 2)
					ITEM_INSTRUCTION(Shift, 79, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 1)

					BEGIN_TRANSITION_ITEM(1, 50)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 9)

					BEGIN_TRANSITION_ITEM(9, 48)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 10)

					BEGIN_TRANSITION_ITEM(10, 49)
					ITEM_INSTRUCTION(LeftRecursiveReduce, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 12)

					BEGIN_TRANSITION_ITEM(12, 36)
					ITEM_STACK_PATTERN(39)
					ITEM_INSTRUCTION(Reduce, 39, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"LoopGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 13)

					BEGIN_TRANSITION_ITEM(13, 51)
					ITEM_STACK_PATTERN(105)
					ITEM_INSTRUCTION(Reduce, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"RuleFragmentDecl");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(13, 19)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 17)

					BEGIN_TRANSITION_ITEM(17, 21)
					ITEM_STACK_PATTERN(21)
					ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"grammars", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 21)

					BEGIN_TRANSITION_ITEM(21, 36)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"grammar", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"OptionalGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(80, 23)

					BEGIN_TRANSITION_ITEM(23, 47)
					ITEM_STACK_PATTERN(40)
					ITEM_INSTRUCTION(Reduce, 40, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(81, 0)

					BEGIN_TRANSITION_ITEM(0, 82)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(82, 6)

					BEGIN_TRANSITION_ITEM(6, 26)
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(82, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(83, 0)

					BEGIN_TRANSITION_ITEM(0, 84)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(84, 25)

					BEGIN_TRANSITION_ITEM(25, 23)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(85, 0)

					BEGIN_TRANSITION_ITEM(0, 86)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(86, 5)

					BEGIN_TRANSITION_ITEM(5, 27)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(87, 0)

					BEGIN_TRANSITION_ITEM(0, 88)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(88, 6)

					BEGIN_TRANSITION_ITEM(6, 26)
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TokenTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(88, 25)

					BEGIN_TRANSITION_ITEM(25, 26)
					ITEM_INSTRUCTION(Shift, 88, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveTypeObj", L"", L"Type");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(89, 0)

					BEGIN_TRANSITION_ITEM(0, 90)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(90, 3)

					BEGIN_TRANSITION_ITEM(3, 28)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(91, 0)

					BEGIN_TRANSITION_ITEM(0, 92)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(92, 3)

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(92, 5)

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(93, 0)

					BEGIN_TRANSITION_ITEM(0, 94)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(94, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(94, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(94, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(94, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(94, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(94, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(95, 0)

					BEGIN_TRANSITION_ITEM(0, 96)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(96, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(96, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(96, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(96, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(96, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(96, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(97, 0)

					BEGIN_TRANSITION_ITEM(0, 98)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(98, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(98, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(98, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(98, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(98, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(98, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(99, 0)

					BEGIN_TRANSITION_ITEM(0, 100)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(100, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(100, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(100, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(100, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(100, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(100, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(101, 0)

					BEGIN_TRANSITION_ITEM(0, 102)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(102, 6)

					BEGIN_TRANSITION_ITEM(6, 33)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(102, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(103, 0)

					BEGIN_TRANSITION_ITEM(0, 104)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(104, 17)

					BEGIN_TRANSITION_ITEM(17, 105)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(105, 11)

					BEGIN_TRANSITION_ITEM(11, 39)
					ITEM_INSTRUCTION(Shift, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(105, 18)

					BEGIN_TRANSITION_ITEM(18, 37)
					ITEM_INSTRUCTION(Shift, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(105, 20)

					BEGIN_TRANSITION_ITEM(20, 38)
					ITEM_INSTRUCTION(Shift, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(105, 22)

					BEGIN_TRANSITION_ITEM(22, 40)
					ITEM_INSTRUCTION(Shift, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(105, 25)

					BEGIN_TRANSITION_ITEM(25, 36)
					ITEM_INSTRUCTION(Shift, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"PrimitiveGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(105, 26)

					BEGIN_TRANSITION_ITEM(26, 36)
					ITEM_INSTRUCTION(Shift, 105, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 100, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 98, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 96, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"text", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"TextGrammarDef", L"", L"PrimitiveGrammar");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(106, 0)

					BEGIN_TRANSITION_ITEM(0, 107)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(107, 8)

					BEGIN_TRANSITION_ITEM(8, 32)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(108, 0)

					BEGIN_TRANSITION_ITEM(0, 109)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(109, 3)

					BEGIN_TRANSITION_ITEM(3, 28)
					ITEM_INSTRUCTION(Shift, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(109, 5)

					BEGIN_TRANSITION_ITEM(5, 27)
					ITEM_INSTRUCTION(Shift, 109, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 92, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(109, 6)

					BEGIN_TRANSITION_ITEM(6, 33)
					ITEM_INSTRUCTION(Shift, 109, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(109, 7)

					BEGIN_TRANSITION_ITEM(7, 34)
					ITEM_INSTRUCTION(Shift, 109, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(109, 8)

					BEGIN_TRANSITION_ITEM(8, 32)
					ITEM_INSTRUCTION(Shift, 109, L"", L"", L"");
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
