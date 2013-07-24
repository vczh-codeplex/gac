/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTELEMENTOPERATOR
#define VCZH_PRESENTATION_CONTROLS_GUITEXTELEMENTOPERATOR

#include "..\GuiContainerControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Common Operations
***********************************************************************/

			class ICommonTextEditCallback : public virtual IDescriptable, public Description<ICommonTextEditCallback>
			{
			public:
				virtual void							Attach(elements::GuiColorizedTextElement* element, SpinLock& elementModifyLock)=0;
				virtual void							Detach()=0;
				virtual void							TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
				virtual void							TextEditFinished()=0;
			};
		}
	}
}

#endif