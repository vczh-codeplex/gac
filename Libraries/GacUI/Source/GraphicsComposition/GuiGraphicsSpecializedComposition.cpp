#include "GuiGraphicsSpecializedComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace compositions
		{

/***********************************************************************
GuiSideAlignedComposition
***********************************************************************/

			GuiSideAlignedComposition::GuiSideAlignedComposition()
				:direction(Top)
				,maxLength(10)
				,maxRatio(1.0)
			{
			}

			GuiSideAlignedComposition::~GuiSideAlignedComposition()
			{
			}

			GuiSideAlignedComposition::Direction GuiSideAlignedComposition::GetDirection()
			{
				return direction;
			}

			void GuiSideAlignedComposition::SetDirection(Direction value)
			{
				direction=value;
			}

			int GuiSideAlignedComposition::GetMaxLength()
			{
				return maxLength;
			}

			void GuiSideAlignedComposition::SetMaxLength(int value)
			{
				if(value<0) value=0;
				maxLength=value;
			}

			double GuiSideAlignedComposition::GetMaxRatio()
			{
				return maxRatio;
			}

			void GuiSideAlignedComposition::SetMaxRatio(double value)
			{
				maxRatio=
					value<0?0:
					value>1?1:
					value;
			}

			GuiGraphicsComposition::ParentSizeAffection GuiSideAlignedComposition::GetAffectionFromParent()
			{
				return GuiGraphicsComposition::TotallyDecidedByParent;
			}

			bool GuiSideAlignedComposition::IsSizeAffectParent()
			{
				return false;
			}

			Rect GuiSideAlignedComposition::GetBounds()
			{
				GuiGraphicsComposition* parent=GetParent();
				if(parent)
				{
					Rect bounds=parent->GetBounds();
					int w=(int)(bounds.Width()*maxRatio);
					int h=(int)(bounds.Height()*maxRatio);
					if(w>maxLength) w=maxLength;
					if(h>maxLength) h=maxLength;
					switch(direction)
					{
					case Left:
						{
							bounds.x2=bounds.x1+w;
						}
						break;
					case Top:
						{
							bounds.y2=bounds.y1+h;
						}
						break;
					case Right:
						{
							bounds.x1=bounds.x2-w;
						}
						break;
					case Bottom:
						{
							bounds.y1=bounds.y2-h;
						}
						break;
					}
					return bounds;
				}
				return Rect();
			}

/***********************************************************************
GuiPartialViewComposition
***********************************************************************/

			GuiPartialViewComposition::GuiPartialViewComposition()
				:wRatio(0.0)
				,wPageSize(1.0)
				,hRatio(0.0)
				,hPageSize(1.0)
			{
			}

			GuiPartialViewComposition::~GuiPartialViewComposition()
			{
			}

			double GuiPartialViewComposition::GetWidthRatio()
			{
				return wRatio;
			}

			double GuiPartialViewComposition::GetWidthPageSize()
			{
				return wPageSize;
			}

			double GuiPartialViewComposition::GetHeightRatio()
			{
				return hRatio;
			}

			double GuiPartialViewComposition::GetHeightPageSize()
			{
				return hPageSize;
			}

			void GuiPartialViewComposition::SetWidthRatio(double value)
			{
				wRatio=value;
			}

			void GuiPartialViewComposition::SetWidthPageSize(double value)
			{
				wPageSize=value;
			}

			void GuiPartialViewComposition::SetHeightRatio(double value)
			{
				hRatio=value;
			}

			void GuiPartialViewComposition::SetHeightPageSize(double value)
			{
				hPageSize=value;
			}

			GuiGraphicsComposition::ParentSizeAffection GuiPartialViewComposition::GetAffectionFromParent()
			{
				return GuiGraphicsComposition::TotallyDecidedByParent;
			}

			bool GuiPartialViewComposition::IsSizeAffectParent()
			{
				return false;
			}

			Rect GuiPartialViewComposition::GetBounds()
			{
				GuiGraphicsComposition* parent=GetParent();
				if(parent)
				{
					Rect bounds=parent->GetBounds();
					int w=bounds.Width();
					int h=bounds.Height();
					int pw=(int)(wPageSize*w);
					int ph=(int)(hPageSize*h);

					int ow=preferredMinSize.x-pw;
					if(ow<0) ow=0;
					int oh=preferredMinSize.y-ph;
					if(oh<0) oh=0;

					w-=ow;
					h-=oh;
					pw+=ow;
					ph+=oh;

					return Rect(Point((int)(wRatio*w), (int)(hRatio*h)), Size(pw, ph));
				}
				return Rect();
			}
		}
	}
}
