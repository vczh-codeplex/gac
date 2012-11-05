#include "..\..\Public\Source\GacUI.h"
#include <math.h>
#include <Windows.h>

using namespace vl::presentation::windows;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsGDIRenderer();
}

class GDIWindow : public GuiWindow
{
protected:
	static const int		Radius=200;
	static const int		LongScale=10;
	static const int		ShortScale=5;
	static const int		SecondLength=180;
	static const int		MinuteLength=150;
	static const int		HourLength=120;

	Ptr<WinPen>				borderPen;
	Ptr<WinPen>				secondPen;
	Ptr<WinPen>				minutePen;
	Ptr<WinPen>				hourPen;

	void CreateGDIResources()
	{
		borderPen=new WinPen(PS_SOLID, 1, RGB(0, 0, 0));
		secondPen=new WinPen(PS_SOLID, 1, RGB(0, 0, 255));
		minutePen=new WinPen(PS_SOLID, 3, RGB(0, 255, 0));
		hourPen=new WinPen(PS_SOLID, 5, RGB(255, 0, 0));
	}

	double GetAngle(int second)
	{
		return (second-15)*3.1416/30;
	}

	void element_Rendering(GuiGraphicsComposition* sender, GuiGDIElementEventArgs& arguments)
	{
		int w=arguments.bounds.Width();
		int h=arguments.bounds.Height();
		int x=arguments.bounds.Left()+(w-Radius*2)/2;
		int y=arguments.bounds.Left()+(h-Radius*2)/2;

		arguments.dc->SetPen(borderPen);
		arguments.dc->Ellipse(x, y, x+Radius*2, y+Radius*2);
		for(int i=0;i<60;i++)
		{
			int scale=i%5==0?LongScale:ShortScale;
			double angle=GetAngle(i);
			double s=sin(angle);
			double c=cos(angle);
			int x1=(int)(c*Radius)+x+Radius;
			int y1=(int)(s*Radius)+y+Radius;
			int x2=(int)(c*(Radius-scale))+x+Radius;
			int y2=(int)(s*(Radius-scale))+y+Radius;
			arguments.dc->MoveTo(x1, y1);
			arguments.dc->LineTo(x2, y2);
		}

		DateTime dt=DateTime::LocalTime();
		{
			arguments.dc->SetPen(hourPen);
			double angle=GetAngle(dt.hour*5);
			double s=sin(angle);
			double c=cos(angle);
			int px=(int)(c*HourLength)+x+Radius;
			int py=(int)(s*HourLength)+y+Radius;
			arguments.dc->MoveTo(x+Radius, y+Radius);
			arguments.dc->LineTo(px, py);
		}
		{
			arguments.dc->SetPen(minutePen);
			double angle=GetAngle(dt.minute);
			double s=sin(angle);
			double c=cos(angle);
			int px=(int)(c*MinuteLength)+x+Radius;
			int py=(int)(s*MinuteLength)+y+Radius;
			arguments.dc->MoveTo(x+Radius, y+Radius);
			arguments.dc->LineTo(px, py);
		}
		{
			arguments.dc->SetPen(secondPen);
			double angle=GetAngle(dt.second);
			double s=sin(angle);
			double c=cos(angle);
			int px=(int)(c*SecondLength)+x+Radius;
			int py=(int)(s*SecondLength)+y+Radius;
			arguments.dc->MoveTo(x+Radius, y+Radius);
			arguments.dc->LineTo(px, py);
		}
	}
public:
	GDIWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Rendering.RawAPI.GDI");
		SetClientSize(Size(640, 480));
		GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		MoveToScreenCenter();
		{
			CreateGDIResources();

			GuiGDIElement* element=GuiGDIElement::Create();
			element->Rendering.AttachMethod(this, &GDIWindow::element_Rendering);

			GuiBoundsComposition* composition=new GuiBoundsComposition;
			composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			composition->SetOwnedElement(element);
			GetContainerComposition()->AddChild(composition);
		}
	}
};

void GuiMain()
{
	GDIWindow window;
	GetApplication()->Run(&window);
}