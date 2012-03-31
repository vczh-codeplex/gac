/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Common Style Helpers

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUICOMMONSTYLES
#define VCZH_PRESENTATION_CONTROLS_GUICOMMONSTYLES

#include "..\GuiBasicControls.h"
#include "..\GuiTextControls.h"
#include "..\GuiListControls.h"
#include "..\GuiWindowControls.h"
#include "..\ExtendedControls\GuiContainerControls.h"
#include "..\ExtendedControls\GuiTextListControls.h"
#include "..\ExtendedControls\GuiListViewControls.h"
#include "..\ExtendedControls\GuiTreeViewControls.h"
#include "..\ExtendedControls\GuiMenuControls.h"
#include "..\ExtendedControls\GuiComboControls.h"

namespace vl
{
	namespace presentation
	{
		namespace common_styles
		{

/***********************************************************************
Scrolls
***********************************************************************/

			class CommonScrollStyle : public Object, public virtual controls::GuiScroll::IStyleController, public Description<CommonScrollStyle>
			{
			public:
				enum Direction
				{
					Horizontal,
					Vertical,
				};
			protected:
				Direction											direction;
				controls::GuiScroll::ICommandExecutor*				commandExecutor;
				controls::GuiButton*								decreaseButton;
				controls::GuiButton*								increaseButton;
				controls::GuiButton*								handleButton;
				compositions::GuiPartialViewComposition*			handleComposition;
				compositions::GuiBoundsComposition*					boundsComposition;

				int													totalSize;
				int													pageSize;
				int													position;
				Point												draggingStartLocation;
				bool												draggingHandle;

				void												UpdateHandle();
				void												OnDecreaseButtonClicked(compositions::GuiGraphicsComposition* sender,compositions::GuiEventArgs& arguments);
				void												OnIncreaseButtonClicked(compositions::GuiGraphicsComposition* sender,compositions::GuiEventArgs& arguments);
				void												OnHandleMouseDown(compositions::GuiGraphicsComposition* sender,compositions::GuiMouseEventArgs& arguments);
				void												OnHandleMouseMove(compositions::GuiGraphicsComposition* sender,compositions::GuiMouseEventArgs& arguments);
				void												OnHandleMouseUp(compositions::GuiGraphicsComposition* sender,compositions::GuiMouseEventArgs& arguments);;
				void												OnBigMoveMouseDown(compositions::GuiGraphicsComposition* sender,compositions::GuiMouseEventArgs& arguments);

				virtual controls::GuiButton::IStyleController*		CreateDecreaseButtonStyle(Direction direction)=0;
				virtual controls::GuiButton::IStyleController*		CreateIncreaseButtonStyle(Direction direction)=0;
				virtual controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)=0;
				virtual void										InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)=0;
				void												BuildStyle(int defaultSize, int arrowSize);
			public:
				CommonScrollStyle(Direction _direction);
				~CommonScrollStyle();

				compositions::GuiBoundsComposition*					GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*				GetContainerComposition()override;
				void												SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void												SetText(const WString& value)override;
				void												SetFont(const FontProperties& value)override;
				void												SetVisuallyEnabled(bool value)override;
				void												SetCommandExecutor(controls::GuiScroll::ICommandExecutor* value)override;
				void												SetTotalSize(int value)override;
				void												SetPageSize(int value)override;
				void												SetPosition(int value)override;
			};

			class CommonTrackStyle : public Object, public virtual controls::GuiScroll::IStyleController, public Description<CommonTrackStyle>
			{
			public:
				enum Direction
				{
					Horizontal,
					Vertical,
				};
			protected:
				Direction											direction;
				controls::GuiScroll::ICommandExecutor*				commandExecutor;
				compositions::GuiBoundsComposition*					boundsComposition;
				controls::GuiButton*								handleButton;
				compositions::GuiTableComposition*					handleComposition;

				int													totalSize;
				int													pageSize;
				int													position;
				Point												draggingStartLocation;
				bool												draggingHandle;

				void												UpdateHandle();
				void												OnHandleMouseDown(compositions::GuiGraphicsComposition* sender,compositions::GuiMouseEventArgs& arguments);
				void												OnHandleMouseMove(compositions::GuiGraphicsComposition* sender,compositions::GuiMouseEventArgs& arguments);
				void												OnHandleMouseUp(compositions::GuiGraphicsComposition* sender,compositions::GuiMouseEventArgs& arguments);
				
				virtual controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)=0;
				virtual void										InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)=0;
				virtual void										InstallTrack(compositions::GuiGraphicsComposition* trackComposition, Direction direction)=0;
				void												BuildStyle(int trackThickness, int trackPadding, int handleLong, int handleShort);
			public:
				CommonTrackStyle(Direction _direction);
				~CommonTrackStyle();

				compositions::GuiBoundsComposition*					GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*				GetContainerComposition()override;
				void												SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void												SetText(const WString& value)override;
				void												SetFont(const FontProperties& value)override;
				void												SetVisuallyEnabled(bool value)override;
				void												SetCommandExecutor(controls::GuiScroll::ICommandExecutor* value)override;
				void												SetTotalSize(int value)override;
				void												SetPageSize(int value)override;
				void												SetPosition(int value)override;
			};
		}
	}
}

#endif