/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIDOCUMENTVIEWER
#define VCZH_PRESENTATION_CONTROLS_GUIDOCUMENTVIEWER

#include "..\GuiContainerControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiDocumentViewer
***********************************************************************/
			
			/// <summary>Document viewer for displaying <see cref="DocumentModel"/>.</summary>
			class GuiDocumentViewer : public GuiScrollContainer, public Description<GuiDocumentViewer>
			{
			protected:
				elements::GuiDocumentElement*				documentElement;

			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				GuiDocumentViewer(GuiDocumentViewer::IStyleProvider* styleProvider);
				~GuiDocumentViewer();
				
				/// <summary>Get the document.</summary>
				/// <returns>The document.</returns>
				Ptr<DocumentModel>							GetDocument();
				/// <summary>Set the document. When a document is set to this element, modifying the document without invoking <see cref="NotifyParagraphUpdated"/> will lead to undefined behavior.</summary>
				/// <param name="value">The document.</param>
				void										SetDocument(Ptr<DocumentModel> value);
				/// <summary>Notify that a specified paragraph is updated.</summary>
				/// <param name="index">The paragraph index.</param>
				void										NotifyParagraphUpdated(vint index);
			};
		}
	}
}

#endif