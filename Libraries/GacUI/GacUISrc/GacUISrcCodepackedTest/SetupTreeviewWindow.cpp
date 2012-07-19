#include "..\..\Public\Source\GacUIIncludes.h"

void SetupTreeviewWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTreeView* treeControl=new GuiTreeView(new win7::Win7TreeViewProvider);
	treeControl->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, 5, 5));
	treeControl->GetBoundsComposition()->SetBounds(Rect(0, 0, 300, 200));
	treeControl->SetHorizontalAlwaysVisible(false);
	treeControl->SetVerticalAlwaysVisible(false);
	container->GetBoundsComposition()->AddChild(treeControl->GetBoundsComposition());
		
	INativeImageService* imageService=GetCurrentController()->ImageService();
	Ptr<INativeImage> image=imageService->CreateImageFromFile(L"Resources\\SmallDoc.png");
	Ptr<GuiImageData> imageData=new GuiImageData(image, 0);

	treeControl->Nodes()->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"Microsoft")));
	{
		Ptr<tree::MemoryNodeProvider> node=treeControl->Nodes()->Children()[0];
		node->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"Visual C++")));
		node->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"Visual C#")));
		node->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"Visual Basic.NET")));
		node->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"Visual F#")));
	}
	treeControl->Nodes()->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"IBM")));
	{
		Ptr<tree::MemoryNodeProvider> node=treeControl->Nodes()->Children()[1];
		node->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"Eclipse")));
	}
	treeControl->Nodes()->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"Borland")));
	{
		Ptr<tree::MemoryNodeProvider> node=treeControl->Nodes()->Children()[2];
		node->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"Delphi")));
		node->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"C++ Builder")));
		node->Children().Add(new tree::MemoryNodeProvider(new tree::TreeViewItem(imageData, L"JBuilder")));
	}
}