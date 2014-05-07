#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIReflection.h"

using namespace vl::console;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::stream;
using namespace vl::reflection::description;
using namespace vl::parsing;
using namespace vl::parsing::tabling;
using namespace vl::workflow;
using namespace vl::workflow::analyzer;

Array<WString>* arguments = 0;

void PrintErrorMessage(const WString& message)
{
	Console::SetColor(true, false, false, true);
	Console::WriteLine(message);
	Console::SetColor(true, true, true, false);
}

void PrintSuccessMessage(const WString& message)
{
	Console::SetColor(false, true, false, true);
	Console::WriteLine(message);
	Console::SetColor(true, true, true, false);
}

void PrintInformationMessage(const WString& message)
{
	Console::SetColor(true, true, false, true);
	Console::WriteLine(message);
	Console::SetColor(true, true, true, false);
}

int wmain(int argc, wchar_t* argv[])
{
	Array<WString> _arguments(argc - 1);
	for (vint i = 1; i < argc; i++)
	{
		_arguments[i - 1] = argv[i];
	}
	arguments = &_arguments;
	SetupWindowsDirect2DRenderer();
}

/***********************************************************************
Object Model
***********************************************************************/

struct Instance
{
	Ptr<GuiInstanceContext>						context;
	ITypeDescriptor*							baseType;
	List<WString>								namespaces;
	WString										typeName;
	Dictionary<WString, GuiConstructorRepr*>	fields;
	Dictionary<WString, ITypeDescriptor*>		eventHandlers;

	WString GetFullName()
	{
		return From(namespaces)
			.Aggregate(WString(), [](const WString& a, const WString& b)->WString
			{
				return a + b + L"::";
			})
			+ typeName;
	}
};

struct InstanceSchema
{
	List<WString>								namespaces;
	WString										typeName;
	Ptr<GuiInstanceTypeSchema>					schema;

	WString GetFullName()
	{
		return From(namespaces)
			.Aggregate(WString(), [](const WString& a, const WString& b)->WString
			{
				return a + b + L"::";
			})
			+ typeName;
	}
};

class CodegenConfig
{
public:
	Ptr<GuiResource>							resource;
	WString										include;
	WString										name;
	WString										prefix;

	Ptr<ParsingTable>							workflowTable;
	Ptr<WfLexicalScopeManager>					workflowManager;

	static bool LoadConfigString(Ptr<GuiResource> resource, const WString& name, WString& value)
	{
		if (auto includeItem = resource->GetValueByPath(L"GacGenConfig/" + name).Cast<GuiTextData>())
		{
			value = includeItem->GetText();
			return true;
		}
		else
		{
			PrintErrorMessage(L"error> Cannot find configuration in resource \"GacGenConfig/" + name + L"\".");
			return false;
		}
	}

	static Ptr<CodegenConfig> LoadConfig(Ptr<GuiResource> resource)
	{
		Ptr<CodegenConfig> config = new CodegenConfig;
		config->resource = resource;
		if (!LoadConfigString(resource, L"Include", config->include)) return false;
		if (!LoadConfigString(resource, L"Name", config->name)) return false;
		if (!LoadConfigString(resource, L"Prefix", config->prefix)) return false;
		return config;
	}

	WString GetControlClassHeaderFileName(Ptr<Instance> instance)
	{
		return prefix + instance->typeName + L".h";
	}

	WString GetControlClassCppFileName(Ptr<Instance> instance)
	{
		return prefix + instance->typeName + L".cpp";
	}

	WString GetPartialClassHeaderFileName()
	{
		return name + L"PartialClasses.h";
	}

	WString GetPartialClassCppFileName()
	{
		return name + L"PartialClasses.cpp";
	}

	WString GetGlobalHeaderFileName()
	{
		return name + L".h";
	}
};

/***********************************************************************
TypeTransformation
***********************************************************************/

bool DetermineCppType(
	Ptr<CodegenConfig> config,
	Dictionary<WString, Ptr<Instance>>& instances,
	Ptr<Instance> instance,
	GuiConstructorRepr* ctor,
	WString& typeName,
	IGuiInstanceLoader*& loader,
	const Func<bool()>& stop
	)
{
	Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(config->resource, config->resource->GetWorkingDirectory());
	Ptr<GuiInstanceContext> currentContext = instance->context;
	GuiConstructorRepr* currentCtor = ctor;
	typeName = L"";
	loader = 0;

	while (currentContext)
	{
		Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(currentContext, resolver);
		auto loadingSource = FindInstanceLoadingSource(env, currentCtor);
		currentContext = loadingSource.context;
		currentCtor = currentContext ? currentContext->instance.Obj() : 0;
		typeName = loadingSource.typeName;
		loader = loadingSource.loader;

		if (stop())
		{
			return true;
		}
	}

	return false;
}

WString GetCppTypeName(ITypeDescriptor* typeDescriptor)
{
	return L"vl::" + typeDescriptor->GetTypeName();
}

WString GetCppTypeName(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances, Ptr<Instance> instance, GuiConstructorRepr* ctor)
{
	WString typeName;
	IGuiInstanceLoader* loader = 0;
	if (DetermineCppType(config, instances, instance, ctor, typeName, loader, [&]()
		{
			return instances.Keys().Contains(typeName);
		}))
	{
		return typeName;
	}
	else
	{
		auto manager = GetInstanceLoaderManager();
		return GetCppTypeName(manager->GetTypeDescriptorForType(typeName));
	}
}

IGuiInstanceLoader::TypeInfo GetCppTypeInfo(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances, Ptr<Instance> instance, GuiConstructorRepr* ctor)
{
	WString typeName;
	IGuiInstanceLoader* loader = 0;
	DetermineCppType(config, instances, instance, ctor, typeName, loader, [&]()
	{
		return loader != 0;
	});
	auto manager = GetInstanceLoaderManager();
	ITypeDescriptor* typeDescriptor = manager->GetTypeDescriptorForType(typeName);
	return IGuiInstanceLoader::TypeInfo(typeName, typeDescriptor);
}

/***********************************************************************
WorkflowTypeTransformation
***********************************************************************/

class GetCppTypeNameFromWorkflowTypeVisitor : public Object, public WfType::IVisitor
{
public:
	WString					result;

	static WString Call(Ptr<WfType> node)
	{
		GetCppTypeNameFromWorkflowTypeVisitor visitor;
		node->Accept(&visitor);
		return visitor.result;
	}

	void VisitReferenceType(WfType* node)
	{
		WString typeName;
		{
			MemoryStream stream;
			{
				StreamWriter writer(stream);
				WfPrint(node, L"", writer);
			}

			stream.SeekFromBegin(0);
			StreamReader reader(stream);
			typeName = reader.ReadToEnd();
		}

		auto td = description::GetTypeDescriptor(typeName);
		if (td)
		{
			typeName = td->GetTypeName();
			if (typeName == L"system::Void")
			{
				result = L"void";
			}
			else if (typeName == L"system::Interface")
			{
				result = L"IDescriptable";
			}
			else if (typeName == L"system::ReferenceType")
			{
				result = L"DescriptableObject";
			}
			else if (typeName == L"system::Object")
			{
				result = L"descriptable::Value";
			}
			else if (typeName == L"system::UInt8")
			{
				result = L"vuint8_t";
			}
			else if (typeName == L"system::UInt16")
			{
				result = L"vuint16_t";
			}
			else if (typeName == L"system::UInt32")
			{
				result = L"vuint32_t";
			}
			else if (typeName == L"system::UInt64")
			{
				result = L"vuint64_t";
			}
			else if (typeName == L"system::Int8")
			{
				result = L"vint8_t";
			}
			else if (typeName == L"system::Int16")
			{
				result = L"vint16_t";
			}
			else if (typeName == L"system::Int32")
			{
				result = L"vint32_t";
			}
			else if (typeName == L"system::Int64")
			{
				result = L"vint64_t";
			}
			else if (typeName == L"system::Single")
			{
				result = L"float";
			}
			else if (typeName == L"system::Double")
			{
				result = L"double";
			}
			else if (typeName == L"system::Boolean")
			{
				result = L"bool";
			}
			else if (typeName == L"system::Char")
			{
				result = L"wchar_t";
			}
			else if (typeName == L"system::String")
			{
				result = L"WString";
			}
			else if (typeName == L"system::DateTime")
			{
				result = L"DateTime";
			}
			else if (typeName == L"system::Locale")
			{
				result = L"Locale";
			}
			else if (typeName == L"system::ObservableList")
			{
				result = L"presentation::description::IValueObservableList";
			}
			else if (typeName.Length() >= 11 && typeName.Left(11) == L"system::Xml")
			{
				result = L"parsing::xml::" + typeName.Right(typeName.Length() - 8);
			}
			else if (typeName.Length() >= 12 && typeName.Left(12) == L"system::Json")
			{
				result = L"parsing::json::" + typeName.Right(typeName.Length() - 8);
			}
			else
			{
				result = typeName;
			}
		}
		else
		{
			result = typeName;
		}
	}

	void Visit(WfPredefinedType* node)override
	{
		switch (node->name)
		{
		case WfPredefinedTypeName::Void:
			result = L"void";
			break;
		case WfPredefinedTypeName::Object:
			result = L"description::Value";
			break;
		case WfPredefinedTypeName::Interface:
			result = L"IDescriptable";
			break;
		case WfPredefinedTypeName::Int:
			result = L"vint";
			break;
		case WfPredefinedTypeName::UInt:
			result = L"vuint";
			break;
		case WfPredefinedTypeName::Float:
			result = L"float";
			break;
		case WfPredefinedTypeName::Double:
			result = L"double";
			break;
		case WfPredefinedTypeName::String:
			result = L"WString";
			break;
		case WfPredefinedTypeName::Char:
			result = L"wchar_t";
			break;
		case WfPredefinedTypeName::Bool:
			result = L"bool";
			break;
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
		auto type = Call(node->element);
		if (type != L"")
		{
			result = type + L"*";
		}
	}

	void Visit(WfSharedPointerType* node)override
	{
		auto type = Call(node->element);
		if (type != L"")
		{
			result = L"Ptr<" + type + L">";
		}
	}

	void Visit(WfNullableType* node)override
	{
		auto type = Call(node->element);
		if (type != L"")
		{
			result = L"Nullable<" + type + L">";
		}
	}

	void Visit(WfEnumerableType* node)override
	{
		auto type = Call(node->element);
		if (type != L"")
		{
			result = L"collections::LazyList<" + type + L">";
		}
	}

	void Visit(WfMapType* node)override
	{
		auto key = node->key ? Call(node->key) : L"";
		auto value = Call(node->value);
		if (value != L"" && (!node->key || key != L""))
		{
			if (node->writability == WfMapWritability::Readonly)
			{
				result = L"const ";
			}

			if (key == L"")
			{
				result += L"collections::List<" + value + L">&";
			}
			else
			{
				result += L"collections::Dictionary<" + key + L", " + value + L">&";
			}
		}
	}

	void Visit(WfFunctionType* node)override
	{
		auto resultType = Call(node->result);
		if (resultType == L"")
		{
			return;
		}

		List<WString> argumentTypes;
		FOREACH(Ptr<WfType>, type, node->arguments)
		{
			auto argumentType = Call(type);
			if (argumentType == L"")
			{
				return;
			}
			argumentTypes.Add(argumentType);
		}

		result = L"Func<" + resultType + L"(";
		FOREACH_INDEXER(WString, type, index, argumentTypes)
		{
			if (index > 0)result += L", ";
			result += type;
		}
		result += L")>";
	}

	void Visit(WfChildType* node)override
	{
		VisitReferenceType(node);
	}
};

WString GetCppTypeNameFromWorkflowType(Ptr<CodegenConfig> config, const WString& workflowType)
{
	if (!config->workflowTable)
	{
		config->workflowTable = WfLoadTable();
	}
	if (!config->workflowManager)
	{
		config->workflowManager = new WfLexicalScopeManager(config->workflowTable);
		const wchar_t* moduleCode = LR"workflow(
module TypeConversion;

using presentation::controls::Gui*;
using presentation::elements::Gui*Element;
using presentation::compositions::Gui*Composition;
using presentation::compositions::Gui*;
using presentation::templates::Gui*;
using system::*;
using system::reflection::*;
using presentation::*;
using presentation::Gui*;
using presentation::controls::*;
using presentation::controls::list::*;
using presentation::controls::tree::*;
using presentation::elements::*;
using presentation::elements::Gui*;
using presentation::elements::text*;
using presentation::compositions::*;
using presentation::templates::*;

)workflow";
		auto module = config->workflowManager->AddModule(moduleCode);
		if (!module)
		{
			PrintErrorMessage(L"Internal error: wrong module code.");
		}
		config->workflowManager->Rebuild(true);
	}

	auto type = WfParseType(workflowType, config->workflowTable);
	if (!type)
	{
		PrintErrorMessage(L"Invalid workflow type: \"" + workflowType + L"\".");
		return L"vint";
	}

	auto module = config->workflowManager->modules[0];
	auto scope = config->workflowManager->moduleScopes[module.Obj()];
	analyzer::ValidateTypeStructure(config->workflowManager.Obj(), type, true);
	if (config->workflowManager->errors.Count() > 0)
	{
		PrintErrorMessage(L"Invalid workflow type: \"" + workflowType + L"\".");
		FOREACH(Ptr<ParsingError>, error, config->workflowManager->errors)
		{
			PrintErrorMessage(error->errorMessage);
		}
		config->workflowManager->errors.Clear();
		return L"vint";
	}

	auto cppType = GetCppTypeNameFromWorkflowTypeVisitor::Call(type.Obj());
	return cppType == L"" ? L"vint" : cppType;
}

/***********************************************************************
SearchAllFields
***********************************************************************/

class SearchAllFieldsVisitor : public Object, public GuiValueRepr::IVisitor
{
protected:
	Ptr<GuiInstanceEnvironment>						env;
	Dictionary<WString, GuiConstructorRepr*>&		fields;
	IGuiInstanceLoader::TypeInfo					typeInfo;

public:
	SearchAllFieldsVisitor(Ptr<GuiInstanceEnvironment> _env, Dictionary<WString, GuiConstructorRepr*>& _fields)
		:env(_env)
		, fields(_fields)
	{
	}

	void Visit(GuiTextRepr* repr)
	{
	}

	void Visit(GuiAttSetterRepr* repr)
	{
		FOREACH(Ptr<GuiAttSetterRepr::SetterValue>, setterValue, repr->setters.Values())
		{
			FOREACH(Ptr<GuiValueRepr>, value, setterValue->values)
			{
				value->Accept(this);
			}
		}
	}

	void Visit(GuiConstructorRepr* repr)
	{
		if (repr->instanceName && !fields.Keys().Contains(repr->instanceName.Value()))
		{
			auto loadingSource = FindInstanceLoadingSource(env, repr);
			fields.Add(repr->instanceName.Value(), repr);
		}
		Visit((GuiAttSetterRepr*)repr);
	}
};

void SearchAllFields(Ptr<GuiInstanceEnvironment> env, Ptr<GuiInstanceContext> context, Dictionary<WString, GuiConstructorRepr*>& fields)
{
	SearchAllFieldsVisitor visitor(env, fields);
	context->instance->Accept(&visitor);
}

/***********************************************************************
SearchAllSchemas
***********************************************************************/

void SearchAllSchemas(const Regex& regexClassName, Ptr<GuiResourceFolder> folder, Dictionary<WString, Ptr<InstanceSchema>>& typeSchemas, List<WString>& typeSchemaOrder)
{
	FOREACH(Ptr<GuiResourceItem>, item, folder->GetItems())
	{
		auto schema = item->GetContent().Cast<GuiInstanceSchema>();
		if (!schema) continue;

		FOREACH(Ptr<GuiInstanceTypeSchema>, typeSchema, schema->schemas)
		{
			if (typeSchemas.Keys().Contains(typeSchema->typeName)) continue;
			auto match = regexClassName.MatchHead(typeSchema->typeName);
			if (!match)
			{
				PrintErrorMessage(L"Skip code generation for \"" + typeSchema->typeName + L"\" because this type name is illegal.");
				continue;
			}

			auto instance = MakePtr<InstanceSchema>();
			if (match->Groups().Contains(L"namespace"))
			{
				CopyFrom(
					instance->namespaces,
					From(match->Groups()[L"namespace"])
						.Select([](const RegexString& str)->WString
						{
							return str.Value();
						})
					);
			}
			instance->typeName = match->Groups()[L"type"][0].Value();
			instance->schema = typeSchema;
			typeSchemaOrder.Add(typeSchema->typeName);
			typeSchemas.Add(typeSchema->typeName, instance);
		}
	}
	FOREACH(Ptr<GuiResourceFolder>, subFolder, folder->GetFolders())
	{
		SearchAllSchemas(regexClassName, subFolder, typeSchemas, typeSchemaOrder);
	}
}

/***********************************************************************
SearchAllInstances
***********************************************************************/

void SearchAllInstances(const Regex& regexClassName, Ptr<GuiResourcePathResolver> resolver, Ptr<GuiResourceFolder> folder, Dictionary<WString, Ptr<Instance>>& instances)
{
	FOREACH(Ptr<GuiResourceItem>, item, folder->GetItems())
	{
		auto context = item->GetContent().Cast<GuiInstanceContext>();
		if (!context) continue;
		if (!context->className) continue;
		if (instances.Keys().Contains(context->className.Value())) continue;
		auto match = regexClassName.MatchHead(context->className.Value());
		if (!match)
		{
			PrintErrorMessage(L"Skip code generation for \"" + context->className.Value() + L"\" because this type name is illegal.");
			continue;
		}

		Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(context, resolver);
		auto loadingSource = FindInstanceLoadingSource(env, context->instance.Obj());
		if (!loadingSource.loader) continue;
		auto typeDescriptor = GetGlobalTypeManager()->GetTypeDescriptor(loadingSource.typeName);
		if (!typeDescriptor) continue;

		Ptr<Instance> instance = new Instance;
		instance->context = context;
		instance->baseType = typeDescriptor;
		if (match->Groups().Contains(L"namespace"))
		{
			CopyFrom(
				instance->namespaces,
				From(match->Groups()[L"namespace"])
					.Select([](const RegexString& str)->WString
					{
						return str.Value();
					})
				);
		}
		instance->typeName = match->Groups()[L"type"][0].Value();
		SearchAllFields(env, context, instance->fields);

		instances.Add(context->className.Value(), instance);
	}
	FOREACH(Ptr<GuiResourceFolder>, subFolder, folder->GetFolders())
	{
		SearchAllInstances(regexClassName, resolver, subFolder, instances);
	}
}

/***********************************************************************
SearchAllEventHandlers
***********************************************************************/

class SearchAllEventHandlersVisitor : public Object, public GuiValueRepr::IVisitor
{
protected:
	Ptr<CodegenConfig>								config;
	Dictionary<WString, Ptr<Instance>>&				instances;
	Ptr<Instance>									instance;
	Ptr<GuiInstanceEnvironment>						env;
	Dictionary<WString, ITypeDescriptor*>&			eventHandlers;
	IGuiInstanceLoader::TypeInfo					typeInfo;

public:
	SearchAllEventHandlersVisitor(Ptr<CodegenConfig> _config, Dictionary<WString, Ptr<Instance>>& _instances, Ptr<Instance> _instance, Ptr<GuiInstanceEnvironment> _env, Dictionary<WString, ITypeDescriptor*>& _eventHandlers)
		:config(_config)
		, instances(_instances)
		, instance(_instance)
		, env(_env)
		, eventHandlers(_eventHandlers)
	{
	}

	static Ptr<GuiInstancePropertyInfo> GetPropertyInfo(const IGuiInstanceLoader::TypeInfo& typeInfo, const WString& name, IGuiInstanceLoader*& loader)
	{
		loader = GetInstanceLoaderManager()->GetLoader(typeInfo.typeName);
		auto propertyInfo = IGuiInstanceLoader::PropertyInfo(typeInfo, name);
		while (loader)
		{
			if (auto info = loader->GetPropertyType(propertyInfo))
			{
				if (info->support == GuiInstancePropertyInfo::NotSupport)
				{
					return 0;
				}
				else if (info->acceptableTypes.Count() > 0)
				{
					return info;
				}
			}
			loader = GetInstanceLoaderManager()->GetParentLoader(loader);
		}
		return 0;
	}

	static Ptr<GuiInstanceEventInfo> GetEventInfo(const IGuiInstanceLoader::TypeInfo& typeInfo, const WString& name, IGuiInstanceLoader*& loader)
	{
		loader = GetInstanceLoaderManager()->GetLoader(typeInfo.typeName);
		auto propertyInfo = IGuiInstanceLoader::PropertyInfo(typeInfo, name);
		while (loader)
		{
			if (auto info = loader->GetEventType(propertyInfo))
			{
				if (info->support == GuiInstanceEventInfo::NotSupport)
				{
					return 0;
				}
				else
				{
					return info;
				}
			}
			loader = GetInstanceLoaderManager()->GetParentLoader(loader);
		}
		return 0;
	}

	void Visit(GuiTextRepr* repr)
	{
	}

	void Visit(GuiAttSetterRepr* repr)
	{
		FOREACH_INDEXER(WString, eventName, index, repr->eventHandlers.Keys())
		{
			auto handler = repr->eventHandlers.Values()[index];
			if (handler->binding == L"")
			{
				if (!eventHandlers.Keys().Contains(eventName))
				{
					IGuiInstanceLoader* loader = 0;
					if (auto info = GetEventInfo(typeInfo, eventName, loader))
					{
						eventHandlers.Add(handler->value, info->argumentType);
					}
				}
			}
		}

		auto oldTypeInfo = typeInfo;
		FOREACH(WString, propertyName, repr->setters.Keys())
		{
			auto setterValue = repr->setters[propertyName];
			if (setterValue->binding == L"set")
			{
				int a = 0;
			}
			IGuiInstanceLoader* loader = 0;
			if (auto info = GetPropertyInfo(typeInfo, propertyName, loader))
			{
				typeInfo = IGuiInstanceLoader::TypeInfo(info->acceptableTypes[0]->GetTypeName(), info->acceptableTypes[0]);
				FOREACH(Ptr<GuiValueRepr>, value, setterValue->values)
				{
					value->Accept(this);
				}
				typeInfo = oldTypeInfo;
			}
		}
		typeInfo = oldTypeInfo;
	}

	void Visit(GuiConstructorRepr* repr)
	{
		auto oldTypeInfo = typeInfo;
		typeInfo = GetCppTypeInfo(config, instances, instance, repr);
		Visit((GuiAttSetterRepr*)repr);
		typeInfo = oldTypeInfo;
	}
};

void SearchAllEventHandlers(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances, Ptr<Instance> instance, Ptr<GuiInstanceEnvironment> env, Dictionary<WString, ITypeDescriptor*>& eventHandlers)
{
	SearchAllEventHandlersVisitor visitor(config, instances, instance, env, eventHandlers);
	instance->context->instance->Accept(&visitor);
}

/***********************************************************************
Codegen::Utility
***********************************************************************/

void WriteFileComment(const WString& name, bool doNotModify, StreamWriter& writer)
{
	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Vczh Library++ 3.0");
	writer.WriteLine(L"Developer: 陈梓瀚(vczh)");
	writer.WriteLine(L"GacUI::" + name);
	writer.WriteLine(L"");
	writer.WriteLine(L"本文件使用Vczh GacUI Resource Code Generator工具自动生成");
	if (doNotModify)
	{
		writer.WriteLine(L"************************************************************************");
		writer.WriteLine(L"DO NOT MODIFY");
	}
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
}

void FillReflectionNamespaces(List<WString>& namespaces)
{
	namespaces.Add(L"vl");
	namespaces.Add(L"reflection");
	namespaces.Add(L"description");
}

WString WriteNamespaceBegin(List<WString>& namespaces, StreamWriter& writer)
{
	WString prefix;
	FOREACH(WString, ns, namespaces)
	{
		writer.WriteLine(prefix + L"namespace " + ns);
		writer.WriteLine(prefix + L"{");
		prefix += L"\t";
	}
	return prefix;
}

void WriteNamespaceEnd(List<WString>& namespaces, StreamWriter& writer)
{
	FOREACH_INDEXER(WString, ns, index, namespaces)
	{
		WString prefix;
		for (vint i = 0; i < namespaces.Count() - index - 1; i++)
		{
			prefix += L"\t";
		}
		writer.WriteLine(prefix + L"}");
	}
}

/***********************************************************************
Codegen::EventHandlers
***********************************************************************/

WString GetEventHandlerCommentBegin(const WString& prefix)
{
	return prefix + L"// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)";
}

WString GetEventHandlerCommentEnd(const WString& prefix)
{
	return prefix + L"// #endregion CLASS_MEMBER_GUIEVENT_HANDLER";
}

WString GetEventHandlerHeader(const WString& prefix, Ptr<Instance> instance, const WString& name, bool addClassName)
{
	return prefix + L"void " + (addClassName ? instance->typeName + L"::" : L"") + name + L"(GuiGraphicsComposition* sender, " + GetCppTypeName(instance->eventHandlers[name]) + L"& arguments)" + (addClassName ? L"" : L";");
}

void WriteControlClassHeaderFileEventHandlers(Ptr<CodegenConfig> config, Ptr<Instance> instance, const WString& prefix, StreamWriter& writer)
{
	writer.WriteLine(GetEventHandlerCommentBegin(prefix + L"\t"));
	FOREACH(WString, name, instance->eventHandlers.Keys())
	{
		writer.WriteLine(GetEventHandlerHeader(prefix + L"\t", instance, name, false));
	}
	writer.WriteLine(GetEventHandlerCommentEnd(prefix + L"\t"));
}

void WriteControlClassCppFileEventHandlers(Ptr<CodegenConfig> config, Ptr<Instance> instance, const WString& prefix, Group<WString, WString>& existingEventHandlers, List<WString>& additionalLines, StreamWriter& writer)
{
	writer.WriteLine(GetEventHandlerCommentBegin(prefix));
	writer.WriteLine(L"");
	FOREACH(WString, name, instance->eventHandlers.Keys())
	{
		writer.WriteLine(GetEventHandlerHeader(prefix, instance, name, true));
		vint index = existingEventHandlers.Keys().IndexOf(name);
		if (index == -1)
		{
			writer.WriteLine(prefix + L"{");
			writer.WriteLine(prefix + L"}");
		}
		else
		{
			FOREACH(WString, line, existingEventHandlers.GetByIndex(index))
			{
				writer.WriteLine(line);
			}
		}
		writer.WriteLine(L"");
	}
	FOREACH(WString, line, additionalLines)
	{
		writer.WriteLine(line);
	}
	writer.WriteLine(GetEventHandlerCommentEnd(prefix));
}

/***********************************************************************
Codegen::ControlClassContent
***********************************************************************/

void WriteControlClassHeaderFileContent(Ptr<CodegenConfig> config, Ptr<Instance> instance, StreamWriter& writer)
{
	WString prefix = WriteNamespaceBegin(instance->namespaces, writer);
	writer.WriteLine(prefix + L"class " + instance->typeName + L" : public " + instance->typeName + L"_<" + instance->typeName + L">");
	writer.WriteLine(prefix + L"{");
	writer.WriteLine(prefix + L"\tfriend class " + instance->typeName + L"_<" + instance->typeName + L">;");
	writer.WriteLine(prefix + L"\tfriend struct vl::reflection::description::CustomTypeDescriptorSelector<" + instance->typeName + L">;");
	writer.WriteLine(prefix + L"protected:");
	writer.WriteLine(L"");
	WriteControlClassHeaderFileEventHandlers(config, instance, prefix, writer);
	writer.WriteLine(prefix + L"public:");
	writer.WriteString(prefix + L"\t" + instance->typeName + L"(");
	FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
	{
		if (index > 0)
		{
			writer.WriteString(L", ");
		}
		writer.WriteString(L"Ptr<");
		writer.WriteString(parameter->className);
		writer.WriteString(L"> ");
		writer.WriteString(parameter->name);
	}
	writer.WriteLine(L");");
	writer.WriteLine(prefix + L"};");
	WriteNamespaceEnd(instance->namespaces, writer);
	writer.WriteLine(L"");
}

void WriteControlClassCppFileContent(Ptr<CodegenConfig> config, Ptr<Instance> instance, StreamWriter& writer)
{
	Group<WString, WString> existingEventHandlers;
	List<WString> additionalLines;
	WString prefix = WriteNamespaceBegin(instance->namespaces, writer);
	WriteControlClassCppFileEventHandlers(config, instance, prefix, existingEventHandlers, additionalLines, writer);
	writer.WriteLine(L"");
	writer.WriteString(prefix + instance->typeName + L"::" + instance->typeName + L"(");
	FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
	{
		if (index > 0)
		{
			writer.WriteString(L", ");
		}
		writer.WriteString(L"Ptr<");
		writer.WriteString(parameter->className);
		writer.WriteString(L"> ");
		writer.WriteString(parameter->name);
	}
	writer.WriteLine(L")");
	writer.WriteLine(prefix + L"{");
	writer.WriteString(prefix + L"\tInitializeComponents(");
	FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
	{
		if (index > 0)
		{
			writer.WriteString(L", ");
		}
		writer.WriteString(parameter->name);
	}
	writer.WriteLine(L");");
	writer.WriteLine(prefix + L"}");
	WriteNamespaceEnd(instance->namespaces, writer);
	writer.WriteLine(L"");
}

/***********************************************************************
Codegen::ControlClass
***********************************************************************/

#define OPEN_FILE(NAME)\
	FileStream fileStream(config->resource->GetWorkingDirectory() + fileName, FileStream::WriteOnly); \
	if (!fileStream.IsAvailable()) \
	{ \
		PrintErrorMessage(L"error> Failed to generate " + fileName); \
		return; \
	} \
	BomEncoder encoder(BomEncoder::Utf8); \
	EncoderStream encoderStream(fileStream, encoder); \
	StreamWriter writer(encoderStream); \
	PrintSuccessMessage(L"gacgen> Generating " + fileName);

#define OPEN_FILE_WITH_COMMENT(NAME, DONOTMODIFY)\
	OPEN_FILE(NAME)\
	WriteFileComment(NAME, DONOTMODIFY, writer);

#define CANNOT_MERGE_CONTENT\
	PrintErrorMessage(L"error> Don't know how to override " + fileName + L". Please open " + config->GetPartialClassHeaderFileName() + L" to get the latest content in the comment and modify the file by yourself.")

bool TryReadFile(Ptr<CodegenConfig> config, const WString& fileName, List<WString>& lines)
{
	FileStream fileStream(config->resource->GetWorkingDirectory() + fileName, FileStream::ReadOnly);
	if (!fileStream.IsAvailable())
	{
		return false;
	}

	BomDecoder decoder;
	DecoderStream decoderStream(fileStream, decoder);
	StreamReader reader(decoderStream);

	while (!reader.IsEnd())
	{
		lines.Add(reader.ReadLine());
	}

	for (vint i = lines.Count() - 1; i >= 0; i--)
	{
		if (lines[i] != L"") break;
		lines.RemoveAt(i);
	}
	return true;
}

void WriteControlClassHeaderFile(Ptr<CodegenConfig> config, Ptr<Instance> instance)
{
	WString fileName = config->GetControlClassHeaderFileName(instance);
	List<WString> lines;
	if (TryReadFile(config, fileName, lines))
	{
		WString prefix;
		for (vint i = 0; i < instance->namespaces.Count(); i++)
		{
			prefix += L"\t";
		}
		vint begin = lines.IndexOf(GetEventHandlerCommentBegin(prefix + L"\t"));
		vint end = lines.IndexOf(GetEventHandlerCommentEnd(prefix + L"\t"));
		if (begin == -1 || end == -1)
		{
			CANNOT_MERGE_CONTENT;
			return;
		}
		
		OPEN_FILE(instance->typeName);
		PrintInformationMessage(L"gacgen> Merging content into " + fileName);

		for (vint i = 0; i < begin; i++)
		{
			writer.WriteLine(lines[i]);
		}
		WriteControlClassHeaderFileEventHandlers(config, instance, prefix, writer);
		for (vint i = end + 1; i < lines.Count(); i++)
		{
			writer.WriteLine(lines[i]);
		}
	}
	else
	{
		OPEN_FILE_WITH_COMMENT(instance->typeName, false);

		writer.WriteLine(L"#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_" + instance->typeName);
		writer.WriteLine(L"#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_" + instance->typeName);
		writer.WriteLine(L"");
		writer.WriteLine(L"#include \"" + config->GetPartialClassHeaderFileName() + L"\"");
		writer.WriteLine(L"");
		WriteControlClassHeaderFileContent(config, instance, writer);
		writer.WriteLine(L"#endif");
	}
}

void WriteControlClassCppFile(Ptr<CodegenConfig> config, Ptr<Instance> instance)
{
	WString fileName = config->GetControlClassCppFileName(instance);
	List<WString> lines;
	if (TryReadFile(config, fileName, lines))
	{
		WString prefix;
		for (vint i = 0; i < instance->namespaces.Count(); i++)
		{
			prefix += L"\t";
		}
		vint begin = lines.IndexOf(GetEventHandlerCommentBegin(prefix));
		vint end = lines.IndexOf(GetEventHandlerCommentEnd(prefix));
		if (begin == -1 || end == -1)
		{
			CANNOT_MERGE_CONTENT;
			return;
		}

		Group<WString, WString> existingEventHandlers;
		List<WString> additionalLines;

		bool inCode = false;
		WString eventHandlerName;
		for (vint i = begin + 1; i < end; i++)
		{
			WString line = lines[i];
			if (!inCode)
			{
				if (eventHandlerName != L"")
				{
					inCode = true;
				}
				else
				{
					bool allSpaces = true;
					for (vint j = 0; j < line.Length(); j++)
					{
						wchar_t c = line[j];
						if (c != L' '&&c != L'\t')
						{
							allSpaces = false;
							break;
						}
					}

					if (!allSpaces)
					{
						FOREACH(WString, name, instance->eventHandlers.Keys())
						{
							if (line == GetEventHandlerHeader(prefix, instance, name, true))
							{
								eventHandlerName = name;
								break;
							}
						}

						if (eventHandlerName == L"")
						{
							inCode = true;
						}
					}
				}
			}

			if (inCode)
			{
				if (eventHandlerName == L"")
				{
					additionalLines.Add(line);
				}
				else
				{
					existingEventHandlers.Add(eventHandlerName, line);
				}

				if (line == prefix + L"}")
				{
					inCode = false;
					if (eventHandlerName == L"")
					{
						additionalLines.Add(L"");
					}
					else
					{
						eventHandlerName = L"";
					}
				}
			}
		}
		
		OPEN_FILE(instance->typeName);
		PrintInformationMessage(L"gacgen> Merging content into " + fileName);

		for (vint i = 0; i < begin; i++)
		{
			writer.WriteLine(lines[i]);
		}
		WriteControlClassCppFileEventHandlers(config, instance, prefix, existingEventHandlers, additionalLines, writer);
		for (vint i = end + 1; i < lines.Count(); i++)
		{
			writer.WriteLine(lines[i]);
		}
	}
	else
	{
		OPEN_FILE_WITH_COMMENT(instance->typeName, false);
		writer.WriteLine(L"#include \"" + config->GetGlobalHeaderFileName() + L"\"");
		writer.WriteLine(L"");
		WriteControlClassCppFileContent(config, instance, writer);
	}
}

/***********************************************************************
Codegen::PartialClass
***********************************************************************/

void WritePartialClassHeaderFile(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<InstanceSchema>>& typeSchemas, List<WString>& typeSchemaOrder, Dictionary<WString, Ptr<Instance>>& instances)
{
	WString fileName = config->GetPartialClassHeaderFileName();
	OPEN_FILE_WITH_COMMENT(L"Partial Classes", true);

	writer.WriteLine(L"#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_PARTIAL_CLASSES");
	writer.WriteLine(L"#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_PARTIAL_CLASSES");
	writer.WriteLine(L"");
	writer.WriteLine(L"#include \"" + config->include + L"\"");
	writer.WriteLine(L"");

	FOREACH(WString, typeSchemaName, typeSchemaOrder)
	{
		auto instance = typeSchemas[typeSchemaName];
		WString prefix = WriteNamespaceBegin(instance->namespaces, writer);
		if (auto data = instance->schema.Cast<GuiInstanceDataSchema>())
		{
			if (data->referenceType)
			{
				WString parent = data->parentType == L"" ? L"vl::Object" : data->parentType;
				writer.WriteLine(prefix + L"class " + instance->typeName + L" : public " + parent + L", public vl::reflection::Description<" + instance->typeName + L">");
			}
			else
			{
				writer.WriteLine(prefix + L"struct " + instance->typeName);
			}
			writer.WriteLine(prefix + L"{");
			if (data->referenceType)
			{
				writer.WriteLine(prefix + L"public:");
			}
			FOREACH(Ptr<GuiInstancePropertySchame>, prop, data->properties)
			{
				writer.WriteLine(prefix + L"\t" + GetCppTypeNameFromWorkflowType(config, prop->typeName) + L" " + prop->name + L";");
			}
			writer.WriteLine(prefix + L"};");
		}
		else if (auto itf = instance->schema.Cast<GuiInstanceInterfaceSchema>())
		{
			WString parent = itf->parentType == L"" ? L"vl::reflection::IDescriptable" : itf->parentType;
			writer.WriteLine(prefix + L"class " + instance->typeName + L" : public virtual " + parent + L", public vl::reflection::Description<" + instance->typeName + L">");
			writer.WriteLine(prefix + L"{");
			writer.WriteLine(prefix + L"public:");
			FOREACH(Ptr<GuiInstancePropertySchame>, prop, itf->properties)
			{
				writer.WriteLine(L"");
				writer.WriteLine(prefix + L"\tvirtual " + GetCppTypeNameFromWorkflowType(config, prop->typeName) + L" Get" + prop->name + L"() = 0;");
				if (!prop->readonly)
				{
					writer.WriteLine(prefix + L"\tvirtual void Set" + prop->name + L"(" + GetCppTypeNameFromWorkflowType(config, prop->typeName) + L" value) = 0;");
				}
				if (prop->observable)
				{
					writer.WriteLine(prefix + L"\tvl::Event<void()> " + prop->name + L"Changed;");
				}
			}
			if (itf->methods.Count() > 0)
			{
				writer.WriteLine(L"");
				FOREACH(Ptr<GuiInstanceMethodSchema>, method, itf->methods)
				{
					writer.WriteString(prefix + L"\tvirtual " + GetCppTypeNameFromWorkflowType(config, method->returnType) + L" " + method->name + L"(");
					FOREACH_INDEXER(Ptr<GuiInstancePropertySchame>, argument, index, method->arguments)
					{
						if (index > 0)
						{
							writer.WriteString(L", ");
						}
						writer.WriteString(GetCppTypeNameFromWorkflowType(config, argument->typeName) + L" " + argument->name);
					}
					writer.WriteLine(L") = 0;");
				}
			}
			writer.WriteLine(prefix + L"};");
		}
		WriteNamespaceEnd(instance->namespaces, writer);
		writer.WriteLine(L"");
	}

	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		WString prefix = WriteNamespaceBegin(instance->namespaces, writer);
		writer.WriteLine(prefix + L"template<typename TImpl>");
		writer.WriteLine(prefix + L"class " + instance->typeName + L"_ : public " + GetCppTypeName(instance->baseType) + L", public vl::presentation::GuiInstancePartialClass<vl::" + instance->baseType->GetTypeName() + L">, public vl::reflection::Description<TImpl>");
		writer.WriteLine(prefix + L"{");
		writer.WriteLine(prefix + L"private:");
		FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
		{
			writer.WriteString(prefix + L"\t");
			writer.WriteString(L"Ptr<");
			writer.WriteString(parameter->className);
			writer.WriteString(L"> ");
			writer.WriteString(parameter->name);
			writer.WriteLine(L"_;");
		}
		writer.WriteLine(prefix + L"protected:");
		FOREACH(WString, field, instance->fields.Keys())
		{
			writer.WriteLine(prefix + L"\t" + GetCppTypeName(config, instances, instance, instance->fields[field]) + L"* " + field + L";");
		}
		writer.WriteLine(L"");
		writer.WriteString(L"\t\tvoid InitializeComponents(");
		FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
		{
			if (index > 0)
			{
				writer.WriteString(L", ");
			}
			writer.WriteString(L"Ptr<");
			writer.WriteString(parameter->className);
			writer.WriteString(L"> ");
			writer.WriteString(parameter->name);
		}
		writer.WriteLine(L")");
		writer.WriteLine(prefix + L"\t{");
		FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
		{
			writer.WriteString(prefix + L"\t\t");
			writer.WriteString(parameter->name);
			writer.WriteString(L"_ = ");
			writer.WriteString(parameter->name);
			writer.WriteLine(L";");
		}
		writer.WriteLine(prefix + L"\t\tif (InitializeFromResource())");
		writer.WriteLine(prefix + L"\t\t{");
		FOREACH(WString, field, instance->fields.Keys())
		{
			writer.WriteLine(prefix + L"\t\t\tGUI_INSTANCE_REFERENCE(" + field + L");");
		}
		writer.WriteLine(prefix + L"\t\t}");
		writer.WriteLine(prefix + L"\t\telse");
		writer.WriteLine(prefix + L"\t\t{");
		FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
		{
			writer.WriteString(prefix + L"\t\t\t");
			writer.WriteString(parameter->name);
			writer.WriteLine(L"_ = 0;");
		}
		writer.WriteLine(prefix + L"\t\t}");
		writer.WriteLine(prefix + L"\t}");
		writer.WriteLine(prefix + L"public:");
		writer.WriteLine(prefix + L"\t" + instance->typeName + L"_()");
		writer.WriteLine(prefix + L"\t\t:vl::presentation::GuiInstancePartialClass<" + GetCppTypeName(instance->baseType) + L">(L\"" + instance->GetFullName() + L"\")");
		if (instance->baseType == GetTypeDescriptor<GuiWindow>())
		{
			writer.WriteLine(prefix + L"\t\t," + GetCppTypeName(instance->baseType) + L"(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())");
		}
		else if (instance->baseType == GetTypeDescriptor<GuiCustomControl>())
		{
			writer.WriteLine(prefix + L"\t\t," + GetCppTypeName(instance->baseType) + L"(vl::presentation::theme::GetCurrentTheme()->CreateCustomControlStyle())");
		}
		FOREACH(WString, field, instance->fields.Keys())
		{
			writer.WriteLine(prefix + L"\t\t," + field + L"(0)");
		}
		writer.WriteLine(prefix + L"\t{");
		writer.WriteLine(prefix + L"\t}");
		FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
		{
			writer.WriteLine(L"");
			writer.WriteString(prefix + L"\t");
			writer.WriteString(L"Ptr<");
			writer.WriteString(parameter->className);
			writer.WriteString(L"> Get");
			writer.WriteString(parameter->name);
			writer.WriteLine(L"()");
			writer.WriteLine(prefix + L"\t{");
			writer.WriteLine(prefix + L"\t\treturn " + parameter->name + L"_;");
			writer.WriteLine(prefix + L"\t}");
		}
		writer.WriteLine(prefix + L"};");
		writer.WriteLine(L"");
		writer.WriteLine(prefix + L"class " + instance->typeName + L";");
		WriteNamespaceEnd(instance->namespaces, writer);
		writer.WriteLine(L"");
	}

	{
		List<WString> ns;
		FillReflectionNamespaces(ns);
		WString prefix = WriteNamespaceBegin(ns, writer);

		FOREACH(Ptr<InstanceSchema>, instance, typeSchemas.Values())
		{
			writer.WriteLine(prefix + L"DECL_TYPE_INFO(" + instance->GetFullName() + L")");
		}
		FOREACH(Ptr<Instance>, instance, instances.Values())
		{
			writer.WriteLine(prefix + L"DECL_TYPE_INFO(" + instance->GetFullName() + L")");
		}
		WriteNamespaceEnd(ns, writer);
		writer.WriteLine(L"");
	}

	writer.WriteLine(L"/*");
	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(config->GetControlClassHeaderFileName(instance) + L" :");
		WriteControlClassHeaderFileContent(config, instance, writer);
		writer.WriteLine(L"");
		writer.WriteLine(config->GetControlClassCppFileName(instance) + L" :");
		WriteControlClassCppFileContent(config, instance, writer);
		writer.WriteLine(L"");
	}
	writer.WriteLine(L"*/");
	writer.WriteLine(L"");

	writer.WriteLine(L"#endif");
}

void WritePartialClassCppFile(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<InstanceSchema>>& typeSchemas, List<WString>& typeSchemaOrder, Dictionary<WString, Ptr<Instance>>& instances)
{
	WString fileName = config->GetPartialClassCppFileName();
	OPEN_FILE_WITH_COMMENT(L"Partial Classes", true);

	writer.WriteLine(L"#include \"" + config->GetGlobalHeaderFileName() + L"\"");
	writer.WriteLine(L"");

	List<WString> ns;
	FillReflectionNamespaces(ns);
	WString prefix = WriteNamespaceBegin(ns, writer);
	writer.WriteLine(prefix + L"#define _ ,");
	
	FOREACH(Ptr<InstanceSchema>, instance, typeSchemas.Values())
	{
		writer.WriteLine(prefix + L"IMPL_TYPE_INFO(" + instance->GetFullName() + L")");
	}
	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(prefix + L"IMPL_TYPE_INFO(" + instance->GetFullName() + L")");
	}
	writer.WriteLine(L"");

	FOREACH(Ptr<InstanceSchema>, instance, typeSchemas.Values())
	{
		if (auto data = instance->schema.Cast<GuiInstanceDataSchema>())
		{
			if (data->referenceType)
			{
				writer.WriteLine(prefix + L"BEGIN_CLASS_MEMBER(" + instance->GetFullName() + L")");
				if (data->parentType != L"")
				{
					writer.WriteLine(prefix + L"\tCLASS_MEMBER_BASE(" + data->parentType + L")");
				}
				writer.WriteLine(prefix + L"\tCLASS_MEMBER_CONSTRUCTOR(vl::Ptr<" + instance->GetFullName() + L">(), NO_PARAMETER)");
				FOREACH(Ptr<GuiInstancePropertySchame>, prop, data->properties)
				{
					writer.WriteLine(prefix + L"\tCLASS_MEMBER_FIELD(" + prop->name + L")");
				}
				writer.WriteLine(prefix + L"END_CLASS_MEMBER(" + instance->GetFullName() + L")");
			}
			else
			{
				writer.WriteLine(prefix + L"BEGIN_STRUCT_MEMBER(" + instance->GetFullName() + L")");
				FOREACH(Ptr<GuiInstancePropertySchame>, prop, data->properties)
				{
					writer.WriteLine(prefix + L"\tSTRUCT_MEMBER(" + prop->name + L")");
				}
				writer.WriteLine(prefix + L"END_STRUCT_MEMBER(" + instance->GetFullName() + L")");
			}
		}
		else if (auto itf = instance->schema.Cast<GuiInstanceInterfaceSchema>())
		{
			writer.WriteLine(prefix + L"BEGIN_CLASS_MEMBER(" + instance->GetFullName() + L")");
			WString parent = itf->parentType == L"" ? L"vl::reflection::IDescriptable" : itf->parentType;
			writer.WriteLine(prefix + L"\tCLASS_MEMBER_BASE(" + parent + L")");
			FOREACH(Ptr<GuiInstancePropertySchame>, prop, itf->properties)
			{
				if (prop->observable)
				{
					writer.WriteLine(prefix + L"\tCLASS_MEMBER_EVENT(" + prop->name + L"Changed)");
					if (prop->readonly)
					{
						writer.WriteLine(prefix + L"\tCLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(" + prop->name + L", " + prop->name + L"Changed)");
					}
					else
					{
						writer.WriteLine(prefix + L"\tCLASS_MEMBER_PROPERTY_EVENT_FAST(" + prop->name + L", " + prop->name + L"Changed)");
					}
				}
				else
				{
					if (prop->readonly)
					{
						writer.WriteLine(prefix + L"\tCLASS_MEMBER_PROPERTY_READONLY_FAST(" + prop->name + L")");
					}
					else
					{
						writer.WriteLine(prefix + L"\tCLASS_MEMBER_PROPERTY_FAST(" + prop->name + L")");
					}
				}
			}
			FOREACH(Ptr<GuiInstanceMethodSchema>, method, itf->methods)
			{
				writer.WriteString(prefix + L"\tCLASS_MEMBER_METHOD(" + method->name + L", ");
				if (method->arguments.Count() == 0)
				{
					writer.WriteString(L"NO_PARAMETER");
				}
				else
				{
					writer.WriteString(L"{ ");
					FOREACH_INDEXER(Ptr<GuiInstancePropertySchame>, argument, index, method->arguments)
					{
						if (index > 0)
						{
							writer.WriteString(L" _ ");
						}
						writer.WriteString(L"L\"" + argument->name + L"\"");
					}
					writer.WriteString(L" }");
				}
				writer.WriteLine(L");");
			}
			writer.WriteLine(prefix + L"END_CLASS_MEMBER(" + instance->GetFullName() + L")");
		}
		writer.WriteLine(L"");
	}

	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(prefix + L"BEGIN_CLASS_MEMBER(" + instance->GetFullName() + L")");
		writer.WriteLine(prefix + L"\tCLASS_MEMBER_BASE(" + GetCppTypeName(instance->baseType) + L")");
		writer.WriteString(prefix + L"\tCLASS_MEMBER_CONSTRUCTOR(" + instance->GetFullName() + L"*(");
		FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
		{
			if (index > 0)
			{
				writer.WriteString(L", ");
			}
			writer.WriteString(L"Ptr<");
			writer.WriteString(parameter->className);
			writer.WriteString(L">");
		}
		writer.WriteString(L"), ");
		if (instance->context->parameters.Count() == 0)
		{
			writer.WriteString(L"NO_PARAMETER");
		}
		else
		{
			writer.WriteString(L"{ ");
			FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
			{
				if (index > 0)
				{
					writer.WriteString(L" _ ");
				}
				writer.WriteString(L"L\"");
				writer.WriteString(parameter->name);
				writer.WriteString(L"\"");
			}
			writer.WriteString(L" }");
		}
		writer.WriteLine(L")");

		if (instance->eventHandlers.Count() > 0)
		{
			writer.WriteLine(L"");
			FOREACH(WString, name, instance->eventHandlers.Keys())
			{
				writer.WriteLine(prefix + L"\tCLASS_MEMBER_GUIEVENT_HANDLER(" + name + L", " + GetCppTypeName(instance->eventHandlers[name]) + L")");
			}
		}
		
		if (instance->context->parameters.Count() > 0)
		{
			writer.WriteLine(L"");
			FOREACH_INDEXER(Ptr<GuiInstanceParameter>, parameter, index, instance->context->parameters)
			{
				writer.WriteString(prefix + L"\tCLASS_MEMBER_PROPERTY_READONLY_FAST(");
				writer.WriteString(parameter->name);
				writer.WriteLine(L")");
			}
		}

		writer.WriteLine(prefix + L"END_CLASS_MEMBER(" + instance->GetFullName() + L")");
		writer.WriteLine(L"");
	}

	writer.WriteLine(prefix + L"#undef _");
	writer.WriteLine(L"");

	writer.WriteLine(prefix + L"class " + config->name + L"ResourceLoader : public Object, public ITypeLoader");
	writer.WriteLine(prefix + L"{");
	writer.WriteLine(prefix + L"public:");
	writer.WriteLine(prefix + L"\tvoid Load(ITypeManager* manager)");
	writer.WriteLine(prefix + L"\t{");
	FOREACH(Ptr<InstanceSchema>, instance, typeSchemas.Values())
	{
		writer.WriteLine(prefix + L"\t\tADD_TYPE_INFO(" + instance->GetFullName() + L")");
	}
	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(prefix + L"\t\tADD_TYPE_INFO(" + instance->GetFullName() + L")");
	}
	writer.WriteLine(prefix + L"\t}");
	writer.WriteLine(L"");
	writer.WriteLine(prefix + L"\tvoid Unload(ITypeManager* manager)");
	writer.WriteLine(prefix + L"\t{");
	writer.WriteLine(prefix + L"\t}");
	writer.WriteLine(prefix + L"};");
	writer.WriteLine(L"");
	writer.WriteLine(prefix + L"class " + config->name + L"ResourcePlugin : public Object, public vl::presentation::controls::IGuiPlugin");
	writer.WriteLine(prefix + L"{");
	writer.WriteLine(prefix + L"public:");
	writer.WriteLine(prefix + L"\tvoid Load()override");
	writer.WriteLine(prefix + L"\t{");
	writer.WriteLine(prefix + L"\t\tGetGlobalTypeManager()->AddTypeLoader(new " + config->name + L"ResourceLoader);");
	writer.WriteLine(prefix + L"\t}");
	writer.WriteLine(L"");
	writer.WriteLine(prefix + L"\tvoid AfterLoad()override");
	writer.WriteLine(prefix + L"\t{");
	writer.WriteLine(prefix + L"\t}");
	writer.WriteLine(L"");
	writer.WriteLine(prefix + L"\tvoid Unload()override");
	writer.WriteLine(prefix + L"\t{");
	writer.WriteLine(prefix + L"\t}");
	writer.WriteLine(prefix + L"};");
	writer.WriteLine(prefix + L"GUI_REGISTER_PLUGIN(" + config->name + L"ResourcePlugin)");

	WriteNamespaceEnd(ns, writer);
	writer.WriteLine(L"");
}

/***********************************************************************
Codegen::GlobalHeader
***********************************************************************/

void WriteGlobalHeaderFile(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances)
{
	WString fileName = config->GetGlobalHeaderFileName();
	OPEN_FILE_WITH_COMMENT(config->name, true);

	writer.WriteLine(L"#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name);
	writer.WriteLine(L"#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name);
	writer.WriteLine(L"");
	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(L"#include \"" + config->GetControlClassHeaderFileName(instance) + L"\"");
	}
	writer.WriteLine(L"");

	writer.WriteLine(L"#endif");
}

/***********************************************************************
Main
***********************************************************************/

void GuiMain()
{
	Console::WriteLine(L"Vczh GacUI Resource Code Generator for C++");
	if (arguments->Count() != 1)
	{
		PrintErrorMessage(L"GacGen.exe only accept 1 input file.");
		return;
	}

	WString inputPath = arguments->Get(0);
	PrintSuccessMessage(L"gacgen>Making : " + inputPath);
	List<WString> errors;
	auto resource = GuiResource::LoadFromXml(arguments->Get(0), errors);
	if (!resource)
	{
		PrintErrorMessage(L"error> Failed to load resource.");
		return;
	}
	FOREACH(WString, error, errors)
	{
		PrintInformationMessage(error);
	}
	GetInstanceLoaderManager()->SetResource(L"GACGEN", resource);

	auto config = CodegenConfig::LoadConfig(resource);
	if (!config)
	{
		PrintErrorMessage(L"error> Failed to load config.");
		return;
	}

	Regex regexClassName(L"((<namespace>[^:]+)::)*(<type>[^:]+)");
	Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(resource, resource->GetWorkingDirectory());
	Dictionary<WString, Ptr<InstanceSchema>> typeSchemas;
	List<WString> typeSchemaOrder;
	Dictionary<WString, Ptr<Instance>> instances;
	SearchAllSchemas(regexClassName, resource, typeSchemas, typeSchemaOrder);
	SearchAllInstances(regexClassName, resolver, resource, instances);
		
	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(instance->context, resolver);
		SearchAllEventHandlers(config, instances, instance, env, instance->eventHandlers);
	}

	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		WriteControlClassHeaderFile(config, instance);
		WriteControlClassCppFile(config, instance);
	}
	WritePartialClassHeaderFile(config, typeSchemas, typeSchemaOrder, instances);
	WritePartialClassCppFile(config, typeSchemas, typeSchemaOrder, instances);
	WriteGlobalHeaderFile(config, instances);
}