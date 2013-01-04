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

			class GuiTextBoxCommonInterface;

			class GuiTextElementOperator : public Object, public Description<GuiTextElementOperator>
			{
			public:
				class ICallback : public virtual IDescriptable, public Description<ICallback>
				{
				public:
					virtual TextPos							GetLeftWord(TextPos pos)=0;
					virtual TextPos							GetRightWord(TextPos pos)=0;
					virtual void							GetWord(TextPos pos, TextPos& begin, TextPos& end)=0;
					virtual vint								GetPageRows()=0;
					virtual bool							BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)=0;
					virtual void							AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
					virtual void							ScrollToView(Point point)=0;
					virtual vint								GetTextMargin()=0;
				};

				class DefaultCallback : public Object, public ICallback, public Description<DefaultCallback>
				{
				protected:
					elements::GuiColorizedTextElement*		textElement;
					compositions::GuiGraphicsComposition*	textComposition;
					bool									readonly;
				public:
					DefaultCallback(elements::GuiColorizedTextElement* _textElement, compositions::GuiGraphicsComposition* _textComposition);
					~DefaultCallback();

					TextPos									GetLeftWord(TextPos pos)override;
					TextPos									GetRightWord(TextPos pos)override;
					void									GetWord(TextPos pos, TextPos& begin, TextPos& end)override;
					vint										GetPageRows()override;
					bool									BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)override;
				};

				class ITextEditCallback : public virtual IDescriptable, public Description<ITextEditCallback>
				{
				public:
					virtual void							Attach(elements::GuiColorizedTextElement* element, SpinLock& elementModifyLock)=0;
					virtual void							Detach()=0;
					virtual void							TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
				};

				class ShortcutCommand
				{
				protected:
					bool									ctrl;
					bool									shift;
					vint										key;
					Func<void()>							action;
				public:
					ShortcutCommand(bool _ctrl, bool _shift, vint _key, const Func<void()> _action);
					~ShortcutCommand();

					bool									IsTheRightKey(bool _ctrl, bool _shift, vint _key);
					void									Execute();
				};

			protected:
				elements::GuiColorizedTextElement*			textElement;
				compositions::GuiGraphicsComposition*		textComposition;
				GuiControl*									textControl;
				GuiTextBoxCommonInterface*					textBoxCommonInterface;
				ICallback*									callback;
				bool										dragging;
				bool										readonly;

				SpinLock									elementModifyLock;
				collections::List<Ptr<ITextEditCallback>>	textEditCallbacks;
				collections::List<Ptr<ShortcutCommand>>		shortcutCommands;

				void										UpdateCaretPoint();
				void										Move(TextPos pos, bool shift);
				void										Modify(TextPos start, TextPos end, const WString& input);
				bool										ProcessKey(vint code, bool shift, bool ctrl);
					
				void										OnGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnCaretNotify(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);

				void										OnLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnLeftButtonUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments);
				void										OnCharInput(compositions::GuiGraphicsComposition* sender, compositions::GuiCharEventArgs& arguments);
			public:
				GuiTextElementOperator();
				~GuiTextElementOperator();

				void										Install(elements::GuiColorizedTextElement* _textElement, compositions::GuiGraphicsComposition* _textComposition, GuiControl* _textControl);
				ICallback*									GetCallback();
				void										SetCallback(ICallback* value);
				bool										AttachTextEditCallback(Ptr<ITextEditCallback> value);
				bool										DetachTextEditCallback(Ptr<ITextEditCallback> value);
				GuiTextBoxCommonInterface*					GetTextBoxCommonInterface();
				void										SetTextBoxCommonInterface(GuiTextBoxCommonInterface* value);
				void										AddShortcutCommand(Ptr<ShortcutCommand> shortcutCommand);

				elements::GuiColorizedTextElement*			GetTextElement();
				compositions::GuiGraphicsComposition*		GetTextComposition();
				TextPos										GetNearestTextPos(Point point);
				void										Select(TextPos begin, TextPos end);
				WString										GetSelectionText();
				void										SetSelectionText(const WString& value);
				void										SetText(const WString& value);

				bool										CanCut();
				bool										CanCopy();
				bool										CanPaste();
				void										SelectAll();
				bool										Cut();
				bool										Copy();
				bool										Paste();

				bool										GetReadonly();
				void										SetReadonly(bool value);
			};
		}
	}
}

#endif