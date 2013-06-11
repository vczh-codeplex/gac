#include "GuiDateTimeControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;

/***********************************************************************
GuiDatePicker::StyleController
***********************************************************************/

			GuiDatePicker::StyleController::StyleController(IStyleProvider* _styleProvider)
				:styleProvider(_styleProvider)
				,datePicker(0)
			{
			}

			GuiDatePicker::StyleController::~StyleController()
			{
			}

			compositions::GuiBoundsComposition* GuiDatePicker::StyleController::GetBoundsComposition()
			{
				return 0;
			}

			compositions::GuiGraphicsComposition* GuiDatePicker::StyleController::GetContainerComposition()
			{
				return 0;
			}

			void GuiDatePicker::StyleController::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void GuiDatePicker::StyleController::SetText(const WString& value)
			{
			}

			void GuiDatePicker::StyleController::SetFont(const FontProperties& value)
			{
			}

			void GuiDatePicker::StyleController::SetVisuallyEnabled(bool value)
			{
			}
					
			void GuiDatePicker::StyleController::SetDatePicker(GuiDatePicker* _datePicker)
			{
				datePicker=_datePicker;
			}

			const DateTime& GuiDatePicker::StyleController::GetDate()
			{
				return currentDate;
			}

			void GuiDatePicker::StyleController::SetDate(const DateTime& value)
			{
				currentDate=value;
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
				DateLocaleChanged.Execute(GetNotifyEventArguments());
			}

			void GuiDatePicker::SetText(const WString& value)
			{
			}
		}
	}
}