#include "MoeDocumentEditor.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
MoeEditorColorizer
***********************************************************************/
		
		class MoeEditorColorizer : public GuiTextBoxRegexColorizer
		{
		private:
			unsigned char Unit(wchar_t c)
			{
				return c>=L'A'?10+c-L'A':c-L'0';
			}
			Color Convert(const wchar_t* color)
			{
				return Color(
					(Unit(color[0])<<4)+Unit(color[1]),
					(Unit(color[2])<<4)+Unit(color[3]),
					(Unit(color[4])<<4)+Unit(color[5])
					);
			}
		public:
			MoeEditorColorizer()
			{
				text::ColorEntry entry;
				entry.normal.background=Convert(L"FFFFFF");
				entry.normal.text=Convert(L"4D4D4C");
				entry.selectedFocused.background=Convert(L"D6D6D6");
				entry.selectedFocused.text=entry.normal.text;
				entry.selectedUnfocused=entry.selectedFocused;
				SetDefaultColor(entry);

				// 0 == constant.numeric.moescript
				entry.normal.text=entry.selectedFocused.text=entry.selectedUnfocused.text=Convert(L"F5871F");
				AddToken(L"0x[a-fA-F0-9]+|/d+(./d+)?([eE]-?/d+)?", entry);

				// 1, 2, 3 == punctuation.definition.string
				entry.normal.text=entry.selectedFocused.text=entry.selectedUnfocused.text=Convert(L"C82829");
				AddToken(L"('[^']*')+", entry);
				AddToken(L"\"([^/\\\"]|/\\/S|/\\/s+/\\)*\"", entry);
				AddToken(L"`([^/\\`]|/\\/.|/[([^/\\/[/]]|/\\/.)*/])*`", entry);

				// 4 == keyword.control.moescript
				entry.normal.text=entry.selectedFocused.text=entry.selectedUnfocused.text=Convert(L"8959A8");
				AddToken(L"break|else|for|if|return|repeat|while|until|when|where|try|catch|piecewise|case|otherwise", entry);

				// 5 == keyword.control.declare.moescript
				entry.normal.text=entry.selectedFocused.text=entry.selectedUnfocused.text=Convert(L"8959A8");
				AddToken(L"function|def|var", entry);
				
				// 6 == constant.language.moescript
				entry.normal.text=entry.selectedFocused.text=entry.selectedUnfocused.text=Convert(L"F5871F");
				AddToken(L"do|resend|new|wait|throw", entry);
				
				// 7 == variable.language.semiargs.moescript
				entry.normal.text=entry.selectedFocused.text=entry.selectedUnfocused.text=Convert(L"C82829");
				AddToken(L"this|arguments", entry);
				
				// 8, 9 == keyword.operator.moescript
				entry.normal.text=entry.selectedFocused.text=entry.selectedUnfocused.text=Convert(L"3E999F");
				AddToken(L"and|or|is|as|not|negate", entry);
				AddToken(L"/+|-|%|#|/*|//|/^|/=/=?|~/=?|/</=?|/>/=?|...?|&&|/|/|", entry);

				// 10 = punctuation.definition.comment.moescript
				entry.normal.text=entry.selectedFocused.text=entry.selectedUnfocused.text=Convert(L"8E908C");
				AddToken(L"(--|////)[^/r/n]*", entry);

				// 11 = identifier
				entry.normal.text=entry.selectedFocused.text=entry.selectedUnfocused.text=Convert(L"4D4D4C");
				AddToken(L"[a-zA-Z_]/w*", entry);

				Setup();
			}

			void ColorizeTokenContextSensitive(int lineIndex, const wchar_t* text, vint start, vint length, vint& token, int& contextState)override
			{
				// 0 < 1 name 2 att > 0
				switch(token)
				{
				case 3:
					if(length==1)
					{
						switch(text[start])
						{
						case '<':
							contextState=1;
							break;
						case '>':
							contextState=0;
							break;
						}
					}
					break;
				case 4:
					switch(contextState)
					{
					case 0:
						token=-1;
						break;
					case 1:
						token=5;
						contextState=2;
						break;
					}
					break;
				}
			}

			int GetContextStartState()override
			{
				return 0;
			}
		};

/***********************************************************************
MoeTextDocumentEditor
***********************************************************************/

		GuiControl* MoeTextDocumentEditor::CreateEditorControlInternal()
		{
			GuiControl* control=TextDocumentEditor::CreateEditorControlInternal();
			textBox->SetColorizer(new MoeEditorColorizer);
			return control;
		}

		MoeTextDocumentEditor::MoeTextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView)
			:TextDocumentEditor(_editorFactory, _editingView)
		{
		}

		MoeTextDocumentEditor::~MoeTextDocumentEditor()
		{
		}

/***********************************************************************
MoeTextDocumentEditorFactory
***********************************************************************/

		const wchar_t* MoeTextDocumentEditorFactory::EditorTypeId = L"vl::gactools::MoeTextDocumentEditorFactory";

		Ptr<IDocumentEditor> MoeTextDocumentEditorFactory::CreateEditorInternal(IDocumentView* view)
		{
			return new MoeTextDocumentEditor(this, view);
		}

		MoeTextDocumentEditorFactory::MoeTextDocumentEditorFactory()
			:DocumentEditorFactory(EditorTypeId, L"Moe Text Editor")
		{
			AddSupportedViewTypeId(MoeTextView::ViewTypeId);
		}

		MoeTextDocumentEditorFactory::~MoeTextDocumentEditorFactory()
		{
		}
	}
}