#include "SDI.h"

namespace vl
{
	namespace sdi
	{

/***********************************************************************
SDIConfiguration
***********************************************************************/

		const SDIConfiguration* sdiConfiguration=0;

/***********************************************************************
MainWindow
***********************************************************************/

		class MainWindow : public GuiWindow
		{
		public:
			MainWindow()
				:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			{
				this->SetText(L"GacUI Tools "+sdiConfiguration->applicationName+L" (www.gaclib.net)");

				this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
				this->ForceCalculateSizeImmediately();
				this->MoveToScreenCenter();
			}

			~MainWindow()
			{
			}
		};

/***********************************************************************
StartSDI
***********************************************************************/

		int StartSDI(const SDIConfiguration& config)
		{
			sdiConfiguration=&config;
			int result=SetupWindowsDirect2DRenderer();
			sdiConfiguration=0;
			return result;
		}
	}
}

/***********************************************************************
GuiMain
***********************************************************************/

using namespace vl::sdi;

void GuiMain()
{
	MainWindow window;
	GetApplication()->Run(&window);
}