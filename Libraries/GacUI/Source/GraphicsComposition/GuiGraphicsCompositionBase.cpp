#include "GuiGraphicsCompositionBase.h"
#include "..\Controls\GuiWindowControls.h"

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
GuiGraphicsComposition
***********************************************************************/

			void GuiGraphicsComposition::OnControlParentChanged(controls::GuiControl* control)
			{
				if(associatedControl && associatedControl!=control)
				{
					if(associatedControl->GetParent())
					{
						associatedControl->GetParent()->OnChildRemoved(associatedControl);
					}
					if(control)
					{
						control->OnChildInserted(associatedControl);
					}
				}
				else
				{
					for(int i=0;i<children.Count();i++)
					{
						children[i]->OnControlParentChanged(control);
					}
				}
			}

			void GuiGraphicsComposition::OnChildInserted(GuiGraphicsComposition* child)
			{
				child->OnControlParentChanged(GetRelatedControl());
			}

			void GuiGraphicsComposition::OnChildRemoved(GuiGraphicsComposition* child)
			{
				child->OnControlParentChanged(0);
			}

			void GuiGraphicsComposition::OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent)
			{
			}

			void GuiGraphicsComposition::OnRenderTargetChanged()
			{
				if(associatedControl)
				{
					associatedControl->OnRenderTargetChanged(renderTarget);
				}
			}

			GuiGraphicsComposition::GuiGraphicsComposition()
				:parent(0)
				,visible(true)
				,minSizeLimitation(NoLimit)
				,renderTarget(0)
				,associatedControl(0)
				,associatedHost(0)
				,associatedCursor(0)
				,associatedHitTestResult(INativeWindowListener::NoDecision)
			{
			}

			GuiGraphicsComposition::~GuiGraphicsComposition()
			{
				for(int i=0;i<children.Count();i++)
				{
					delete children[i];
				}
			}

			GuiGraphicsComposition* GuiGraphicsComposition::GetParent()
			{
				return parent;
			}

			const GuiGraphicsComposition::ICompositionList& GuiGraphicsComposition::Children()
			{
				return children.Wrap();
			}

			bool GuiGraphicsComposition::AddChild(GuiGraphicsComposition* child)
			{
				return InsertChild(children.Count(), child);
			}

			bool GuiGraphicsComposition::InsertChild(int index, GuiGraphicsComposition* child)
			{
				if(!child) return false;
				if(child->GetParent()) return false;
				children.Insert(index, child);
				child->parent=this;
				child->SetRenderTarget(renderTarget);
				OnChildInserted(child);
				child->OnParentChanged(0, child->parent);
				return true;
			}

			bool GuiGraphicsComposition::RemoveChild(GuiGraphicsComposition* child)
			{
				if(!child) return false;
				int index=children.IndexOf(child);
				if(index==-1) return false;
				child->OnParentChanged(child->parent, 0);
				OnChildRemoved(child);
				child->SetRenderTarget(0);
				child->parent=0;
				GuiGraphicsHost* host=GetRelatedGraphicsHost();
				if(host)
				{
					host->DisconnectComposition(child);
				}
				children.RemoveAt(index);
				return true;
			}

			bool GuiGraphicsComposition::MoveChild(GuiGraphicsComposition* child, int newIndex)
			{
				if(!child) return false;
				int index=children.IndexOf(child);
				if(index==-1) return false;
				children.RemoveAt(index);
				children.Insert(newIndex, child);
				return true;
			}

			Ptr<IGuiGraphicsElement> GuiGraphicsComposition::GetOwnedElement()
			{
				return ownedElement;
			}

			void GuiGraphicsComposition::SetOwnedElement(Ptr<IGuiGraphicsElement> element)
			{
				if(ownedElement)
				{
					ownedElement->GetRenderer()->SetRenderTarget(0);
				}
				ownedElement=element;
				if(ownedElement)
				{
					ownedElement->GetRenderer()->SetRenderTarget(renderTarget);
				}
			}

			bool GuiGraphicsComposition::GetVisible()
			{
				return visible;
			}

			void GuiGraphicsComposition::SetVisible(bool value)
			{
				visible=value;
			}

			GuiGraphicsComposition::MinSizeLimitation GuiGraphicsComposition::GetMinSizeLimitation()
			{
				return minSizeLimitation;
			}

			void GuiGraphicsComposition::SetMinSizeLimitation(MinSizeLimitation value)
			{
				minSizeLimitation=value;
			}

			IGuiGraphicsRenderTarget* GuiGraphicsComposition::GetRenderTarget()
			{
				return renderTarget;
			}

			void GuiGraphicsComposition::SetRenderTarget(IGuiGraphicsRenderTarget* value)
			{
				renderTarget=value;
				if(ownedElement)
				{
					ownedElement->GetRenderer()->SetRenderTarget(renderTarget);
				}
				for(int i=0;i<children.Count();i++)
				{
					children[i]->SetRenderTarget(renderTarget);
				}
				OnRenderTargetChanged();
			}

			void GuiGraphicsComposition::Render(Size offset)
			{
				if(visible && renderTarget && !renderTarget->IsClipperCoverWholeTarget())
				{
					Rect bounds=GetBounds();
					bounds.x1+=margin.left;
					bounds.y1+=margin.top;
					bounds.x2-=margin.right;
					bounds.y2-=margin.bottom;

					if(bounds.x1<=bounds.x2 && bounds.y1<=bounds.y2)
					{
						bounds.x1+=offset.x;
						bounds.x2+=offset.x;
						bounds.y1+=offset.y;
						bounds.y2+=offset.y;

						if(ownedElement)
						{
							ownedElement->GetRenderer()->Render(bounds);
						}
						if(children.Count()>0)
						{
							bounds.x1+=internalMargin.left;
							bounds.y1+=internalMargin.top;
							bounds.x2-=internalMargin.right;
							bounds.y2-=internalMargin.bottom;
							if(bounds.x1<=bounds.x2 && bounds.y1<=bounds.y2)
							{
								offset=bounds.GetSize();
								renderTarget->PushClipper(bounds);
								if(!renderTarget->IsClipperCoverWholeTarget())
								{
									for(int i=0;i<children.Count();i++)
									{
										children[i]->Render(Size(bounds.x1, bounds.y1));
									}
								}
								renderTarget->PopClipper();
							}
						}
					}
				}
			}

			GuiGraphicsEventReceiver* GuiGraphicsComposition::GetEventReceiver()
			{
				if(!eventReceiver)
				{
					eventReceiver=new GuiGraphicsEventReceiver(this);
				}
				return eventReceiver.Obj();
			}

			bool GuiGraphicsComposition::HasEventReceiver()
			{
				return eventReceiver;
			}

			GuiGraphicsComposition* GuiGraphicsComposition::FindComposition(Point location)
			{
				if(!visible) return 0;
				Rect bounds=GetBounds();
				Rect relativeBounds=Rect(Point(0, 0), bounds.GetSize());
				if(relativeBounds.Contains(location))
				{
					Rect clientArea=GetClientArea();
					for(int i=children.Count()-1;i>=0;i--)
					{
						GuiGraphicsComposition* child=children[i];
						Rect childBounds=child->GetBounds();
						int offsetX=childBounds.x1+(clientArea.x1-bounds.x1);
						int offsetY=childBounds.y1+(clientArea.y1-bounds.y1);
						Point newLocation=location-Size(offsetX, offsetY);
						GuiGraphicsComposition* childResult=child->FindComposition(newLocation);
						if(childResult)
						{
							return childResult;
						}
					}
					return this;
				}
				else
				{
					return 0;
				}
			}

			Rect GuiGraphicsComposition::GetGlobalBounds()
			{
				Rect bounds=GetBounds();
				GuiGraphicsComposition* composition=parent;
				while(composition)
				{
					Rect clientArea=composition->GetClientArea();
					Rect parentBounds=composition->GetBounds();
					bounds.x1+=clientArea.x1;
					bounds.x2+=clientArea.x1;
					bounds.y1+=clientArea.y1;
					bounds.y2+=clientArea.y1;
					composition=composition->parent;
				}
				return bounds;
			}

			controls::GuiControl* GuiGraphicsComposition::GetAssociatedControl()
			{
				return associatedControl;
			}

			void GuiGraphicsComposition::SetAssociatedControl(controls::GuiControl* control)
			{
				if(associatedControl)
				{
					for(int i=0;i<children.Count();i++)
					{
						children[i]->OnControlParentChanged(0);
					}
				}
				associatedControl=control;
				if(associatedControl)
				{
					for(int i=0;i<children.Count();i++)
					{
						children[i]->OnControlParentChanged(associatedControl);
					}
				}
			}

			GuiGraphicsHost* GuiGraphicsComposition::GetAssociatedHost()
			{
				return associatedHost;
			}

			void GuiGraphicsComposition::SetAssociatedHost(GuiGraphicsHost* host)
			{
				associatedHost=host;
			}

			INativeCursor* GuiGraphicsComposition::GetAssociatedCursor()
			{
				return associatedCursor;
			}

			void GuiGraphicsComposition::SetAssociatedCursor(INativeCursor* cursor)
			{
				associatedCursor=cursor;
			}

			INativeWindowListener::HitTestResult GuiGraphicsComposition::GetAssociatedHitTestResult()
			{
				return associatedHitTestResult;
			}

			void GuiGraphicsComposition::SetAssociatedHitTestResult(INativeWindowListener::HitTestResult value)
			{
				associatedHitTestResult=value;
			}

			controls::GuiControl* GuiGraphicsComposition::GetRelatedControl()
			{
				GuiGraphicsComposition* composition=this;
				while(composition)
				{
					if(composition->GetAssociatedControl())
					{
						return composition->GetAssociatedControl();
					}
					else
					{
						composition=composition->GetParent();
					}
				}
				return 0;
			}

			GuiGraphicsHost* GuiGraphicsComposition::GetRelatedGraphicsHost()
			{
				GuiGraphicsComposition* composition=this;
				while(composition)
				{
					if(composition->GetAssociatedHost())
					{
						return composition->GetAssociatedHost();
					}
					else
					{
						composition=composition->GetParent();
					}
				}
				return 0;
			}

			controls::GuiControlHost* GuiGraphicsComposition::GetRelatedControlHost()
			{
				GuiGraphicsComposition* composition=this;
				while(composition)
				{
					if(composition->GetAssociatedControl())
					{
						GuiControlHost* controlHost=dynamic_cast<GuiControlHost*>(composition->GetAssociatedControl());
						if(controlHost)
						{
							return controlHost;
						}
					}
					composition=composition->GetParent();
				}
				return 0;
			}

			INativeCursor* GuiGraphicsComposition::GetRelatedCursor()
			{
				GuiGraphicsComposition* composition=this;
				while(composition)
				{
					if(composition->GetAssociatedCursor())
					{
						return composition->GetAssociatedCursor();
					}
					else
					{
						composition=composition->GetParent();
					}
				}
				return 0;
			}

			Margin GuiGraphicsComposition::GetMargin()
			{
				return margin;
			}

			void GuiGraphicsComposition::SetMargin(Margin value)
			{
				margin=value;
			}

			Margin GuiGraphicsComposition::GetInternalMargin()
			{
				return internalMargin;
			}

			void GuiGraphicsComposition::SetInternalMargin(Margin value)
			{
				internalMargin=value;
			}

			Size GuiGraphicsComposition::GetPreferredMinSize()
			{
				return preferredMinSize;
			}

			void GuiGraphicsComposition::SetPreferredMinSize(Size value)
			{
				preferredMinSize=value;
			}

			Rect GuiGraphicsComposition::GetClientArea()
			{
				Rect bounds=GetBounds();
				bounds.x1+=margin.left+internalMargin.left;
				bounds.y1+=margin.top+internalMargin.top;
				bounds.x2-=margin.right+internalMargin.right;
				bounds.y2-=margin.bottom+internalMargin.bottom;
				return bounds;
			}

			void GuiGraphicsComposition::ForceCalculateSizeImmediately()
			{
				for(int i=0;i<children.Count();i++)
				{
					children[i]->ForceCalculateSizeImmediately();
				}
			}

/***********************************************************************
GuiGraphicsSite
***********************************************************************/

			Rect GuiGraphicsSite::GetBoundsInternal(Rect expectedBounds)
			{
				Size minSize=GetMinPreferredClientSize();
				if(minSize.x<preferredMinSize.x) minSize.x=preferredMinSize.x;
				if(minSize.y<preferredMinSize.y) minSize.y=preferredMinSize.y;

				minSize.x+=margin.left+margin.right+internalMargin.left+internalMargin.right;
				minSize.y+=margin.top+margin.bottom+internalMargin.top+internalMargin.bottom;
				int w=expectedBounds.Width();
				int h=expectedBounds.Height();
				if(minSize.x<w) minSize.x=w;
				if(minSize.y<h) minSize.y=h;
				return Rect(expectedBounds.LeftTop(), minSize);
			}

			GuiGraphicsSite::GuiGraphicsSite()
			{
			}

			GuiGraphicsSite::~GuiGraphicsSite()
			{
			}

			bool GuiGraphicsSite::IsSizeAffectParent()
			{
				return true;
			}

			Size GuiGraphicsSite::GetMinPreferredClientSize()
			{
				Size minSize;
				if(minSizeLimitation!=GuiGraphicsComposition::NoLimit)
				{
					if(ownedElement)
					{
						minSize=ownedElement->GetRenderer()->GetMinSize();
					}
				}
				if(minSizeLimitation==GuiGraphicsComposition::LimitToElementAndChildren)
				{
					int childCount=Children().Count();
					for(int i=0;i<childCount;i++)
					{
						GuiGraphicsComposition* child=children[i];
						if(child->IsSizeAffectParent())
						{
							Rect childBounds=child->GetPreferredBounds();
							if(minSize.x<childBounds.x2) minSize.x=childBounds.x2;
							if(minSize.y<childBounds.y2) minSize.y=childBounds.y2;
						}
					}
				}
				return minSize;
			}

			Rect GuiGraphicsSite::GetPreferredBounds()
			{
				return GetBoundsInternal(Rect(Point(0, 0), GetMinPreferredClientSize()));
			}
		}
	}
}