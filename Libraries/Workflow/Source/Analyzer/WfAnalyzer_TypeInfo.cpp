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
GetScopeNameFromReferenceType
***********************************************************************/

			class GetScopeNameFromReferenceTypeVisitor : public Object, public WfType::IVisitor
			{
			public:
				WfLexicalScope*				scope;
				Ptr<WfLexicalScopeName>		result;

				GetScopeNameFromReferenceTypeVisitor(WfLexicalScope* _scope)
					:scope(_scope)
				{
				}

				Ptr<WfLexicalScopeName> Call(WfType* node)
				{
					node->Accept(this);
					Ptr<WfLexicalScopeName> scopeName = result;
					result = 0;
					return scopeName;
				}

				void Visit(WfPredefinedType* node)override
				{
					WString name;
					switch (node->name)
					{
					case WfPredefinedTypeName::Void:
						name = L"Void";
						break;
					case WfPredefinedTypeName::Object:
						name = L"Object";
						break;
					case WfPredefinedTypeName::Interface:
						name = L"Interface";
						break;
					case WfPredefinedTypeName::Int:
#if defined VCZH_64
						name = L"Int64";
#else
						name = L"Int32";
#endif
						break;
					case WfPredefinedTypeName::UInt:
#if defined VCZH_64
						name = L"UInt64";
#else
						name = L"UInt32";
#endif
						break;
					case WfPredefinedTypeName::Float:
						name = L"Single";
						break;
					case WfPredefinedTypeName::Double:
						name = L"Double";
						break;
					case WfPredefinedTypeName::String:
						name = L"String";
						break;
					case WfPredefinedTypeName::Char:
						name = L"Char";
						break;
					case WfPredefinedTypeName::Bool:
						name = L"Boolean";
						break;
					default:
						CHECK_FAIL(L"GetScopeNameFromReferenceTypeVisitor::Visit(WfPredefinedType*)#Internal error, ValidateTypeStructure function should check correctly.");
					}

					Ptr<WfTopQualifiedType> ns = new WfTopQualifiedType;
					ns->name.value = L"system";

					Ptr<WfChildType> type = new WfChildType;
					type->parent = ns;
					type->name.value = name;

					type->Accept(this);
				}

				void Visit(WfTopQualifiedType* node)override
				{
					auto manager = scope->FindManager();
					if (manager->globalName)
					{
						vint index = manager->globalName->children.Keys().IndexOf(node->name.value);
						if (index != -1)
						{
							result = manager->globalName->children.Values()[index];
							return;
						}
					}
					manager->errors.Add(WfErrors::TopQualifiedSymbolNotExists(node, node->name.value));
				}

				void Visit(WfReferenceType* node)override
				{
					auto manager = scope->FindManager();

					List<Ptr<WfLexicalSymbol>> symbols;
					manager->ResolveSymbol(scope, node->name.value, symbols);
					if (symbols.Count() > 1)
					{
						manager->errors.Add(WfErrors::TooManyTargets(node, symbols, node->name.value));
						return;
					}
					else if (symbols.Count() == 1)
					{
						manager->errors.Add(WfErrors::TypeNotExists(node, symbols[0]));
						return;
					}

					List<Ptr<WfLexicalScopeName>> scopeNames;
					manager->ResolveScopeName(scope, node->name.value, scopeNames);
					if (scopeNames.Count() > 1)
					{
						manager->errors.Add(WfErrors::TooManyTargets(node, scopeNames, node->name.value));
					}
					else if (scopeNames.Count() == 1)
					{
						result = scopeNames[0];
					}
					else
					{
						manager->errors.Add(WfErrors::ReferenceNotExists(node, node->name.value));
					}
				}

				void Visit(WfRawPointerType* node)override
				{
					CHECK_FAIL(L"GetScopeNameFromReferenceTypeVisitor::Visit(WfRawPointerType*)#Internal error, ValidateTypeStructure function should check correctly.");
				}

				void Visit(WfSharedPointerType* node)override
				{
					CHECK_FAIL(L"GetScopeNameFromReferenceTypeVisitor::Visit(WfSharedPointerType*)#Internal error, ValidateTypeStructure function should check correctly.");
				}

				void Visit(WfNullableType* node)override
				{
					CHECK_FAIL(L"GetScopeNameFromReferenceTypeVisitor::Visit(WfNullableType*)#Internal error, ValidateTypeStructure function should check correctly.");
				}

				void Visit(WfEnumerableType* node)override
				{
					CHECK_FAIL(L"GetScopeNameFromReferenceTypeVisitor::Visit(WfEnumerableType*)#Internal error, ValidateTypeStructure function should check correctly.");
				}

				void Visit(WfMapType* node)override
				{
					CHECK_FAIL(L"GetScopeNameFromReferenceTypeVisitor::Visit(WfMapType*)#Internal error, ValidateTypeStructure function should check correctly.");
				}

				void Visit(WfFunctionType* node)override
				{
					CHECK_FAIL(L"GetScopeNameFromReferenceTypeVisitor::Visit(WfFunctionType*)#Internal error, ValidateTypeStructure function should check correctly.");
				}

				void Visit(WfChildType* node)override
				{
					if (Ptr<WfLexicalScopeName> scopeName = Call(node->parent.Obj()))
					{
						vint index = scopeName->children.Keys().IndexOf(node->name.value);
						if (index != -1)
						{
							result = scopeName->children.Values()[index];
							return;
						}
						scope->FindManager()->errors.Add(WfErrors::ChildSymbolNotExists(node, scopeName, node->name.value));
					}
				}

				static Ptr<WfLexicalScopeName> Execute(WfLexicalScope* scope, WfType* type)
				{
					return GetScopeNameFromReferenceTypeVisitor(scope).Call(type);
				}
			};

			Ptr<WfLexicalScopeName> GetScopeNameFromReferenceType(WfLexicalScope* scope, Ptr<WfType> type)
			{
				return GetScopeNameFromReferenceTypeVisitor::Execute(scope, type.Obj());
			}

/***********************************************************************
CreateTypeInfoFromType
***********************************************************************/

			class CreateTypeInfoFromTypeVisitor : public Object, public WfType::IVisitor
			{
			public:
				WfLexicalScope*				scope;
				Ptr<ITypeInfo>				result;

				CreateTypeInfoFromTypeVisitor(WfLexicalScope* _scope)
					:scope(_scope)
				{
				}

				Ptr<ITypeInfo> Call(WfType* node, bool checkTypeForValue)
				{
					node->Accept(this);
					Ptr<ITypeInfo> typeInfo = result;
					result = 0;
					if (typeInfo)
					{
						auto manager = scope->FindManager();
						switch (typeInfo->GetDecorator())
						{
						case ITypeInfo::RawPtr:
						case ITypeInfo::SharedPtr:
							{
								auto element = typeInfo->GetElementType();
								if (element->GetDecorator() == ITypeInfo::Generic)
								{
									element = element->GetElementType();
								}

								if (element->GetDecorator() == ITypeInfo::TypeDescriptor)
								{
									if (element->GetTypeDescriptor()->GetValueSerializer() == 0)
									{
										goto RAW_SHARED_POINTER_CORRECT;
									}
								}

								if (typeInfo->GetDecorator() == ITypeInfo::RawPtr)
								{
									manager->errors.Add(WfErrors::RawPointerToNonReferenceType(node, element));
								}
								else
								{
									manager->errors.Add(WfErrors::SharedPointerToNonReferenceType(node, element));
								}
							RAW_SHARED_POINTER_CORRECT:
								;
							}
							break;
						case ITypeInfo::Nullable:
							{
								auto element = typeInfo->GetElementType();
								if (element->GetDecorator() == ITypeInfo::Generic)
								{
									element = element->GetElementType();
								}

								if (element->GetDecorator() == ITypeInfo::TypeDescriptor)
								{
									if (element->GetTypeDescriptor()->GetValueSerializer() != 0)
									{
										goto NULLABLE_CORRECT;
									}
								}

								manager->errors.Add(WfErrors::NullableToNonReferenceType(node, element));
							NULLABLE_CORRECT:
								;
							}
							break;
						case ITypeInfo::TypeDescriptor:
							if (checkTypeForValue)
							{
								if (typeInfo->GetTypeDescriptor()->GetValueSerializer() == 0)
								{
									manager->errors.Add(WfErrors::TypeNotForValue(node, typeInfo.Obj()));
								}
							}
							break;
						case ITypeInfo::Generic:
							if (checkTypeForValue)
							{
								manager->errors.Add(WfErrors::TypeNotForValue(node, typeInfo.Obj()));
							}
							break;
						}
					}
					return typeInfo;
				}

				void VisitReferenceType(WfType* node)
				{
					if (auto scopeName = GetScopeNameFromReferenceTypeVisitor::Execute(scope, node))
					{
						if (auto typeDescriptor = description::GetTypeDescriptor(scopeName->GetFriendlyName()))
						{
							Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
							typeInfo->SetTypeDescriptor(typeDescriptor);
							result = typeInfo;
						}
						else
						{
							scope->FindManager()->errors.Add(WfErrors::TypeNotExists(node, scopeName));
						}
					}
				}

				void Visit(WfPredefinedType* node)override
				{
					ITypeDescriptor* typeDescriptor = 0;
					switch (node->name)
					{
					case WfPredefinedTypeName::Void:
						typeDescriptor = description::GetTypeDescriptor<VoidValue>();
						break;
					case WfPredefinedTypeName::Object:
						typeDescriptor = description::GetTypeDescriptor<Value>();
						break;
					case WfPredefinedTypeName::Interface:
						typeDescriptor = description::GetTypeDescriptor<IDescriptable>();
						break;
					case WfPredefinedTypeName::Int:
						typeDescriptor = description::GetTypeDescriptor<vint>();
						break;
					case WfPredefinedTypeName::UInt:
						typeDescriptor = description::GetTypeDescriptor<vuint>();
						break;
					case WfPredefinedTypeName::Float:
						typeDescriptor = description::GetTypeDescriptor<float>();
						break;
					case WfPredefinedTypeName::Double:
						typeDescriptor = description::GetTypeDescriptor<double>();
						break;
					case WfPredefinedTypeName::String:
						typeDescriptor = description::GetTypeDescriptor<WString>();
						break;
					case WfPredefinedTypeName::Char:
						typeDescriptor = description::GetTypeDescriptor<wchar_t>();
						break;
					case WfPredefinedTypeName::Bool:
						typeDescriptor = description::GetTypeDescriptor<bool>();
						break;
					default:
						CHECK_FAIL(L"CreateTypeInfoFromTypeVisitor::Visit(WfPredefinedType*)#Internal error, ValidateTypeStructure function should check correctly.");
					}
					if (typeDescriptor)
					{
						Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
						typeInfo->SetTypeDescriptor(typeDescriptor);
						result = typeInfo;
					}
				}

				void Visit(WfTopQualifiedType* node)override
				{
					VisitReferenceType(node);
				}

				void Visit(WfReferenceType* node)override
				{
					VisitReferenceType(node);
				}

				void Visit(WfRawPointerType* node)override
				{
					if (Ptr<ITypeInfo> element = Call(node->element.Obj(), false))
					{
						Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::RawPtr);
						typeInfo->SetElementType(element);
						result = typeInfo;
					}
				}

				void Visit(WfSharedPointerType* node)override
				{
					if (Ptr<ITypeInfo> element = Call(node->element.Obj(), false))
					{
						Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::SharedPtr);
						typeInfo->SetElementType(element);
						result = typeInfo;
					}
				}

				void Visit(WfNullableType* node)override
				{
					if (Ptr<ITypeInfo> element = Call(node->element.Obj(), false))
					{
						Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::Nullable);
						typeInfo->SetElementType(element);
						result = typeInfo;
					}
				}

				void Visit(WfEnumerableType* node)override
				{
					if (Ptr<ITypeInfo> element = Call(node->element.Obj(), true))
					{
						Ptr<TypeInfoImpl> enumerableTypeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
						enumerableTypeInfo->SetTypeDescriptor(description::GetTypeDescriptor<IValueEnumerable>());

						Ptr<TypeInfoImpl> genericTypeInfo = new TypeInfoImpl(ITypeInfo::Generic);
						genericTypeInfo->SetElementType(enumerableTypeInfo);
						genericTypeInfo->AddGenericArgument(element);

						Ptr<TypeInfoImpl> shared = new TypeInfoImpl(ITypeInfo::SharedPtr);
						shared->SetElementType(genericTypeInfo);
						result = shared;
					}
				}

				void Visit(WfMapType* node)override
				{
					Ptr<ITypeInfo> key, value;
					if (!(value = Call(node->value.Obj(), true))) return;
					if (node->key)
					{
						if (!(key = Call(node->key.Obj(), true))) return;
					}
					
					Ptr<TypeInfoImpl> mapTypeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					if (node->writability == WfMapWritability::Writable)
					{
						if (node->key)
						{
							mapTypeInfo->SetTypeDescriptor(description::GetTypeDescriptor<IValueDictionary>());
						}
						else
						{
							mapTypeInfo->SetTypeDescriptor(description::GetTypeDescriptor<IValueList>());
						}
					}
					else
					{
						if (node->key)
						{
							mapTypeInfo->SetTypeDescriptor(description::GetTypeDescriptor<IValueReadonlyDictionary>());
						}
						else
						{
							mapTypeInfo->SetTypeDescriptor(description::GetTypeDescriptor<IValueReadonlyList>());
						}
					}

					Ptr<TypeInfoImpl> genericTypeInfo = new TypeInfoImpl(ITypeInfo::Generic);
					genericTypeInfo->SetElementType(mapTypeInfo);
					if (key) genericTypeInfo->AddGenericArgument(key);
					genericTypeInfo->AddGenericArgument(value);

					Ptr<TypeInfoImpl> shared = new TypeInfoImpl(ITypeInfo::SharedPtr);
					shared->SetElementType(genericTypeInfo);
					result = shared;
				}

				void Visit(WfFunctionType* node)override
				{
					if (Ptr<ITypeInfo> returnType = Call(node->result.Obj(), true))
					{
						Ptr<TypeInfoImpl> enumerableTypeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
						enumerableTypeInfo->SetTypeDescriptor(description::GetTypeDescriptor<IValueFunctionProxy>());

						Ptr<TypeInfoImpl> genericTypeInfo = new TypeInfoImpl(ITypeInfo::Generic);
						genericTypeInfo->SetElementType(enumerableTypeInfo);
						genericTypeInfo->AddGenericArgument(returnType);
						FOREACH(Ptr<WfType>, argument, node->arguments)
						{
							if (Ptr<ITypeInfo> argumentType = Call(argument.Obj(), true))
							{
								genericTypeInfo->AddGenericArgument(argumentType);
							}
							else
							{
								return;
							}
						}

						Ptr<TypeInfoImpl> shared = new TypeInfoImpl(ITypeInfo::SharedPtr);
						shared->SetElementType(genericTypeInfo);
						result = shared;
					}
				}

				void Visit(WfChildType* node)override
				{
					VisitReferenceType(node);
				}

				static Ptr<ITypeInfo> Execute(WfLexicalScope* scope, WfType* type)
				{
					return CreateTypeInfoFromTypeVisitor(scope).Call(type, true);
				}
			};

			Ptr<reflection::description::ITypeInfo>	CreateTypeInfoFromType(WfLexicalScope* scope, Ptr<WfType> type)
			{
				return CreateTypeInfoFromTypeVisitor::Execute(scope, type.Obj());
			}
		}
	}
}