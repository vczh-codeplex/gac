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

			void GuiDatePicker::StyleController::DisplayMonth(int year, vint month)
			{
			}

			void GuiDatePicker::StyleController::SelectDay(vint day)
			{
			}

			GuiDatePicker::StyleController::StyleController(IStyleProvider* _styleProvider)
				:styleProvider(_styleProvider)
				,datePicker(0)
			{
				GuiTableComposition* monthTable=0;
				GuiTableComposition* dayTable=0;
				{
					listYears=styleProvider->CreateTextList();
					for(vint i=YearFirst;i<=YearLast;i++)
					{
						listYears->GetItems().Add(new list::TextItem(itow(i)));
					}
					comboYear=new GuiComboBoxListControl(styleProvider->CreateComboBoxStyle(), listYears);
					comboYear->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 1, 0));
				}
				{
					listMonths=styleProvider->CreateTextList();
					comboMonth=new GuiComboBoxListControl(styleProvider->CreateComboBoxStyle(), listMonths);
					comboMonth->GetBoundsComposition()->SetAlignmentToParent(Margin(1, 0, 0, 0));
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
					dayTable->SetCellPadding(2);
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
							cell->AddChild(button->GetBoundsComposition());
							buttonDays[j*DaysOfWeek+i]=button;

							GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
							element->SetAlignments(Alignment::Center, Alignment::Center);
							element->SetText(L"0");
							labelDays[j*DaysOfWeek+i]=element;

							GuiBoundsComposition* elementBounds=new GuiBoundsComposition;
							elementBounds->SetOwnedElement(element);
							elementBounds->SetAlignmentToParent(Margin(0, 0, 0, 0));
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
			}

			const DateTime& GuiDatePicker::StyleController::GetDate()
			{
				return currentDate;
			}

			void GuiDatePicker::StyleController::SetDate(const DateTime& value)
			{
				currentDate=value;
				DisplayMonth(value.year, value.month);
				SelectDay(value.day);
			}

/***********************************************************************
GuiDatePicker
***********************************************************************/

			void GuiDatePicker::NotifyDateChanged()
			{
				DateChanged.Execute(GetNotifyEventArguments());
				GuiControl::SetText(dateLocale.FormatDate(dateFormat, styleController->GetDate()));
			}

			GuiDatePicker::GuiDatePicker(IStyleProvider* _styleProvider)
				:GuiControl(new StyleController(_styleProvider))
			{
				styleController=dynamic_cast<StyleController*>(GetStyleController());
				styleController->SetDatePicker(this);
				styleController->SetDate(DateTime::LocalTime());

				dateLocale=Locale::UserDefault();
				List<WString> formats;
				dateLocale.GetLongDateFormats(formats);
				if(formats.Count()>0)
				{
					dateFormat=formats[0];
				}
				styleController->SetDateLocale(dateLocale);

				DateChanged.SetAssociatedComposition(GetBoundsComposition());
				DateFormatChanged.SetAssociatedComposition(GetBoundsComposition());
				DateLocaleChanged.SetAssociatedComposition(GetBoundsComposition());
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
				DateFormatChanged.Execute(GetNotifyEventArguments());
			}

			const Locale& GuiDatePicker::GetDateLocale()
			{
				return dateLocale;
			}

			void GuiDatePicker::SetDateLocale(const Locale& value)
			{
				dateLocale=value;
				styleController->SetDateLocale(dateLocale);
				DateLocaleChanged.Execute(GetNotifyEventArguments());
			}

			void GuiDatePicker::SetText(const WString& value)
			{
			}
		}
	}
}