#include "..\..\Public\Source\GacUIIncludes.h"

namespace SetupDialogWindowHelper
{
	GuiComboBoxListControl* CreateComboBox(const wchar_t** items, int count)
	{
		GuiTextList* listControl=new GuiTextList(new win7::Win7MultilineTextBoxProvider, new win7::Win7TextListProvider);
		listControl->SetHorizontalAlwaysVisible(false);
		listControl->SetVerticalAlwaysVisible(true);
		for(int i=0;i<count;i++)
		{
			listControl->GetItems().Add(list::TextItem(items[i]));
		}

		GuiComboBoxListControl* comboBox=new GuiComboBoxListControl(new win7::Win7DropDownComboBoxStyle, listControl);
		comboBox->SetSelectedIndex(0);
		return comboBox;
	}
}
using namespace SetupDialogWindowHelper;

void SetupDialogWindow(GuiControlHost* controlHost, GuiControl* container)
{
	{
		const wchar_t* input[]={L"OK", L"OKCancel", L"YesNo", L"YesNoCancel", L"RetryCancel", L"AbortRetryIgnore", L"CancelTryAgainContinue"};
		const wchar_t* defaultButton[]={L"First", L"Second", L"Third"};
		const wchar_t* icons[]={L"None", L"Error", L"Question", L"Warning", L"Information"};
		const wchar_t* modal[]={L"Window", L"Task", L"System"};

		GuiComboBoxListControl* comboInput=CreateComboBox(input, sizeof(input)/sizeof(*input));
		GuiComboBoxListControl* comboDefault=CreateComboBox(defaultButton, sizeof(defaultButton)/sizeof(*defaultButton));
		GuiComboBoxListControl* comboIcons=CreateComboBox(icons, sizeof(icons)/sizeof(*icons));
		GuiComboBoxListControl* comboModal=CreateComboBox(modal, sizeof(modal)/sizeof(*modal));

		comboInput->GetBoundsComposition()->SetBounds(Rect(Point(10, 10), Size(200, 0)));
		comboDefault->GetBoundsComposition()->SetBounds(Rect(Point(10, 40), Size(200, 0)));
		comboIcons->GetBoundsComposition()->SetBounds(Rect(Point(10, 70), Size(200, 0)));
		comboModal->GetBoundsComposition()->SetBounds(Rect(Point(10, 100), Size(200, 0)));

		GuiButton* buttonMessageBox=new GuiButton(new win7::Win7ButtonStyle);
		buttonMessageBox->SetText(L"ShowMessageBox");
		buttonMessageBox->GetBoundsComposition()->SetBounds(Rect(Point(10, 130), Size(0, 0)));

		container->AddChild(comboInput);
		container->AddChild(comboDefault);
		container->AddChild(comboIcons);
		container->AddChild(comboModal);
		container->AddChild(buttonMessageBox);

		buttonMessageBox->Clicked.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			INativeDialogService::MessageBoxButtonsInput buttons=(INativeDialogService::MessageBoxButtonsInput)comboInput->GetSelectedIndex();
			INativeDialogService::MessageBoxDefaultButton defaultButton=(INativeDialogService::MessageBoxDefaultButton)comboDefault->GetSelectedIndex();
			INativeDialogService::MessageBoxIcons icon=(INativeDialogService::MessageBoxIcons)comboIcons->GetSelectedIndex();
			INativeDialogService::MessageBoxModalOptions modal=(INativeDialogService::MessageBoxModalOptions)comboModal->GetSelectedIndex();

			INativeDialogService::MessageBoxButtonsOutput result=GetCurrentController()->DialogService()->ShowMessageBox(controlHost->GetNativeWindow(), L"This is a message box!", L"The Title", buttons, defaultButton, icon, modal);
			WString selected;
#define SELECT(NAME) case INativeDialogService::Select##NAME: selected=L#NAME; break
			switch(result)
			{
			SELECT(OK);
			SELECT(Cancel);
			SELECT(Yes);
			SELECT(No);
			SELECT(Retry);
			SELECT(Abort);
			SELECT(Ignore);
			SELECT(TryAgain);
			SELECT(Continue);
			}
#undef SELECT
			GetCurrentController()->DialogService()->ShowMessageBox(controlHost->GetNativeWindow(), L"You selected the \""+selected+L"\" button.", L"Result");
		});
	}
	{
		GuiButton* buttonColorDialog1=new GuiButton(new win7::Win7ButtonStyle);
		buttonColorDialog1->SetText(L"SimpleCustomColor");
		buttonColorDialog1->GetBoundsComposition()->SetBounds(Rect(Point(10, 160), Size(0, 0)));
		container->AddChild(buttonColorDialog1);

		buttonColorDialog1->Clicked.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			Color selection;
			if(GetCurrentController()->DialogService()->ShowColorDialog(controlHost->GetNativeWindow(), selection, false, INativeDialogService::CustomColorDisabled, 0))
			{
				WString selected=L"("+itow(selection.r)+L", "+itow(selection.g)+L", "+itow(selection.b)+L")";
				GetCurrentController()->DialogService()->ShowMessageBox(controlHost->GetNativeWindow(), L"You selected the \""+selected+L"\" color.", L"Result");
			}
		});

		GuiButton* buttonColorDialog2=new GuiButton(new win7::Win7ButtonStyle);
		buttonColorDialog2->SetText(L"RichCustomColor");
		buttonColorDialog2->GetBoundsComposition()->SetBounds(Rect(Point(10, 190), Size(0, 0)));
		container->AddChild(buttonColorDialog2);

		buttonColorDialog2->Clicked.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			Color selection(255, 0, 0);
			Color customColors[]=
			{
				Color(0, 0, 0),
				Color(128, 0, 0),
				Color(0, 128, 0),
				Color(0, 0, 128),
				Color(128, 128, 0),
				Color(0, 128, 128),
				Color(128, 0, 128),
				Color(192, 192, 192),
				Color(128, 128, 128),
				Color(255, 0, 0),
				Color(0, 255, 0),
				Color(0, 0, 255),
				Color(255, 255, 0),
				Color(0, 255, 255),
				Color(255, 0, 255),
				Color(255, 255, 255),
			};
			if(GetCurrentController()->DialogService()->ShowColorDialog(controlHost->GetNativeWindow(), selection, true, INativeDialogService::CustomColorOpened, customColors))
			{
				WString selected=L"("+itow(selection.r)+L", "+itow(selection.g)+L", "+itow(selection.b)+L")";
				GetCurrentController()->DialogService()->ShowMessageBox(controlHost->GetNativeWindow(), L"You selected the \""+selected+L"\" color.", L"Result");
			}
		});
	}
}