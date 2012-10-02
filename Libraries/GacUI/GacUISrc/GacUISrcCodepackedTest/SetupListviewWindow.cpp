#include "..\..\Public\Source\GacUI.h"

void SetupListviewWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiListView* listControl=0;
	{
		listControl=new GuiListView(new win7::Win7ListViewProvider);
		listControl->GetBoundsComposition()->SetAlignmentToParent(Margin(200, 5, 5, 5));
		listControl->GetBoundsComposition()->SetBounds(Rect(0, 0, 300, 200));
		listControl->SetHorizontalAlwaysVisible(false);
		listControl->SetVerticalAlwaysVisible(false);
		listControl->SetMultiSelect(true);
		container->GetBoundsComposition()->AddChild(listControl->GetBoundsComposition());
		
		INativeImageService* imageService=GetCurrentController()->ImageService();
		Ptr<INativeImage> largeImage=imageService->CreateImageFromFile(L"Resources\\BigDoc.png");
		Ptr<GuiImageData> largeImageData=new GuiImageData(largeImage, 0);
		Ptr<INativeImage> smallImage=imageService->CreateImageFromFile(L"Resources\\SmallDoc.png");
		Ptr<GuiImageData> smallImageData=new GuiImageData(smallImage, 0);

		for(int i=0;i<100;i++)
		{
			Ptr<list::ListViewItem> item=new list::ListViewItem;
			if(i<10)
			{
				item->text=L"This is a very long List View Item "+itow(i+1);
			}
			else
			{
				item->text=L"List View Item "+itow(i+1);
			}
			item->largeImage=largeImageData;
			item->smallImage=smallImageData;
			item->subItems.Add(L"Description of "+item->text);
			item->subItems.Add(itow(i+1));
			item->subItems.Add(i<10?L"Long":L"Short");
			listControl->GetItems().Add(item);
		}
		listControl->GetItems().GetColumns().Add(new list::ListViewColumn(L"Name"));
		listControl->GetItems().GetColumns().Add(new list::ListViewColumn(L"Description"));
		listControl->GetItems().GetColumns().Add(new list::ListViewColumn(L"Index"));
		listControl->GetItems().GetColumns().Add(new list::ListViewColumn(L"Type"));
	}
	{
		GuiTextList* typeList=new GuiTextList(new win7::Win7MultilineTextBoxProvider, new win7::Win7TextListProvider);
		typeList->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, -1, 5));
		typeList->GetBoundsComposition()->SetBounds(Rect(0, 0, 190, 200));
		typeList->SetHorizontalAlwaysVisible(false);
		container->GetBoundsComposition()->AddChild(typeList->GetBoundsComposition());

		typeList->GetItems().Add(L"Big Icon");
		typeList->GetItems().Add(L"Small Icon");
		typeList->GetItems().Add(L"List");
		typeList->GetItems().Add(L"Detail");
		typeList->GetItems().Add(L"Tile");
		typeList->GetItems().Add(L"Information");
		typeList->SetSelected(0, true);

		typeList->SelectionChanged.AttachLambda([listControl, typeList](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(typeList->GetSelectedItems().Count()>0)
			{
				listControl->GetItems().GetDataColumns().Clear();
				switch(typeList->GetSelectedItems()[0])
				{
				case 0:
					listControl->ChangeItemStyle(new list::ListViewBigIconContentProvider);
					break;
				case 1:
					listControl->ChangeItemStyle(new list::ListViewSmallIconContentProvider);
					break;
				case 2:
					listControl->ChangeItemStyle(new list::ListViewListContentProvider);
					break;
				case 3:
					listControl->ChangeItemStyle(new list::ListViewDetailContentProvider);
					break;
				case 4:
					listControl->GetItems().GetDataColumns().Add(0);
					listControl->GetItems().GetDataColumns().Add(2);
					listControl->ChangeItemStyle(new list::ListViewTileContentProvider);
					break;
				case 5:
					listControl->GetItems().GetDataColumns().Add(1);
					listControl->GetItems().GetDataColumns().Add(2);
					listControl->ChangeItemStyle(new list::ListViewInformationContentProvider);
					break;
				}
			}
		});
	}
}