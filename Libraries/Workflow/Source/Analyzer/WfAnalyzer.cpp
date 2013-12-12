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
				throw 0;
			}

/***********************************************************************
WfLexicalScopeName
***********************************************************************/

			WfLexicalScopeName::WfLexicalScopeName()
				:parent(0)
				, typeDescriptor(0)
			{
			}

			WfLexicalScopeName::~WfLexicalScopeName()
			{
			}

/***********************************************************************
WfLexicalScopeManager
***********************************************************************/

			void WfLexicalScopeManager::BuildGlobalNameFromTypeDescriptors()
			{
				for (vint i = 0; i < GetGlobalTypeManager()->GetTypeDescriptorCount(); i++)
				{
					ITypeDescriptor* typeDescriptor = GetGlobalTypeManager()->GetTypeDescriptor(i);
					WString name = typeDescriptor->GetTypeName();
					const wchar_t* reading = name.Buffer();
					Ptr<WfLexicalScopeName> currentName = globalName;

					while (true)
					{
						WString fragment;
						const wchar_t* delimiter = wcsstr(reading, L"::");
						if (delimiter)
						{
							fragment = WString(reading, delimiter - reading);
							reading = delimiter + 2;
						}
						else
						{
							fragment = reading;
							reading = 0;
						}

						vint index = currentName->children.Keys().IndexOf(fragment);
						if (index == -1)
						{
							Ptr<WfLexicalScopeName> newName = new WfLexicalScopeName;
							newName->name = fragment;
							currentName->children.Add(fragment, newName);
							currentName = newName;
						}
						else
						{
							currentName = currentName->children.Values()[index];
						}

						if (!reading)
						{
							currentName->typeDescriptor = typeDescriptor;
							break;
						}
					}
				}
			}

			void WfLexicalScopeManager::BuildGlobalNameFromModules()
			{
				FOREACH(Ptr<WfModule>, module, modules)
				{
					FOREACH(Ptr<WfDeclaration>, declaration, module->declarations)
					{
						BuildName(globalName, declaration);
					}
				}
			}

			void WfLexicalScopeManager::BuildName(Ptr<WfLexicalScopeName> name, Ptr<WfDeclaration> declaration)
			{
				vint index = name->children.Keys().IndexOf(declaration->name.value);
				if (index == -1)
				{
					Ptr<WfLexicalScopeName> newName = new WfLexicalScopeName;
					newName->name = declaration->name.value;
					name->children.Add(declaration->name.value, newName);
					name = newName;
				}
				else
				{
					name = name->children.Values()[index];
				}

				name->declarations.Add(declaration);
				if (auto ns = declaration.Cast<WfNamespaceDeclaration>())
				{
					FOREACH(Ptr<WfDeclaration>, subDecl, ns->declarations)
					{
						BuildName(name, subDecl);
					}
				}
			}

			WfLexicalScopeManager::WfLexicalScopeManager()
			{
			}

			WfLexicalScopeManager::~WfLexicalScopeManager()
			{
			}

			void WfLexicalScopeManager::BuildGlobalName()
			{
				globalName = new WfLexicalScopeName;
				BuildGlobalNameFromTypeDescriptors();
				BuildGlobalNameFromModules();
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
				Ptr<ITypeInfo>				result;

				CreateTypeInfoFromTypeVisitor(WfLexicalScope* _scope)
					:scope(_scope)
				{
				}

				Ptr<ITypeInfo> Call(WfType* node)
				{
					node->Accept(this);
					Ptr<ITypeInfo> typeInfo = result;
					result = 0;
					return typeInfo;
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
					throw 0;
				}

				void Visit(WfReferenceType* node)override
				{
					throw 0;
				}

				void Visit(WfRawPointerType* node)override
				{
					if (Ptr<ITypeInfo> element = Call(node->element.Obj()))
					{
						Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::RawPtr);
						typeInfo->SetElementType(element);
						result = typeInfo;
					}
				}

				void Visit(WfSharedPointerType* node)override
				{
					if (Ptr<ITypeInfo> element = Call(node->element.Obj()))
					{
						Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::SharedPtr);
						typeInfo->SetElementType(element);
						result = typeInfo;
					}
				}

				void Visit(WfNullableType* node)override
				{
					if (Ptr<ITypeInfo> element = Call(node->element.Obj()))
					{
						Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::Nullable);
						typeInfo->SetElementType(element);
						result = typeInfo;
					}
				}

				void Visit(WfEnumerableType* node)override
				{
					if (Ptr<ITypeInfo> element = Call(node->element.Obj()))
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
					if (!(value = Call(node->value.Obj()))) return;
					if (node->key)
					{
						if (!(key = Call(node->key.Obj()))) return;
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
					if (Ptr<ITypeInfo> returnType = Call(node->result.Obj()))
					{
						Ptr<TypeInfoImpl> enumerableTypeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
						enumerableTypeInfo->SetTypeDescriptor(description::GetTypeDescriptor<IValueFunctionProxy>());

						Ptr<TypeInfoImpl> genericTypeInfo = new TypeInfoImpl(ITypeInfo::Generic);
						genericTypeInfo->SetElementType(enumerableTypeInfo);
						genericTypeInfo->AddGenericArgument(returnType);
						FOREACH(Ptr<WfType>, argument, node->arguments)
						{
							if (Ptr<ITypeInfo> argumentType = Call(argument.Obj()))
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
					throw 0;
				}

				static Ptr<ITypeInfo> Execute(WfLexicalScope* scope, Ptr<WfType> type)
				{
					CreateTypeInfoFromTypeVisitor visitor(scope);
					type->Accept(&visitor);
					return visitor.result;
				}
			};

			Ptr<reflection::description::ITypeInfo>	CreateTypeInfoFromType(WfLexicalScope* scope, Ptr<WfType> type)
			{
				return CreateTypeInfoFromTypeVisitor::Execute(scope, type);
			}
		}
	}
}