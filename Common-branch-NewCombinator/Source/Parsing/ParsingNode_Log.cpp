#include "ParsingNode.h"
#include "..\Collections\OperationForEach.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		namespace parsing_internal
		{
			class RuleFragmentLogger : public Object, public IParsingNodeVisitor
			{
				static const vint				LevelAlt=0;
				static const vint				LevelSeq=1;
				static const vint				LevelLoop=2;
			protected:
				stream::TextWriter&				writer;
				bool							logAction;
				ParsingNode*					referenceNode;
				bool							referenceBefore;
				vint							level;

				void BuildReferenceBefore(ParsingNode* currentNode)
				{
					if(currentNode==referenceNode && referenceBefore)
					{
						writer.WriteString(L"$ ");
					}
				}

				void BuildReferenceAfter(ParsingNode* currentNode)
				{
					if(currentNode==referenceNode && !referenceBefore)
					{
						writer.WriteString(L" $");
					}
				}
			public:
				RuleFragmentLogger(stream::TextWriter& _writer, bool _logAction, ParsingNode* _referenceNode, bool _referenceBefore)
					:writer(_writer)
					,logAction(_logAction)
					,referenceNode(_referenceNode)
					,referenceBefore(_referenceBefore)
					,level(LevelAlt)
				{
				}

				void Log(ParsingNode* node)
				{
					BuildReferenceBefore(node);
					node->Accept(this);
					BuildReferenceAfter(node);
				}

				void Visit(parsing_internal::_Seq* node)override
				{
					vint oldLevel=level;
					level=LevelSeq;
					if(oldLevel<=LevelSeq)
					{
						node->first->Accept(this);
						writer.WriteString(L" ");
						node->second->Accept(this);
					}
					else
					{
						writer.WriteString(L"(");
						node->first->Accept(this);
						writer.WriteString(L" ");
						node->second->Accept(this);
						writer.WriteString(L")");
					}
					level=oldLevel;
				}

				void Visit(parsing_internal::_Alt* node)override
				{
					vint oldLevel=level;
					level=LevelAlt;
					if(oldLevel<=LevelAlt)
					{
						node->first->Accept(this);
						writer.WriteString(L" | ");
						node->second->Accept(this);
					}
					else
					{
						writer.WriteString(L"(");
						node->first->Accept(this);
						writer.WriteString(L" | ");
						node->second->Accept(this);
						writer.WriteString(L")");
					}
					level=oldLevel;
				}

				void Visit(parsing_internal::_Loop* node)override
				{
					vint oldLevel=level;
					level=LevelLoop;
					if(oldLevel<=LevelLoop)
					{
						node->node->Accept(this);
						writer.WriteString(L"*");
					}
					else
					{
						writer.WriteString(L"(");
						node->node->Accept(this);
						writer.WriteString(L")*");
					}
					level=oldLevel;
				}

				void Visit(parsing_internal::_Token* node)override
				{
					writer.WriteString(node->name);
				}

				void Visit(parsing_internal::_Rule* node)override
				{
					writer.WriteString(node->rule->name);
				}

				void Visit(parsing_internal::_Action* node)override
				{
					if(logAction)
					{
						vint oldLevel=level;
						level=LevelLoop;
						if(oldLevel<=LevelLoop)
						{
							node->node->Accept(this);
						}
						else
						{
							writer.WriteString(L"(");
							node->node->Accept(this);
							writer.WriteString(L")");
						}
						writer.WriteString(L"#");
						writer.WriteString(node->action->GetName());
						level=oldLevel;
					}
					else
					{
						node->node->Accept(this);
					}
				}
			};

			class RuleLogger : public Object, public IParsingNodeVisitor
			{
			protected:
				stream::TextWriter&				writer;
				bool							logAction;
				ParsingNode*					referenceNode;
				bool							referenceBefore;
				const RuleNode*					currentRule;
			public:
				RuleLogger(stream::TextWriter& _writer, bool _logAction, ParsingNode* _referenceNode, bool _referenceBefore)
					:writer(_writer)
					,logAction(_logAction)
					,referenceNode(_referenceNode)
					,referenceBefore(_referenceBefore)
					,currentRule(0)
				{
				}

				void Log(const RuleNode* rootRule)
				{
					currentRule=rootRule;
					rootRule->node->Accept(this);
				}

				void VisitOther(ParsingNode* node)
				{
					if(referenceNode)
					{
						if(!IsNodeContained(node, referenceNode))
						{
							return;
						}
					}
					writer.WriteString(currentRule->name);
					writer.WriteString(L" ::= ");
					RuleFragmentLogger ruleFragmentLogger(writer, logAction, referenceNode, referenceBefore);
					ruleFragmentLogger.Log(node);
					writer.WriteLine(L"");
				}

				void Visit(parsing_internal::_Seq* node)override
				{
					VisitOther(node);
				}

				void Visit(parsing_internal::_Alt* node)override
				{
					node->first->Accept(this);
					node->second->Accept(this);
				}

				void Visit(parsing_internal::_Loop* node)override
				{
					VisitOther(node);
				}

				void Visit(parsing_internal::_Token* node)override
				{
					VisitOther(node);
				}

				void Visit(parsing_internal::_Rule* node)override
				{
					VisitOther(node);
				}

				void Visit(parsing_internal::_Action* node)override
				{
					VisitOther(node);
				}
			};

			void LogGrammarFromRule(const RuleNode* rootRule, bool logAction, stream::TextWriter& writer)
			{
				List<const RuleNode*> rules;
				SearchRulesFromRule(rootRule, rules);

				RuleLogger ruleLogger(writer, logAction, 0, false);
				FOREACH(const RuleNode*, rule, rules.Wrap())
				{
					ruleLogger.Log(rule);
					writer.WriteLine(L"");
				}
			}

			void LogGrammarFromReferencedRule(const RuleNode* rootRule, bool logAction, ParsingNode* referenceNode, bool referenceBefore, stream::TextWriter& writer)
			{
				RuleLogger ruleLogger(writer, logAction, referenceNode, referenceBefore);
				ruleLogger.Log(rootRule);
			}
		}
	}
}