#include "GuiGraphicsLayoutProviderWindowsGDI.h"
#include "GuiGraphicsUniscribe.h"
#include <math.h>

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_gdi
		{
			using namespace elements;
			using namespace collections;
			using namespace windows;

/***********************************************************************
WindowsGDIParagraph
***********************************************************************/

			class WindowsGDIParagraph : public Object, public IGuiGraphicsParagraph
			{
			protected:
				IGuiGraphicsLayoutProvider*			provider;
				Ptr<UniscribeParagraph>				paragraph;
				WString								text;
				IWindowsGDIRenderTarget*			renderTarget;

			public:
				WindowsGDIParagraph(IGuiGraphicsLayoutProvider* _provider, const WString& _text, IGuiGraphicsRenderTarget* _renderTarget)
					:provider(_provider)
					,text(_text)
					,renderTarget(dynamic_cast<IWindowsGDIRenderTarget*>(_renderTarget))
				{
					paragraph=new UniscribeParagraph;
					paragraph->paragraphText=text;

					Ptr<UniscribeFragment> fragment=new UniscribeFragment;
					fragment->text=_text;
					fragment->fontColor=Color(0, 0, 0);
					fragment->fontStyle=GetCurrentController()->ResourceService()->GetDefaultFont();
					paragraph->documentFragments.Add(fragment);
				}

				~WindowsGDIParagraph()
				{
				}

				IGuiGraphicsLayoutProvider* GetProvider()override
				{
					return provider;
				}

				IGuiGraphicsRenderTarget* GetRenderTarget()override
				{
					return renderTarget;
				}

				bool GetWrapLine()override
				{
					return true;
				}

				void SetWrapLine(bool value)override
				{
				}

				vint GetMaxWidth()override
				{
					return paragraph->lastAvailableWidth;
				}

				void SetMaxWidth(vint value)override
				{
					paragraph->BuildUniscribeData(renderTarget->GetDC());
					paragraph->Layout(value, paragraph->paragraphAlignment);
				}

				Alignment GetParagraphAlignment()override
				{
					return paragraph->paragraphAlignment;
				}

				void SetParagraphAlignment(Alignment value)override
				{
					paragraph->BuildUniscribeData(renderTarget->GetDC());
					paragraph->Layout(paragraph->lastAvailableWidth, value);
				}

				bool SetFont(vint start, vint length, const WString& value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetFont(start, length, value);
					}
					else
					{
						return false;
					}
				}

				bool SetSize(vint start, vint length, vint value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetSize(start, length, value);
					}
					else
					{
						return false;
					}
				}

				bool SetStyle(vint start, vint length, TextStyle value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetStyle(start, length, (value&Bold)!=0, (value&Italic)!=0, (value&Underline)!=0, (value&Strikeline)!=0);
					}
					else
					{
						return false;
					}
				}

				bool SetColor(vint start, vint length, Color value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetColor(start, length, value);
					}
					else
					{
						return false;
					}
				}

				bool SetInlineObject(vint start, vint length, const InlineObjectProperties& properties, Ptr<IGuiGraphicsElement> value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						if(paragraph->SetInlineObject(start, length, properties, value))
						{
							IGuiGraphicsRenderer* renderer=value->GetRenderer();
							if(renderer)
							{
								renderer->SetRenderTarget(renderTarget);
							}
							return true;
						}
					}
					return false;
				}

				bool ResetInlineObject(vint start, vint length)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						if(Ptr<IGuiGraphicsElement> element=paragraph->ResetInlineObject(start, length))
						{
							IGuiGraphicsRenderer* renderer=element->GetRenderer();
							if(renderer)
							{
								renderer->SetRenderTarget(0);
							}
							return true;
						}
					}
					return false;
				}

				bool SetInteractionId(vint start, vint length, vint value)override
				{
					if(length==0) return true;
					if(0<=start && start<text.Length() && length>=0 && 0<=start+length && start+length<=text.Length())
					{
						return paragraph->SetInteractionId(start, length, value);
					}
					else
					{
						return false;
					}
				}

				bool HitTestPoint(Point point, vint& start, vint& length, vint& interactionId)override
				{
					return paragraph->HitTestPoint(point, start, length, interactionId);
				}

				vint GetHeight()override
				{
					paragraph->BuildUniscribeData(renderTarget->GetDC());
					if(paragraph->lastAvailableWidth==-1)
					{
						paragraph->Layout(65536, paragraph->paragraphAlignment);
					}
					return paragraph->bounds.Height();
				}

				void Render(Rect bounds)override
				{
					paragraph->Render(renderTarget->GetDC(), bounds.Left(), bounds.Top());
				}

				vint GetCaret(vint comparingCaret, CaretRelativePosition position, bool preferFrontSide)override
				{
					return paragraph->GetCaret(comparingCaret, position, preferFrontSide);
				}

				Rect GetCaretBounds(vint caret, bool frontSide)override
				{
					return paragraph->GetCaretBounds(caret, frontSide);
				}

				vint GetCaretFromPoint(Point point)override
				{
					return paragraph->GetCaretFromPoint(point);
				}

				vint GetNearestCaretFromTextPos(vint textPos, bool frontSide)override
				{
					return paragraph->GetNearestCaretFromTextPos(textPos, frontSide);
				}

				bool IsValidCaret(vint caret)override
				{
					return paragraph->IsValidCaret(caret);
				}

				bool IsValidTextPos(vint textPos)override
				{
					return paragraph->IsValidTextPos(textPos);
				}
			};

/***********************************************************************
WindowsGDILayoutProvider
***********************************************************************/

			Ptr<IGuiGraphicsParagraph> WindowsGDILayoutProvider::CreateParagraph(const WString& text, IGuiGraphicsRenderTarget* renderTarget)
			{
				return new WindowsGDIParagraph(this, text, renderTarget);
			}
		}
	}
}