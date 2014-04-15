#include "HelloWorld.h"
#include <Windows.h>

using namespace vl::collections;
using namespace demos;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class ControlViewModel : public Object, public virtual IControlViewModel
{
private:
	vint			first = 0;
	vint			second = 0;
public:
	//vint GetFirst()override
	//{
	//	return first;
	//}

	//void SetFirst(vint value)override
	//{
	//	if (first != value)
	//	{
	//		first = value;
	//		SumChanged();
	//	}
	//}

	//vint GetSecond()override
	//{
	//	return second;
	//}

	//void SetSecond(vint value)override
	//{
	//	if (second != value)
	//	{
	//		second = value;
	//		SumChanged();
	//	}
	//}

	//vint GetSum()override
	//{
	//	return first + second;
	//}
};

class WindowViewModel : public Object, public virtual IWindowViewModel
{
private:
	Ptr<IControlViewModel>		controlViewModel;
public:
	WindowViewModel()
	{
		controlViewModel = new ControlViewModel;
	}

	//Ptr<IControlViewModel> GetControlViewModel()override
	//{
	//	return controlViewModel;
	//}
};

void GuiMain()
{
	List<WString> errors;
	auto resource = GuiResource::LoadFromXml(L"XmlWindowResource.xml", errors);
	GetInstanceLoaderManager()->SetResource(L"HelloWorld", resource);

	MainWindow window(new WindowViewModel);
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}