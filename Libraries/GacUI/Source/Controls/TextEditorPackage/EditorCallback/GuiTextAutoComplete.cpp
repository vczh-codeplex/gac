#include "GuiTextAutoComplete.h"
#include "../../Styles/GuiThemeStyleFactory.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;

/***********************************************************************
GuiTextBoxAutoCompleteBase
***********************************************************************/

			bool GuiTextBoxAutoCompleteBase::IsPrefix(const WString& prefix, const WString& candidate)
			{
				if(candidate.Length()>=prefix.Length())
				{
					if(INVLOC.Compare(prefix, candidate.Sub(0, prefix.Length()), Locale::IgnoreCase)==0)
					{
						return true;
					}
				}
				return false;
			}

			GuiTextBoxAutoCompleteBase::GuiTextBoxAutoCompleteBase()
				:element(0)
				,elementModifyLock(0)
				,ownerComposition(0)
			{
				autoCompleteList=new GuiTextList(theme::GetCurrentTheme()->CreateTextListStyle(), theme::GetCurrentTheme()->CreateTextListItemStyle());
				autoCompleteList->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				autoCompleteList->SetHorizontalAlwaysVisible(false);
				autoCompleteList->SetVerticalAlwaysVisible(false);

				autoCompletePopup=new GuiPopup(theme::GetCurrentTheme()->CreateMenuStyle());
				autoCompletePopup->AddChild(autoCompleteList);
			}

			GuiTextBoxAutoCompleteBase::~GuiTextBoxAutoCompleteBase()
			{
				delete autoCompletePopup;
			}

			void GuiTextBoxAutoCompleteBase::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, compositions::GuiGraphicsComposition* _ownerComposition, vuint editVersion)
			{
				if(_element)
				{
					SPIN_LOCK(_elementModifyLock)
					{
						element=_element;
						elementModifyLock=&_elementModifyLock;
						ownerComposition=_ownerComposition;
					}
				}
			}

			void GuiTextBoxAutoCompleteBase::Detach()
			{
				if(element && elementModifyLock)
				{
					SPIN_LOCK(*elementModifyLock)
					{
						element=0;
						elementModifyLock=0;
					}
				}
			}

			void GuiTextBoxAutoCompleteBase::TextEditPreview(TextEditPreviewStruct& arguments)
			{
			}

			void GuiTextBoxAutoCompleteBase::TextEditNotify(const TextEditNotifyStruct& arguments)
			{
				if(element && elementModifyLock)
				{
					if(IsListOpening())
					{
						TextPos begin=GetListStartPosition();
						TextPos end=arguments.inputEnd;
						WString editingText=element->GetLines().GetText(begin, end);
						HighlightList(editingText);
					}
				}
			}

			void GuiTextBoxAutoCompleteBase::TextCaretChanged(const TextCaretChangedStruct& arguments)
			{
			}

			void GuiTextBoxAutoCompleteBase::TextEditFinished(vuint editVersion)
			{
			}

			bool GuiTextBoxAutoCompleteBase::IsListOpening()
			{
				return autoCompletePopup->GetOpening();
			}

			void GuiTextBoxAutoCompleteBase::OpenList(TextPos startPosition)
			{
				if(element && elementModifyLock)
				{
					autoCompleteStartPosition=startPosition;
					Rect bounds=element->GetLines().GetRectFromTextPos(startPosition);
					Point viewPosition=element->GetViewPosition();
					GuiControl* ownerControl=ownerComposition->GetRelatedControl();
					Rect compositionBounds=ownerComposition->GetGlobalBounds();
					Rect controlBounds=ownerControl->GetBoundsComposition()->GetGlobalBounds();
					vint px=compositionBounds.x1-controlBounds.x1-viewPosition.x;
					vint py=compositionBounds.y1-controlBounds.y1-viewPosition.y;
					bounds.x1+=px;
					bounds.x2+=px;
					bounds.y1+=py+5;
					bounds.y2+=py+5;
					autoCompletePopup->ShowPopup(ownerControl, bounds, true);
				}
			}

			void GuiTextBoxAutoCompleteBase::CloseList()
			{
				autoCompletePopup->Close();
			}

			void GuiTextBoxAutoCompleteBase::SetListContent(const collections::SortedList<WString>& items)
			{
				if(items.Count()==0)
				{
					CloseList();
				}
				List<WString> sortedItems;
				CopyFrom(
					sortedItems,
					From(items)
						.OrderBy([](const WString& a, const WString& b)
						{
							return INVLOC.Compare(a, b, Locale::IgnoreCase);
						})
					);

				autoCompleteList->GetItems().Clear();
				CopyFrom(
					autoCompleteList->GetItems(),
					From(sortedItems)
						.Select([](const WString& item)
						{
							return new list::TextItem(item);
						})
					);
				autoCompleteList->GetBoundsComposition()->SetPreferredMinSize(Size(200, 200));
			}

			TextPos GuiTextBoxAutoCompleteBase::GetListStartPosition()
			{
				return autoCompleteStartPosition;
			}

			bool GuiTextBoxAutoCompleteBase::SelectPreviousListItem()
			{
				if(!IsListOpening()) return false;
				if(autoCompleteList->GetSelectedItems().Count()==0)
				{
					autoCompleteList->SetSelected(0, true);
				}
				else
				{
					vint index=autoCompleteList->GetSelectedItems()[0];
					if(index>0) index--;
					autoCompleteList->SetSelected(index, true);
				}
				return true;
			}

			bool GuiTextBoxAutoCompleteBase::SelectNextListItem()
			{
				if(!IsListOpening()) return false;
				if(autoCompleteList->GetSelectedItems().Count()==0)
				{
					autoCompleteList->SetSelected(0, true);
				}
				else
				{
					vint index=autoCompleteList->GetSelectedItems()[0];
					if(index<autoCompleteList->GetItems().Count()-1) index++;
					autoCompleteList->SetSelected(index, true);
				}
				return true;
			}

			bool GuiTextBoxAutoCompleteBase::ApplySelectedListItem()
			{
				if(!IsListOpening()) return false;
				if(!ownerComposition) return false;
				if(autoCompleteList->GetSelectedItems().Count()==0) return false;
				GuiTextBoxCommonInterface* ci=dynamic_cast<GuiTextBoxCommonInterface*>(ownerComposition->GetRelatedControl());
				if(!ci) return false;

				vint index=autoCompleteList->GetSelectedItems()[0];
				WString selectedItem=autoCompleteList->GetItems()[index]->GetText();
				TextPos begin=autoCompleteStartPosition;
				TextPos end=ci->GetCaretEnd();
				ci->Select(begin, end);
				ci->SetSelectionText(selectedItem);
				CloseList();
				return true;
			}

			WString GuiTextBoxAutoCompleteBase::GetSelectedListItem()
			{
				if(!IsListOpening()) return L"";
				if(autoCompleteList->GetSelectedItems().Count()==0) return L"";
				vint index=autoCompleteList->GetSelectedItems()[0];
				return autoCompleteList->GetItems()[index]->GetText();
			}

			void GuiTextBoxAutoCompleteBase::HighlightList(const WString& editingText)
			{
				if(IsListOpening())
				{
					vint first=0;
					vint last=autoCompleteList->GetItems().Count()-1;
					vint selected=-1;

					while(first<=last)
					{
						vint middle=(first+last)/2;
						WString text=autoCompleteList->GetItems()[middle]->GetText();
						if(IsPrefix(editingText, text))
						{
							selected=middle;
							break;
						}

						vint result=INVLOC.Compare(editingText, text, Locale::IgnoreCase);
						if(result<=0)
						{
							last=middle-1;
						}
						else
						{
							first=middle+1;
						}
					}

					while(selected>0)
					{
						WString text=autoCompleteList->GetItems()[selected-1]->GetText();
						if(IsPrefix(editingText, text))
						{
							selected--;
						}
						else
						{
							break;
						}
					}

					if(selected!=-1)
					{
						autoCompleteList->SetSelected(selected, true);
						autoCompleteList->EnsureItemVisible(selected);
					}
				}
			}
		}
	}
}