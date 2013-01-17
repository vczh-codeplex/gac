#include "SDI.h"
#include "Implementation\DocumentManager.h"
#include "PackageManager.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
SDIConfiguration
***********************************************************************/

		ISDIApplication* sdiApplication=0;

/***********************************************************************
MainWindow
***********************************************************************/

		class MainWindow : public GuiWindow
		{
		protected:
			Ptr<GuiResource>									resource;
			List<Ptr<XmlElement>>								packages;
			Dictionary<WString, Ptr<GuiToolstripCommand>>		commands;
		public:
			MainWindow()
				:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			{
				resource=sdiApplication->GetApplicationResource();
				WString applicationName;
				if(auto name=resource->GetValueByPath(L"Application\\Name").Cast<ObjectBox<WString>>())
				{
					applicationName=name->Unbox();
				}

				this->SetText(L"GacUI Tools "+applicationName+L" (www.gaclib.net)");
				this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
				this->ForceCalculateSizeImmediately();
				this->MoveToScreenCenter();

				EnumeratePackages(resource, packages);
				EnumerateCommands(resource, packages, commands);
			}

			~MainWindow()
			{
			}
		};

/***********************************************************************
StartSDI
***********************************************************************/

		int StartSDI(ISDIApplication* application)
		{
			sdiApplication=application;
			int result=SetupWindowsDirect2DRenderer();
			sdiApplication=0;
			return result;
		}
	}
}

/***********************************************************************
GuiMain
***********************************************************************/

using namespace vl::gactools;

void GuiMain()
{
	DocumentManager documentManager;
	SetDocumentManager(&documentManager);
	{
		MainWindow window;
		GetApplication()->Run(&window);
	}
	SetDocumentManager(0);
}