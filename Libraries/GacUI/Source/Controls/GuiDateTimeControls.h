/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIDATETIMECONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIDATETIMECONTROLS

#include "ListControlPackage\GuiComboControls.h"
#include "ListControlPackage\GuiTextListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
DatePicker
***********************************************************************/

			class GuiDatePicker : public GuiControl, public Description<GuiDatePicker>
			{
			public:
				class IStyleProvider : public virtual GuiControl::IStyleProvider, public Description<IStyleProvider>
				{
				public:
					virtual GuiSelectableButton::IStyleController*		CreateDateButtonStyle()=0;
					virtual GuiTextList*								CreateTextList()=0;
					virtual GuiComboBoxListControl::IStyleController*	CreateComboBoxStyle()=0;

					virtual Color										GetBackgroundColor()=0;
					virtual Color										GetPrimaryTextColor()=0;
					virtual Color										GetSecondaryTextColor()=0;
				};

				class StyleController : public Object, public virtual GuiControl::IStyleController, public Description<StyleController>
				{
				protected:
					static const vint									DaysOfWeek=7;
					static const vint									DayRows=6;
					static const vint									DayRowStart=2;
					static const vint									YearFirst=1990;
					static const vint									YearLast=2099;

					IStyleProvider*										styleProvider;
					GuiDatePicker*										datePicker;
					DateTime											currentDate;
					Locale												dateLocale;
					compositions::GuiTableComposition*					boundsComposition;

					GuiComboBoxListControl*								comboYear;
					GuiTextList*										listYears;
					GuiComboBoxListControl*								comboMonth;
					GuiTextList*										listMonths;
					collections::Array<elements::GuiSolidLabelElement*>	labelDaysOfWeek;
					collections::Array<GuiSelectableButton*>			buttonDays;
					collections::Array<elements::GuiSolidLabelElement*>	labelDays;
					collections::Array<DateTime>						dateDays;
					Ptr<GuiSelectableButton::GroupController>			dayMutexController;

					void												SetDay(const DateTime& day, vint& index, bool currentMonth);
					void												DisplayMonth(vint year, vint month);
					void												SelectDay(vint day);
				public:
					StyleController(IStyleProvider* _styleProvider);
					~StyleController();

					compositions::GuiBoundsComposition*					GetBoundsComposition()override;
					compositions::GuiGraphicsComposition*				GetContainerComposition()override;
					void												SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
					void												SetText(const WString& value)override;
					void												SetFont(const FontProperties& value)override;
					void												SetVisuallyEnabled(bool value)override;
					
					void												SetDatePicker(GuiDatePicker* _datePicker);
					void												SetDateLocale(const Locale& _dateLocale);
					const DateTime&										GetDate();
					void												SetDate(const DateTime& value);
				};

			protected:
				StyleController*										styleController;
				WString													dateFormat;
				Locale													dateLocale;

				void													NotifyDateChanged();
			public:
				GuiDatePicker(IStyleProvider* _styleProvider);
				~GuiDatePicker();

				compositions::GuiNotifyEvent							DateChanged;
				compositions::GuiNotifyEvent							DateFormatChanged;
				compositions::GuiNotifyEvent							DateLocaleChanged;

				const DateTime&											GetDate();
				void													SetDate(const DateTime& value);
				const WString&											GetDateFormat();
				void													SetDateFormat(const WString& value);
				const Locale&											GetDateLocale();
				void													SetDateLocale(const Locale& value);

				void													SetText(const WString& value)override;
			};
		}
	}
}

#endif