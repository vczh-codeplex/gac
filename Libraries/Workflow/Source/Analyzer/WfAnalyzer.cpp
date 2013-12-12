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

			Ptr<WfLexicalScopeName> WfLexicalScopeName::AccessChild(const WString& name)
			{
				vint index = children.Keys().IndexOf(name);
				if (index == -1)
				{
					Ptr<WfLexicalScopeName> newName = new WfLexicalScopeName;
					newName->name = name;
					newName->parent = this;
					children.Add(name, newName);
					return newName;
				}
				else
				{
					return children.Values()[index];
				}
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

						currentName = currentName->AccessChild(fragment);
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
				name = name->AccessChild(declaration->name.value);
				name->declarations.Add(declaration);
				if (auto ns = declaration.Cast<WfNamespaceDeclaration>())
				{
					namespaceNames.Add(ns, name);
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
				namespaceNames.Clear();
				BuildGlobalNameFromTypeDescriptors();
				BuildGlobalNameFromModules();
			}

			void WfLexicalScopeManager::BuildScopes()
			{
				moduleScopes.Clear();
				declarationScopes.Clear();
				statementScopes.Clear();
				expressionScopes.Clear();

				FOREACH(Ptr<WfModule>, module, modules)
				{
					BuildScopeForModule(this, module);
				}
			}
		}
	}
}