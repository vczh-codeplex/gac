#include "WfExpression.h"

namespace vl
{
	namespace workflow
	{
		using namespace stream;

/***********************************************************************
Unescaping Functions
***********************************************************************/

		void UnescapeStringInternal(vl::parsing::ParsingToken& value, bool formatString)
		{
			MemoryStream memoryStream;
			{
				WString input = formatString ? value.value.Sub(2, value.value.Length() - 3) : value.value.Sub(1, value.value.Length() - 2);
				const wchar_t* reading = input.Buffer();
				StreamWriter writer(memoryStream);

				while (wchar_t c = *reading++)
				{
					if (c == L'\\')
					{
						switch (wchar_t e = *reading++)
						{
						case L'r':
							writer.WriteChar('\r');
							break;
						case L'n':
							writer.WriteChar('\n');
							break;
						case L't':
							writer.WriteChar('\t');
							break;
						default:
							writer.WriteChar(e);
						}
					}
					else
					{
						writer.WriteChar(c);
					}
				}
			}

			memoryStream.SeekFromBegin(0);
			{
				StreamReader reader(memoryStream);
				value.value = reader.ReadToEnd();
			}
		}

		void UnescapeFormatString(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			UnescapeStringInternal(value, true);
		}

		void UnescapeString(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			UnescapeStringInternal(value, false);
		}

/***********************************************************************
Print (Type)
***********************************************************************/

		class PrintTypeVisitor : public Object, public WfType::IVisitor
		{
		public:
			WString								indent;
			TextWriter&							writer;

			PrintTypeVisitor(const WString& _indent, stream::TextWriter& _writer)
				:indent(_indent), writer(_writer)
			{
			}

			void Visit(WfPredefinedType* node)override
			{
			}

			void Visit(WfTopQualifiedType* node)override
			{
			}

			void Visit(WfReferenceType* node)override
			{
			}

			void Visit(WfRawPointerType* node)override
			{
			}

			void Visit(WfSharedPointerType* node)override
			{
			}

			void Visit(WfNullableType* node)override
			{
			}

			void Visit(WfEnumerableType* node)override
			{
			}

			void Visit(WfMapType* node)override
			{
			}

			void Visit(WfFunctionType* node)override
			{
			}

			void Visit(WfChildType* node)override
			{
			}
		};

		void WfPrint(Ptr<WfType> node, const WString& indent, stream::TextWriter& writer)
		{
			PrintTypeVisitor visitor(indent, writer);
			node->Accept(&visitor);
		}

/***********************************************************************
Print (Expression)
***********************************************************************/

		class PrintExpressionVisitor : public Object, public WfExpression::IVisitor
		{
		public:
			WString								indent;
			TextWriter&							writer;

			PrintExpressionVisitor(const WString& _indent, stream::TextWriter& _writer)
				:indent(_indent), writer(_writer)
			{
			}

			void Visit(WfTopQualifiedExpression* node)override
			{
			}

			void Visit(WfReferenceExpression* node)override
			{
			}

			void Visit(WfOrderedNameExpression* node)override
			{
			}

			void Visit(WfOrderedLambdaExpression* node)override
			{
			}

			void Visit(WfMemberExpression* node)override
			{
			}

			void Visit(WfChildExpression* node)override
			{
			}

			void Visit(WfLiteralExpression* node)override
			{
			}

			void Visit(WfFloatingExpression* node)override
			{
			}

			void Visit(WfIntegerExpression* node)override
			{
			}

			void Visit(WfStringExpression* node)override
			{
			}

			void Visit(WfFormatExpression* node)override
			{
			}

			void Visit(WfUnaryExpression* node)override
			{
			}

			void Visit(WfBinaryExpression* node)override
			{
			}

			void Visit(WfLetExpression* node)override
			{
			}

			void Visit(WfIfExpression* node)override
			{
			}

			void Visit(WfRangeExpression* node)override
			{
			}

			void Visit(WfSetTestingExpression* node)override
			{
			}

			void Visit(WfConstructorExpression* node)override
			{
			}

			void Visit(WfInferExpression* node)override
			{
			}

			void Visit(WfTypeCastingExpression* node)override
			{
			}

			void Visit(WfTypeTestingExpression* node)override
			{
			}

			void Visit(WfTypeOfTypeExpression* node)override
			{
			}

			void Visit(WfTypeOfExpressionExpression* node)override
			{
			}

			void Visit(WfAttachEventExpression* node)override
			{
			}

			void Visit(WfDetachEventExpression* node)override
			{
			}

			void Visit(WfBindExpression* node)override
			{
			}

			void Visit(WfObserveExpression* node)override
			{
			}

			void Visit(WfCallExpression* node)override
			{
			}

			void Visit(WfFunctionExpression* node)override
			{
			}

			void Visit(WfNewTypeExpression* node)override
			{
			}
		};

		void WfPrint(Ptr<WfExpression> node, const WString& indent, stream::TextWriter& writer)
		{
			PrintExpressionVisitor visitor(indent, writer);
			node->Accept(&visitor);
		}

/***********************************************************************
Print (Statement)
***********************************************************************/

		class PrintStatementVisitor : public Object, public WfStatement::IVisitor
		{
		public:
			WString								indent;
			TextWriter&							writer;

			PrintStatementVisitor(const WString& _indent, stream::TextWriter& _writer)
				:indent(_indent), writer(_writer)
			{
			}

			void Visit(WfBreakStatement* node)override
			{
			}

			void Visit(WfContinueStatement* node)override
			{
			}

			void Visit(WfReturnStatement* node)override
			{
			}

			void Visit(WfDeleteStatement* node)override
			{
			}

			void Visit(WfRaiseExceptionStatement* node)override
			{
			}

			void Visit(WfIfStatement* node)override
			{
			}

			void Visit(WfSwitchStatement* node)override
			{
			}

			void Visit(WfWhileStatement* node)override
			{
			}

			void Visit(WfForEachStatement* node)override
			{
			}

			void Visit(WfTryStatement* node)override
			{
			}

			void Visit(WfBlockStatement* node)override
			{
			}

			void Visit(WfExpressionStatement* node)override
			{
			}

			void Visit(WfVariableStatement* node)override
			{
			}
		};

		void WfPrint(Ptr<WfStatement> node, const WString& indent, stream::TextWriter& writer)
		{
			PrintStatementVisitor visitor(indent, writer);
			node->Accept(&visitor);
		}

/***********************************************************************
Print (Declaration)
***********************************************************************/

		class PrintDeclarationVisitor : public Object, public WfDeclaration::IVisitor
		{
		public:
			WString								indent;
			TextWriter&							writer;

			PrintDeclarationVisitor(const WString& _indent, stream::TextWriter& _writer)
				:indent(_indent), writer(_writer)
			{
			}

			void Visit(WfNamespaceDeclaration* node)override
			{
			}

			void Visit(WfFunctionDeclaration* node)override
			{
			}

			void Visit(WfVariableDeclaration* node)override
			{
			}
		};

		void WfPrint(Ptr<WfDeclaration> node, const WString& indent, stream::TextWriter& writer)
		{
			PrintDeclarationVisitor visitor(indent, writer);
			node->Accept(&visitor);
		}

/***********************************************************************
Print (Module)
***********************************************************************/

		void WfPrint(Ptr<WfModule> node, const WString& indent, stream::TextWriter& writer)
		{

		}
	}
}
