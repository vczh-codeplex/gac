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

			class IGuiGraphicsLayoutProvider;

			/// <summary>Represents a paragraph of a layouted rich text content.</summary>
			class IGuiGraphicsParagraph : public Interface
			{
			public:
				/// <summary>Get the <see cref="IGuiGraphicsLayoutProvider"/> object that created this paragraph.</summary>
				/// <returns>The layout provider object.</returns>
				virtual IGuiGraphicsLayoutProvider*			GetProvider()=0;
				/// <summary>Get if line auto-wrapping is enabled for this paragraph.</summary>
				/// <returns>Return true if line auto-wrapping is enabled for this paragraph.</returns>
				virtual bool								GetWrapLine()=0;
				/// <summary>Set if line auto-wrapping is enabled for this paragraph.</summary>
				/// <param name="value">True if line auto-wrapping is enabled for this paragraph.</param>
				virtual void								SetWrapLine(bool value)=0;
				/// <summary>Get the text for this paragraph.</summary>
				/// <returns>The text for this paragraph.</returns>
				virtual const WString&						GetParagraphText()=0;
				/// <summary>Set the text for this paragraph using the default font comes from [M:vl.presentation.INativeResourceService.GetDefaultFont].</summary>
				/// <param name="value">The new text.</param>
				virtual void								SetParagraphText(const WString& value)=0;
				/// <summary>Get the max width for this paragraph. If there is no max width limitation, it returns -1.</summary>
				/// <returns>The max width for this paragraph.</returns>
				virtual int									GetMaxWidth()=0;
				/// <summary>Set the max width for this paragraph. If the max width is set to -1, the max width limitation will be removed.</summary>
				/// <param name="value">The max width.</param>
				virtual void								SetMaxWidth(int value)=0;

				/// <summary>Replace the text within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The text</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetText(int start, int length, const WString& value)=0;
				/// <summary>Replace the font within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The font</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetFont(int start, int length, const WString& value)=0;
				/// <summary>Replace the size within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The size</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetSize(int start, int length, int size)=0;
				/// <summary>Replace the bold style within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The bold style</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetBold(int start, int length, bool value)=0;
				/// <summary>Replace the italic style within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The italic style</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetItalic(int start, int length, bool value)=0;
				/// <summary>Replace the underline style within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The underline style</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetUnderline(int start, int length, bool value)=0;
				/// <summary>Replace the strikeline style within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The strikeline style</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetStrikeline(int start, int length, bool value)=0;
				/// <summary>Replace the color within the specified range.</summary>
				/// <param name="start">The position of the first character of the specified range.</param>
				/// <param name="length">The length of the specified range by character.</param>
				/// <param name="value">The color</param>
				/// <returns>Returns true if this operation succeeded.</returns>
				virtual bool								SetColor(int start, int length, Color color)=0;

				/// <summary>Get the layouted height of the text. The result depends on rich styled text and the two important properties that can be set using <see cref="SetWrapLine"/> and <see cref="SetMaxWidth"/>.</summary>
				/// <returns>The layouted height.</returns>
				virtual int									GetHeight()=0;
				/// <summary>Set a <see cref="IGuiGraphicsRenderTarget"></see> to this element.</summary>
				/// <param name="renderTarget">The graphics render target. It can be NULL.</param>
				virtual void								SetRenderTarget(IGuiGraphicsRenderTarget* renderTarget)=0;
				/// <summary>Render the graphics element using a specified bounds.</summary>
				/// <param name="bounds">Bounds to decide the size and position of the binded graphics element.</param>
				virtual void								Render(Rect bounds)=0;
			};

			/// <summary>Renderer awared rich text document layout engine provider interface.</summary>
			class IGuiGraphicsLayoutProvider : public Interface
			{
			public:
				/// <summary>Create a paragraph with internal renderer device dependent objects initialized.</summary>
				virtual IGuiGraphicsParagraph*				CreateParagraph()=0;
			};
		}
	}
}

#endif