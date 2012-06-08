/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Windows Implementation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSRESOURCESERVICE
#define VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSRESOURCESERVICE

#include "..\..\GuiNativeWindow.h"
#include <windows.h>

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class WindowsCursor : public Object, public INativeCursor
			{
			protected:
				HCURSOR										handle;
				bool										isSystemCursor;
				SystemCursorType							systemCursorType;
			public:
				WindowsCursor(HCURSOR _handle);
				WindowsCursor(SystemCursorType type);
				bool										IsSystemCursor();
				SystemCursorType							GetSystemCursorType();
				HCURSOR										GetCursorHandle();
			};

			class WindowsResourceService : public Object, public INativeResourceService
			{
			protected:
				collections::Array<Ptr<WindowsCursor>>		systemCursors;
				FontProperties								defaultFont;
			public:
				WindowsResourceService();
				INativeCursor*								GetSystemCursor(INativeCursor::SystemCursorType type);
				INativeCursor*								GetDefaultSystemCursor();
				FontProperties								GetDefaultFont();
				void										SetDefaultFont(const FontProperties& value);
			};
		}
	}
}

#endif