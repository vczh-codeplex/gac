/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Package Manager

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_PACKAGEMANAGER
#define GACUI_TOOLS_PACKAGEMANAGER

#include "DocumentFramework.h"
#include "ResourceManager.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
UI Builder
***********************************************************************/

		struct PackageXmlMenuItem
		{
			WString							id;
			Ptr<XmlElement>					definition;
			GuiToolstripBuilder*			builder;
			GuiToolstripButton*				menuButton;

			PackageXmlMenuItem()
				:builder(0)
				,menuButton(0)
			{
			}
		};

		struct PackageXmlMenuGroup
		{
			bool							hasSeparator;
			List<Ptr<PackageXmlMenuItem>>	menuItems;

			PackageXmlMenuGroup()
				:hasSeparator(false)
			{
			}
		};

		typedef Pair<vint, Ptr<PackageXmlMenuGroup>> ProprityMenuGroup;

		extern void			EnumeratePackages(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages);
		extern void			EnumerateCommands(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, Dictionary<WString, Ptr<DocumentToolstripCommand>>& commands);
		extern void			EnumerateMenuDefinitions(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, Group<WString, ProprityMenuGroup>& existingMenuGroups);
		extern void			BuildMenu(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, GuiToolstripMenuBar* menu, const WString& containerName, Dictionary<WString, Ptr<DocumentToolstripCommand>>& commands, Group<WString, ProprityMenuGroup>& existingMenuGroups);
		extern void			BuildToolbar(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, GuiToolstripToolbar* toolbar, const WString& containerName, Dictionary<WString, Ptr<DocumentToolstripCommand>>& commands, Group<WString, ProprityMenuGroup>& existingMenuGroups);
		extern void			BuildDialogs(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, IFileDialogService* service);
		extern void			LoadLegalDocumentPackages(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages);

/***********************************************************************
Main Application Package
***********************************************************************/

		class MainApplicationPackage : public Object, public IDocumentPackage
		{
		private:
			Ptr<GuiResource>										resource;
			List<Ptr<XmlElement>>									packages;
			Dictionary<WString, Ptr<DocumentToolstripCommand>>		commands;
			Group<WString, ProprityMenuGroup>						existingMenuGroups;
			Ptr<IFileDialogService>									fileDialogService;
			Ptr<IMessageDialogService>								messageDialogService;
			GuiWindow*												mainWindow;

		protected:

			virtual Ptr<GuiResource>								LoadPackageResource()=0;
		public:
			MainApplicationPackage();
			~MainApplicationPackage();

			void													OnBeforeInit()override;
			void													OnAfterInit()override;
			GuiWindow*												GetMainWindow();
			void													SetMainWindow(GuiWindow* window);
			void													BuildApplicationMenu(GuiToolstripMenuBar* menu, const WString& containerName);
			void													BuildApplicationToolbar(GuiToolstripToolbar* toolbar, const WString& containerName);

			static MainApplicationPackage*							BeforeInit(Ptr<GuiResource> resource, GuiWindow* mainWindow, const WString& mainApplicationPackageId);
			static void												AfterInit();
		};
	}
}

#endif