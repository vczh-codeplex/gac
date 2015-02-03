#include "GacGen.h"

Array<WString>* arguments = 0;

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

class ResourceMockTypeLoader : public Object, public ITypeLoader
{
protected:
	Ptr<GuiResource>							resource;
	Ptr<CodegenConfig>							config;
	Regex										regexClassName;

public:
	Dictionary<WString, Ptr<InstanceSchema>>	typeSchemas;
	List<WString>								typeSchemaOrder;
	Dictionary<WString, Ptr<Instance>>			instances;

private:

	class ClassTypeDescriptor : public TypeDescriptorImpl
	{
	protected:
		ResourceMockTypeLoader*					loader;
		Ptr<GuiInstanceDataSchema>				schema;

		void LoadInternal()override
		{
		}
	public:
		ClassTypeDescriptor(ResourceMockTypeLoader* _loader, Ptr<GuiInstanceDataSchema> _schema)
			:TypeDescriptorImpl(_schema->typeName)
			, loader(_loader)
			, schema(_schema)
		{
		}
	};

	class StructTypeSerializer : public Object, public virtual IValueSerializer
	{
	protected:
		ITypeDescriptor*						ownerTypeDescriptor;
	public:
		StructTypeSerializer(ITypeDescriptor* _ownerTypeDescriptor)
			:ownerTypeDescriptor(_ownerTypeDescriptor)
		{
		}

		ITypeDescriptor* GetOwnerTypeDescriptor()override
		{
			return ownerTypeDescriptor;
		}

		bool Validate(const WString& text)override
		{
			return false;
		}

		bool Parse(const WString& input, Value& output)override
		{
			return false;
		}

		WString GetDefaultText()override
		{
			return L"";
		}

		bool HasCandidate()override
		{
			return false;
		}

		vint GetCandidateCount()override
		{
			return 0;
		}

		WString GetCandidate(vint index)override
		{
			return L"";
		}

		bool CanMergeCandidate()override
		{
			return false;
		}
	};

	class StructTypeDescriptor : public TypeDescriptorImpl
	{
	protected:
		ResourceMockTypeLoader*					loader;
		Ptr<GuiInstanceDataSchema>				schema;
		Ptr<IValueSerializer>					serializer;

		void LoadInternal()override
		{
		}
	public:
		StructTypeDescriptor(ResourceMockTypeLoader* _loader, Ptr<GuiInstanceDataSchema> _schema)
			:TypeDescriptorImpl(_schema->typeName)
			, loader(_loader)
			, schema(_schema)
		{
			serializer = new StructTypeSerializer(this);
		}

		IValueSerializer* GetValueSerializer()override
		{
			return serializer.Obj();
		}
	};

	class InterfaceTypeDescriptor : public TypeDescriptorImpl
	{
	protected:
		ResourceMockTypeLoader*					loader;
		Ptr<GuiInstanceInterfaceSchema>			schema;

		void LoadInternal()override
		{
		}
	public:
		InterfaceTypeDescriptor(ResourceMockTypeLoader* _loader, Ptr<GuiInstanceInterfaceSchema> _schema)
			:TypeDescriptorImpl(_schema->typeName)
			, loader(_loader)
			, schema(_schema)
		{
		}
	};

	class InstanceTypeDescriptor : public TypeDescriptorImpl
	{
	protected:
		ResourceMockTypeLoader*					loader;
		Ptr<GuiInstanceContext>					context;

		void LoadInternal()override
		{
		}
	public:
		InstanceTypeDescriptor(ResourceMockTypeLoader* _loader, Ptr<GuiInstanceContext> _context)
			:TypeDescriptorImpl(_context->className.Value())
			, loader(_loader)
			, context(_context)
		{
		}
	};

public:
	ResourceMockTypeLoader(Ptr<GuiResource> _resource, Ptr<CodegenConfig> _config)
		:resource(_resource)
		, config(_config)
		, regexClassName(L"((<namespace>[^:]+)::)*(<type>[^:]+)")
	{
		Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(resource, resource->GetWorkingDirectory());

		SearchAllSchemas(regexClassName, resource, typeSchemas, typeSchemaOrder);
		SearchAllInstances(regexClassName, resolver, resource, instances);
		
		FOREACH(Ptr<Instance>, instance, instances.Values())
		{
			Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(instance->context, resolver);
			SearchAllEventHandlers(config, instances, instance, env, instance->eventHandlers);
		}
	}

	void Load(ITypeManager* manager)
	{
		FOREACH(Ptr<InstanceSchema>, schema, typeSchemas.Values())
		{
			Ptr<ITypeDescriptor> typeDescriptor;
			if (auto dataType = schema->schema.Cast<GuiInstanceDataSchema>())
			{
				if (dataType->referenceType)
				{
					typeDescriptor = new ClassTypeDescriptor(this, dataType);
				}
				else
				{
					typeDescriptor = new StructTypeDescriptor(this, dataType);
				}
			}
			else if (auto interfaceType = schema->schema.Cast<GuiInstanceInterfaceSchema>())
			{
				typeDescriptor = new InterfaceTypeDescriptor(this, interfaceType);
			}
			manager->SetTypeDescriptor(typeDescriptor->GetTypeName(), typeDescriptor);
		}

		FOREACH(Ptr<Instance>, instance, instances.Values())
		{
			if (instance->context->className)
			{
				Ptr<ITypeDescriptor> typeDescriptor = new InstanceTypeDescriptor(this, instance->context);
				manager->SetTypeDescriptor(typeDescriptor->GetTypeName(), typeDescriptor);
			}
		}
	}

	void Unload(ITypeManager* manager)
	{
	}
};

void GuiMain()
{
	Console::WriteLine(L"Vczh GacUI Resource Code Generator for C++");
	if (arguments->Count() != 1)
	{
		PrintErrorMessage(L"GacGen.exe only accept 1 input file.");
		return;
	}

	WString inputPath = arguments->Get(0);
	PrintSuccessMessage(L"gacgen> Making : " + inputPath);
	List<WString> errors;
	auto resource = GuiResource::LoadFromXml(arguments->Get(0), errors);
	if (!resource)
	{
		PrintErrorMessage(L"error> Failed to load resource.");
		return;
	}

	auto config = CodegenConfig::LoadConfig(resource);
	if (!config)
	{
		PrintErrorMessage(L"error> Failed to load config.");
		return;
	}

	auto typeLoader = MakePtr<ResourceMockTypeLoader>(resource, config);
	GetGlobalTypeManager()->AddTypeLoader(typeLoader);
	PrintSuccessMessage(L"gacgen> Compiling...");
	resource->Precompile(errors);
	FOREACH(WString, error, errors)
	{
		PrintErrorMessage(error);
	}
	GetInstanceLoaderManager()->SetResource(L"GACGEN", resource);

	FOREACH(Ptr<Instance>, instance, typeLoader->instances.Values())
	{
		WriteControlClassHeaderFile(config, instance);
		WriteControlClassCppFile(config, instance);
	}
	WritePartialClassHeaderFile(config, typeLoader->typeSchemas, typeLoader->typeSchemaOrder, typeLoader->instances);
	WritePartialClassCppFile(config, typeLoader->typeSchemas, typeLoader->typeSchemaOrder, typeLoader->instances);
	WriteGlobalHeaderFile(config, typeLoader->instances);

	if (errors.Count() > 0)
	{
		PrintErrorMessage(L"Skip generate precompiled resource because there are compilation errors.");
	}
	else
	{
		if (config->precompiledOutput != L"")
		{
			WString fileName = config->precompiledOutput;
			auto xml = resource->SaveToXml(true);
			OPEN_FILE(L"Precompiled Resource Xml");
			XmlPrint(xml, writer);
		}
		if (config->precompiledBinary != L"")
		{
			FileStream fileStream(config->resource->GetWorkingDirectory() + config->precompiledBinary, FileStream::WriteOnly);
			resource->SavePrecompiledBinary(fileStream);
		}
		if (config->precompiledCompressed != L"")
		{
			FileStream fileStream(config->resource->GetWorkingDirectory() + config->precompiledCompressed, FileStream::WriteOnly);
			LzwEncoder encoder;
			EncoderStream encoderStream(fileStream, encoder);
			resource->SavePrecompiledBinary(encoderStream);
		}
	}
}