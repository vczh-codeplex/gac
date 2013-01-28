#include "XmlDocumentEditor.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
XmlEditorColorizer
***********************************************************************/
		
		class XmlEditorColorizer : public GuiTextBoxRegexColorizer
		{
		public:
			XmlEditorColorizer()
			{
				text::ColorEntry entry=GetCurrentTheme()->GetDefaultTextBoxColorEntry();
				SetDefaultColor(entry);

				entry.normal.text=Color(0, 128, 0);
				AddToken(L"/<!--([^/-]|-[^/-]|--[^>])*--/>", entry);

				entry.normal.text=Color(128, 0, 255);
				AddToken(L"/<!/[CDATA/[([^/]]|/][^/]]|/]/][^>])*/]/]/>", entry);

				entry.normal.text=Color(0, 0, 0);
				AddToken(L"\"[^\"]*\"", entry);

				entry.normal.text=Color(0, 0, 255);
				AddToken(L"[<>=]", entry);

				entry.normal.text=Color(255, 0, 0);
				AddToken(L"[a-zA-Z0-9_/-:]+", entry);

				entry.normal.text=Color(163, 21, 21);
				AddExtraToken(entry);

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
XmlTextDocumentEditor
***********************************************************************/

		GuiControl* XmlTextDocumentEditor::CreateEditorControlInternal()
		{
			GuiControl* control=TextDocumentEditor::CreateEditorControlInternal();
			textBox->SetColorizer(new XmlEditorColorizer);
			return control;
		}

		XmlTextDocumentEditor::XmlTextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView)
			:TextDocumentEditor(_editorFactory, _editingView)
		{
		}

		XmlTextDocumentEditor::~XmlTextDocumentEditor()
		{
		}

/***********************************************************************
XmlTextDocumentEditorFactory
***********************************************************************/

		const wchar_t* XmlTextDocumentEditorFactory::EditorTypeId = L"vl::gactools::XmlTextDocumentEditorFactory";

		Ptr<IDocumentEditor> XmlTextDocumentEditorFactory::CreateEditorInternal(IDocumentView* view)
		{
			return new XmlTextDocumentEditor(this, view);
		}

		XmlTextDocumentEditorFactory::XmlTextDocumentEditorFactory()
			:DocumentEditorFactory(EditorTypeId, L"Xml Text Editor")
		{
			AddSupportedViewTypeId(XmlTextView::ViewTypeId);
		}

		XmlTextDocumentEditorFactory::~XmlTextDocumentEditorFactory()
		{
		}
	}
}