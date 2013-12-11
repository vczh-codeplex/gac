#include "WfAnalyzer.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			using namespace collections;
			using namespace reflection;
			using namespace reflection::description;

/***********************************************************************
WfLexicalSymbol
***********************************************************************/

			WfLexicalSymbol::WfLexicalSymbol(Ptr<WfDeclaration> declaration)
				:type(GetTypeFromDeclaration(declaration))
			{
			}

			WfLexicalSymbol::WfLexicalSymbol(Ptr<reflection::description::ITypeInfo> _typeInfo)
				:type(GetTypeFromTypeInfo(_typeInfo.Obj()))
				, typeInfo(_typeInfo)
			{
			}

			WfLexicalSymbol::~WfLexicalSymbol()
			{
			}

/***********************************************************************
WfLexicalScope
***********************************************************************/

			WfLexicalScope::WfLexicalScope(WfLexicalScopeManager* _ownerManager)
				:ownerManager(_ownerManager)
			{
			}

			WfLexicalScope::WfLexicalScope(Ptr<WfLexicalScope> _parentScope)
				:ownerManager(0)
				, parentScope(_parentScope)
			{
			}

			WfLexicalScope::~WfLexicalScope()
			{
			}

			WfLexicalScopeManager* WfLexicalScope::FindManager()
			{
				WfLexicalScope* scope = this;
				while (scope)
				{
					if (scope->ownerManager)
					{
						return scope->ownerManager;
					}
					scope = scope->parentScope.Obj();
				}
				return 0;
			}

			Ptr<WfModule> WfLexicalScope::FindModule()
			{
				WfLexicalScope* scope = this;
				while (scope)
				{
					if (scope->ownerModule)
					{
						return scope->ownerModule;
					}
					scope = scope->parentScope.Obj();
				}
				return 0;
			}

			Ptr<WfDeclaration> WfLexicalScope::FindDeclaration()
			{
				WfLexicalScope* scope = this;
				while (scope)
				{
					if (scope->ownerDeclaration)
					{
						return scope->ownerDeclaration;
					}
					scope = scope->parentScope.Obj();
				}
				return 0;
			}

			Ptr<WfLexicalSymbol> WfLexicalScope::ResolveSymbol(const WString& symbolName)
			{
				WfLexicalScope* scope = this;
				while (scope)
				{
					vint index = scope->symbols.Keys().IndexOf(symbolName);
					if (index != -1)
					{
						return scope->symbols.Values()[index];
					}
					scope = scope->parentScope.Obj();
				}
				return 0;
			}

/***********************************************************************
GetTypeFromDeclaration
***********************************************************************/

			class GetTypeFromDeclarationVisitor : public Object, public WfDeclaration::IVisitor
			{
			public:
				Ptr<WfType>					result;

				void Visit(WfNamespaceDeclaration* node)override
				{
					Ptr<WfPredefinedType> type = new WfPredefinedType;
					type->name = WfPredefinedTypeName::Namespace;
					result = type;
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					Ptr<WfFunctionType> type = new WfFunctionType;
					type->result = node->returnType;
					FOREACH(Ptr<WfFunctionArgument>, argument, node->arguments)
					{
						type->arguments.Add(argument->type);
					}
					result = type;
				}

				void Visit(WfVariableDeclaration* node)override
				{
					result = node->type;
				}

				static Ptr<WfType> Execute(Ptr<WfDeclaration> declaration)
				{
					GetTypeFromDeclarationVisitor visitor;
					declaration->Accept(&visitor);
					return visitor.result;
				}
			};

			Ptr<WfType> GetTypeFromDeclaration(Ptr<WfDeclaration> declaration)
			{
				return GetTypeFromDeclarationVisitor::Execute(declaration);
			}

/***********************************************************************
GetTypeFromTypeInfo
***********************************************************************/

			Ptr<WfType> GetTypeFromTypeInfo(reflection::description::ITypeInfo* typeInfo)
			{
				switch (typeInfo->GetDecorator())
				{
				case ITypeInfo::RawPtr:
					{
						Ptr<WfType> element = GetTypeFromTypeInfo(typeInfo->GetElementType());
						if (element)
						{
							Ptr<WfRawPointerType> type = new WfRawPointerType;
							type->element = element;
							return type;
						}
						return 0;
					}
				case ITypeInfo::SharedPtr:
					{
						if (typeInfo->GetElementType()->GetDecorator() == ITypeInfo::Generic)
						{
							return GetTypeFromTypeInfo(typeInfo->GetElementType());
						}
						else
						{
							Ptr<WfType> element = GetTypeFromTypeInfo(typeInfo->GetElementType());
							if (element)
							{
								Ptr<WfSharedPointerType> type = new WfSharedPointerType;
								type->element = element;
								return type;
							}
							return 0;
						}
					}
				case ITypeInfo::Nullable:
					{
						Ptr<WfType> element = GetTypeFromTypeInfo(typeInfo->GetElementType());
						if (element)
						{
							Ptr<WfNullableType> type = new WfNullableType;
							type->element = element;
							return type;
						}
						return 0;
					}
				case ITypeInfo::TypeDescriptor:
					{
						List<WString> fragments;
						{
							WString name = typeInfo->GetTypeDescriptor()->GetTypeName();
							const wchar_t* reading = name.Buffer();
							while (reading)
							{
								const wchar_t* delimiter = wcsstr(reading, L"::");
								if (delimiter)
								{
									fragments.Add(WString(reading, delimiter - reading));
									reading = delimiter + 2;
								}
								else
								{
									fragments.Add(reading);
									break;
								}
							}
						}

						Ptr<WfType> parentType;
						FOREACH(WString, fragment, fragments)
						{
							if (parentType)
							{
								Ptr<WfTopQualifiedType> type = new WfTopQualifiedType;
								type->name.value = fragment;
								parentType = type;
							}
							else
							{
								Ptr<WfChildType> type = new WfChildType;
								type->parent = parentType;
								type->name.value = fragment;
								parentType = type;
							}
						}
						return parentType;
					}
				case ITypeInfo::Generic:
					{
						ITypeInfo* genericType = typeInfo->GetElementType();
						if (genericType->GetDecorator() == ITypeInfo::TypeDescriptor)
						{
							if (genericType->GetTypeDescriptor() == GetTypeDescriptor<IValueEnumerable>())
							{
								if (genericType->GetGenericArgumentCount() == 1)
								{
									if (Ptr<WfType> elementType = GetTypeFromTypeInfo(genericType->GetGenericArgument(0)))
									{
										Ptr<WfEnumerableType> type = new WfEnumerableType;
										type->element = elementType;
										return type;
									}
								}
							}
							else if (genericType->GetTypeDescriptor() == GetTypeDescriptor<IValueReadonlyList>())
							{
								if (genericType->GetGenericArgumentCount() == 1)
								{
									if (Ptr<WfType> valueType = GetTypeFromTypeInfo(genericType->GetGenericArgument(0)))
									{
										Ptr<WfMapType> type = new WfMapType;
										type->writability = WfMapWritability::Readonly;
										type->value = valueType;
										return type;
									}
								}
							}
							else if (genericType->GetTypeDescriptor() == GetTypeDescriptor<IValueList>())
							{
								if (genericType->GetGenericArgumentCount() == 1)
								{
									if (Ptr<WfType> valueType = GetTypeFromTypeInfo(genericType->GetGenericArgument(0)))
									{
										Ptr<WfMapType> type = new WfMapType;
										type->writability = WfMapWritability::Writable;
										type->value = valueType;
										return type;
									}
								}
							}
							else if (genericType->GetTypeDescriptor() == GetTypeDescriptor<IValueReadonlyDictionary>())
							{
								if (genericType->GetGenericArgumentCount() == 2)
								{
									if (Ptr<WfType> keyType = GetTypeFromTypeInfo(genericType->GetGenericArgument(0)))
									if (Ptr<WfType> valueType = GetTypeFromTypeInfo(genericType->GetGenericArgument(1)))
									{
										Ptr<WfMapType> type = new WfMapType;
										type->writability = WfMapWritability::Readonly;
										type->key = keyType;
										type->value = valueType;
										return type;
									}
								}
							}
							else if (genericType->GetTypeDescriptor() == GetTypeDescriptor<IValueDictionary>())
							{
								if (genericType->GetGenericArgumentCount() == 2)
								{
									if (Ptr<WfType> keyType = GetTypeFromTypeInfo(genericType->GetGenericArgument(0)))
									if (Ptr<WfType> valueType = GetTypeFromTypeInfo(genericType->GetGenericArgument(1)))
									{
										Ptr<WfMapType> type = new WfMapType;
										type->writability = WfMapWritability::Writable;
										type->key = keyType;
										type->value = valueType;
										return type;
									}
								}
							}
							else if (genericType->GetTypeDescriptor() == GetTypeDescriptor<IValueFunctionProxy>())
							{
								if (genericType->GetGenericArgumentCount() >= 1)
								{
									if (Ptr<WfType> returnType = GetTypeFromTypeInfo(genericType->GetGenericArgument(0)))
									{
										Ptr<WfFunctionType> type = new WfFunctionType;
										type->result = type;
										for (vint i = 1; i < genericType->GetGenericArgumentCount(); i++)
										{
											if (Ptr<WfType> argumentType = GetTypeFromTypeInfo(genericType->GetGenericArgument(i)))
											{
												type->arguments.Add(argumentType);
											}
											else
											{
												return 0;
											}
										}
										return type;
									}
								}
							}
						}
						return 0;
					}
				default:
					return 0;
				}
			}

/***********************************************************************
CreateTypeInfoFromType
***********************************************************************/

			class CreateTypeInfoFromTypeVisitor : public Object, public WfType::IVisitor
			{
			public:
				WfLexicalScope*				scope;
				Ptr<ITypeInfo>				typeInfo;

				CreateTypeInfoFromTypeVisitor(WfLexicalScope* _scope)
					:scope(_scope)
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

				static Ptr<ITypeInfo> Execute(WfLexicalScope* scope, Ptr<WfType> type)
				{
					CreateTypeInfoFromTypeVisitor visitor(scope);
					type->Accept(&visitor);
					return visitor.typeInfo;
				}
			};

			Ptr<reflection::description::ITypeInfo>	CreateTypeInfoFromType(WfLexicalScope* scope, Ptr<WfType> type)
			{
				return CreateTypeInfoFromTypeVisitor::Execute(scope, type);
			}
		}
	}
}