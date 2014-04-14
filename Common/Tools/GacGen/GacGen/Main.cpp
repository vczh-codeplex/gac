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
		FOREACH(WString, eventName, repr->eventHandlers.Keys())
		{
			if (!eventHandlers.Keys().Contains(eventName))
			{
				IGuiInstanceLoader* loader = 0;
				if (auto info = GetEventInfo(typeInfo, eventName, loader))
				{
					eventHandlers.Add(repr->eventHandlers[eventName], info->argumentType);
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
	writer.WriteLine(L"Developer: �����(vczh)");
	writer.WriteLine(L"GacUI::" + name);
	writer.WriteLine(L"");
	writer.WriteLine(L"���ļ�ʹ��Vczh GacUI Resource Code Generator�����Զ�����");
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
	writer.WriteLine(prefix + L"\t" + instance->typeName + L"();");
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
	writer.WriteLine(prefix + instance->typeName + L"::" + instance->typeName + L"()");
	writer.WriteLine(prefix + L"{");
	writer.WriteLine(prefix + L"\tInitializeComponents();");
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

void WritePartialClassHeaderFile(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances)
{
	WString fileName = config->GetPartialClassHeaderFileName();
	OPEN_FILE_WITH_COMMENT(L"Partial Classes", true);

	writer.WriteLine(L"#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_PARTIAL_CLASSES");
	writer.WriteLine(L"#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_" + config->name + L"_PARTIAL_CLASSES");
	writer.WriteLine(L"");
	writer.WriteLine(L"#include \"" + config->include + L"\"");
	writer.WriteLine(L"");

	FOREACH(Ptr<Instance>, instance, instances.Values())
	{
		WString prefix = WriteNamespaceBegin(instance->namespaces, writer);
		writer.WriteLine(prefix + L"template<typename TImpl>");
		writer.WriteLine(prefix + L"class " + instance->typeName + L"_ : public " + GetCppTypeName(instance->baseType) + L", public vl::presentation::GuiInstancePartialClass<vl::" + instance->baseType->GetTypeName() + L">, public vl::reflection::Description<TImpl>");
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
			writer.WriteLine(prefix + L"\t\t:" + GetCppTypeName(instance->baseType) + L"(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())");
		}
		else if (instance->baseType == GetTypeDescriptor<GuiCustomControl>())
		{
			writer.WriteLine(prefix + L"\t\t:" + GetCppTypeName(instance->baseType) + L"(vl::presentation::theme::GetCurrentTheme()->CreateCustomControlStyle())");
		}
		writer.WriteLine(prefix + L"\t\t,vl::presentation::GuiInstancePartialClass<" + GetCppTypeName(instance->baseType) + L">(L\"" + instance->GetFullName() + L"\")");
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

	{
		List<WString> ns;
		FillReflectionNamespaces(ns);
		WString prefix = WriteNamespaceBegin(ns, writer);
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

void WritePartialClassCppFile(Ptr<CodegenConfig> config, Dictionary<WString, Ptr<Instance>>& instances)
{
	WString fileName = config->GetPartialClassCppFileName();
	OPEN_FILE_WITH_COMMENT(L"Partial Classes", true);

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
		writer.WriteLine(prefix + L"\tCLASS_MEMBER_BASE(" + GetCppTypeName(instance->baseType) + L")");
		writer.WriteLine(prefix + L"\tCLASS_MEMBER_CONSTRUCTOR(" + instance->GetFullName() + L"*(), NO_PARAMETER)");
		writer.WriteLine(L"");
		FOREACH(WString, name, instance->eventHandlers.Keys())
		{
			writer.WriteLine(prefix + L"\tCLASS_MEMBER_GUIEVENT_HANDLER(" + name + L", " + GetCppTypeName(instance->eventHandlers[name]) + L")");
		}
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
	auto resource = GuiResource::LoadFromXml(arguments->Get(0));
	if (!resource)
	{
		PrintErrorMessage(L"error> Failed to load resource.");
		return;
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
	Dictionary<WString, Ptr<Instance>> instances;
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
	WritePartialClassHeaderFile(config, instances);
	WritePartialClassCppFile(config, instances);
	WriteGlobalHeaderFile(config, instances);
}