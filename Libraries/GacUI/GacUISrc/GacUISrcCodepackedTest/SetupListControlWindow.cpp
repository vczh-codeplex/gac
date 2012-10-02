#include "..\..\Public\Source\GacUI.h"

void SetupListControlWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTextList* listControl=0;
	{
		listControl=new GuiTextList(new win7::Win7MultilineTextBoxProvider, new win7::Win7TextListProvider);
		listControl->GetBoundsComposition()->SetAlignmentToParent(Margin(200, 5, 5, 5));
		listControl->GetBoundsComposition()->SetBounds(Rect(0, 0, 300, 200));
		listControl->SetHorizontalAlwaysVisible(false);
		listControl->SetMultiSelect(true);
		container->GetBoundsComposition()->AddChild(listControl->GetBoundsComposition());

		for(int i=0;i<30;i++)
		{
			listControl->GetItems().Add(L"Text Item "+itow(i+1));
			listControl->GetItems().SetChecked(i, i%2==0);
		}
	}
	{
		GuiTextList* typeList=new GuiTextList(new win7::Win7MultilineTextBoxProvider, new win7::Win7TextListProvider);
		typeList->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, -1, 5));
		typeList->GetBoundsComposition()->SetBounds(Rect(0, 0, 190, 200));
		typeList->SetHorizontalAlwaysVisible(false);
		container->GetBoundsComposition()->AddChild(typeList->GetBoundsComposition());

		typeList->GetItems().Add(L"Text List");
		typeList->GetItems().Add(L"Check List");
		typeList->GetItems().Add(L"Radio List");
		typeList->SetSelected(0, true);

		typeList->SelectionChanged.AttachLambda([listControl, typeList](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(typeList->GetSelectedItems().Count()>0)
			{
				switch(typeList->GetSelectedItems()[0])
				{
				case 0:
					listControl->ChangeItemStyle(new win7::Win7TextListProvider);
					break;
				case 1:
					listControl->ChangeItemStyle(new win7::Win7CheckTextListProvider);
					break;
				case 2:
					listControl->ChangeItemStyle(new win7::Win7RadioTextListProvider);
					break;
				}
			}
		});
	}
}