#include "GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;
			using namespace elements;
			using namespace compositions;

			const wchar_t* const GuiListControl::IItemPrimaryTextView::Identifier = L"vl::presnetation::controls::GuiListControl::IItemPrimaryTextView";

/***********************************************************************
GuiListControl::ItemCallback
***********************************************************************/

			GuiListControl::ItemCallback::ItemCallback(GuiListControl* _listControl)
				:listControl(_listControl)
			{
			}

			GuiListControl::ItemCallback::~ItemCallback()
			{
				ClearCache();
			}

			void GuiListControl::ItemCallback::ClearCache()
			{
				for(int i=0;i<cachedStyles.Count();i++)
				{
					listControl->itemStyleProvider->DestroyItemStyle(cachedStyles[i]);
				}
				for(int i=0;i<installedStyles.Count();i++)
				{
					listControl->itemStyleProvider->DestroyItemStyle(installedStyles[i]);
				}
				cachedStyles.Clear();
				installedStyles.Clear();
			}

			void GuiListControl::ItemCallback::OnAttached(IItemProvider* provider)
			{
			}

			void GuiListControl::ItemCallback::OnItemModified(int start, int count, int newCount)
			{
				listControl->OnItemModified(start, count, newCount);
			}

			GuiListControl::IItemStyleController* GuiListControl::ItemCallback::RequestItem(int itemIndex)
			{
				int id=listControl->itemStyleProvider->GetItemStyleId(itemIndex);
				IItemStyleController* style=0;
				for(int i=0;i<cachedStyles.Count();i++)
				{
					IItemStyleController* cachedStyle=cachedStyles[i];
					if(cachedStyle->GetItemStyleId()==id)
					{
						style=cachedStyle;
						cachedStyles.RemoveAt(i);
						break;
					}
				}
				if(!style)
				{
					style=listControl->itemStyleProvider->CreateItemStyle(id);
				}
				listControl->itemStyleProvider->Install(style, itemIndex);
				style->OnInstalled();
				installedStyles.Add(style);
				listControl->GetContainerComposition()->AddChild(style->GetBoundsComposition());
				listControl->OnStyleInstalled(itemIndex, style);
				return style;
			}

			void GuiListControl::ItemCallback::ReleaseItem(IItemStyleController* style)
			{
				int index=installedStyles.IndexOf(style);
				if(index!=-1)
				{
					listControl->OnStyleUninstalled(style);
					listControl->GetContainerComposition()->RemoveChild(style->GetBoundsComposition());
					installedStyles.RemoveAt(index);
					style->OnUninstalled();
					if(style->IsCacheable())
					{
						cachedStyles.Add(style);
					}
					else
					{
						listControl->itemStyleProvider->DestroyItemStyle(style);
					}
				}
			}

			void GuiListControl::ItemCallback::SetViewLocation(Point value)
			{
				Rect virtualRect(value, listControl->GetViewSize());
				Rect realRect=listControl->itemCoordinateTransformer->VirtualRectToRealRect(listControl->fullSize, virtualRect);
				listControl->GetHorizontalScroll()->SetPosition(realRect.Left());
				listControl->GetVerticalScroll()->SetPosition(realRect.Top());
			}

			Size GuiListControl::ItemCallback::GetStylePreferredSize(IItemStyleController* style)
			{
				Size size=style->GetBoundsComposition()->GetPreferredBounds().GetSize();
				return listControl->itemCoordinateTransformer->RealSizeToVirtualSize(size);
			}

			void GuiListControl::ItemCallback::SetStyleAlignmentToParent(IItemStyleController* style, Margin margin)
			{
				Margin newMargin=listControl->itemCoordinateTransformer->VirtualMarginToRealMargin(margin);
				style->GetBoundsComposition()->SetAlignmentToParent(newMargin);
			}

			Rect GuiListControl::ItemCallback::GetStyleBounds(IItemStyleController* style)
			{
				Rect bounds=style->GetBoundsComposition()->GetBounds();
				return listControl->itemCoordinateTransformer->RealRectToVirtualRect(listControl->GetViewSize(), bounds);
			}

			void GuiListControl::ItemCallback::SetStyleBounds(IItemStyleController* style, Rect bounds)
			{
				Rect newBounds=listControl->itemCoordinateTransformer->VirtualRectToRealRect(listControl->GetViewSize(), bounds);
				return style->GetBoundsComposition()->SetBounds(newBounds);
			}

			compositions::GuiGraphicsComposition* GuiListControl::ItemCallback::GetContainerComposition()
			{
				return listControl->GetContainerComposition();
			}

			void GuiListControl::ItemCallback::OnTotalSizeChanged()
			{
				listControl->CalculateView();
			}

/***********************************************************************
GuiListControl
***********************************************************************/

			void GuiListControl::OnItemModified(int start, int count, int newCount)
			{
			}

			void GuiListControl::OnStyleInstalled(int itemIndex, IItemStyleController* style)
			{
				AttachItemEvents(style);
			}

			void GuiListControl::OnStyleUninstalled(IItemStyleController* style)
			{
				DetachItemEvents(style);
			}

			void GuiListControl::OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)
			{
				SetStyleProviderAndArranger(itemStyleProvider, itemArranger);
				GuiScrollView::OnRenderTargetChanged(renderTarget);
			}

			void GuiListControl::OnBeforeReleaseGraphicsHost()
			{
				GuiScrollView::OnBeforeReleaseGraphicsHost();
				SetStyleProviderAndArranger(0, 0);
			}

			Size GuiListControl::QueryFullSize()
			{
				Size virtualSize=itemArranger?itemArranger->GetTotalSize():Size(0, 0);
				fullSize=itemCoordinateTransformer->VirtualSizeToRealSize(virtualSize);
				return fullSize;
			}

			void GuiListControl::UpdateView(Rect viewBounds)
			{
				if(itemArranger)
				{
					Rect newBounds=itemCoordinateTransformer->RealRectToVirtualRect(fullSize, viewBounds);
					itemArranger->OnViewChanged(newBounds);
				}
			}

			void GuiListControl::OnBoundsMouseButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(GetVisuallyEnabled())
				{
					boundsComposition->GetRelatedGraphicsHost()->SetFocus(boundsComposition);
				}
			}

			void GuiListControl::SetStyleProviderAndArranger(Ptr<IItemStyleProvider> styleProvider, Ptr<IItemArranger> arranger)
			{
				if(itemStyleProvider)
				{
					itemStyleProvider->DetachListControl();
				}
				if(itemArranger)
				{
					itemArranger->DetachListControl();
					itemArranger->SetCallback(0);
					itemProvider->DetachCallback(itemArranger.Obj());
				}
				callback->ClearCache();

				itemStyleProvider=styleProvider;
				itemArranger=arranger;
				GetVerticalScroll()->SetPosition(0);
				GetHorizontalScroll()->SetPosition(0);

				if(itemStyleProvider)
				{
					itemStyleProvider->AttachListControl(this);
				}
				if(itemArranger)
				{
					itemProvider->AttachCallback(itemArranger.Obj());
					itemArranger->SetCallback(callback.Obj());
					itemArranger->AttachListControl(this);
				}
				CalculateView();
			}

			void GuiListControl::OnItemMouseEvent(compositions::GuiItemMouseEvent& itemEvent, IItemStyleController* style, compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(itemArranger && GetVisuallyEnabled())
				{
					int itemIndex=itemArranger->GetVisibleIndex(style);
					if(itemIndex!=-1)
					{
						GuiItemMouseEventArgs redirectArguments;
						(GuiMouseEventArgs&)redirectArguments=arguments;
						redirectArguments.compositionSource=GetBoundsComposition();
						redirectArguments.eventSource=GetBoundsComposition();
						redirectArguments.itemIndex=itemIndex;
						itemEvent.Execute(redirectArguments);
						arguments=redirectArguments;
					}
				}
			}

			void GuiListControl::OnItemNotifyEvent(compositions::GuiItemNotifyEvent& itemEvent, IItemStyleController* style, compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(itemArranger && GetVisuallyEnabled())
				{
					int itemIndex=itemArranger->GetVisibleIndex(style);
					if(itemIndex!=-1)
					{
						GuiItemEventArgs redirectArguments;
						(GuiEventArgs&)redirectArguments=arguments;
						redirectArguments.compositionSource=GetBoundsComposition();
						redirectArguments.eventSource=GetBoundsComposition();
						redirectArguments.itemIndex=itemIndex;
						itemEvent.Execute(redirectArguments);
						arguments=redirectArguments;
					}
				}
			}

#define ATTACH_ITEM_MOUSE_EVENT(EVENTNAME, ITEMEVENTNAME)\
					{\
						Func<void(GuiItemMouseEvent&, IItemStyleController*, GuiGraphicsComposition*, GuiMouseEventArgs&)> func(this, &GuiListControl::OnItemMouseEvent);\
						helper->EVENTNAME##Handler=style->GetBoundsComposition()->GetEventReceiver()->EVENTNAME.AttachFunction(\
							Curry(Curry(func)(ITEMEVENTNAME))(style)\
							);\
					}\

#define ATTACH_ITEM_NOTIFY_EVENT(EVENTNAME, ITEMEVENTNAME)\
					{\
						Func<void(GuiItemNotifyEvent&, IItemStyleController*, GuiGraphicsComposition*, GuiEventArgs&)> func(this, &GuiListControl::OnItemNotifyEvent);\
						helper->EVENTNAME##Handler=style->GetBoundsComposition()->GetEventReceiver()->EVENTNAME.AttachFunction(\
							Curry(Curry(func)(ITEMEVENTNAME))(style)\
							);\
					}\

			void GuiListControl::AttachItemEvents(IItemStyleController* style)
			{
				int index=visibleStyles.Keys().IndexOf(style);
				if(index==-1)
				{
					Ptr<VisibleStyleHelper> helper=new VisibleStyleHelper;
					visibleStyles.Add(style, helper);

					ATTACH_ITEM_MOUSE_EVENT(leftButtonDown, ItemLeftButtonDown);
					ATTACH_ITEM_MOUSE_EVENT(leftButtonUp, ItemLeftButtonUp);
					ATTACH_ITEM_MOUSE_EVENT(leftButtonDoubleClick, ItemLeftButtonDoubleClick);
					ATTACH_ITEM_MOUSE_EVENT(middleButtonDown, ItemMiddleButtonDown);
					ATTACH_ITEM_MOUSE_EVENT(middleButtonUp, ItemMiddleButtonUp);
					ATTACH_ITEM_MOUSE_EVENT(middleButtonDoubleClick, ItemMiddleButtonDoubleClick);
					ATTACH_ITEM_MOUSE_EVENT(rightButtonDown, ItemRightButtonDown);
					ATTACH_ITEM_MOUSE_EVENT(rightButtonUp, ItemRightButtonUp);
					ATTACH_ITEM_MOUSE_EVENT(rightButtonDoubleClick, ItemRightButtonDoubleClick);
					ATTACH_ITEM_MOUSE_EVENT(mouseMove, ItemMouseMove);
					ATTACH_ITEM_NOTIFY_EVENT(mouseEnter, ItemMouseEnter);
					ATTACH_ITEM_NOTIFY_EVENT(mouseLeave, ItemMouseLeave);
				}
			}

#undef ATTACH_ITEM_MOUSE_EVENT
#undef ATTACH_ITEM_NOTIFY_EVENT

#define DETACH_ITEM_EVENT(EVENTNAME) style->GetBoundsComposition()->GetEventReceiver()->EVENTNAME.Detach(helper->EVENTNAME##Handler)

			void GuiListControl::DetachItemEvents(IItemStyleController* style)
			{
				int index=visibleStyles.Keys().IndexOf(style);
				if(index!=-1)
				{
					Ptr<VisibleStyleHelper> helper=visibleStyles.Values().Get(index);
					visibleStyles.Remove(style);
					
					DETACH_ITEM_EVENT(leftButtonDown);
					DETACH_ITEM_EVENT(leftButtonUp);
					DETACH_ITEM_EVENT(leftButtonDoubleClick);
					DETACH_ITEM_EVENT(middleButtonDown);
					DETACH_ITEM_EVENT(middleButtonUp);
					DETACH_ITEM_EVENT(middleButtonDoubleClick);
					DETACH_ITEM_EVENT(rightButtonDown);
					DETACH_ITEM_EVENT(rightButtonUp);
					DETACH_ITEM_EVENT(rightButtonDoubleClick);
					DETACH_ITEM_EVENT(mouseMove);
					DETACH_ITEM_EVENT(mouseEnter);
					DETACH_ITEM_EVENT(mouseLeave);
				}
			}

#undef DETACH_ITEM_EVENT

			GuiListControl::GuiListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider, bool acceptFocus)
				:GuiScrollView(_styleProvider)
				,itemProvider(_itemProvider)
			{
				StyleProviderChanged.SetAssociatedComposition(boundsComposition);
				ArrangerChanged.SetAssociatedComposition(boundsComposition);
				CoordinateTransformerChanged.SetAssociatedComposition(boundsComposition);
				
				ItemLeftButtonDown.SetAssociatedComposition(boundsComposition);
				ItemLeftButtonUp.SetAssociatedComposition(boundsComposition);
				ItemLeftButtonDoubleClick.SetAssociatedComposition(boundsComposition);
				ItemMiddleButtonDown.SetAssociatedComposition(boundsComposition);
				ItemMiddleButtonUp.SetAssociatedComposition(boundsComposition);
				ItemMiddleButtonDoubleClick.SetAssociatedComposition(boundsComposition);
				ItemRightButtonDown.SetAssociatedComposition(boundsComposition);
				ItemRightButtonUp.SetAssociatedComposition(boundsComposition);
				ItemRightButtonDoubleClick.SetAssociatedComposition(boundsComposition);
				ItemMouseMove.SetAssociatedComposition(boundsComposition);
				ItemMouseEnter.SetAssociatedComposition(boundsComposition);
				ItemMouseLeave.SetAssociatedComposition(boundsComposition);

				callback=new ItemCallback(this);
				itemProvider->AttachCallback(callback.Obj());
				itemCoordinateTransformer=new list::DefaultItemCoordinateTransformer;

				if(acceptFocus)
				{
					boundsComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiListControl::OnBoundsMouseButtonDown);
					boundsComposition->GetEventReceiver()->middleButtonDown.AttachMethod(this, &GuiListControl::OnBoundsMouseButtonDown);
					boundsComposition->GetEventReceiver()->rightButtonDown.AttachMethod(this, &GuiListControl::OnBoundsMouseButtonDown);
					SetFocusableComposition(boundsComposition);
				}
			}

			GuiListControl::~GuiListControl()
			{
				if(itemArranger)
				{
					itemProvider->DetachCallback(itemArranger.Obj());
				}
				callback->ClearCache();
				itemStyleProvider=0;
				itemArranger=0;
			}

			GuiListControl::IItemProvider* GuiListControl::GetItemProvider()
			{
				return itemProvider.Obj();
			}

			GuiListControl::IItemStyleProvider* GuiListControl::GetStyleProvider()
			{
				return itemStyleProvider.Obj();
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiListControl::SetStyleProvider(Ptr<IItemStyleProvider> value)
			{
				Ptr<IItemStyleProvider> old=itemStyleProvider;
				SetStyleProviderAndArranger(value, itemArranger);
				StyleProviderChanged.Execute(GetNotifyEventArguments());
				return old;
			}

			GuiListControl::IItemArranger* GuiListControl::GetArranger()
			{
				return itemArranger.Obj();
			}

			Ptr<GuiListControl::IItemArranger> GuiListControl::SetArranger(Ptr<IItemArranger> value)
			{
				Ptr<IItemArranger> old=itemArranger;
				SetStyleProviderAndArranger(itemStyleProvider, value);
				ArrangerChanged.Execute(GetNotifyEventArguments());
				return old;
			}

			GuiListControl::IItemCoordinateTransformer* GuiListControl::GetCoordinateTransformer()
			{
				return itemCoordinateTransformer.Obj();
			}

			Ptr<GuiListControl::IItemCoordinateTransformer> GuiListControl::SetCoordinateTransformer(Ptr<IItemCoordinateTransformer> value)
			{
				Ptr<IItemCoordinateTransformer> old=itemCoordinateTransformer;
				itemCoordinateTransformer=value;
				SetStyleProviderAndArranger(itemStyleProvider, itemArranger);
				CoordinateTransformerChanged.Execute(GetNotifyEventArguments());
				return old;
			}

			bool GuiListControl::EnsureItemVisible(int itemIndex)
			{
				if(itemIndex<0 || itemIndex>=itemProvider->Count())
				{
					return false;
				}
				return itemArranger?itemArranger->EnsureItemVisible(itemIndex):false;
			}

/***********************************************************************
GuiSelectableListControl
***********************************************************************/

			void GuiSelectableListControl::OnItemModified(int start, int count, int newCount)
			{
				GuiListControl::OnItemModified(start, count, newCount);
				if(count!=newCount)
				{
					ClearSelection();
				}
			}

			void GuiSelectableListControl::OnStyleInstalled(int itemIndex, IItemStyleController* style)
			{
				GuiListControl::OnStyleInstalled(itemIndex, style);
				selectableStyleProvider->SetStyleSelected(style, selectedItems.Contains(itemIndex));
			}

			void GuiSelectableListControl::OnStyleUninstalled(IItemStyleController* style)
			{
				GuiListControl::OnStyleUninstalled(style);
			}

			void GuiSelectableListControl::OnItemSelectionChanged(int itemIndex, bool value)
			{
				GuiListControl::IItemStyleController* style=itemArranger->GetVisibleStyle(itemIndex);
				if(style)
				{
					selectableStyleProvider->SetStyleSelected(style, value);
				}
			}

			void GuiSelectableListControl::OnItemSelectionCleared()
			{
				for(int i=0;i<visibleStyles.Count();i++)
				{
					selectableStyleProvider->SetStyleSelected(visibleStyles.Keys()[i], false);
				}
			}

			void GuiSelectableListControl::OnItemLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiItemMouseEventArgs& arguments)
			{
				if(GetVisuallyEnabled())
				{
					SelectItemsByClick(arguments.itemIndex, arguments.ctrl, arguments.shift);
				}
			}

			void GuiSelectableListControl::NormalizeSelectedItemIndexStartEnd()
			{
				if(selectedItemIndexStart<0 || selectedItemIndexStart>=itemProvider->Count())
				{
					selectedItemIndexStart=0;
				}
				if(selectedItemIndexEnd<0 || selectedItemIndexEnd>=itemProvider->Count())
				{
					selectedItemIndexEnd=0;
				}
			}

			void GuiSelectableListControl::SetMultipleItemsSelectedSilently(int start, int end, bool selected)
			{
				if(start>end)
				{
					int temp=start;
					start=end;
					end=temp;
				}
				int count=itemProvider->Count();
				if(start<0) start=0;
				if(end>=count) end=count-1;
				for(int i=start;i<=end;i++)
				{
					if(selected)
					{
						if(!selectedItems.Contains(i))
						{
							selectedItems.Add(i);
						}
					}
					else
					{
						selectedItems.Remove(i);
					}
					OnItemSelectionChanged(i, selected);
				}
			}

			void GuiSelectableListControl::OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments)
			{
				if(GetVisuallyEnabled())
				{
					if(SelectItemsByKey(arguments.code, arguments.ctrl, arguments.shift))
					{
						arguments.handled=true;
					}
				}
			}

			GuiSelectableListControl::GuiSelectableListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider)
				:GuiListControl(_styleProvider, _itemProvider, true)
				,multiSelect(false)
				,selectedItemIndexStart(-1)
				,selectedItemIndexEnd(-1)
			{
				SelectionChanged.SetAssociatedComposition(boundsComposition);
				ItemLeftButtonDown.AttachMethod(this, &GuiSelectableListControl::OnItemLeftButtonDown);
				if(focusableComposition)
				{
					focusableComposition->GetEventReceiver()->keyDown.AttachMethod(this, &GuiSelectableListControl::OnKeyDown);
				}
			}

			GuiSelectableListControl::~GuiSelectableListControl()
			{
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiSelectableListControl::SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)
			{
				selectableStyleProvider=value?value.Cast<IItemStyleProvider>():0;
				return GuiListControl::SetStyleProvider(value);
			}

			bool GuiSelectableListControl::GetMultiSelect()
			{
				return multiSelect;
			}

			void GuiSelectableListControl::SetMultiSelect(bool value)
			{
				if(multiSelect!=value)
				{
					multiSelect=value;
					ClearSelection();
				}
			}

			const collections::SortedList<int>& GuiSelectableListControl::GetSelectedItems()
			{
				return selectedItems;
			}

			bool GuiSelectableListControl::GetSelected(int itemIndex)
			{
				return selectedItems.Contains(itemIndex);
			}

			void GuiSelectableListControl::SetSelected(int itemIndex, bool value)
			{
				if(value)
				{
					if(!selectedItems.Contains(itemIndex))
					{
						if(!multiSelect)
						{
							selectedItems.Clear();
							OnItemSelectionCleared();
						}
						selectedItems.Add(itemIndex);
						OnItemSelectionChanged(itemIndex, value);
						SelectionChanged.Execute(GetNotifyEventArguments());
					}
				}
				else
				{
					if(selectedItems.Remove(itemIndex))
					{
						OnItemSelectionChanged(itemIndex, value);
						SelectionChanged.Execute(GetNotifyEventArguments());
					}
				}
			}

			bool GuiSelectableListControl::SelectItemsByClick(int itemIndex, bool ctrl, bool shift)
			{
				NormalizeSelectedItemIndexStartEnd();
				if(0<=itemIndex && itemIndex<itemProvider->Count())
				{
					if(!multiSelect)
					{
						shift=false;
						ctrl=false;
					}
					if(shift)
					{
						if(!ctrl)
						{
							SetMultipleItemsSelectedSilently(selectedItemIndexStart, selectedItemIndexEnd, false);
						}
						selectedItemIndexEnd=itemIndex;
						SetMultipleItemsSelectedSilently(selectedItemIndexStart, selectedItemIndexEnd, true);
						SelectionChanged.Execute(GetNotifyEventArguments());
					}
					else
					{
						if(ctrl)
						{
							int index=selectedItems.IndexOf(itemIndex);
							if(index==-1)
							{
								selectedItems.Add(itemIndex);
							}
							else
							{
								selectedItems.RemoveAt(index);
							}
							OnItemSelectionChanged(itemIndex, index==-1);
							SelectionChanged.Execute(GetNotifyEventArguments());
						}
						else
						{
							selectedItems.Clear();
							OnItemSelectionCleared();
							selectedItems.Add(itemIndex);
							OnItemSelectionChanged(itemIndex, true);
							SelectionChanged.Execute(GetNotifyEventArguments());
						}
						selectedItemIndexStart=itemIndex;
						selectedItemIndexEnd=itemIndex;
					}
					return true;
				}
				return false;
			}

			bool GuiSelectableListControl::SelectItemsByKey(int code, bool ctrl, bool shift)
			{
				if(!GetArranger()) return false;

				NormalizeSelectedItemIndexStartEnd();
				KeyDirection keyDirection=Up;
				switch(code)
				{
				case VKEY_UP:
					keyDirection=Up;
					break;
				case VKEY_DOWN:
					keyDirection=Down;
					break;
				case VKEY_LEFT:
					keyDirection=Left;
					break;
				case VKEY_RIGHT:
					keyDirection=Right;
					break;
				case VKEY_HOME:
					keyDirection=Home;
					break;
				case VKEY_END:
					keyDirection=End;
					break;
				case VKEY_PRIOR:
					keyDirection=PageUp;
					break;
				case VKEY_NEXT:
					keyDirection=PageDown;
					break;
				default:
					return false;
				}

				if(GetCoordinateTransformer())
				{
					keyDirection=GetCoordinateTransformer()->RealKeyDirectionToVirtualKeyDirection(keyDirection);
				}
				int itemIndex=GetArranger()->FindItem(selectedItemIndexEnd, keyDirection);
				if(SelectItemsByClick(itemIndex, ctrl, shift))
				{
					return EnsureItemVisible(itemIndex);
				}
				else
				{
					return false;
				}
			}

			void GuiSelectableListControl::ClearSelection()
			{
				if(selectedItems.Count()>0)
				{
					selectedItems.Clear();
					OnItemSelectionCleared();
					SelectionChanged.Execute(GetNotifyEventArguments());
				}
			}

			namespace list
			{

/***********************************************************************
DefaultItemCoordinateTransformer
***********************************************************************/

				DefaultItemCoordinateTransformer::DefaultItemCoordinateTransformer()
				{
				}

				DefaultItemCoordinateTransformer::~DefaultItemCoordinateTransformer()
				{
				}

				Size DefaultItemCoordinateTransformer::RealSizeToVirtualSize(Size size)
				{
					return size;
				}

				Size DefaultItemCoordinateTransformer::VirtualSizeToRealSize(Size size)
				{
					return size;
				}

				Point DefaultItemCoordinateTransformer::RealPointToVirtualPoint(Size realFullSize, Point point)
				{
					return point;
				}

				Point DefaultItemCoordinateTransformer::VirtualPointToRealPoint(Size realFullSize, Point point)
				{
					return point;
				}

				Rect DefaultItemCoordinateTransformer::RealRectToVirtualRect(Size realFullSize, Rect rect)
				{
					return rect;
				}

				Rect DefaultItemCoordinateTransformer::VirtualRectToRealRect(Size realFullSize, Rect rect)
				{
					return rect;
				}

				Margin DefaultItemCoordinateTransformer::RealMarginToVirtualMargin(Margin margin)
				{
					return margin;
				}

				Margin DefaultItemCoordinateTransformer::VirtualMarginToRealMargin(Margin margin)
				{
					return margin;
				}

				GuiListControl::KeyDirection DefaultItemCoordinateTransformer::RealKeyDirectionToVirtualKeyDirection(GuiListControl::KeyDirection key)
				{
					return key;
				}

/***********************************************************************
AxisAlignedItemCoordinateTransformer
***********************************************************************/

				AxisAlignedItemCoordinateTransformer::AxisAlignedItemCoordinateTransformer(Alignment _alignment)
					:alignment(_alignment)
				{
				}

				AxisAlignedItemCoordinateTransformer::~AxisAlignedItemCoordinateTransformer()
				{
				}

				AxisAlignedItemCoordinateTransformer::Alignment AxisAlignedItemCoordinateTransformer::GetAlignment()
				{
					return alignment;
				}

				Size AxisAlignedItemCoordinateTransformer::RealSizeToVirtualSize(Size size)
				{
					switch(alignment)
					{
					case LeftDown:
					case RightDown:
					case LeftUp:
					case RightUp:
						return Size(size.x, size.y);
					case DownLeft:
					case DownRight:
					case UpLeft:
					case UpRight:
						return Size(size.y, size.x);
					}
					return size;
				}

				Size AxisAlignedItemCoordinateTransformer::VirtualSizeToRealSize(Size size)
				{
					return RealSizeToVirtualSize(size);
				}

				Point AxisAlignedItemCoordinateTransformer::RealPointToVirtualPoint(Size realFullSize, Point point)
				{
					Rect rect(point, Size(0, 0));
					return RealRectToVirtualRect(realFullSize, rect).LeftTop();
				}

				Point AxisAlignedItemCoordinateTransformer::VirtualPointToRealPoint(Size realFullSize, Point point)
				{
					Rect rect(point, Size(0, 0));
					return VirtualRectToRealRect(realFullSize, rect).LeftTop();
				}

				Rect AxisAlignedItemCoordinateTransformer::RealRectToVirtualRect(Size realFullSize, Rect rect)
				{
					int x1=rect.x1;
					int x2=realFullSize.x-rect.x2;
					int y1=rect.y1;
					int y2=realFullSize.y-rect.y2;
					int w=rect.Width();
					int h=rect.Height();
					switch(alignment)
					{
					case LeftDown:
						return Rect(Point(x2, y1), Size(w, h));
					case RightDown:
						return Rect(Point(x1, y1), Size(w, h));
					case LeftUp:
						return Rect(Point(x2, y2), Size(w, h));
					case RightUp:
						return Rect(Point(x1, y2), Size(w, h));
					case DownLeft:
						return Rect(Point(y1, x2), Size(h, w));
					case DownRight:
						return Rect(Point(y1, x1), Size(h, w));
					case UpLeft:
						return Rect(Point(y2, x2), Size(h, w));
					case UpRight:
						return Rect(Point(y2, x1), Size(h, w));
					}
					return rect;
				}

				Rect AxisAlignedItemCoordinateTransformer::VirtualRectToRealRect(Size realFullSize, Rect rect)
				{
					realFullSize=RealSizeToVirtualSize(realFullSize);
					int x1=rect.x1;
					int x2=realFullSize.x-rect.x2;
					int y1=rect.y1;
					int y2=realFullSize.y-rect.y2;
					int w=rect.Width();
					int h=rect.Height();
					switch(alignment)
					{
					case LeftDown:
						return Rect(Point(x2, y1), Size(w, h));
					case RightDown:
						return Rect(Point(x1, y1), Size(w, h));
					case LeftUp:
						return Rect(Point(x2, y2), Size(w, h));
					case RightUp:
						return Rect(Point(x1, y2), Size(w, h));
					case DownLeft:
						return Rect(Point(y2, x1), Size(h, w));
					case DownRight:
						return Rect(Point(y1, x1), Size(h, w));
					case UpLeft:
						return Rect(Point(y2, x2), Size(h, w));
					case UpRight:
						return Rect(Point(y1, x2), Size(h, w));
					}
					return rect;
				}

				Margin AxisAlignedItemCoordinateTransformer::RealMarginToVirtualMargin(Margin margin)
				{
					int x1=margin.left;
					int x2=margin.right;
					int y1=margin.top;
					int y2=margin.bottom;
					switch(alignment)
					{
					case LeftDown:
						return Margin(x2, y1, x1, y2);
					case RightDown:
						return Margin(x1, y1, x2, y2);
					case LeftUp:
						return Margin(x2, y2, x1, y1);
					case RightUp:
						return Margin(x1, y2, x2, y1);
					case DownLeft:
						return Margin(y1, x2, y2, x1);
					case DownRight:
						return Margin(y1, x1, y2, x2);
					case UpLeft:
						return Margin(y2, x2, y1, x1);
					case UpRight:
						return Margin(y2, x1, y1, x2);
					}
					return margin;
				}

				Margin AxisAlignedItemCoordinateTransformer::VirtualMarginToRealMargin(Margin margin)
				{
					int x1=margin.left;
					int x2=margin.right;
					int y1=margin.top;
					int y2=margin.bottom;
					switch(alignment)
					{
					case LeftDown:
						return Margin(x2, y1, x1, y2);
					case RightDown:
						return Margin(x1, y1, x2, y2);
					case LeftUp:
						return Margin(x2, y2, x1, y1);
					case RightUp:
						return Margin(x1, y2, x2, y1);
					case DownLeft:
						return Margin(y2, x1, y1, x2);
					case DownRight:
						return Margin(y1, x1, y2, x2);
					case UpLeft:
						return Margin(y2, x2, y1, x1);
					case UpRight:
						return Margin(y1, x2, y2, x1);
					}
					return margin;
				}

				GuiListControl::KeyDirection AxisAlignedItemCoordinateTransformer::RealKeyDirectionToVirtualKeyDirection(GuiListControl::KeyDirection key)
				{
					bool pageKey=false;
					switch(key)
					{
					case GuiListControl::PageUp:
						pageKey=true;
						key=GuiListControl::Up;
						break;
					case GuiListControl::PageDown:
						pageKey=true;
						key=GuiListControl::Down;
						break;
					case GuiListControl::PageLeft:
						pageKey=true;
						key=GuiListControl::Left;
						break;
					case GuiListControl::PageRight:
						pageKey=true;
						key=GuiListControl::Right;
						break;
					}

					switch(key)
					{
					case GuiListControl::Up:
						switch(alignment)
						{
						case LeftDown:	key=GuiListControl::Up;		break;
						case RightDown:	key=GuiListControl::Up;		break;
						case LeftUp:	key=GuiListControl::Down;	break;
						case RightUp:	key=GuiListControl::Down;	break;
						case DownLeft:	key=GuiListControl::Left;	break;
						case DownRight:	key=GuiListControl::Left;	break;
						case UpLeft:	key=GuiListControl::Right;	break;
						case UpRight:	key=GuiListControl::Right;	break;
						}
						break;
					case GuiListControl::Down:
						switch(alignment)
						{
						case LeftDown:	key=GuiListControl::Down;	break;
						case RightDown:	key=GuiListControl::Down;	break;
						case LeftUp:	key=GuiListControl::Up;		break;
						case RightUp:	key=GuiListControl::Up;		break;
						case DownLeft:	key=GuiListControl::Right;	break;
						case DownRight:	key=GuiListControl::Right;	break;
						case UpLeft:	key=GuiListControl::Left;	break;
						case UpRight:	key=GuiListControl::Left;	break;
						}
						break;
					case GuiListControl::Left:
						switch(alignment)
						{
						case LeftDown:	key=GuiListControl::Right;	break;
						case RightDown:	key=GuiListControl::Left;	break;
						case LeftUp:	key=GuiListControl::Right;	break;
						case RightUp:	key=GuiListControl::Left;	break;
						case DownLeft:	key=GuiListControl::Down;	break;
						case DownRight:	key=GuiListControl::Up;		break;
						case UpLeft:	key=GuiListControl::Down;	break;
						case UpRight:	key=GuiListControl::Up;		break;
						}
						break;
					case GuiListControl::Right:
						switch(alignment)
						{
						case LeftDown:	key=GuiListControl::Left;	break;
						case RightDown:	key=GuiListControl::Right;	break;
						case LeftUp:	key=GuiListControl::Left;	break;
						case RightUp:	key=GuiListControl::Right;	break;
						case DownLeft:	key=GuiListControl::Up;		break;
						case DownRight:	key=GuiListControl::Down;	break;
						case UpLeft:	key=GuiListControl::Up;		break;
						case UpRight:	key=GuiListControl::Down;	break;
						}
						break;
					case GuiListControl::Home:
						switch(alignment)
						{
						case LeftDown:	key=GuiListControl::Home;	break;
						case RightDown:	key=GuiListControl::Home;	break;
						case LeftUp:	key=GuiListControl::End;	break;
						case RightUp:	key=GuiListControl::End;	break;
						case DownLeft:	key=GuiListControl::Home;	break;
						case DownRight:	key=GuiListControl::Home;	break;
						case UpLeft:	key=GuiListControl::End;	break;
						case UpRight:	key=GuiListControl::End;	break;
						}
						break;
					case GuiListControl::End:
						switch(alignment)
						{
						case LeftDown:	key=GuiListControl::End;	break;
						case RightDown:	key=GuiListControl::End;	break;
						case LeftUp:	key=GuiListControl::Home;	break;
						case RightUp:	key=GuiListControl::Home;	break;
						case DownLeft:	key=GuiListControl::End;	break;
						case DownRight:	key=GuiListControl::End;	break;
						case UpLeft:	key=GuiListControl::Home;	break;
						case UpRight:	key=GuiListControl::Home;	break;
						}
						break;
					}

					if(pageKey)
					{
						switch(key)
						{
						case GuiListControl::Up:
							key=GuiListControl::PageUp;
							break;
						case GuiListControl::Down:
							key=GuiListControl::PageDown;
							break;
						case GuiListControl::Left:
							key=GuiListControl::PageLeft;
							break;
						case GuiListControl::Right:
							key=GuiListControl::PageRight;
							break;
						}
					}
					return key;
				}

/***********************************************************************
RangedItemArrangerBase
***********************************************************************/

				void RangedItemArrangerBase::ClearStyles()
				{
					startIndex=0;
					if(callback)
					{
						for(int i=0;i<visibleStyles.Count();i++)
						{
							GuiListControl::IItemStyleController* style=visibleStyles[i];
							callback->ReleaseItem(style);
						}
					}
					visibleStyles.Clear();
					viewBounds=Rect(0, 0, 0, 0);
					OnStylesCleared();
				}

				RangedItemArrangerBase::RangedItemArrangerBase()
					:callback(0)
					,startIndex(0)
				{
				}

				RangedItemArrangerBase::~RangedItemArrangerBase()
				{
				}

				void RangedItemArrangerBase::OnAttached(GuiListControl::IItemProvider* provider)
				{
					itemProvider=provider;
					if(provider)
					{
						OnItemModified(0, 0, provider->Count());
					}
				}

				void RangedItemArrangerBase::OnItemModified(int start, int count, int newCount)
				{
					if(callback)
					{
						int visibleCount=visibleStyles.Count();
						int itemCount=itemProvider->Count();
						SortedList<GuiListControl::IItemStyleController*> reusedStyles;
						for(int i=0;i<visibleCount;i++)
						{
							int index=startIndex+i;
							if(index>=itemCount)
							{
								break;
							}

							int oldIndex=-1;
							if(index<start)
							{
								oldIndex=index;
							}
							else if(index>=start+newCount)
							{
								oldIndex=index-newCount+count;
							}

							if(oldIndex!=-1)
							{
								if(oldIndex>=startIndex && oldIndex<startIndex+visibleCount)
								{
									GuiListControl::IItemStyleController* style=visibleStyles[oldIndex-startIndex];
									reusedStyles.Add(style);
									visibleStyles.Add(style);
								}
								else
								{
									oldIndex=-1;
								}
							}
							if(oldIndex==-1)
							{
								GuiListControl::IItemStyleController* style=callback->RequestItem(index);
								visibleStyles.Add(style);
							}
						}

						for(int i=0;i<visibleCount;i++)
						{
							GuiListControl::IItemStyleController* style=visibleStyles[i];
							if(!reusedStyles.Contains(style))
							{
								callback->ReleaseItem(style);
							}
						}
						visibleStyles.RemoveRange(0, visibleCount);

						callback->OnTotalSizeChanged();
						callback->SetViewLocation(viewBounds.LeftTop());
					}
				}

				void RangedItemArrangerBase::AttachListControl(GuiListControl* value)
				{
				}

				void RangedItemArrangerBase::DetachListControl()
				{
				}

				GuiListControl::IItemArrangerCallback* RangedItemArrangerBase::GetCallback()
				{
					return callback;
				}

				void RangedItemArrangerBase::SetCallback(GuiListControl::IItemArrangerCallback* value)
				{
					if(!value)
					{
						ClearStyles();
					}
					callback=value;
				}

				Size RangedItemArrangerBase::GetTotalSize()
				{
					return OnCalculateTotalSize();
				}

				GuiListControl::IItemStyleController* RangedItemArrangerBase::GetVisibleStyle(int itemIndex)
				{
					if(startIndex<=itemIndex && itemIndex<startIndex+visibleStyles.Count())
					{
						return visibleStyles[itemIndex-startIndex];
					}
					else
					{
						return 0;
					}
				}

				int RangedItemArrangerBase::GetVisibleIndex(GuiListControl::IItemStyleController* style)
				{
					int index=visibleStyles.IndexOf(style);
					return index==-1?-1:index+startIndex;
				}

				void RangedItemArrangerBase::OnViewChanged(Rect bounds)
				{
					Rect oldBounds=viewBounds;
					viewBounds=bounds;
					if(callback)
					{
						OnViewChangedInternal(oldBounds, viewBounds);
					}
				}

/***********************************************************************
FixedHeightItemArranger
***********************************************************************/

				void FixedHeightItemArranger::RearrangeItemBounds()
				{
					int x0=-viewBounds.Left();
					int y0=-viewBounds.Top()+GetYOffset();
					int width=GetWidth();
					for(int i=0;i<visibleStyles.Count();i++)
					{
						GuiListControl::IItemStyleController* style=visibleStyles[i];
						int top=y0+(startIndex+i)*rowHeight;
						if(width==-1)
						{
							callback->SetStyleAlignmentToParent(style, Margin(0, -1, 0, -1));
							callback->SetStyleBounds(style, Rect(Point(0, top), Size(0, rowHeight)));
						}
						else
						{
							callback->SetStyleAlignmentToParent(style, Margin(-1, -1, -1, -1));
							callback->SetStyleBounds(style, Rect(Point(x0, top), Size(width, rowHeight)));
						}
					}
				}

				int FixedHeightItemArranger::GetWidth()
				{
					return -1;
				}

				int FixedHeightItemArranger::GetYOffset()
				{
					return 0;
				}

				void FixedHeightItemArranger::OnStylesCleared()
				{
					rowHeight=1;
				}

				Size FixedHeightItemArranger::OnCalculateTotalSize()
				{
					if(callback)
					{
						int width=GetWidth();
						if(width<0) width=0;
						return Size(width, rowHeight*itemProvider->Count()+GetYOffset());
					}
					else
					{
						return Size(0, 0);
					}
				}

				void FixedHeightItemArranger::OnViewChangedInternal(Rect oldBounds, Rect newBounds)
				{
					if(callback)
					{
						if(!suppressOnViewChanged)
						{
							int oldVisibleCount=visibleStyles.Count();
							int newRowHeight=rowHeight;
							int newStartIndex=(newBounds.Top()-GetYOffset())/rowHeight;
							if(newStartIndex<0) newStartIndex=0;

							int endIndex=startIndex+visibleStyles.Count()-1;
							int newEndIndex=(newBounds.Bottom()-1)/newRowHeight;
							int itemCount=itemProvider->Count();

							for(int i=newStartIndex;i<=newEndIndex && i<itemCount;i++)
							{
								if(startIndex<=i && i<=endIndex)
								{
									GuiListControl::IItemStyleController* style=visibleStyles[i-startIndex];
									visibleStyles.Add(style);
								}
								else
								{
									GuiListControl::IItemStyleController* style=callback->RequestItem(i);
									visibleStyles.Add(style);
									int styleHeight=callback->GetStylePreferredSize(style).y;
									if(newRowHeight<styleHeight)
									{
										newRowHeight=styleHeight;
										newEndIndex=(newBounds.Bottom()-1)/newRowHeight;
									}
								}
							}

							for(int i=0;i<oldVisibleCount;i++)
							{
								int index=startIndex+i;
								if(index<newStartIndex || newEndIndex<index)
								{
									GuiListControl::IItemStyleController* style=visibleStyles[i];
									callback->ReleaseItem(style);
								}
							}
							visibleStyles.RemoveRange(0, oldVisibleCount);

							if(rowHeight!=newRowHeight)
							{
								int offset=oldBounds.Top()-rowHeight*startIndex;
								rowHeight=newRowHeight;
								suppressOnViewChanged=true;
								callback->OnTotalSizeChanged();
								callback->SetViewLocation(Point(0, rowHeight*newStartIndex+offset));
								suppressOnViewChanged=false;
							}
							startIndex=newStartIndex;
							RearrangeItemBounds();
						}
					}
				}

				FixedHeightItemArranger::FixedHeightItemArranger()
					:rowHeight(1)
					,suppressOnViewChanged(false)
				{
				}

				FixedHeightItemArranger::~FixedHeightItemArranger()
				{
				}

				int FixedHeightItemArranger::FindItem(int itemIndex, GuiListControl::KeyDirection key)
				{
					int count=itemProvider->Count();
					if(count==0) return -1;
					int groupCount=viewBounds.Height()/rowHeight;
					if(groupCount==0) groupCount=1;
					switch(key)
					{
					case GuiListControl::Up:
						itemIndex--;
						break;
					case GuiListControl::Down:
						itemIndex++;
						break;
					case GuiListControl::Home:
						itemIndex=0;
						break;
					case GuiListControl::End:
						itemIndex=count;
						break;
					case GuiListControl::PageUp:
						itemIndex-=groupCount;
						break;
					case GuiListControl::PageDown:
						itemIndex+=groupCount;
						break;
					default:
						return -1;
					}
					
					if(itemIndex<0) return 0;
					else if(itemIndex>=count) return count-1;
					else return itemIndex;
				}

				bool FixedHeightItemArranger::EnsureItemVisible(int itemIndex)
				{
					if(callback)
					{
						if(itemIndex<0 || itemIndex>=itemProvider->Count())
						{
							return false;
						}
						while(true)
						{
							int yOffset=GetYOffset();
							int top=itemIndex*rowHeight;
							int bottom=top+rowHeight+yOffset;

							if(viewBounds.Height()<rowHeight)
							{
								if(viewBounds.Top()<bottom && top<viewBounds.Bottom())
								{
									break;
								}
							}

							Point location=viewBounds.LeftTop();
							if(top<viewBounds.Top())
							{
								location.y=top;
							}
							else if(viewBounds.Bottom()<bottom)
							{
								location.y=bottom-viewBounds.Height();
							}
							else
							{
								break;
							}
							callback->SetViewLocation(location);
						}
						return true;
					}
					return false;
				}

/***********************************************************************
FixedSizeMultiColumnItemArranger
***********************************************************************/

				void FixedSizeMultiColumnItemArranger::RearrangeItemBounds()
				{
					int y0=-viewBounds.Top();
					int rowItems=viewBounds.Width()/itemSize.x;
					if(rowItems<1) rowItems=1;

					for(int i=0;i<visibleStyles.Count();i++)
					{
						GuiListControl::IItemStyleController* style=visibleStyles[i];
						int row=(startIndex+i)/rowItems;
						int col=(startIndex+i)%rowItems;
						callback->SetStyleBounds(style, Rect(Point(col*itemSize.x, y0+row*itemSize.y), itemSize));
					}
				}

				void FixedSizeMultiColumnItemArranger::CalculateRange(Size itemSize, Rect bounds, int count, int& start, int& end)
				{
					int startRow=bounds.Top()/itemSize.y;
					if(startRow<0) startRow=0;
					int endRow=(bounds.Bottom()-1)/itemSize.y;
					int cols=bounds.Width()/itemSize.x;
					if(cols<1) cols=1;

					start=startRow*cols;
					end=(endRow+1)*cols-1;
					if(end>=count) end=count-1;
				}

				void FixedSizeMultiColumnItemArranger::OnStylesCleared()
				{
					itemSize=Size(1, 1);
				}

				Size FixedSizeMultiColumnItemArranger::OnCalculateTotalSize()
				{
					if(callback)
					{
						int rowItems=viewBounds.Width()/itemSize.x;
						if(rowItems<1) rowItems=1;
						int rows=itemProvider->Count()/rowItems;
						if(itemProvider->Count()%rowItems) rows++;

						return Size(itemSize.x*rowItems, itemSize.y*rows);
					}
					else
					{
						return Size(0, 0);
					}
				}

				void FixedSizeMultiColumnItemArranger::OnViewChangedInternal(Rect oldBounds, Rect newBounds)
				{
					if(callback)
					{
						if(!suppressOnViewChanged)
						{
							int oldVisibleCount=visibleStyles.Count();
							Size newItemSize=itemSize;
							int endIndex=startIndex+visibleStyles.Count()-1;

							int newStartIndex=0;
							int newEndIndex=0;
							int itemCount=itemProvider->Count();
							CalculateRange(newItemSize, newBounds, itemCount, newStartIndex, newEndIndex);
							if(newItemSize==Size(1, 1) && newStartIndex<newEndIndex)
							{
								newEndIndex=newStartIndex;
							}

							int previousStartIndex=-1;
							int previousEndIndex=-1;

							while(true)
							{
								for(int i=newStartIndex;i<=newEndIndex;i++)
								{
									if(startIndex<=i && i<=endIndex)
									{
										GuiListControl::IItemStyleController* style=visibleStyles[i-startIndex];
										visibleStyles.Add(style);
									}
									else if(i<previousStartIndex || i>previousEndIndex)
									{
										GuiListControl::IItemStyleController* style=callback->RequestItem(i);

										if(i<previousStartIndex)
										{
											visibleStyles.Insert(oldVisibleCount+(i-newStartIndex), style);
										}
										else
										{
											visibleStyles.Add(style);
										}
										
										Size styleSize=callback->GetStylePreferredSize(style);
										if(newItemSize.x<styleSize.x) newItemSize.x=styleSize.x;
										if(newItemSize.y<styleSize.y) newItemSize.y=styleSize.y;
									}
								}

								int updatedStartIndex=0;
								int updatedEndIndex=0;
								CalculateRange(newItemSize, newBounds, itemCount, updatedStartIndex, updatedEndIndex);
								bool again=updatedStartIndex<newStartIndex || updatedEndIndex>newEndIndex;
								previousStartIndex=newStartIndex;
								previousEndIndex=newEndIndex;
								if(updatedStartIndex<newStartIndex) newStartIndex=updatedStartIndex;
								if(updatedEndIndex>newEndIndex) newEndIndex=updatedEndIndex;
								if(!again) break;
							}

							for(int i=0;i<oldVisibleCount;i++)
							{
								int index=startIndex+i;
								if(index<newStartIndex || newEndIndex<index)
								{
									GuiListControl::IItemStyleController* style=visibleStyles[i];
									callback->ReleaseItem(style);
								}
							}
							visibleStyles.RemoveRange(0, oldVisibleCount);

							if(itemSize!=newItemSize)
							{
								itemSize=newItemSize;
								suppressOnViewChanged=true;
								callback->OnTotalSizeChanged();
								suppressOnViewChanged=false;
							}
							startIndex=newStartIndex;
							RearrangeItemBounds();
						}
					}
				}

				FixedSizeMultiColumnItemArranger::FixedSizeMultiColumnItemArranger()
					:itemSize(1, 1)
					,suppressOnViewChanged(false)
				{
				}

				FixedSizeMultiColumnItemArranger::~FixedSizeMultiColumnItemArranger()
				{
				}

				int FixedSizeMultiColumnItemArranger::FindItem(int itemIndex, GuiListControl::KeyDirection key)
				{
					int count=itemProvider->Count();
					int columnCount=viewBounds.Width()/itemSize.x;
					if(columnCount==0) columnCount=1;
					int rowCount=viewBounds.Height()/itemSize.y;
					if(rowCount==0) rowCount=1;

					switch(key)
					{
					case GuiListControl::Up:
						itemIndex-=columnCount;
						break;
					case GuiListControl::Down:
						itemIndex+=columnCount;
						break;
					case GuiListControl::Left:
						itemIndex--;
						break;
					case GuiListControl::Right:
						itemIndex++;
						break;
					case GuiListControl::Home:
						itemIndex=0;
						break;
					case GuiListControl::End:
						itemIndex=count;
						break;
					case GuiListControl::PageUp:
						itemIndex-=columnCount*rowCount;
						break;
					case GuiListControl::PageDown:
						itemIndex+=columnCount*rowCount;
						break;
					case GuiListControl::PageLeft:
						itemIndex-=itemIndex%columnCount;
						break;
					case GuiListControl::PageRight:
						itemIndex+=columnCount-itemIndex%columnCount-1;
						break;
					default:
						return -1;
					}
					
					if(itemIndex<0) return 0;
					else if(itemIndex>=count) return count-1;
					else return itemIndex;
				}

				bool FixedSizeMultiColumnItemArranger::EnsureItemVisible(int itemIndex)
				{
					if(callback)
					{
						if(itemIndex<0 || itemIndex>=itemProvider->Count())
						{
							return false;
						}
						while(true)
						{
							int rowHeight=itemSize.y;
							int columnCount=viewBounds.Width()/itemSize.x;
							if(columnCount==0) columnCount=1;
							int rowIndex=itemIndex/columnCount;

							int top=rowIndex*rowHeight;
							int bottom=top+rowHeight;

							if(viewBounds.Height()<rowHeight)
							{
								if(viewBounds.Top()<bottom && top<viewBounds.Bottom())
								{
									break;
								}
							}

							Point location=viewBounds.LeftTop();
							if(top<viewBounds.Top())
							{
								location.y=top;
							}
							else if(viewBounds.Bottom()<bottom)
							{
								location.y=bottom-viewBounds.Height();
							}
							else
							{
								break;
							}
							callback->SetViewLocation(location);
						}
						return true;
					}
					return false;
				}

/***********************************************************************
FixedHeightMultiColumnItemArranger
***********************************************************************/

				void FixedHeightMultiColumnItemArranger::RearrangeItemBounds()
				{
					int rows=0;
					int startColumn=0;
					CalculateRange(itemHeight, viewBounds, rows, startColumn);
					int currentWidth=0;
					int totalWidth=0;
					for(int i=0;i<visibleStyles.Count();i++)
					{
						int column=i%rows;
						if(column==0)
						{
							totalWidth+=currentWidth;
							currentWidth=0;
						}
						GuiListControl::IItemStyleController* style=visibleStyles[i];
						int itemWidth=callback->GetStylePreferredSize(style).x;
						if(currentWidth<itemWidth) currentWidth=itemWidth;
						callback->SetStyleBounds(style, Rect(Point(totalWidth, itemHeight*column), Size(0, 0)));
					}
				}

				void FixedHeightMultiColumnItemArranger::CalculateRange(int itemHeight, Rect bounds, int& rows, int& startColumn)
				{
					rows=bounds.Height()/itemHeight;
					if(rows<1) rows=1;
					startColumn=bounds.Left()/bounds.Width();
				}

				void FixedHeightMultiColumnItemArranger::OnStylesCleared()
				{
					itemHeight=1;
				}

				Size FixedHeightMultiColumnItemArranger::OnCalculateTotalSize()
				{
					if(callback)
					{
						int rows=viewBounds.Height()/itemHeight;
						if(rows<1) rows=1;
						int columns=itemProvider->Count()/rows;
						if(itemProvider->Count()%rows) columns+=1;
						return Size(viewBounds.Width()*columns, 0);
					}
					else
					{
						return Size(0, 0);
					}
				}

				void FixedHeightMultiColumnItemArranger::OnViewChangedInternal(Rect oldBounds, Rect newBounds)
				{
					if(callback)
					{
						if(!suppressOnViewChanged)
						{
							int oldVisibleCount=visibleStyles.Count();
							int endIndex=startIndex+oldVisibleCount-1;

							int newItemHeight=itemHeight;
							int itemCount=itemProvider->Count();

							int previousStartIndex=-1;
							int previousEndIndex=-1;
							int newStartIndex=-1;
							int newEndIndex=-1;

							while(true)
							{
								int newRows=0;
								int newStartColumn=0;
								int currentWidth=0;
								int totalWidth=0;
								CalculateRange(newItemHeight, newBounds, newRows, newStartColumn);
								newStartIndex=newRows*newStartColumn;
								int currentItemHeight=newItemHeight;

								for(int i=newStartIndex;i<itemCount;i++)
								{
									if(i%newRows==0)
									{
										totalWidth+=currentWidth;
										currentWidth=0;
										if(totalWidth>=newBounds.Width())
										{
											break;
										}
									}
									newEndIndex=i;

									if(startIndex<=i && i<=endIndex)
									{
										GuiListControl::IItemStyleController* style=visibleStyles[i-startIndex];
										visibleStyles.Add(style);
									}
									else if(i<previousStartIndex || i>previousEndIndex)
									{
										GuiListControl::IItemStyleController* style=callback->RequestItem(i);

										if(i<previousStartIndex)
										{
											visibleStyles.Insert(oldVisibleCount+(i-newStartIndex), style);
										}
										else
										{
											visibleStyles.Add(style);
										}
										
										Size styleSize=callback->GetStylePreferredSize(style);
										if(currentWidth<styleSize.x) currentWidth=styleSize.x;
										if(newItemHeight<styleSize.y) newItemHeight=styleSize.y;
										if(currentItemHeight!=newItemHeight) break;
									}
								}

								if(previousStartIndex==-1 || previousStartIndex<newStartIndex) previousStartIndex=newStartIndex;
								if(previousEndIndex==-1 || previousEndIndex>newEndIndex) previousEndIndex=newEndIndex;
								if(currentItemHeight==newItemHeight)
								{
									break;
								}
							}
							newStartIndex=previousStartIndex;
							newEndIndex=previousEndIndex;

							for(int i=0;i<oldVisibleCount;i++)
							{
								int index=startIndex+i;
								if(index<newStartIndex || newEndIndex<index)
								{
									GuiListControl::IItemStyleController* style=visibleStyles[i];
									callback->ReleaseItem(style);
								}
							}
							visibleStyles.RemoveRange(0, oldVisibleCount);

							if(itemHeight!=newItemHeight)
							{
								itemHeight=newItemHeight;
								suppressOnViewChanged=true;
								callback->OnTotalSizeChanged();
								suppressOnViewChanged=false;
							}
							startIndex=newStartIndex;
							RearrangeItemBounds();
						}
					}
				}

				FixedHeightMultiColumnItemArranger::FixedHeightMultiColumnItemArranger()
					:itemHeight(1)
					,suppressOnViewChanged(false)
				{
				}

				FixedHeightMultiColumnItemArranger::~FixedHeightMultiColumnItemArranger()
				{
				}

				int FixedHeightMultiColumnItemArranger::FindItem(int itemIndex, GuiListControl::KeyDirection key)
				{
					int count=itemProvider->Count();
					int groupCount=viewBounds.Height()/itemHeight;
					if(groupCount==0) groupCount=1;
					switch(key)
					{
					case GuiListControl::Up:
						itemIndex--;
						break;
					case GuiListControl::Down:
						itemIndex++;
						break;
					case GuiListControl::Left:
						itemIndex-=groupCount;
						break;
					case GuiListControl::Right:
						itemIndex+=groupCount;
						break;
					case GuiListControl::Home:
						itemIndex=0;
						break;
					case GuiListControl::End:
						itemIndex=count;
						break;
					case GuiListControl::PageUp:
						itemIndex-=itemIndex%groupCount;
						break;
					case GuiListControl::PageDown:
						itemIndex+=groupCount-itemIndex%groupCount-1;
						break;
					default:
						return -1;
					}
					
					if(itemIndex<0) return 0;
					else if(itemIndex>=count) return count-1;
					else return itemIndex;
				}

				bool FixedHeightMultiColumnItemArranger::EnsureItemVisible(int itemIndex)
				{
					if(callback)
					{
						if(itemIndex<0 || itemIndex>=itemProvider->Count())
						{
							return false;
						}
						while(true)
						{
							int rowCount=viewBounds.Height()/itemHeight;
							if(rowCount==0) rowCount=1;
							int columnIndex=itemIndex/rowCount;
							int minIndex=startIndex;
							int maxIndex=startIndex+visibleStyles.Count()-1;

							Point location=viewBounds.LeftTop();
							if(minIndex<=itemIndex && itemIndex<=maxIndex)
							{
								Rect bounds=callback->GetStyleBounds(visibleStyles[itemIndex-startIndex]);
								if(0<bounds.Bottom() && bounds.Top()<viewBounds.Width() && bounds.Width()>viewBounds.Width())
								{
									break;
								}
								else if(bounds.Left()<0)
								{
									location.x-=viewBounds.Width();
								}
								else if(bounds.Right()>viewBounds.Width())
								{
									location.x+=viewBounds.Width();
								}
								else
								{
									break;
								}
							}
							else if(columnIndex<minIndex/rowCount)
							{
								location.x-=viewBounds.Width();
							}
							else if(columnIndex>=maxIndex/rowCount)
							{
								location.x+=viewBounds.Width();
							}
							else
							{
								break;
							}
							callback->SetViewLocation(location);
						}
						return true;
					}
					return false;
				}

/***********************************************************************
ItemStyleControllerBase
***********************************************************************/

				void ItemStyleControllerBase::Initialize(compositions::GuiBoundsComposition* _boundsComposition, GuiControl* _associatedControl)
				{
					boundsComposition=_boundsComposition;
					associatedControl=_associatedControl;
				}

				void ItemStyleControllerBase::Finalize()
				{
					if(boundsComposition && !isInstalled)
					{
						if(associatedControl)
						{
							delete associatedControl;
						}
						else
						{
							delete boundsComposition;
						}
					}
					boundsComposition=0;
					associatedControl=0;
				}

				ItemStyleControllerBase::ItemStyleControllerBase(GuiListControl::IItemStyleProvider* _provider, int _styleId)
					:provider(_provider)
					,styleId(_styleId)
					,boundsComposition(0)
					,associatedControl(0)
					,isInstalled(false)
				{
				}

				ItemStyleControllerBase::~ItemStyleControllerBase()
				{
					Finalize();
				}
					
				GuiListControl::IItemStyleProvider* ItemStyleControllerBase::GetStyleProvider()
				{
					return provider;
				}

				int ItemStyleControllerBase::GetItemStyleId()
				{
					return styleId;
				}

				compositions::GuiBoundsComposition* ItemStyleControllerBase::GetBoundsComposition()
				{
					return boundsComposition;
				}

				bool ItemStyleControllerBase::IsCacheable()
				{
					return true;
				}

				bool ItemStyleControllerBase::IsInstalled()
				{
					return isInstalled;
				}

				void ItemStyleControllerBase::OnInstalled()
				{
					isInstalled=true;
				}

				void ItemStyleControllerBase::OnUninstalled()
				{
					isInstalled=false;
				}

/***********************************************************************
ItemProviderBase
***********************************************************************/

				void ItemProviderBase::InvokeOnItemModified(int start, int count, int newCount)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnItemModified(start, count, newCount);
					}
				}

				ItemProviderBase::ItemProviderBase()
				{
				}

				ItemProviderBase::~ItemProviderBase()
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnAttached(0);
					}
				}

				bool ItemProviderBase::AttachCallback(GuiListControl::IItemProviderCallback* value)
				{
					if(callbacks.Contains(value))
					{
						return false;
					}
					else
					{
						callbacks.Add(value);
						value->OnAttached(this);
						return true;
					}
				}

				bool ItemProviderBase::DetachCallback(GuiListControl::IItemProviderCallback* value)
				{
					int index=callbacks.IndexOf(value);
					if(index==-1)
					{
						return false;
					}
					else
					{
						value->OnAttached(0);
						callbacks.Remove(value);
						return true;
					}
				}
			}
		}
	}
}