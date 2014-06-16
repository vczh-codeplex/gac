#include "GuiDateTimeControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;
			using namespace compositions;
			using namespace elements;

/***********************************************************************
GuiDatePicker::StyleController
***********************************************************************/

			vint GetDayCountForMonth(vint year, vint month)
			{
				bool isLeapYear=(year%100==0)?(year%400==0):(year%4==0);
				switch(month)
				{
				case 1:case 3:case 5:case 7:case 8:case 10:case 12:
					return 31;
				case 4:case 6:case 9:case 11:
					return 30;
				default:
					return isLeapYear?29:28;
				}
			}

			void StepPreviousMonth(vint& year, vint& month)
			{
				if(month==1)
				{
					year--;
					month=12;
				}
				else
				{
					month--;
				}
			}

			void StepNextMonth(vint& year, vint& month)
			{
				if(month==12)
				{
					year++;
					month=1;
				}
				else
				{
					month++;
				}
			}

			void GuiDatePicker::StyleController::SetDay(const DateTime& day, vint& index, bool currentMonth)
			{
				dateDays[index]=day;
				GuiSolidLabelElement* label=labelDays[index];
				label->SetText(itow(day.day));
				label->SetColor(currentMonth?styleProvider->GetPrimaryTextColor():styleProvider->GetSecondaryTextColor());
				index++;
			}

			void GuiDatePicker::StyleController::comboYearMonth_SelectedIndexChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(!preventComboEvent)
				{
					if(comboYear->GetSelectedIndex()!=-1 && comboMonth->GetSelectedIndex()!=-1)
					{
						vint year=comboYear->GetSelectedIndex()+YearFirst;
						vint month=comboMonth->GetSelectedIndex()+1;
						SetDate(DateTime::FromDateTime(year, month, 1));
						datePicker->NotifyDateChanged();
						datePicker->DateNavigated.Execute(datePicker->GetNotifyEventArguments());
					}
				}
			}

			void GuiDatePicker::StyleController::buttonDay_SelectedChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(!preventButtonEvent)
				{
					GuiSelectableButton* button=dynamic_cast<GuiSelectableButton*>(sender->GetRelatedControl());
					if(button->GetSelected())
					{
						vint index=buttonDays.IndexOf(button);
						if(index!=-1)
						{
							DateTime day=dateDays[index];
							if(day.year!=currentDate.year || day.month!=currentDate.month)
							{
								SetDate(day);
							}
							else
							{
								currentDate=day;
							}
							datePicker->NotifyDateChanged();
							datePicker->DateSelected.Execute(datePicker->GetNotifyEventArguments());
						}
					}
				}
			}

			void GuiDatePicker::StyleController::DisplayMonth(vint year, vint month)
			{
				if(YearFirst<=year && year<=YearLast && 1<=month && month<=12)
				{
					preventComboEvent=true;
					comboYear->SetSelectedIndex(year-YearFirst);
					comboMonth->SetSelectedIndex(month-1);
					preventComboEvent=false;
				}

				vint yearPrev=year, yearNext=year, monthPrev=month, monthNext=month;
				StepPreviousMonth(yearPrev, monthPrev);
				StepNextMonth(yearNext, monthNext);

				vint countPrev=GetDayCountForMonth(yearPrev, monthPrev);
				vint count=GetDayCountForMonth(year, month);
				vint countNext=GetDayCountForMonth(yearNext, monthNext);

				DateTime firstDay=DateTime::FromDateTime(year, month, 1);
				vint showPrev=firstDay.dayOfWeek;
				if(showPrev==0) showPrev=DaysOfWeek;
				vint show=count;
				vint showNext=DaysOfWeek*DayRows-showPrev-show;

				vint index=0;
				for(vint i=0;i<showPrev;i++)
				{
					DateTime day=DateTime::FromDateTime(yearPrev, monthPrev, countPrev-(showPrev-i-1));
					SetDay(day, index, false);
				}
				for(vint i=0;i<show;i++)
				{
					DateTime day=DateTime::FromDateTime(year, month, i+1);
					SetDay(day, index, true);
				}
				for(vint i=0;i<showNext;i++)
				{
					DateTime day=DateTime::FromDateTime(yearNext, monthNext, i+1);
					SetDay(day, index, false);
				}
			}

			void GuiDatePicker::StyleController::SelectDay(vint day)
			{
				for(vint i=0;i<dateDays.Count();i++)
				{
					const DateTime& dt=dateDays[i];
					if(dt.year==currentDate.year && dt.month==currentDate.month && dt.day==day)
					{
						preventButtonEvent=true;
						buttonDays[i]->SetSelected(true);
						preventButtonEvent=false;
						break;
					}
				}
			}

			GuiDatePicker::StyleController::StyleController(IStyleProvider* _styleProvider)
				:styleProvider(_styleProvider)
				,datePicker(0)
				,preventComboEvent(false)
				,preventButtonEvent(false)
			{
				GuiTableComposition* monthTable=0;
				GuiTableComposition* dayTable=0;
				{
					listYears=styleProvider->CreateTextList();
					listYears->SetHorizontalAlwaysVisible(false);
					for(vint i=YearFirst;i<=YearLast;i++)
					{
						listYears->GetItems().Add(new list::TextItem(itow(i)));
					}
					comboYear=new GuiComboBoxListControl(styleProvider->CreateComboBoxStyle(), listYears);
					comboYear->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 2, 0));
					comboYear->SelectedIndexChanged.AttachMethod(this, &StyleController::comboYearMonth_SelectedIndexChanged);
				}
				{
					listMonths=styleProvider->CreateTextList();
					listMonths->SetHorizontalAlwaysVisible(false);
					comboMonth=new GuiComboBoxListControl(styleProvider->CreateComboBoxStyle(), listMonths);
					comboMonth->GetBoundsComposition()->SetAlignmentToParent(Margin(2, 0, 0, 0));
					comboMonth->SelectedIndexChanged.AttachMethod(this, &StyleController::comboYearMonth_SelectedIndexChanged);
				}
				{
					monthTable=new GuiTableComposition;
					monthTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
					monthTable->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					monthTable->SetRowsAndColumns(1, 2);
					monthTable->SetRowOption(0, GuiCellOption::MinSizeOption());
					monthTable->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
					monthTable->SetColumnOption(1, GuiCellOption::PercentageOption(0.5));
					{
						GuiCellComposition* cell=new GuiCellComposition;
						monthTable->AddChild(cell);
						cell->SetSite(0, 0, 1, 1);
						cell->AddChild(comboYear->GetBoundsComposition());
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						monthTable->AddChild(cell);
						cell->SetSite(0, 1, 1, 1);
						cell->AddChild(comboMonth->GetBoundsComposition());
					}
				}
				{
					dayTable=new GuiTableComposition;
					dayTable->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					dayTable->SetCellPadding(4);
					dayTable->SetRowsAndColumns(DayRows+DayRowStart, DaysOfWeek);

					for(vint i=0;i<DayRowStart;i++)
					{
						dayTable->SetRowOption(i, GuiCellOption::MinSizeOption());
					}
					for(vint i=0;i<DayRows;i++)
					{
						dayTable->SetRowOption(i+DayRowStart, GuiCellOption::PercentageOption(1.0));
					}
					for(vint i=0;i<DaysOfWeek;i++)
					{
						dayTable->SetColumnOption(i, GuiCellOption::PercentageOption(1.0));
					}

					{
						GuiCellComposition* cell=new GuiCellComposition;
						dayTable->AddChild(cell);
						cell->SetSite(0, 0, 1, DaysOfWeek);
						cell->AddChild(monthTable);
					}

					labelDaysOfWeek.Resize(7);
					for(vint i=0;i<DaysOfWeek;i++)
					{
						GuiCellComposition* cell=new GuiCellComposition;
						dayTable->AddChild(cell);
						cell->SetSite(1, i, 1, 1);

						GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
						element->SetAlignments(Alignment::Center, Alignment::Center);
						element->SetColor(styleProvider->GetPrimaryTextColor());
						labelDaysOfWeek[i]=element;
						cell->SetOwnedElement(element);
					}

					buttonDays.Resize(DaysOfWeek*DayRows);
					labelDays.Resize(DaysOfWeek*DayRows);
					dateDays.Resize(DaysOfWeek*DayRows);
					dayMutexController=new GuiSelectableButton::MutexGroupController;
					for(vint i=0;i<DaysOfWeek;i++)
					{
						for(vint j=0;j<DayRows;j++)
						{
							GuiCellComposition* cell=new GuiCellComposition;
							dayTable->AddChild(cell);
							cell->SetSite(j+DayRowStart, i, 1, 1);

							GuiSelectableButton* button=new GuiSelectableButton(styleProvider->CreateDateButtonStyle());
							button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
							button->SetGroupController(dayMutexController.Obj());
							button->SelectedChanged.AttachMethod(this, &StyleController::buttonDay_SelectedChanged);
							cell->AddChild(button->GetBoundsComposition());
							buttonDays[j*DaysOfWeek+i]=button;

							GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
							element->SetAlignments(Alignment::Center, Alignment::Center);
							element->SetText(L"0");
							labelDays[j*DaysOfWeek+i]=element;

							GuiBoundsComposition* elementBounds=new GuiBoundsComposition;
							elementBounds->SetOwnedElement(element);
							elementBounds->SetAlignmentToParent(Margin(0, 0, 0, 0));
							elementBounds->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
							button->GetContainerComposition()->AddChild(elementBounds);
						}
					}
				}
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(styleProvider->GetBackgroundColor());
					dayTable->SetOwnedElement(element);
				}

				boundsComposition=dayTable;
			}

			GuiDatePicker::StyleController::~StyleController()
			{
				delete styleProvider;
			}

			compositions::GuiBoundsComposition* GuiDatePicker::StyleController::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* GuiDatePicker::StyleController::GetContainerComposition()
			{
				return boundsComposition;
			}

			void GuiDatePicker::StyleController::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void GuiDatePicker::StyleController::SetText(const WString& value)
			{
			}

			void GuiDatePicker::StyleController::SetFont(const FontProperties& value)
			{
				comboYear->SetFont(value);
				listYears->SetFont(value);
				comboMonth->SetFont(value);
				listMonths->SetFont(value);
				FOREACH(GuiSolidLabelElement*, label, From(labelDaysOfWeek).Concat(labelDays))
				{
					label->SetFont(value);
				}
			}

			void GuiDatePicker::StyleController::SetVisuallyEnabled(bool value)
			{
			}
					
			void GuiDatePicker::StyleController::SetDatePicker(GuiDatePicker* _datePicker)
			{
				datePicker=_datePicker;
			}

			void GuiDatePicker::StyleController::SetDateLocale(const Locale& _dateLocale)
			{
				dateLocale=_dateLocale;
				for(vint i=0;i<DaysOfWeek;i++)
				{
					labelDaysOfWeek[i]->SetText(dateLocale.GetShortDayOfWeekName(i));
				}
				
				listMonths->GetItems().Clear();
				for(vint i=1;i<=12;i++)
				{
					listMonths->GetItems().Add(new list::TextItem(dateLocale.GetLongMonthName(i)));
				}

				SetDate(currentDate, true);
			}

			const DateTime& GuiDatePicker::StyleController::GetDate()
			{
				return currentDate;
			}

			void GuiDatePicker::StyleController::SetDate(const DateTime& value, bool forceUpdate)
			{
				if(forceUpdate || currentDate.year!=value.year || currentDate.month!=value.month || currentDate.day!=value.day)
				{
					currentDate=value;
					DisplayMonth(value.year, value.month);
					SelectDay(value.day);
				}
			}

/***********************************************************************
GuiDatePicker
***********************************************************************/

			void GuiDatePicker::UpdateText()
			{
				GuiControl::SetText(dateLocale.FormatDate(dateFormat, styleController->GetDate()));
			}

			void GuiDatePicker::NotifyDateChanged()
			{
				UpdateText();
				DateChanged.Execute(GetNotifyEventArguments());
			}

			GuiDatePicker::GuiDatePicker(IStyleProvider* _styleProvider)
				:GuiControl(new StyleController(_styleProvider))
			{
				styleController=dynamic_cast<StyleController*>(GetStyleController());
				styleController->SetDatePicker(this);

				SetDateLocale(Locale::UserDefault());
				SetDate(DateTime::LocalTime());

				DateChanged.SetAssociatedComposition(GetBoundsComposition());
				DateNavigated.SetAssociatedComposition(GetBoundsComposition());
				DateSelected.SetAssociatedComposition(GetBoundsComposition());
				DateFormatChanged.SetAssociatedComposition(GetBoundsComposition());
				DateLocaleChanged.SetAssociatedComposition(GetBoundsComposition());

				NotifyDateChanged();
			}

			GuiDatePicker::~GuiDatePicker()
			{
			}

			const DateTime& GuiDatePicker::GetDate()
			{
				return styleController->GetDate();
			}

			void GuiDatePicker::SetDate(const DateTime& value)
			{
				styleController->SetDate(value);
			}

			const WString& GuiDatePicker::GetDateFormat()
			{
				return dateFormat;
			}

			void GuiDatePicker::SetDateFormat(const WString& value)
			{
				dateFormat=value;
				UpdateText();
				DateFormatChanged.Execute(GetNotifyEventArguments());
			}

			const Locale& GuiDatePicker::GetDateLocale()
			{
				return dateLocale;
			}

			void GuiDatePicker::SetDateLocale(const Locale& value)
			{
				dateLocale=value;
				List<WString> formats;
				dateLocale.GetLongDateFormats(formats);
				if(formats.Count()>0)
				{
					dateFormat=formats[0];
				}
				styleController->SetDateLocale(dateLocale);

				UpdateText();
				DateFormatChanged.Execute(GetNotifyEventArguments());
				DateLocaleChanged.Execute(GetNotifyEventArguments());
			}

			void GuiDatePicker::SetText(const WString& value)
			{
			}

/***********************************************************************
GuiDateComboBox
***********************************************************************/

			void GuiDateComboBox::UpdateText()
			{
				SetText(datePicker->GetDateLocale().FormatDate(datePicker->GetDateFormat(), selectedDate));
			}

			void GuiDateComboBox::NotifyUpdateSelectedDate()
			{
				UpdateText();
				SelectedDateChanged.Execute(GetNotifyEventArguments());
			}

			void GuiDateComboBox::OnSubMenuOpeningChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				datePicker->SetDate(selectedDate);
			}

			void GuiDateComboBox::datePicker_DateLocaleChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				UpdateText();
			}

			void GuiDateComboBox::datePicker_DateFormatChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				UpdateText();
			}

			void GuiDateComboBox::datePicker_DateSelected(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				selectedDate=datePicker->GetDate();
				GetSubMenu()->Hide();
				SelectItem();
				NotifyUpdateSelectedDate();
			}

			GuiDateComboBox::GuiDateComboBox(IStyleController* _styleController, GuiDatePicker* _datePicker)
				:GuiComboBoxBase(_styleController)
				,datePicker(_datePicker)
			{
				SelectedDateChanged.SetAssociatedComposition(GetBoundsComposition());

				datePicker->DateSelected.AttachMethod(this, &GuiDateComboBox::datePicker_DateSelected);
				datePicker->DateLocaleChanged.AttachMethod(this, &GuiDateComboBox::datePicker_DateLocaleChanged);
				datePicker->DateFormatChanged.AttachMethod(this, &GuiDateComboBox::datePicker_DateFormatChanged);
				datePicker->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				GetSubMenu()->GetContainerComposition()->AddChild(datePicker->GetBoundsComposition());

				selectedDate=datePicker->GetDate();
				SubMenuOpeningChanged.AttachMethod(this, &GuiDateComboBox::OnSubMenuOpeningChanged);
				SetFont(GetFont());
				SetText(datePicker->GetText());
			}

			GuiDateComboBox::~GuiDateComboBox()
			{
			}

			void GuiDateComboBox::SetFont(const FontProperties& value)
			{
				GuiComboBoxBase::SetFont(value);
				datePicker->SetFont(value);
			}

			const DateTime& GuiDateComboBox::GetSelectedDate()
			{
				return selectedDate;
			}

			void GuiDateComboBox::SetSelectedDate(const DateTime& value)
			{
				selectedDate=value;
				NotifyUpdateSelectedDate();
			}

			GuiDatePicker* GuiDateComboBox::GetDatePicker()
			{
				return datePicker;
			}
		}
	}
}