#include "HelloWorld.h"
#include <Windows.h>

using namespace vl::collections;
using namespace demos;
using namespace data;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class ViewModel : public Object, public virtual PointProvider
{
public:
	vint GetCount()
	{
		return 5;
	}

	data::Point GetPoint(vint index)
	{
		data::Point p{ index, index + 1 };
		return p;
	}
};

void GuiMain()
{
	List<WString> errors;
	auto resource = GuiResource::LoadFromXml(L"XmlWindowResource.precompiled.xml", errors);
	GetInstanceLoaderManager()->SetResource(L"HelloWorld", resource);

	MainWindow window(new ViewModel);
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}