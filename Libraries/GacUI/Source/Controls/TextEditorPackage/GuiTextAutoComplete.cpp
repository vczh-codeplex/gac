#include "GuiTextAutoComplete.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;
			using namespace elements::text;
			using namespace compositions;
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace collections;

/***********************************************************************
GuiTextBoxAutoCompleteBase
***********************************************************************/

			GuiTextBoxAutoCompleteBase::GuiTextBoxAutoCompleteBase()
				:element(0)
				,elementModifyLock(0)
			{
			}

			GuiTextBoxAutoCompleteBase::~GuiTextBoxAutoCompleteBase()
			{
			}

			void GuiTextBoxAutoCompleteBase::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)
			{
				if(_element)
				{
					SpinLock::Scope scope(_elementModifyLock);
					element=_element;
					elementModifyLock=&_elementModifyLock;
				}
			}

			void GuiTextBoxAutoCompleteBase::Detach()
			{
				if(element && elementModifyLock)
				{
					SpinLock::Scope scope(*elementModifyLock);
					element=0;
					elementModifyLock=0;
				}
			}

			void GuiTextBoxAutoCompleteBase::TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)
			{
				if(element && elementModifyLock)
				{
				}
			}

			void GuiTextBoxAutoCompleteBase::TextCaretChanged(TextPos oldBegin, TextPos oldEnd, TextPos newBegin, TextPos newEnd)
			{
			}

			void GuiTextBoxAutoCompleteBase::TextEditFinished()
			{
			}
		}
	}
}