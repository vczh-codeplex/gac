/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUITEXTCONTROLS

#include "GuiTextCommonInterface.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
MultilineTextBox
***********************************************************************/

			/// <summary>Multiline text box control.</summary>
			class GuiMultilineTextBox : public GuiScrollView, public GuiTextBoxCommonInterface, public Description<GuiMultilineTextBox>
			{
			public:
				static const vint							TextMargin=3;

				class StyleController : public GuiScrollView::StyleController, public Description<StyleController>
				{
				protected:
					elements::GuiColorizedTextElement*		textElement;
					compositions::GuiBoundsComposition*		textComposition;
					GuiMultilineTextBox*					textBox;
					Ptr<GuiTextElementOperator::ICallback>	defaultCallback;

				public:
					StyleController(GuiScrollView::IStyleProvider* styleProvider);
					~StyleController();

					void									Initialize(GuiMultilineTextBox* control);
					elements::GuiColorizedTextElement*		GetTextElement();
					compositions::GuiGraphicsComposition*	GetTextComposition();
					void									SetViewPosition(Point value);
					void									SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;

					WString									GetText();
					void									SetText(const WString& value)override;
					void									SetFont(const FontProperties& value)override;
					void									SetVisuallyEnabled(bool value)override;
				};

				class TextElementOperatorCallback : public GuiTextElementOperator::DefaultCallback, public Description<TextElementOperatorCallback>
				{
				protected:
					GuiMultilineTextBox*					textControl;
					StyleController*						textController;
				public:
					TextElementOperatorCallback(GuiMultilineTextBox* _textControl);

					void									AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
					void									ScrollToView(Point point)override;
					vint									GetTextMargin()override;
				};

			protected:
				StyleController*							styleController;

				void										CalculateViewAndSetScroll();
				void										OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)override;
				Size										QueryFullSize()override;
				void										UpdateView(Rect viewBounds)override;
				void										OnBoundsMouseButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				GuiMultilineTextBox(GuiMultilineTextBox::IStyleProvider* styleProvider);
				~GuiMultilineTextBox();

				const WString&								GetText()override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
			};

/***********************************************************************
SinglelineTextBox
***********************************************************************/
			
			/// <summary>Single text box control.</summary>
			class GuiSinglelineTextBox : public GuiControl, public GuiTextBoxCommonInterface, public Description<GuiSinglelineTextBox>
			{
			public:
				static const vint							TextMargin=3;

				class IStyleProvider : public virtual GuiControl::IStyleProvider, public Description<IStyleProvider>
				{
				public:
					virtual compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* background)=0;
				};

				class StyleController : public Object, public GuiControl::IStyleController, public Description<StyleController>
				{
				protected:
					Ptr<IStyleProvider>						styleProvider;
					compositions::GuiBoundsComposition*		boundsComposition;
					compositions::GuiGraphicsComposition*	containerComposition;

					GuiSinglelineTextBox*					textBox;
					elements::GuiColorizedTextElement*		textElement;
					compositions::GuiTableComposition*		textCompositionTable;
					compositions::GuiCellComposition*		textComposition;
					Ptr<GuiTextElementOperator::ICallback>	defaultCallback;

				public:
					StyleController(IStyleProvider* _styleProvider);
					~StyleController();

					void									SetTextBox(GuiSinglelineTextBox* control);
					void									RearrangeTextElement();
					compositions::GuiBoundsComposition*		GetBoundsComposition();
					compositions::GuiGraphicsComposition*	GetContainerComposition();
					void									SetFocusableComposition(compositions::GuiGraphicsComposition* value);

					WString									GetText();
					void									SetText(const WString& value);
					void									SetFont(const FontProperties& value);
					void									SetVisuallyEnabled(bool value);

					elements::GuiColorizedTextElement*		GetTextElement();
					compositions::GuiGraphicsComposition*	GetTextComposition();
					void									SetViewPosition(Point value);
				};

				class TextElementOperatorCallback : public GuiTextElementOperator::DefaultCallback, public Description<TextElementOperatorCallback>
				{
				protected:
					GuiSinglelineTextBox*					textControl;
					StyleController*						textController;
				public:
					TextElementOperatorCallback(GuiSinglelineTextBox* _textControl);

					bool									BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)override;
					void									AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
					void									ScrollToView(Point point)override;
					vint									GetTextMargin()override;
				};
			protected:
				StyleController*							styleController;
				
				void										OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)override;
				void										OnBoundsMouseButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				GuiSinglelineTextBox(GuiSinglelineTextBox::IStyleProvider* styleProvider);
				~GuiSinglelineTextBox();

				const WString&								GetText()override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				/// <summary>
				/// Get the password mode displaying character.
				/// </summary>
				/// <returns>The password mode displaying character. Returns L'\0' means the password mode is not activated.</returns>
				wchar_t										GetPasswordChar();
				/// <summary>
				/// Set the password mode displaying character.
				/// </summary>
				/// <param name="value">The password mode displaying character. Set to L'\0' to deactivate the password mode.</param>
				void										SetPasswordChar(wchar_t value);
			};
		}
	}
}

#endif