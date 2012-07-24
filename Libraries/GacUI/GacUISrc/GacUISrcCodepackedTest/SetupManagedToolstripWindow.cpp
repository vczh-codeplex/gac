#include "..\..\Public\Source\GacUIIncludes.h"

using namespace collections;

void SetupManagedToolstripWindow(GuiControlHost* controlHost, GuiControl* container)
{
	WString name;
	name=GetCurrentController()->InputService()->GetKeyName(L'A');
	name=GetCurrentController()->InputService()->GetKeyName(VKEY_F1);
	name=GetCurrentController()->InputService()->GetKeyName(VKEY_NUMPAD0);
	name=GetCurrentController()->InputService()->GetKeyName(VKEY_DECIMAL);
	name=GetCurrentController()->InputService()->GetKeyName(VKEY_SPACE);
	name=GetCurrentController()->InputService()->GetKeyName(VKEY_RETURN);
	name=GetCurrentController()->InputService()->GetKeyName(VKEY_INSERT);
}