#include "GuiGraphicsLayoutProviderWindowsGDI.h"
#include "GuiGraphicsWindowsGDI.h"
#include <usp10.h>
#include <math.h>

#pragma comment(lib, "usp10.lib")

bool operator==(const SCRIPT_ITEM&, const SCRIPT_ITEM&){return false;}
bool operator!=(const SCRIPT_ITEM&, const SCRIPT_ITEM&){return false;}

bool operator==(const SCRIPT_VISATTR&, const SCRIPT_VISATTR&){return false;}
bool operator!=(const SCRIPT_VISATTR&, const SCRIPT_VISATTR&){return false;}

bool operator==(const GOFFSET&, const GOFFSET&){return false;}
bool operator!=(const GOFFSET&, const GOFFSET&){return false;}

bool operator==(const SCRIPT_LOGATTR&, const SCRIPT_LOGATTR&){return false;}
bool operator!=(const SCRIPT_LOGATTR&, const SCRIPT_LOGATTR&){return false;}

namespace vl
{
	namespace presentation
	{
		using namespace elements;
		using namespace collections;

		namespace elements_windows_gdi
		{

/***********************************************************************
WindowsGDIParagraph
***********************************************************************/

			class WindowsGDIParagraph : public Object, public IGuiGraphicsParagraph
			{
			protected:
				IGuiGraphicsLayoutProvider*			provider;

			public:
				WindowsGDIParagraph(IGuiGraphicsLayoutProvider* _provider)
					:provider(_provider)
				{
				}

				~WindowsGDIParagraph()
				{
				}

				IGuiGraphicsLayoutProvider* GetProvider()override
				{
					return provider;
				}

				bool GetWrapLine()override
				{
					throw 0;
				}

				void SetWrapLine(bool value)override
				{
					throw 0;
				}

				int GetMaxWidth()override
				{
					throw 0;
				}

				void SetMaxWidth(int value)override
				{
					throw 0;
				}

				bool SetFont(int start, int length, const WString& value)override
				{
					throw 0;
				}

				bool SetSize(int start, int length, int size)override
				{
					throw 0;
				}

				bool SetStyle(int start, int length, TextStyle value)override
				{
					throw 0;
				}

				bool SetColor(int start, int length, Color value)override
				{
					throw 0;
				}

				int GetHeight()override
				{
					throw 0;
				}

				void Render(Rect bounds)override
				{
					throw 0;
				}
			};

/***********************************************************************
WindowsGDILayoutProvider
***********************************************************************/

			Ptr<IGuiGraphicsParagraph> WindowsGDILayoutProvider::CreateParagraph(const WString& text, IGuiGraphicsRenderTarget* renderTarget)
			{
				return new WindowsGDIParagraph(this);
			}
		}
	}
}