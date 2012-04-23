/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Common Style Helpers

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUICOMMONSTYLES
#define VCZH_PRESENTATION_CONTROLS_GUICOMMONSTYLES

#include "GuiThemeStyleFactory.h"

namespace vl
{
	namespace presentation
	{
		namespace common_styles
		{

/***********************************************************************
Scrolls
***********************************************************************/

			/// <summary>A general implementation for <see cref="controls::GuiScroll::IStyleController"/> to make a scroll control.</summary>
			class CommonScrollStyle : public Object, public virtual controls::GuiScroll::IStyleController, public Description<CommonScrollStyle>
			{
			public:
				/// <summary>Scroll direction.</summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.common_styles.CommonScrollStyle.Direction]Horizontal scroll.</summary>
					Horizontal,
					/// <summary>[T:vl.presentation.common_styles.CommonScrollStyle.Direction]Vertical scroll.</summary>
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

				/// <summary>Callback to create a style controller for the decrement button.</summary>
				/// <returns>The created style controller.</returns>
				/// <param name="direction">The direction of the scroll.</param>
				virtual controls::GuiButton::IStyleController*		CreateDecreaseButtonStyle(Direction direction)=0;
				/// <summary>Callback to create a style controller for the increment button.</summary>
				/// <returns>The created style controller.</returns>
				/// <param name="direction">The direction of the scroll.</param>
				virtual controls::GuiButton::IStyleController*		CreateIncreaseButtonStyle(Direction direction)=0;
				/// <summary>Callback to create a style controller for the handle button.</summary>
				/// <returns>The created style controller.</returns>
				/// <param name="direction">The direction of the scroll.</param>
				virtual controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)=0;
				/// <summary>Install necessary compositions and elements to the background.</summary>
				/// <param name="boundsComposition">The background composition.</param>
				/// <param name="direction">The direction of the scroll.</param>
				virtual void										InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)=0;
				/// <summary>Build the style. This function is supposed to be called in the contructor of the sub class.</summary>
				/// <param name="defaultSize">The size of the increment button and decrement button.</param>
				/// <param name="arrowSize">The size of the arrows in the increment button and decrement button.</param>
				void												BuildStyle(int defaultSize, int arrowSize);
			public:
				/// <summary>Create the style controller using a specified direction.</summary>
				/// <param name="_direction">The specified direction</param>
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
			
			/// <summary>A general implementation for <see cref="controls::GuiScroll::IStyleController"/> to make a tracker control (or a slide bar).</summary>
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
				
				/// <summary>Callback to create a style controller for the handle button.</summary>
				/// <returns>The created style controller.</returns>
				/// <param name="direction">The direction of the tracker.</param>
				virtual controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)=0;
				/// <summary>Install necessary compositions and elements to the background.</summary>
				/// <param name="boundsComposition">The background composition.</param>
				/// <param name="direction">The direction of the tracker.</param>
				virtual void										InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)=0;
				/// <summary>Install necessary compositions and elements to the tracker groove.</summary>
				/// <param name="boundsComposition">The tracker groove composition.</param>
				/// <param name="direction">The direction of the tracker.</param>
				virtual void										InstallTrack(compositions::GuiGraphicsComposition* trackComposition, Direction direction)=0;
				/// <summary>Build the style. This function is supposed to be called in the contructor of the sub class.</summary>
				/// <param name="trackThickness">The thickness of the tracker troove.</param>
				/// <param name="trackPadding">The padding between the tracker groove to the control.</param>
				/// <param name="handleLong">The size of the long size of the handle button. Horizontal: height; Vertical: width.</param>
				/// <param name="handleShort">The size of the short size of the handle button. Horizontal: width; Vertical: height.</param>
				void												BuildStyle(int trackThickness, int trackPadding, int handleLong, int handleShort);
			public:
				/// <summary>Create the style controller using a specified direction.</summary>
				/// <param name="_direction">The specified direction</param>
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