/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7TABSTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7TABSTYLES

#include "GuiWin7ButtonStyles.h"
#include "GuiWin7MenuStyles.h"
#include "GuiWin7ControlStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

/***********************************************************************
Tab
***********************************************************************/
			
			/// <summary>Tab page header style (Windows 7).</summary>
			class Win7TabPageHeaderStyle : public Win7ButtonStyleBase, public Description<Win7TabPageHeaderStyle>
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				/// <summary>Create the style.</summary>
				Win7TabPageHeaderStyle();
				~Win7TabPageHeaderStyle();

				void										SetFont(const FontProperties& value)override;
			};
			
			/// <summary>Tab control style (Windows 7).</summary>
			class Win7TabStyle : public Object, public virtual controls::GuiTab::IStyleController, public Description<Win7TabStyle>
			{
			protected:
				compositions::GuiTableComposition*							boundsComposition;
				compositions::GuiBoundsComposition*							containerComposition;
				compositions::GuiStackComposition*							tabHeaderComposition;
				compositions::GuiBoundsComposition*							tabContentTopLineComposition;
				FontProperties												headerFont;
				controls::GuiTab::ICommandExecutor*							commandExecutor;

				Ptr<controls::GuiSelectableButton::MutexGroupController>	headerController;
				collections::List<controls::GuiSelectableButton*>			headerButtons;
				elements::GuiPolygonElement*								headerOverflowArrowElement;
				controls::GuiButton*										headerOverflowButton;
				controls::GuiToolstripMenu*									headerOverflowMenu;

				void										OnHeaderButtonClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnTabHeaderBoundsChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnHeaderOverflowButtonClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnHeaderOverflowMenuButtonClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);

				void										UpdateHeaderOverflowButtonVisibility();
				void										UpdateHeaderZOrder();
				void										UpdateHeaderVisibilityIndex();
				void										UpdateHeaderLayout();
			public:
				/// <summary>Create the style.</summary>
				Win7TabStyle();
				~Win7TabStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;

				void										SetCommandExecutor(controls::GuiTab::ICommandExecutor* value)override;
				void										InsertTab(vint index)override;
				void										SetTabText(vint index, const WString& value)override;
				void										RemoveTab(vint index)override;
				void										MoveTab(vint oldIndex, vint newIndex)override;
				void										SetSelectedTab(vint index)override;
				controls::GuiControl::IStyleController*		CreateTabPageStyleController()override;
			};
		}
	}
}

#endif