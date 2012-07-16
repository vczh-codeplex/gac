#include "Parsing.h"
#include "..\Collections\OperationForEach.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		class RuleFragmentLogger : public Object, public IParsingNodeVisitor
		{
		protected:
			stream::TextWriter&				writer;
		public:
			RuleFragmentLogger(stream::TextWriter& _writer)
				:writer(_writer)
			{
			}

			void Log(ParsingNode* node)
			{
				node->Accept(this);
			}

			void Visit(parsing_internal::_Seq* node)override
			{
			}

			void Visit(parsing_internal::_Alt* node)override
			{
			}

			void Visit(parsing_internal::_Loop* node)override
			{
			}

			void Visit(parsing_internal::_Token* node)override
			{
			}

			void Visit(parsing_internal::_Rule* node)override
			{
			}

			void Visit(parsing_internal::_Action* node)override
			{
			}
		};

		class RuleLogger : public Object, public IParsingNodeVisitor
		{
		protected:
			stream::TextWriter&				writer;
			const RuleNode*					currentRule;
		public:
			RuleLogger(stream::TextWriter& _writer)
				:writer(_writer)
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
				writer.WriteString(currentRule->name);
				writer.WriteString(L" ::= ");
				RuleFragmentLogger ruleFragmentLogger(writer);
				node->Accept(&ruleFragmentLogger);
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

		void LogGrammarFromRule(const RuleNode* rootRule, stream::TextWriter& writer)
		{
			List<const RuleNode*> rules;
			SearchRulesFromRule(rootRule, rules);

			RuleLogger ruleLogger(writer);
			FOREACH(const RuleNode*, rule, rules.Wrap())
			{
				ruleLogger.Log(rule);
				writer.WriteLine(L"");
			}
		}
	}
}