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

			WfLexicalSymbol::WfLexicalSymbol()
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

			WfLexicalScopeName::WfLexicalScopeName(bool _createdByTypeDescriptor)
				:parent(0)
				, createdByTypeDescriptor(_createdByTypeDescriptor)
				, typeDescriptor(0)
			{
			}

			WfLexicalScopeName::~WfLexicalScopeName()
			{
			}

			Ptr<WfLexicalScopeName> WfLexicalScopeName::AccessChild(const WString& name, bool createdByTypeDescriptor)
			{
				vint index = children.Keys().IndexOf(name);
				if (index == -1)
				{
					Ptr<WfLexicalScopeName> newName = new WfLexicalScopeName(createdByTypeDescriptor);
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

			void WfLexicalScopeName::RemoveNonTypeDescriptorNames()
			{
				for (vint i = children.Count() - 1; i >= 0; i--)
				{
					if (!children.Values()[i]->createdByTypeDescriptor)
					{
						children.Remove(children.Keys()[i]);
					}
				}
				
				declarations.Clear();
				FOREACH(Ptr<WfLexicalScopeName>, name, children.Values())
				{
					name->RemoveNonTypeDescriptorNames();
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

						currentName = currentName->AccessChild(fragment, true);
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
				name = name->AccessChild(declaration->name.value, false);
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

			WfLexicalScopeManager::WfLexicalScopeManager(Ptr<parsing::tabling::ParsingTable> _parsingTable)
				:parsingTable(_parsingTable)
			{
			}

			WfLexicalScopeManager::~WfLexicalScopeManager()
			{
			}

			Ptr<WfModule> WfLexicalScopeManager::AddModule(const WString& moduleCode, vint codeIndex)
			{
				if (auto module = WfParseModule(moduleCode, parsingTable, errors, codeIndex))
				{
					modules.Add(module);
					return module;
				}
				return 0;
			}

			void WfLexicalScopeManager::Clear(bool keepTypeDescriptorNames, bool deleteModules)
			{
				if (globalName)
				{
					if (keepTypeDescriptorNames)
					{
						globalName->RemoveNonTypeDescriptorNames();
					}
					else
					{
						globalName = 0;
					}
				}
				
				if (deleteModules)
				{
					modules.Clear();
				}

				errors.Clear();
				namespaceNames.Clear();
				moduleScopes.Clear();
				declarationScopes.Clear();
				statementScopes.Clear();
				expressionScopes.Clear();
			}

			void WfLexicalScopeManager::Rebuild(bool keepTypeDescriptorNames)
			{
				Clear(keepTypeDescriptorNames, false);
				if (!globalName)
				{
					globalName = new WfLexicalScopeName(true);
					BuildGlobalNameFromTypeDescriptors();
				}
				BuildGlobalNameFromModules();
				FOREACH(Ptr<WfModule>, module, modules)
				{
					vint errorCount = errors.Count();
					ValidateModuleStructure(this, module);
					if (errors.Count() == errorCount)
					{
						BuildScopeForModule(this, module);
					}
				}
			}
		}
	}
}