#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIIncludes.h"
#include "..\NestleConsole\NestleSDK.h"

using namespace vl::nestle;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	int result=SetupWindowsDirect2DRenderer();

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

/***********************************************************************
Page
***********************************************************************/

class NestlePage : public GuiControl
{
private:
	class StyleController : public GuiControl::IStyleController
	{
	protected:
		GuiBoundsComposition*			composition;
	public:
		StyleController()
		{
			composition=new GuiBoundsComposition();
			composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			composition->SetAlignmentToParent(Margin(5, 5, 5, 5));
		}

		compositions::GuiBoundsComposition* GetBoundsComposition()
		{
			return composition;
		}

		compositions::GuiGraphicsComposition* GetContainerComposition()
		{
			return composition;
		}

		void SetFocusableComposition(compositions::GuiGraphicsComposition* value)
		{
		}

		void SetText(const WString& value)
		{
		}

		void SetFont(const FontProperties& value)
		{
		}

		void SetVisuallyEnabled(bool value)
		{
		}
	};
public:
	NestlePage()
		:GuiControl(new StyleController)
	{
	}
};

/***********************************************************************
Login Page
***********************************************************************/

class LoginPage : public NestlePage
{
protected:
	GuiSinglelineTextBox*				textUsername;
	GuiSinglelineTextBox*				textPassword;
	GuiButton*							buttonLogin;
	GuiButton*							buttonCancel;
public:
	LoginPage()
	{
		GuiTableComposition* table=new GuiTableComposition;
		GetContainerComposition()->AddChild(table);
		table->SetAlignmentToParent(Margin(0, 0, 0, 0));
		table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		table->SetRowsAndColumns(5, 6);
		table->SetCellPadding(8);
		table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
		table->SetRowOption(1, GuiCellOption::MinSizeOption());
		table->SetRowOption(2, GuiCellOption::MinSizeOption());
		table->SetRowOption(3, GuiCellOption::MinSizeOption());
		table->SetRowOption(4, GuiCellOption::PercentageOption(0.5));
		table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
		table->SetColumnOption(1, GuiCellOption::MinSizeOption());
		table->SetColumnOption(2, GuiCellOption::AbsoluteOption(40));
		table->SetColumnOption(3, GuiCellOption::MinSizeOption());
		table->SetColumnOption(4, GuiCellOption::MinSizeOption());
		table->SetColumnOption(5, GuiCellOption::PercentageOption(0.5));
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 1, 1, 1);

			GuiLabel* label=g::NewLabel();
			label->SetText(L"用户名：");
			cell->AddChild(label->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(2, 1, 1, 1);

			GuiLabel* label=g::NewLabel();
			label->SetText(L"密码：");
			cell->AddChild(label->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 2, 1, 3);

			textUsername=g::NewTextBox();
			textUsername->GetBoundsComposition()->SetPreferredMinSize(Size(0, 32));
			textUsername->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(textUsername->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(2, 2, 1, 3);

			textPassword=g::NewTextBox();
			textPassword->SetPasswordChar(L'*');
			textPassword->GetBoundsComposition()->SetPreferredMinSize(Size(0, 32));
			textPassword->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(textPassword->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(3, 3, 1, 1);

			buttonLogin=g::NewButton();
			buttonLogin->GetBoundsComposition()->SetPreferredMinSize(Size(80, 0));
			buttonLogin->SetText(L"强势登录鸟窝");
			buttonLogin->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(buttonLogin->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(3, 4, 1, 1);

			buttonCancel=g::NewButton();
			buttonCancel->GetBoundsComposition()->SetPreferredMinSize(Size(80, 0));
			buttonCancel->SetText(L"我呸！");
			buttonCancel->SetEnabled(false);
			buttonCancel->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(buttonCancel->GetBoundsComposition());
		}
		{
			GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
			element->SetFont(GetFont());
			element->SetWrapLine(true);
			element->SetText(L"点击登录，畅游@Kula的傻逼鸟窝网站，放松身心与鸟人们进行激情互动！");

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetOwnedElement(element);
			cell->SetSite(4, 1, 1, 4);
		}
	}
};

/***********************************************************************
Main Window
***********************************************************************/

class NestleWindow : public GuiWindow
{
protected:
	GuiBoundsComposition*			pageContainerComposition;

	void InitializeComponents()
	{
		{
			Ptr<INativeImage> image=GetCurrentController()
				->ImageService()
				->CreateImageFromFile(L"..\\Resources\\Background.jpg");

			GuiImageFrameElement* element=GuiImageFrameElement::Create();
			element->SetImage(image);
			element->SetStretch(true);

			GuiBoundsComposition* composition=new GuiBoundsComposition();
			composition->SetOwnedElement(element);
			composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			GetContainerComposition()->AddChild(composition);
		}
		{
			GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
			element->SetColor(Color(255, 255, 255, 128));

			GuiBoundsComposition* composition=new GuiBoundsComposition();
			composition->SetOwnedElement(element);
			composition->SetAlignmentToParent(Margin(24, 24, 24, 32));
			composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			GetContainerComposition()->AddChild(composition);

			pageContainerComposition=composition;
		}
		{
			FontProperties font=GetFont();
			font.bold=true;
			GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
			element->SetFont(font);
			element->SetColor(Color(255, 255, 255));
			element->SetAlignments(Alignment::Center, Alignment::Center);
			element->SetText(L"算法就是构造一个数据结构，把数据插入到数据结构的指定位置 ---- @Kula");

			GuiBoundsComposition* composition=new GuiBoundsComposition();
			composition->SetOwnedElement(element);
			composition->SetAlignmentToParent(Margin(2, -1, 2, 2));
			composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
			GetContainerComposition()->AddChild(composition);
		}
		pageContainerComposition->AddChild((new LoginPage)->GetBoundsComposition());
	}
public:
	//
	NestleWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Vczh 鸟窝客户端 v1.0 (https://niaowo.me)");
		SetClientSize(Size(800, 600));
		GetBoundsComposition()->SetPreferredMinSize(Size(400, 300));
		GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		InitializeComponents();
		MoveToScreenCenter();
	}
};

/***********************************************************************
GuiMain
***********************************************************************/

void GuiMain()
{
	{
		FontProperties font=GetCurrentController()->ResourceService()->GetDefaultFont();
		font.size=16;
		GetCurrentController()->ResourceService()->SetDefaultFont(font);
	}
	NestleWindow window;
	GetApplication()->Run(&window);
}