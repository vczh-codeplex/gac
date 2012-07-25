/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI Header Files and Common Namespaces

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GACUI
#define VCZH_PRESENTATION_GACUI

#include "Controls\GuiBasicControls.h"
#include "Controls\GuiWindowControls.h"
#include "Controls\GuiTextControls.h"
#include "Controls\GuiListControls.h"
#include "Controls\GuiApplication.h"

#include "Controls\ExtendedControls\GuiContainerControls.h"
#include "Controls\ExtendedControls\GuiTextListControls.h"
#include "Controls\ExtendedControls\GuiListViewControls.h"
#include "Controls\ExtendedControls\GuiTreeViewControls.h"
#include "Controls\ExtendedControls\GuiMenuControls.h"
#include "Controls\ExtendedControls\GuiComboControls.h"

#include "Controls\ToolstripPackage\GuiToolstripCommand.h"
#include "Controls\ToolstripPackage\GuiToolstripMenu.h"

#include "Controls\Styles\GuiWin7Styles.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;
using namespace vl::presentation::compositions;
using namespace vl::presentation::controls;
using namespace vl::presentation::theme;

extern int SetupWindowsGDIRenderer();
extern int SetupWindowsDirect2DRenderer();

#endif