#include "WfExpression.h"

namespace vl
{
	namespace workflow
	{
		using namespace stream;
		using namespace collections;

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
				switch (node->name)
				{
				case WfPredefinedTypeName::Void:
					writer.WriteString(L"void");
					break;
				case WfPredefinedTypeName::Object:
					writer.WriteString(L"object");
					break;
				case WfPredefinedTypeName::Interface:
					writer.WriteString(L"interface");
					break;
				case WfPredefinedTypeName::Int:
					writer.WriteString(L"int");
					break;
				case WfPredefinedTypeName::UInt:
					writer.WriteString(L"uint");
					break;
				case WfPredefinedTypeName::Float:
					writer.WriteString(L"float");
					break;
				case WfPredefinedTypeName::Double:
					writer.WriteString(L"double");
					break;
				case WfPredefinedTypeName::String:
					writer.WriteString(L"string");
					break;
				case WfPredefinedTypeName::Char:
					writer.WriteString(L"char");
					break;
				case WfPredefinedTypeName::Bool:
					writer.WriteString(L"bool");
					break;
				}
			}

			void Visit(WfTopQualifiedType* node)override
			{
				writer.WriteString(L"::" + node->name.value);
			}

			void Visit(WfReferenceType* node)override
			{
				writer.WriteString(node->name.value);
			}

			void Visit(WfRawPointerType* node)override
			{
				WfPrint(node->element, indent, writer);
				writer.WriteString(L"*");
			}

			void Visit(WfSharedPointerType* node)override
			{
				WfPrint(node->element, indent, writer);
				writer.WriteString(L"^");
			}

			void Visit(WfNullableType* node)override
			{
				WfPrint(node->element, indent, writer);
				writer.WriteString(L"?");
			}

			void Visit(WfEnumerableType* node)override
			{
				WfPrint(node->element, indent, writer);
				writer.WriteString(L"{}");
			}

			void Visit(WfMapType* node)override
			{
				if (node->writability == WfMapWritability::Readonly)
				{
					writer.WriteString(L"const ");
				}
				WfPrint(node->value, indent, writer);
				writer.WriteString(L"[");
				if (node->key)
				{
					WfPrint(node->key, indent, writer);
				}
				writer.WriteString(L"]");
			}

			void Visit(WfFunctionType* node)override
			{
				writer.WriteString(L"func ");
				writer.WriteString(L"(");
				FOREACH_INDEXER(Ptr<WfType>, type, index, node->arguments)
				{
					if (index > 0)
					{
						writer.WriteString(L", ");
					}
					WfPrint(type, indent, writer);
				}
				writer.WriteString(L")");

				writer.WriteString(L" : ");
				WfPrint(node->result, indent, writer);
			}

			void Visit(WfChildType* node)override
			{
				WfPrint(node->parent, indent, writer);
				writer.WriteString(L"::");
				writer.WriteString(node->name.value);
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
				writer.WriteLine(L"break;");
			}

			void Visit(WfContinueStatement* node)override
			{
				writer.WriteLine(L"continue;");
			}

			void Visit(WfReturnStatement* node)override
			{
				if (node->expression)
				{
					writer.WriteString(L"return ");
					WfPrint(node->expression, indent, writer);
					writer.WriteLine(L";");
				}
				else
				{
					writer.WriteLine(L"return;");
				}
			}

			void Visit(WfDeleteStatement* node)override
			{
				writer.WriteString(L"delete ");
				WfPrint(node->expression, indent, writer);
				writer.WriteLine(L";");
			}

			void Visit(WfRaiseExceptionStatement* node)override
			{
				if (node->expression)
				{
					writer.WriteString(L"raise ");
					WfPrint(node->expression, indent, writer);
					writer.WriteLine(L";");
				}
				else
				{
					writer.WriteLine(L"raise;");
				}
			}

			void Visit(WfIfStatement* node)override
			{
				writer.WriteString(L"if (");
				if (node->type)
				{
					writer.WriteString(L"var ");
					writer.WriteString(node->name.value);
					writer.WriteString(L" : ");
					WfPrint(node->type, indent, writer);
					writer.WriteString(L" = ");
				}
				WfPrint(node->expression, indent, writer);
				writer.WriteLine(L")");

				writer.WriteString(indent);
				WfPrint(node->trueBranch, indent, writer);
				if (node->falseBranch)
				{
					writer.WriteString(indent);
					writer.WriteLine(L"else");
					writer.WriteString(indent);
					WfPrint(node->trueBranch, indent, writer);
				}
			}

			void Visit(WfSwitchStatement* node)override
			{
				writer.WriteString(L"switch (");
				WfPrint(node->expression, indent, writer);
				writer.WriteLine(L")");

				writer.WriteString(indent);
				writer.WriteLine(L"{");

				FOREACH(Ptr<WfSwitchCase>, switchCase, node->caseBranches)
				{
					writer.WriteString(indent);
					writer.WriteString(L"case ");
					WfPrint(switchCase->expression, indent, writer);
					writer.WriteLine(L":");
					writer.WriteString(indent + L"    ");
					WfPrint(switchCase->statement, indent + L"    ", writer);
				}
				if (node->defaultBranch)
				{
					writer.WriteString(indent);
					writer.WriteLine(L"default:");
					writer.WriteString(indent + L"    ");
					WfPrint(node->defaultBranch, indent + L"    ", writer);
				}

				writer.WriteString(indent);
				writer.WriteLine(L"}");
			}

			void Visit(WfWhileStatement* node)override
			{
				writer.WriteString(L"while (");
				WfPrint(node->condition, indent, writer);
				writer.WriteLine(L")");
				writer.WriteString(indent);
				WfPrint(node->statement, indent, writer);
			}

			void Visit(WfForEachStatement* node)override
			{
				writer.WriteString(L"for (");
				writer.WriteString(node->name.value);
				writer.WriteString(L" in ");
				if (node->direction == WfForEachDirection::Reversed)
				{
					writer.WriteString(L"reversed ");
				}
				WfPrint(node->collection, indent, writer);
				writer.WriteLine(L")");
				writer.WriteString(indent);
				WfPrint(node->statement, indent, writer);
			}

			void Visit(WfTryStatement* node)override
			{
				writer.WriteLine(L"try");
				writer.WriteString(indent);
				WfPrint(node->protectedStatement, indent, writer);

				if (node->catchStatement)
				{
					writer.WriteString(indent);
					writer.WriteString(L"catch (");
					writer.WriteString(node->name.value);
					writer.WriteLine(L")");
					writer.WriteString(indent);
					WfPrint(node->catchStatement, indent, writer);
				}

				if (node->finallyStatement)
				{
					writer.WriteString(indent);
					writer.WriteLine(L"finally");
					writer.WriteString(indent);
					WfPrint(node->finallyStatement, indent, writer);
				}
			}

			void Visit(WfBlockStatement* node)override
			{
				writer.WriteLine(L"{");
				FOREACH(Ptr<WfStatement>, statement, node->statements)
				{
					writer.WriteString(indent + L"    ");
					WfPrint(statement, indent + L"    ", writer);
				}
				writer.WriteString(indent);
				writer.WriteLine(L"}");
			}

			void Visit(WfExpressionStatement* node)override
			{
				WfPrint(node->expression, indent, writer);
				writer.WriteLine(L";");
			}

			void Visit(WfVariableStatement* node)override
			{
				WfPrint(Ptr<WfDeclaration>(node->variable), indent, writer);
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
				writer.WriteLine(L"namespace " + node->name.value);
				writer.WriteString(indent);
				writer.WriteLine(L"{");
				FOREACH_INDEXER(Ptr<WfDeclaration>, decl, index, node->declarations)
				{
					if (index > 0)
					{
						writer.WriteLine(L"");
					}
					WfPrint(decl, indent + L"    ", writer);
				}
				writer.WriteString(indent);
				writer.WriteLine(L"}");
			}

			void Visit(WfFunctionDeclaration* node)override
			{
				writer.WriteString(L"func ");
				if (node->anonymity == WfFunctionAnonymity::Named)
				{
					writer.WriteString(node->name.value);
				}

				writer.WriteString(L"(");
				FOREACH_INDEXER(Ptr<WfFunctionArgument>, argument, index, node->arguments)
				{
					if (index > 0)
					{
						writer.WriteString(L", ");
					}
					writer.WriteString(argument->name.value);
					writer.WriteString(L" : ");
					WfPrint(argument->type, indent, writer);
				}
				writer.WriteString(L")");

				writer.WriteString(L" : ");
				WfPrint(node->returnType, indent, writer);
				writer.WriteLine(L"");

				writer.WriteString(indent);
				WfPrint(node->statement, indent, writer);
			}

			void Visit(WfVariableDeclaration* node)override
			{
				writer.WriteString(L"var ");
				writer.WriteString(node->name.value);
				if (node->type)
				{
					writer.WriteString(L" : ");
					WfPrint(node->type, indent, writer);
				}
				writer.WriteString(L" = ");
				WfPrint(node->expression, indent, writer);
				writer.WriteLine(L";");
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
			writer.WriteString(indent);
			switch (node->moduleType)
			{
			case WfModuleType::Module:
				writer.WriteLine(L"module " + node->name.value + L";");
				break;
			case WfModuleType::Unit:
				writer.WriteLine(L"unit " + node->name.value + L";");
				break;
			}

			FOREACH(Ptr<WfModuleUsingPath>, path, node->paths)
			{
				writer.WriteString(indent);
				writer.WriteString(L"using ");
				FOREACH_INDEXER(Ptr<WfModuleUsingItem>, item, index, path->items)
				{
					if (index > 0)
					{
						writer.WriteString(L"::");
					}
					FOREACH(Ptr<WfModuleUsingFragment>, fragment, item->fragments)
					{
						if (auto name = fragment.Cast<WfModuleUsingNameFragment>())
						{
							writer.WriteString(name->name.value);
						}
						else
						{
							writer.WriteString(L"*");
						}
					}
				}
				writer.WriteLine(L";");
			}

			FOREACH(Ptr<WfDeclaration>, decl, node->declarations)
			{
				writer.WriteLine(L"");
				writer.WriteString(indent);
				WfPrint(decl, indent, writer);
			}
		}
	}
}
