#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Public\Source\GacUIIncludes.h"
#include <Windows.h>

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result=SetupWindowsDirect2DRenderer();
#endif

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}
extern void SetupTabPageWindow(GuiControlHost* controlHost, GuiControl* container);

/***********************************************************************
MainWindow
***********************************************************************/

class MainWindow : public GuiWindow
{
public:
	MainWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
#ifdef GUI_GRAPHICS_RENDERER_GDI
		SetText(L"Vczh GUI Demo (GDI): "+GetCurrentController()->GetOSVersion());
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
		SetText(L"Vczh GUI Demo (Direct2D): "+GetCurrentController()->GetOSVersion());
#endif
		SetClientSize(Size(800, 600));
		MoveToScreenCenter();
		//SetupTabPageWindow(this, this);
		{
			const wchar_t* texts[]=
			{
				L"When using ReadFileEx you should check GetLastError even when the function returns \"success\" to check for conditions that are \"successes\" but have some outcome you might want to know about. For example, a buffer overflow when calling ReadFileEx will return TRUE, but GetLastError will report the overflow with ERROR_MORE_DATA. If the function call is successful and there are no warning conditions, GetLastError will return ERROR_SUCCESS.",
				L"The ReadFileEx function may fail if there are too many outstanding asynchronous I/O requests. In the event of such a failure, GetLastError can return ERROR_INVALID_USER_BUFFER or ERROR_NOT_ENOUGH_MEMORY.",
				L"To cancel all pending asynchronous I/O operations, use either: ",
				L"CancelIo¡ªthis function only cancels operations issued by the calling thread for the specified file handle.",
				L"CancelIoEx¡ªthis function cancels all operations issued by the threads for the specified file handle.",
			};
			const int rowCount=sizeof(texts)/sizeof(*texts);
			
			GuiTableComposition* table=new GuiTableComposition;
			table->SetAlignmentToParent(Margin(0, 0, 0, 0));
			table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			table->SetRowsAndColumns(1, 2);
			table->SetRowOption(0, GuiCellOption::MinSizeOption());
			table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
			table->SetColumnOption(1, GuiCellOption::PercentageOption(0.5));
			{
				GuiStackComposition* sub=new GuiStackComposition;
				sub->SetDirection(GuiStackComposition::Vertical);
				sub->SetAlignmentToParent(Margin(0, 0, 0, 0));
				sub->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				for(int i=0;i<rowCount;i++)
				{
					GuiStackItemComposition* item=new GuiStackItemComposition;
					item->AddChild(CreateWrapLineText(texts[i]));
					sub->AddChild(item);
				}

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(0, 0, 1, 1);
				cell->AddChild(sub);
			}
			{
				GuiTableComposition* sub=new GuiTableComposition;
				sub->SetAlignmentToParent(Margin(0, 0, 0, 0));
				sub->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				sub->SetRowsAndColumns(rowCount, 1);
				sub->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				for(int i=0;i<rowCount;i++)
				{
					sub->SetRowOption(i, GuiCellOption::MinSizeOption());
				}
				for(int i=0;i<rowCount;i++)
				{
					GuiCellComposition* cell=new GuiCellComposition;
					sub->AddChild(cell);
					cell->SetSite(i, 0, 1, 1);
					cell->AddChild(CreateWrapLineText(texts[i]));
				}

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(0, 1, 1, 1);
				cell->AddChild(sub);
			}
			GetContainerComposition()->AddChild(table);
		}
	}

	GuiGraphicsComposition* CreateWrapLineText(const WString& text)
	{
		GuiSolidLabelElement* textElement=GuiSolidLabelElement::Create();
		textElement->SetWrapLine(true);
		textElement->SetWrapLineHeightCalculation(true);
		textElement->SetText(text);
		textElement->SetFont(GetCurrentController()->ResourceService()->GetDefaultFont());

		GuiBoundsComposition* textComposition=new GuiBoundsComposition;
		textComposition->SetOwnedElement(textElement);
		textComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
		textComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

		GuiSolidBorderElement* borderElement=GuiSolidBorderElement::Create();
		borderElement->SetColor(Color(0, 0, 255));

		GuiBoundsComposition* borderComposition=new GuiBoundsComposition;
		borderComposition->SetOwnedElement(borderElement);
		borderComposition->SetAlignmentToParent(Margin(10, 10, 10, 10));
		borderComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		borderComposition->AddChild(textComposition);

		return borderComposition;
	}
};

/***********************************************************************
GuiMain
***********************************************************************/

void GuiMain()
{
	MainWindow window;
	GetApplication()->Run(&window);
}