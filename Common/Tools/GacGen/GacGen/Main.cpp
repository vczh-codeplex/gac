#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIReflection.h"

using namespace vl::console;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::stream;
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

struct Instance
{
	Ptr<GuiInstanceContext>						context;
	ITypeDescriptor*							baseType;
	List<WString>								namespaces;
	WString										typeName;
	Dictionary<WString, GuiConstructorRepr*>	fields;

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

class SearchAllFieldsVisitor : public Object, public GuiValueRepr::IVisitor
{
protected:
	Ptr<GuiInstanceEnvironment>						env;
	Dictionary<WString, GuiConstructorRepr*>&		fields;

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

WString GetCppTypeName(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances, Ptr<Instance> instance, GuiConstructorRepr* ctor)
{
	Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(config->resource, config->resource->GetWorkingDirectory());

	Ptr<GuiInstanceContext> currentContext = instance->context;
	GuiConstructorRepr* currentCtor = ctor;
	WString typeName;

	while (currentContext)
	{
		Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(currentContext, resolver);
		auto loadingSource = FindInstanceLoadingSource(env, currentCtor);
		currentContext = loadingSource.context;
		currentCtor = currentContext ? currentContext->instance.Obj() : 0;
		typeName = loadingSource.typeName;

		if (instances.Keys().Contains(typeName))
		{
			return typeName;
		}
	}

	auto manager = GetInstanceLoaderManager();
	return L"vl::" + manager->GetTypeDescriptorForType(typeName)->GetTypeName();
}

void WriteFileComment(const WString& name, StreamWriter& writer)
{
	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Vczh Library++ 3.0");
	writer.WriteLine(L"Developer: 陈梓瀚(vczh)");
	writer.WriteLine(L"GacUI::" + name);
	writer.WriteLine(L"");
	writer.WriteLine(L"本文件使用Vczh GacUI Resource Code Generator工具自动生成");
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

void WriteControlClassHeaderFileContent(Ptr<CodegenConfig> config, Ptr<Instance> instance, const WString& prefix, StreamWriter& writer)
{
	writer.WriteLine(prefix + L"class " + instance->typeName + L" : public " + instance->typeName + L"_<" + instance->typeName + L">");
	writer.WriteLine(prefix + L"{");
	writer.WriteLine(prefix + L"\tfriend class " + instance->typeName + L"_<" + instance->typeName + L">;");
	writer.WriteLine(prefix + L"public:");
	writer.WriteLine(prefix + L"\t" + instance->typeName + L"();");
	writer.WriteLine(prefix + L"};");
}

void WriteControlClassCppFileContent(Ptr<CodegenConfig> config, Ptr<Instance> instance, const WString& prefix, StreamWriter& writer)
{
	writer.WriteLine(prefix + instance->typeName + L"::" + instance->typeName + L"()");
	writer.WriteLine(prefix + L"{");
	writer.WriteLine(prefix + L"\tInitializeComponents();");
	writer.WriteLine(prefix + L"}");
}

#define OPEN_FILE(FILENAME, NAME)\
	WString fileName = FILENAME; \
	FileStream fileStream(config->resource->GetWorkingDirectory() + fileName, FileStream::WriteOnly); \
	if (!fileStream.IsAvailable()) \
	{ \
		PrintErrorMessage(L"gacgen> Failed to generate " + fileName); \
		return; \
	} \
	BomEncoder encoder(BomEncoder::Utf8); \
	EncoderStream encoderStream(fileStream, encoder); \
	StreamWriter writer(encoderStream); \
	PrintSuccessMessage(L"gacgen> Generating " + fileName); \
	WriteFileComment(NAME, writer);

void WriteControlClassHeaderFile(Ptr<CodegenConfig> config, Ptr<Instance> instance)
{
	OPEN_FILE(config->GetControlClassHeaderFileName(instance), instance->typeName);

	writer.WriteLine(L"#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_" + instance->typeName);
	writer.WriteLine(L"#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_" + instance->typeName);
	writer.WriteLine(L"");
	writer.WriteLine(L"#include \"" + config->GetPartialClassHeaderFileName() + L"\"");
	writer.WriteLine(L"");

	WString prefix = WriteNamespaceBegin(instance->namespaces, writer);
	WriteControlClassHeaderFileContent(config, instance, prefix, writer);
	WriteNamespaceEnd(instance->namespaces, writer);

	writer.WriteLine(L"#endif");
}

void WriteControlClassCppFile(Ptr<CodegenConfig> config, Ptr<Instance> instance)
{
	OPEN_FILE(config->GetControlClassCppFileName(instance), instance->typeName);
	writer.WriteLine(L"#include \"" + config->GetGlobalHeaderFileName() + L"\"");
	writer.WriteLine(L"");

	WString prefix = WriteNamespaceBegin(instance->namespaces, writer);
	WriteControlClassCppFileContent(config, instance, prefix, writer);
	WriteNamespaceEnd(instance->namespaces, writer);
}

void WritePartialClassHeaderFile(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances)
{
	OPEN_FILE(config->GetPartialClassHeaderFileName(), L"Partial Classes");

	writer.WriteLine(L"#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_PARTIAL_CLASSES");
	writer.WriteLine(L"#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_PARTIAL_CLASSES");
	writer.WriteLine(L"");
	writer.WriteLine(L"#include \"" + config->include + L"\"");
	writer.WriteLine(L"");

	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		WString prefix = WriteNamespaceBegin(instance->namespaces, writer);
		writer.WriteLine(prefix + L"template<typename TImpl>");
		writer.WriteLine(prefix + L"class " + instance->typeName + L"_ : public vl::" + instance->baseType->GetTypeName() + L", public vl::presentation::GuiInstancePartialClass<vl::" + instance->baseType->GetTypeName() + L">, public vl::reflection::Description<TImpl>");
		writer.WriteLine(prefix + L"{");
		writer.WriteLine(prefix + L"protected:");
		FOREACH(WString, field, instance->fields.Keys())
		{
			writer.WriteLine(prefix + L"\t" + GetCppTypeName(config, instances, instance, instance->fields[field]) + L"* " + field + L";");
		}
		writer.WriteLine(L"");
		writer.WriteLine(L"\t\tvoid InitializeComponents()");
		writer.WriteLine(prefix + L"\t{");
		writer.WriteLine(prefix + L"\t\tif (InitializeFromResource())");
		writer.WriteLine(prefix + L"\t\t{");
		FOREACH(WString, field, instance->fields.Keys())
		{
			writer.WriteLine(prefix + L"\t\t\tGUI_INSTANCE_REFERENCE(" + field + L");");
		}
		writer.WriteLine(prefix + L"\t\t}");
		writer.WriteLine(prefix + L"\t}");
		writer.WriteLine(prefix + L"public:");
		writer.WriteLine(prefix + L"\t" + instance->typeName + L"_()");
		if (instance->baseType == GetTypeDescriptor<GuiWindow>())
		{
			writer.WriteLine(prefix + L"\t\t:vl::" + instance->baseType->GetTypeName() + L"(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())");
		}
		else if (instance->baseType == GetTypeDescriptor<GuiCustomControl>())
		{
			writer.WriteLine(prefix + L"\t\t:vl::" + instance->baseType->GetTypeName() + L"(vl::presentation::theme::GetCurrentTheme()->CreateCustomControlStyle())");
		}
		writer.WriteLine(prefix + L"\t\t,vl::presentation::GuiInstancePartialClass<vl::" + instance->baseType->GetTypeName() + L">(L\"" + instance->GetFullName() + L"\")");
		FOREACH(WString, field, instance->fields.Keys())
		{
			writer.WriteLine(prefix + L"\t\t," + field + L"(0)");
		}
		writer.WriteLine(prefix + L"\t{");
		writer.WriteLine(prefix + L"\t}");
		writer.WriteLine(prefix + L"};");
		writer.WriteLine(L"");
		writer.WriteLine(prefix + L"class " + instance->typeName + L";");
		WriteNamespaceEnd(instance->namespaces, writer);
		writer.WriteLine(L"");
	}

	writer.WriteLine(L"/*");
	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(config->GetControlClassHeaderFileName(instance) + L" :");
		WriteControlClassHeaderFileContent(config, instance, L"\t", writer);
		writer.WriteLine(L"");
		writer.WriteLine(config->GetControlClassCppFileName(instance) + L" :");
		WriteControlClassCppFileContent(config, instance, L"\t", writer);
		writer.WriteLine(L"");
	}
	writer.WriteLine(L"*/");
	writer.WriteLine(L"");

	writer.WriteLine(L"#endif");
}

void WritePartialClassCppFile(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances)
{
	OPEN_FILE(config->GetPartialClassCppFileName(), L"Partial Classes");

	writer.WriteLine(L"#include \"" + config->GetGlobalHeaderFileName() + L"\"");
	writer.WriteLine(L"");

	List<WString> ns;
	FillReflectionNamespaces(ns);
	WString prefix = WriteNamespaceBegin(ns, writer);

	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(prefix + L"IMPL_TYPE_INFO(" + instance->GetFullName() + L")");
	}
	writer.WriteLine(L"");

	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(prefix + L"BEGIN_CLASS_MEMBER(" + instance->GetFullName() + L")");
		writer.WriteLine(prefix + L"\tCLASS_MEMBER_BASE(vl::" + instance->baseType->GetTypeName() + L")");
		writer.WriteLine(prefix + L"\tCLASS_MEMBER_CONSTRUCTOR(" + instance->GetFullName() + L"*(), NO_PARAMETER)");
		writer.WriteLine(prefix + L"END_CLASS_MEMBER(" + instance->GetFullName() + L")");
		writer.WriteLine(L"");
	}

	writer.WriteLine(prefix + L"class " + config->name + L"ResourceLoader : public Object, public ITypeLoader");
	writer.WriteLine(prefix + L"{");
	writer.WriteLine(prefix + L"public:");
	writer.WriteLine(prefix + L"\tvoid Load(ITypeManager* manager)");
	writer.WriteLine(prefix + L"\t{");
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

void WriteGlobalHeaderFile(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances)
{
	OPEN_FILE(config->GetGlobalHeaderFileName(), config->name);

	writer.WriteLine(L"#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name);
	writer.WriteLine(L"#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name);
	writer.WriteLine(L"");
	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(L"#include \"" + config->GetControlClassHeaderFileName(instance) + L"\"");
	}
	writer.WriteLine(L"");

	List<WString> ns;
	FillReflectionNamespaces(ns);
	WString prefix = WriteNamespaceBegin(ns, writer);
	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		writer.WriteLine(prefix + L"DECL_TYPE_INFO(" + instance->GetFullName() + L")");
	}
	WriteNamespaceEnd(ns, writer);
	writer.WriteLine(L"");

	writer.WriteLine(L"#endif");
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
			WriteControlClassHeaderFile(config, instance);
			WriteControlClassCppFile(config, instance);
		}
		WritePartialClassHeaderFile(config, instances);
		WritePartialClassCppFile(config, instances);
		WriteGlobalHeaderFile(config, instances);
	}
}