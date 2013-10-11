/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Header Files and Common Namespaces

Global Objects:
	vl::reflection::description::		GetGlobalTypeManager
	vl::presentation::					GetCurrentController
	vl::presentation::elements::		GetGuiGraphicsResourceManager
	vl::presentation::controls::		GetApplication
	vl::presentation::controls::		GetPluginManager
	vl::presentation::theme::			GetCurrentTheme
***********************************************************************/

#ifndef VCZH_PRESENTATION_GACUI
#define VCZH_PRESENTATION_GACUI

#include "Controls\GuiApplication.h"
#include "Controls\Styles\GuiWin7Styles.h"
#include "Controls\Styles\GuiWin8Styles.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;
using namespace vl::presentation::compositions;
using namespace vl::presentation::controls;
using namespace vl::presentation::theme;

extern int SetupWindowsGDIRenderer();
extern int SetupWindowsDirect2DRenderer();

#endif