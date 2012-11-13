/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Element System and Infrastructure Interfaces

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSDOCUMENTINTERFACES
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSDOCUMENTINTERFACES

#include "GuiGraphicsElementInterfaces.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

		namespace elements
		{

/***********************************************************************
Layout Engine
***********************************************************************/

			class IGuiGraphicsParagraph;
			class IGuiGraphicsLayoutProvider;

			/// <summary>Represents a paragraph of a layouted rich text content.</summary>
			class IGuiGraphicsParagraph : public Interface
			{
			public:
				/// <summary>Text style. Items in this enumeration type can be combined.</summary>
				enum TextStyle
				{
					/// <summary>[T:vl.presentation.elements.IGuiGraphicsParagraph.TextStyle]Bold.</summary>
					Bold=1,
					/// <summary>[T:vl.presentation.elements.IGuiGraphicsParagraph.TextStyle]Italic.</summary>
					Italic=2,
					/// <summary>[T:vl.presentation.elements.IGuiGraphicsParagraph.TextStyle]Underline.</summary>
					Underline=4,
					/// <summary>[T:vl.presentation.elements.IGuiGraphicsParagraph.TextStyle]Strikeline.</summary>
					Strikeline=8,
				};

				/// <summary>Inline object break condition.</summary>
				enum BreakCondition
				{
					/// <summary>[T:vl.presentation.elements.IGuiGraphicsParagraph.BreakCondition]Stay together with the previous run if possible.</summary>
					StickToPreviousRun,
					/// <summary>[T:vl.presentation.elements.IGuiGraphicsParagraph.BreakCondition]Stay together with the next run if possible.</summary>
					StickToNextRun,
					/// <summary>[T:vl.presentation.elements.IGuiGraphicsParagraph.BreakCondition]Treat as a single run.</summary>
					Alone,
				};

				/// <summary>Inline object properties.</summary>
				struct InlineObjectProperties
				{
					/// <summary>The size of the inline object.</summary>
					Size					size;
					/// <summary>The baseline of the inline object.If the baseline is at the bottom, then set the baseline to -1.</summary>
					int						baseline;
					/// <summary>The margin of the inline object.</summary>
					Margin					margin;
					/// <summary>The break condition of the inline object.</summary>
					BreakCondition			breakCondition;

					InlineObjectProperties()
						:baseline(-1)
					{
					}
				};

				/// <summary>Get the <see cref="IGuiGraphicsLayoutProvider"/> object that created this paragraph.</summary>
				/// <returns>The layout provider object.</returns>
				virtual IGuiGraphicsLayoutProvider*			GetProvider()=0;
				/// <summary>Get the associated <see cref="IGuiGraphicsRenderTarget"/> to this paragraph.</summary>
				/// <returns>The associated render target.</returns>
				virtual IGuiGraphicsRenderTarget*			GetRenderTarget()=0;
				/// <summary>Get if line auto-wrapping is enabled for this paragraph.</summary>
				/// <returns>Return true if line auto-wrapping is enabled for this paragraph.</returns>
				virtual bool								GetWrapLine()=0;
				/// <summary>Set if line auto-wrapping is enabled for this paragraph.</summary>
				/// <param name="value">True if line auto-wrapping is enabled for this paragraph.</param>
				virtual void								SetWrapLine(bool value)=0;
				/// <summary>Get the max width for this paragraph. If there is no max width limitation, it returns -1.</summary>
				/// <returns>The max width for this paragraph.</returns>
				virtual int									GetMaxWidth()=0;
				/// <summary>Set the max width for this paragraph. If the max width is set to -1, the max width limitation will be removed.</summary>
				/// <param name="value">The max width.</param>
				virtual void								SetMaxWidth(int value)=0;

				/// <summary>Replace the font within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The font.</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetFont(int start, int length, const WString& value)=0;
				/// <summary>Replace the size within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The size.</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetSize(int start, int length, int value)=0;
				/// <summary>Replace the text style within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The text style.</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetStyle(int start, int length, TextStyle value)=0;
				/// <summary>Replace the color within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The color.</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetColor(int start, int length, Color value)=0;
				/// <summary>Bind an <see cref="IGuiGraphicsElement"/> to a range of text.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="properties">The properties for the inline object.</param>
				/// <param name="value">The element.</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetInlineObject(int start, int length, const InlineObjectProperties& properties, Ptr<IGuiGraphicsElement> value)=0;
				/// <summary>Unbind all inline objects to a range of text.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								ResetInlineObject(int start, int length)=0;

				/// <summary>Get the layouted height of the text. The result depends on rich styled text and the two important properties that can be set using <see cref="SetWrapLine"/> and <see cref="SetMaxWidth"/>.</summary>
				/// <returns>The layouted height.</returns>
				virtual int									GetHeight()=0;
				/// <summary>Render the graphics element using a specified bounds.</summary>
				/// <param name="bounds">Bounds to decide the size and position of the binded graphics element.</param>
				virtual void								Render(Rect bounds)=0;
			};

			/// <summary>Renderer awared rich text document layout engine provider interface.</summary>
			class IGuiGraphicsLayoutProvider : public Interface
			{
			public:
				/// <summary>Create a paragraph with internal renderer device dependent objects initialized.</summary>
				/// <param name="text">The text used to fill the paragraph.</param>
				/// <param name="renderTarget">The render target that the created paragraph will render to.</param>
				/// <returns>The created paragraph object.</returns>
				virtual Ptr<IGuiGraphicsParagraph>			CreateParagraph(const WString& text, IGuiGraphicsRenderTarget* renderTarget)=0;
			};
		}
	}
}

#endif