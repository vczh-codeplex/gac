#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIReflection.h"

using namespace vl::console;
using namespace vl::collections;

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

class CodegenConfig
{
public:
	WString						includePath;

	static Ptr<CodegenConfig> LoadConfig(Ptr<GuiResource> resource)
	{
		Ptr<CodegenConfig> config = new CodegenConfig;
		if (auto includeItem = resource->GetValueByPath(L"GacGenConfig/Include").Cast<GuiTextData>())
		{
			config->includePath = includeItem->GetText();
		}
		else
		{
			PrintErrorMessage(L"Cannot find include path for GacUIResource.h in resource \"GacGenConfig/Include\".");
			return 0;
		}
		return config;
	}
};

void GuiMain()
{
	Console::WriteLine(L"Welcome to GacUI Resource Code Behind Generator");

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
}