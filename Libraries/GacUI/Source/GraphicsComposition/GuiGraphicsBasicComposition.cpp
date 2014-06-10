#include "GuiGraphicsBasicComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace compositions
		{
			using namespace collections;
			using namespace controls;
			using namespace elements;

/***********************************************************************
GuiWindowComposition
***********************************************************************/

			GuiWindowComposition::GuiWindowComposition()
				:attachedWindow(0)
			{
			}

			GuiWindowComposition::~GuiWindowComposition()
			{
			}

			INativeWindow* GuiWindowComposition::GetAttachedWindow()
			{
				return attachedWindow;
			}

			void GuiWindowComposition::SetAttachedWindow(INativeWindow* window)
			{
				attachedWindow=window;
				SetRenderTarget(attachedWindow?GetGuiGraphicsResourceManager()->GetRenderTarget(attachedWindow):0);
			}

			Rect GuiWindowComposition::GetBounds()
			{
				return attachedWindow?Rect(Point(0, 0), attachedWindow->GetClientSize()):Rect();
			}

			void GuiWindowComposition::SetMargin(Margin value)
			{
			}

/***********************************************************************
GuiBoundsComposition
***********************************************************************/

			GuiBoundsComposition::GuiBoundsComposition()
			{
				ClearAlignmentToParent();
			}

			GuiBoundsComposition::~GuiBoundsComposition()
			{
			}

			Rect GuiBoundsComposition::GetPreferredBounds()
			{
				Rect result=GetBoundsInternal(compositionBounds);
				if(GetParent() && IsAlignedToParent())
				{
					if(alignmentToParent.left>=0)
					{
						vint offset=alignmentToParent.left-result.x1;
						result.x1+=offset;
						result.x2+=offset;
					}
					if(alignmentToParent.top>=0)
					{
						vint offset=alignmentToParent.top-result.y1;
						result.y1+=offset;
						result.y2+=offset;
					}
					if(alignmentToParent.right>=0)
					{
						result.x2+=alignmentToParent.right;
					}
					if(alignmentToParent.bottom>=0)
					{
						result.y2+=alignmentToParent.bottom;
					}
				}
				return result;
			}

			Rect GuiBoundsComposition::GetBounds()
			{
				Rect result=GetPreferredBounds();
				if(GetParent() && IsAlignedToParent())
				{
					Size clientSize=GetParent()->GetClientArea().GetSize();
					if(alignmentToParent.left>=0 && alignmentToParent.right>=0)
					{
						result.x1=alignmentToParent.left;
						result.x2=clientSize.x-alignmentToParent.right;
					}
					else if(alignmentToParent.left>=0)
					{
						vint width=result.Width();
						result.x1=alignmentToParent.left;
						result.x2=result.x1+width;
					}
					else if(alignmentToParent.right>=0)
					{
						vint width=result.Width();
						result.x2=clientSize.x-alignmentToParent.right;
						result.x1=result.x2-width;
					}

					if(alignmentToParent.top>=0 && alignmentToParent.bottom>=0)
					{
						result.y1=alignmentToParent.top;
						result.y2=clientSize.y-alignmentToParent.bottom;
					}
					else if(alignmentToParent.top>=0)
					{
						vint height=result.Height();
						result.y1=alignmentToParent.top;
						result.y2=result.y1+height;
					}
					else if(alignmentToParent.bottom>=0)
					{
						vint height=result.Height();
						result.y2=clientSize.y-alignmentToParent.bottom;
						result.y1=result.y2-height;
					}
				}
				UpdatePreviousBounds(result);
				return result;
			}

			void GuiBoundsComposition::SetBounds(Rect value)
			{
				compositionBounds=value;
			}

			void GuiBoundsComposition::ClearAlignmentToParent()
			{
				alignmentToParent=Margin(-1, -1, -1, -1);
			}

			Margin GuiBoundsComposition::GetAlignmentToParent()
			{
				return alignmentToParent;
			}

			void GuiBoundsComposition::SetAlignmentToParent(Margin value)
			{
				alignmentToParent=value;
			}

			bool GuiBoundsComposition::IsAlignedToParent()
			{
				return alignmentToParent!=Margin(-1, -1, -1, -1);
			}
		}
	}
}