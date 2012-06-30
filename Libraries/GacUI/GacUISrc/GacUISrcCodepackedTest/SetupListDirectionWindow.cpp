#include "..\..\Public\Source\GacUIIncludes.h"

void SetupListDirectionWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiListView* listControl=0;
	{
		listControl=new GuiListView(new win7::Win7ListViewProvider);
		listControl->GetBoundsComposition()->SetAlignmentToParent(Margin(200, 5, 5, 5));
		listControl->GetBoundsComposition()->SetBounds(Rect(0, 0, 300, 200));
		listControl->SetHorizontalAlwaysVisible(false);
		listControl->SetVerticalAlwaysVisible(false);
		container->GetBoundsComposition()->AddChild(listControl->GetBoundsComposition());
		
		INativeImageService* imageService=GetCurrentController()->ImageService();
		Ptr<INativeImage> largeImage=imageService->CreateImageFromFile(L"Resources\\New.png");
		Ptr<GuiImageData> largeImageData=new GuiImageData(largeImage, 0);
		Ptr<INativeImage> smallImage=imageService->CreateImageFromFile(L"Resources\\NewSmall.png");
		Ptr<GuiImageData> smallImageData=new GuiImageData(smallImage, 0);

		for(int i=0;i<100;i++)
		{
			Ptr<list::ListViewItem> item=new list::ListViewItem;
			item->text=L"List View Item "+itow(i+1);
			item->largeImage=largeImageData;
			listControl->GetItems().Add(item);
		}
	}
	{
		GuiTextList* typeList=new GuiTextList(new win7::Win7MultilineTextBoxProvider, new win7::Win7TextListProvider);
		typeList->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, -1, 160));
		typeList->GetBoundsComposition()->SetBounds(Rect(0, 0, 190, 200));
		typeList->SetHorizontalAlwaysVisible(false);
		container->GetBoundsComposition()->AddChild(typeList->GetBoundsComposition());

		typeList->GetItems().Add(L"Right Down");
		typeList->GetItems().Add(L"Left Down");
		typeList->GetItems().Add(L"Right Up");
		typeList->GetItems().Add(L"Left Up");
		typeList->GetItems().Add(L"Down Right");
		typeList->GetItems().Add(L"Down Left");
		typeList->GetItems().Add(L"Up Right");
		typeList->GetItems().Add(L"Up Left");
		typeList->SetSelected(0, true);

		typeList->SelectionChanged.AttachLambda([listControl, typeList](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(typeList->GetSelectedItems().Count()>0)
			{
				switch(typeList->GetSelectedItems()[0])
				{
				case 0:
					listControl->SetCoordinateTransformer(new list::AxisAlignedItemCoordinateTransformer(list::AxisAlignedItemCoordinateTransformer::RightDown));
					break;
				case 1:
					listControl->SetCoordinateTransformer(new list::AxisAlignedItemCoordinateTransformer(list::AxisAlignedItemCoordinateTransformer::LeftDown));
					break;
				case 2:
					listControl->SetCoordinateTransformer(new list::AxisAlignedItemCoordinateTransformer(list::AxisAlignedItemCoordinateTransformer::RightUp));
					break;
				case 3:
					listControl->SetCoordinateTransformer(new list::AxisAlignedItemCoordinateTransformer(list::AxisAlignedItemCoordinateTransformer::LeftUp));
					break;
				case 4:
					listControl->SetCoordinateTransformer(new list::AxisAlignedItemCoordinateTransformer(list::AxisAlignedItemCoordinateTransformer::DownRight));
					break;
				case 5:
					listControl->SetCoordinateTransformer(new list::AxisAlignedItemCoordinateTransformer(list::AxisAlignedItemCoordinateTransformer::DownLeft));
					break;
				case 6:
					listControl->SetCoordinateTransformer(new list::AxisAlignedItemCoordinateTransformer(list::AxisAlignedItemCoordinateTransformer::UpRight));
					break;
				case 7:
					listControl->SetCoordinateTransformer(new list::AxisAlignedItemCoordinateTransformer(list::AxisAlignedItemCoordinateTransformer::UpLeft));
					break;
				}
			}
		});
	}
	{
		GuiTextList* typeList=new GuiTextList(new win7::Win7MultilineTextBoxProvider, new win7::Win7TextListProvider);
		typeList->GetBoundsComposition()->SetAlignmentToParent(Margin(5, -1, -1, 5));
		typeList->GetBoundsComposition()->SetBounds(Rect(0, 0, 190, 145));
		typeList->SetHorizontalAlwaysVisible(false);
		container->GetBoundsComposition()->AddChild(typeList->GetBoundsComposition());

		typeList->GetItems().Add(L"Block");
		typeList->GetItems().Add(L"Item");
		typeList->SetSelected(0, true);

		typeList->SelectionChanged.AttachLambda([listControl, typeList](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(typeList->GetSelectedItems().Count()>0)
			{
				switch(typeList->GetSelectedItems()[0])
				{
				case 0:
					listControl->SetArranger(new list::FixedSizeMultiColumnItemArranger);
					break;
				case 1:
					listControl->SetArranger(new list::FixedHeightItemArranger);
					break;
				}
			}
		});
	}
}