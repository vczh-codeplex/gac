#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIReflection.h"

using namespace vl::console;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::reflection::description;

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

class CodegenConfig
{
public:
	WString								folder;
	WString								include;
	WString								name;
	WString								prefix;

	static bool LoadConfigString(Ptr<GuiResource> resource, const WString& name, WString& value)
	{
		if (auto includeItem = resource->GetValueByPath(L"GacGenConfig/" + name).Cast<GuiTextData>())
		{
			value = includeItem->GetText();
			return true;
		}
		else
		{
			PrintErrorMessage(L"Cannot find configuration in resource \"GacGenConfig/" + name + L"\".");
			return false;
		}
	}

	static Ptr<CodegenConfig> LoadConfig(Ptr<GuiResource> resource)
	{
		Ptr<CodegenConfig> config = new CodegenConfig;
		if (!LoadConfigString(resource, L"Folder", config->folder)) return false;
		if (!LoadConfigString(resource, L"Include", config->include)) return false;
		if (!LoadConfigString(resource, L"Name", config->name)) return false;
		if (!LoadConfigString(resource, L"Prefix", config->prefix)) return false;
		return config;
	}
};

struct Instance
{
	Ptr<GuiInstanceContext>				context;
	ITypeDescriptor*					baseType;
	List<WString>						namespaces;
	WString								typeName;
	Dictionary<WString, WString>		fields;
};

class SearchAllFieldsVisitor : public Object, public GuiValueRepr::IVisitor
{
protected:
	Ptr<GuiInstanceEnvironment>			env;
	Dictionary<WString, WString>&		fields;

public:
	SearchAllFieldsVisitor(Ptr<GuiInstanceEnvironment> _env, Dictionary<WString, WString>& _fields)
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
			fields.Add(repr->instanceName.Value(), loadingSource.typeName);
		}
		Visit((GuiAttSetterRepr*)repr);
	}
};

void SearchAllFields(Ptr<GuiInstanceEnvironment> env, Ptr<GuiInstanceContext> context, Dictionary<WString, WString>& fields)
{
	SearchAllFieldsVisitor visitor(env, fields);
	context->instance->Accept(&visitor);
}

void SearchAllInstances(const Regex& regexClassName, Ptr<GuiResourcePathResolver> resolver, Ptr<GuiResourceFolder> folder, Dictionary<WString, Ptr<Instance>>& instances)
{
	FOREACH(Ptr<GuiResourceItem>, item, folder->GetItems())
	{
		auto context = item->GetContent().Cast<GuiInstanceContext>();
		if (!context) continue;
		if (!context->className) continue;
		if (instances.Keys().Contains(context->className.Value())) continue;
		auto match = regexClassName.MatchHead(context->className.Value());
		if (!match) continue;

		Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(context, resolver);
		auto loadingSource = FindInstanceLoadingSource(env, context->instance.Obj());
		if (!loadingSource.loader) continue;
		auto typeDescriptor = GetGlobalTypeManager()->GetTypeDescriptor(loadingSource.typeName);
		if (typeDescriptor != GetTypeDescriptor<GuiWindow>() && typeDescriptor != GetTypeDescriptor<GuiCustomControl>()) continue;

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

void GuiMain()
{
	Console::WriteLine(L"Vczh GacUI Resource Code Generator for C++");
	PrintSuccessMessage(L"gacgen>Files : " + itow(arguments->Count()));

	FOREACH(WString, inputPath, *arguments)
	{
		PrintSuccessMessage(L"gacgen>Making : " + inputPath);
		auto resource = GuiResource::LoadFromXml(arguments->Get(0));
		if (!resource)
		{
			PrintErrorMessage(L"Failed to load resource.");
			return;
		}

		auto config = CodegenConfig::LoadConfig(resource);
		if (!config)
		{
			PrintErrorMessage(L"Failed to load config.");
			return;
		}

		Regex regexClassName(L"((<namespace>[^:]+)::)*(<type>[^:]+)");
		Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(resource, resource->GetWorkingDirectory());
		Dictionary<WString, Ptr<Instance>> instances;
		SearchAllInstances(regexClassName, resolver, resource, instances);

		FOREACH(Ptr<Instance>, instance, instances.Values())
		{
			PrintSuccessMessage(L"gacgen>Generate " + instance->context->className.Value());
			Console::WriteLine(L"\t<Base Type> : " + instance->baseType->GetTypeName());
			FOREACH(WString, field, instance->fields.Keys())
			{
				Console::WriteLine(L"\t" + field + L" : " + instance->fields[field]);
			}
		}
	}
}