/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTAUTOCOMPLETE
#define VCZH_PRESENTATION_CONTROLS_GUITEXTAUTOCOMPLETE

#include "GuiTextGeneralOperations.h"
#include "../../ListControlPackage/GuiTextListControls.h"
#include "../../GuiWindowControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiTextBoxAutoCompleteBase
***********************************************************************/
			
			/// <summary>The base class of text box auto complete controller.</summary>
			class GuiTextBoxAutoCompleteBase : public Object, public virtual ICommonTextEditCallback
			{
			protected:
				elements::GuiColorizedTextElement*					element;
				SpinLock*											elementModifyLock;
				compositions::GuiGraphicsComposition*				ownerComposition;
				GuiPopup*											autoCompletePopup;
				GuiTextList*										autoCompleteList;
				TextPos												autoCompleteStartPosition;

				bool												IsPrefix(const WString& prefix, const WString& candidate);
			public:
				GuiTextBoxAutoCompleteBase();
				~GuiTextBoxAutoCompleteBase();

				void												Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, compositions::GuiGraphicsComposition* _ownerComposition, vuint editVersion)override;
				void												Detach()override;
				void												TextEditPreview(TextEditPreviewStruct& arguments)override;
				void												TextEditNotify(const TextEditNotifyStruct& arguments)override;
				void												TextCaretChanged(const TextCaretChangedStruct& arguments)override;
				void												TextEditFinished(vuint editVersion)override;

				/// <summary>Get the list state.</summary>
				/// <returns>Returns true if the list is visible.</returns>
				bool												IsListOpening();
				/// <summary>Notify the list to be visible.</summary>
				/// <param name="startPosition">The text position to show the list.</param>
				void												OpenList(TextPos startPosition);
				/// <summary>Notify the list to be invisible.</summary>
				void												CloseList();
				/// <summary>Set the content of the list.</summary>
				/// <param name="list">The content of the list.</param>
				void												SetListContent(const collections::SortedList<WString>& items);
				/// <summary>Get the last start position when the list is opened.</summary>
				/// <returns>The start position.</returns>
				TextPos												GetListStartPosition();
				/// <summary>Select the previous item.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool												SelectPreviousListItem();
				/// <summary>Select the next item.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool												SelectNextListItem();
				/// <summary>Apply the selected item into the text box.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool												ApplySelectedListItem();
				/// <summary>Get the selected item.</summary>
				/// <returns>The text of the selected item. Returns empty if there is no selected item.</returns>
				WString												GetSelectedListItem();
				/// <summary>Highlight a candidate item in the list.</summary>
				/// <param name="editingText">The text to match an item.</param>
				void												HighlightList(const WString& editingText);
			};
		}
	}
}

#endif