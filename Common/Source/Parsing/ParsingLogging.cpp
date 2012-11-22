#include "ParsingDefinitions.h"
#include "..\Stream\MemoryStream.h"
#include "..\Collections\OperationForEach.h"

namespace vl
{
	using namespace stream;
	using namespace collections;

	namespace parsing
	{
		namespace definitions
		{
			void LogString(const WString& input, TextWriter& writer)
			{
				writer.WriteChar(L'\"');
				for(int i=0;i<input.Length();i++)
				{
					if(input[i]==L'\"')
					{
						writer.WriteString(L"\"\"");
					}
					else
					{
						writer.WriteChar(input[i]);
					}
				}
				writer.WriteChar(L'\"');
			}

/***********************************************************************
Logger (ParsingDefinitionType)
***********************************************************************/

			class ParsingDefinitionTypeLogger : public Object, public ParsingDefinitionType::IVisitor
			{
			public:
				TextWriter&		writer;

				ParsingDefinitionTypeLogger(TextWriter& _writer)
					:writer(_writer)
				{
				}

				static void LogInternal(Ptr<ParsingDefinitionType> type, TextWriter& writer)
				{
					ParsingDefinitionTypeLogger visitor(writer);
					type->Accept(&visitor);
				}

				void Visit(ParsingDefinitionPrimitiveType* node)override
				{
					writer.WriteString(node->name);
				}

				void Visit(ParsingDefinitionTokenType* node)override
				{
					writer.WriteString(L"token");
				}

				void Visit(ParsingDefinitionSubType* node)override
				{
					LogInternal(node->parentType, writer);
					writer.WriteString(L".");
					writer.WriteString(node->subTypeName);
				}

				void Visit(ParsingDefinitionArrayType* node)override
				{
					LogInternal(node->elementType, writer);
					writer.WriteString(L"[]");
				}
			};

			void Log(Ptr<ParsingDefinitionType> type, TextWriter& writer)
			{
				ParsingDefinitionTypeLogger::LogInternal(type, writer);
			}

/***********************************************************************
Logger (ParsingDefinitionTypeDefinition)
***********************************************************************/

			class ParsingDefinitionTypeDefinitionLogger : public Object, public ParsingDefinitionTypeDefinition::IVisitor
			{
			public:
				WString			prefix;
				TextWriter&		writer;

				static void LogInternal(Ptr<ParsingDefinitionTypeDefinition> definition, const WString& prefix, TextWriter& writer)
				{
					ParsingDefinitionTypeDefinitionLogger visitor(prefix, writer);
					definition->Accept(&visitor);
				}

				ParsingDefinitionTypeDefinitionLogger(const WString& _prefix, TextWriter& _writer)
					:prefix(_prefix)
					,writer(_writer)
				{
				}

				void Visit(ParsingDefinitionClassMemberDefinition* node)override
				{
					writer.WriteString(prefix);
					Log(node->type, writer);
					writer.WriteString(L" ");
					writer.WriteString(node->name);
					writer.WriteLine(L";");
				}

				void Visit(ParsingDefinitionClassDefinition* node)override
				{
					writer.WriteString(prefix);
					writer.WriteString(L"class ");
					if(node->parentType)
					{
						writer.WriteString(node->name);
						writer.WriteString(L" : ");
						Log(node->parentType, writer);
						writer.WriteLine(L"");
					}
					else
					{
						writer.WriteLine(node->name);
					}

					writer.WriteString(prefix);
					writer.WriteLine(L"{");

					for(int i=0;i<node->subTypes.Count();i++)
					{
						LogInternal(node->subTypes[i], prefix+L"    ", writer);
						writer.WriteLine(L"");
					}

					for(int i=0;i<node->members.Count();i++)
					{
						LogInternal(node->members[i], prefix+L"    ", writer);
					}

					writer.WriteString(prefix);
					writer.WriteLine(L"}");
				}

				void Visit(ParsingDefinitionEnumMemberDefinition* node)override
				{
					writer.WriteString(prefix);
					writer.WriteString(node->name);
					writer.WriteLine(L",");
				}

				void Visit(ParsingDefinitionEnumDefinition* node)override
				{
					writer.WriteString(prefix);
					writer.WriteString(L"enum ");
					writer.WriteLine(node->name);

					writer.WriteString(prefix);
					writer.WriteLine(L"{");

					for(int i=0;i<node->members.Count();i++)
					{
						LogInternal(node->members[i], prefix+L"    ", writer);
					}

					writer.WriteString(prefix);
					writer.WriteLine(L"}");
				}
			};

			void Log(Ptr<ParsingDefinitionTypeDefinition> definition, const WString& prefix, TextWriter& writer)
			{
				ParsingDefinitionTypeDefinitionLogger::LogInternal(definition, prefix, writer);
			}

/***********************************************************************
Logger (ParsingDefinitionGrammar)
***********************************************************************/

#define PRIORITY_NONE			0
#define PRIORITY_CREATE			1
#define PRIORITY_SET			1
#define PRIORITY_ALTERNATIVE	2
#define PRIORITY_SEQUENCE		3
#define PRIORITY_USE			4
#define PRIORITY_ASSIGN			4

			class ParsingDefinitionGrammarLogger : public Object, public ParsingDefinitionGrammar::IVisitor
			{
			public:
				TextWriter&		writer;
				int				parentPriority;

				ParsingDefinitionGrammarLogger(TextWriter& _writer, int _parentPriority)
					:writer(_writer)
					,parentPriority(_parentPriority)
				{
				}

				static void LogInternal(Ptr<ParsingDefinitionGrammar> grammar, int parentPriority, TextWriter& writer)
				{
					ParsingDefinitionGrammarLogger visitor(writer, parentPriority);
					grammar->Accept(&visitor);
				}

				void Visit(ParsingDefinitionPrimitiveGrammar* node)override
				{
					writer.WriteString(node->name);
				}

				void Visit(ParsingDefinitionTextGrammar* node)override
				{
					LogString(node->text, writer);
				}

				void Visit(ParsingDefinitionSequenceGrammar* node)override
				{
					int priority=PRIORITY_SEQUENCE;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->first, priority, writer);
					writer.WriteString(L" ");
					LogInternal(node->second, priority, writer);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionAlternativeGrammar* node)override
				{
					int priority=PRIORITY_ALTERNATIVE;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->first, priority, writer);
					writer.WriteString(L" | ");
					LogInternal(node->second, priority, writer);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionLoopGrammar* node)override
				{
					writer.WriteString(L"{ ");
					LogInternal(node->grammar, PRIORITY_NONE, writer);
					writer.WriteString(L" }");
				}

				void Visit(ParsingDefinitionOptionalGrammar* node)override
				{
					writer.WriteString(L"[ ");
					LogInternal(node->grammar, PRIORITY_NONE, writer);
					writer.WriteString(L" ]");
				}

				void Visit(ParsingDefinitionCreateGrammar* node)override
				{
					int priority=PRIORITY_CREATE;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->grammar, priority, writer);
					writer.WriteString(L" as ");
					Log(node->type, writer);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionAssignGrammar* node)override
				{
					int priority=PRIORITY_ASSIGN;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->grammar, priority, writer);
					writer.WriteString(L" : ");
					writer.WriteString(node->memberName);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionUseGrammar* node)override
				{
					int priority=PRIORITY_USE;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					writer.WriteString(L"!");
					LogInternal(node->grammar, priority, writer);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionSetterGrammar* node)override
				{
					int priority=PRIORITY_SET;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->grammar, priority, writer);
					writer.WriteString(L" with { ");
					writer.WriteString(node->memberName);
					writer.WriteString(L" = ");
					LogString(node->value, writer);
					writer.WriteString(L" }");
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}
			};

			void Log(Ptr<ParsingDefinitionGrammar> grammar, TextWriter& writer)
			{
				ParsingDefinitionGrammarLogger::LogInternal(grammar, PRIORITY_NONE, writer);
			}

#undef PRIORITY_NONE
#undef PRIORITY_CREATE
#undef PRIORITY_SET
#undef PRIORITY_ALTERNATIVE
#undef PRIORITY_SEQUENCE
#undef PRIORITY_USE
#undef PRIORITY_ASSIGN

/***********************************************************************
Logger (ParsingDefinitionGrammar)
***********************************************************************/

			WString TypeToString(Ptr<ParsingDefinitionType> type)
			{
				MemoryStream stream(64);
				{
					StreamWriter writer(stream);
					Log(type, writer);
				}
				stream.SeekFromBegin(0);
				{
					StreamReader reader(stream);
					return reader.ReadToEnd();
				}
			}

			WString GrammarToString(Ptr<ParsingDefinitionGrammar> grammar)
			{
				MemoryStream stream(64);
				{
					StreamWriter writer(stream);
					Log(grammar, writer);
				}
				stream.SeekFromBegin(0);
				{
					StreamReader reader(stream);
					return reader.ReadToEnd();
				}
			}

			void Log(Ptr<ParsingDefinition> definition, TextWriter& writer)
			{
				FOREACH(Ptr<ParsingDefinitionTypeDefinition>, type, definition->types.Wrap())
				{
					Log(type, L"", writer);
					writer.WriteLine(L"");
				}
				
				FOREACH(Ptr<ParsingDefinitionTokenDefinition>, token, definition->tokens.Wrap())
				{
					writer.WriteString(L"token ");
					writer.WriteString(token->name);
					writer.WriteString(L" = ");
					LogString(token->regex, writer);
					writer.WriteLine(L";");
				}
				writer.WriteLine(L"");
				
				FOREACH(Ptr<ParsingDefinitionRuleDefinition>, rule, definition->rules.Wrap())
				{
					writer.WriteString(L"rule ");
					Log(rule->type, writer);
					writer.WriteString(L" ");
					writer.WriteLine(rule->name);
					FOREACH(Ptr<ParsingDefinitionGrammar>, grammar, rule->grammars.Wrap())
					{
						writer.WriteString(L"        = ");
						Log(grammar, writer);
						writer.WriteLine(L";");
					}
					writer.WriteLine(L"");
				}
			}
		}
	}
}