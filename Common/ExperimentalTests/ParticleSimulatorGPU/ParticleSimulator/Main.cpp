#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUI.h"
#include <math.h>
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	// SetupWindowsDirect2DRenderer() is required for GuiDirect2DElement
	return SetupWindowsDirect2DRenderer();
}

class Direct2DWindow : public GuiWindow
{
protected:
	static const int						NoGravityRadius=420;

	ComPtr<ID2D1SolidColorBrush>			brushNoGravity;

	float GetAngle(float second)
	{
		return (second-15.0f)*3.1416f/30.0f;
	}

	// arguments.rt is ID2D1RenderTarget.
	void element_Rendering(GuiGraphicsComposition* sender, GuiDirect2DElementEventArgs& arguments)
	{
		int w=arguments.bounds.Width();
		int h=arguments.bounds.Height();
		{
			arguments.rt->Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f));
			int x=arguments.bounds.Left()+(w-NoGravityRadius*2)/2;
			int y=arguments.bounds.Left()+(h-NoGravityRadius*2)/2;
			arguments.rt->DrawEllipse(
				D2D1::Ellipse(
					D2D1::Point2F(
						(float)(x+NoGravityRadius),
						(float)(y+NoGravityRadius)
						),
					(float)NoGravityRadius,
					(float)NoGravityRadius),
				brushNoGravity.Obj(),
				3.0f
				);
		}
	}

	// The render target is going to be destroyed, any binded resources should be released.
	void element_BeforeRenderTargetChanged(GuiGraphicsComposition* sender, GuiDirect2DElementEventArgs& arguments)
	{
		brushNoGravity=0;
	}

	// The new render target is prepared, any binded resources are allowed to recreate now.
	void element_AfterRenderTargetChanged(GuiGraphicsComposition* sender, GuiDirect2DElementEventArgs& arguments)
	{
		ID2D1SolidColorBrush* brush;
		{
			brush=0;
			arguments.rt->CreateSolidColorBrush(D2D1::ColorF(0.8f, 0.4f, 0.2f), D2D1::BrushProperties(), &brush);
			brushNoGravity=brush;
		}
	}
public:
	Direct2DWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Rendering.RawAPI.Direct2D");
		SetMinimizedBox(false);
		SetMaximizedBox(false);
		SetSizeBox(false);
		SetClientSize(Size(960, 960));
		MoveToScreenCenter();
		{
			GuiDirect2DElement* element=GuiDirect2DElement::Create();
			element->Rendering.AttachMethod(this, &Direct2DWindow::element_Rendering);
			element->BeforeRenderTargetChanged.AttachMethod(this, &Direct2DWindow::element_BeforeRenderTargetChanged);
			element->AfterRenderTargetChanged.AttachMethod(this, &Direct2DWindow::element_AfterRenderTargetChanged);

			GuiBoundsComposition* composition=new GuiBoundsComposition;
			composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			composition->SetOwnedElement(element);
			GetContainerComposition()->AddChild(composition);
		}
	}
};

void GuiMain()
{
	Direct2DWindow window;
	GetApplication()->Run(&window);
}