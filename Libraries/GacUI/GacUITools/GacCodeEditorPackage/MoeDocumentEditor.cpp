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
		public:
			MoeEditorColorizer()
			{
				text::ColorEntry entry=GetCurrentTheme()->GetDefaultTextBoxColorEntry();
				SetDefaultColor(entry);

				// 0 == constant.numeric.moescript
				AddToken(L"0x[a-fA-F0-9]+|/d+(./d+)?([eE]-?/d+)?", entry);

				// 1, 2, 3 == punctuation.definition.string
				AddToken(L"('[^']*')+", entry);
				AddToken(L"\"([^/\\\"]|/\\/S|/\\/s+/\\)*\"", entry);
				AddToken(L"`([^/\\`]|/\\/.)*`", entry);

				// 4 == keyword.control.moescript
				AddToken(L"break|else|for|if|return|repeat|while|until|when|where|try|catch|piecewise|case|otherwise", entry);

				// 5 == keyword.control.declare.moescript
				AddToken(L"function|def|var", entry);
				
				// 6 == constant.language.moescript
				AddToken(L"do|resend|new|wait|throw", entry);
				
				// 7 == variable.language.semiargs.moescript
				AddToken(L"this|arguments", entry);
				
				// 8, 9 == keyword.operator.moescript
				AddToken(L"and|or|is|as|not|negate", entry);
				AddToken(L"/+|-|%|#|/*|//|/^|/=/=?|~/=?|/</=?|/>/=?|...?|&&|/|/|", entry);

				// 10 = identifier
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