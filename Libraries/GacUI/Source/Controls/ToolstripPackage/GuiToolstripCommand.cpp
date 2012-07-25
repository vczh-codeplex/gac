#include "GuiToolstripCommand.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace compositions;

/***********************************************************************
GuiToolstripCommand
***********************************************************************/

			void GuiToolstripCommand::InvokeDescriptionChanged()
			{
				GuiEventArgs arguments;
				DescriptionChanged.Execute(arguments);
			}

			GuiToolstripCommand::GuiToolstripCommand()
				:shortcutKeyItem(0)
				,enabled(true)
			{
			}

			GuiToolstripCommand::~GuiToolstripCommand()
			{
			}

			Ptr<GuiImageData> GuiToolstripCommand::GetImage()
			{
				return image;
			}

			void GuiToolstripCommand::SetImage(Ptr<GuiImageData> value)
			{
				if(image!=value)
				{
					image=value;
					InvokeDescriptionChanged();
				}
			}

			const WString& GuiToolstripCommand::GetText()
			{
				return text;
			}

			void GuiToolstripCommand::SetText(const WString& value)
			{
				if(text!=value)
				{
					text=value;
					InvokeDescriptionChanged();
				}
			}

			compositions::IGuiShortcutKeyItem* GuiToolstripCommand::GetShortcut()
			{
				return shortcutKeyItem;
			}

			void GuiToolstripCommand::SetShortcut(compositions::IGuiShortcutKeyItem* value)
			{
				if(shortcutKeyItem!=value)
				{
					shortcutKeyItem=value;
					InvokeDescriptionChanged();
				}
			}

			bool GuiToolstripCommand::GetEnabled()
			{
				return enabled;
			}

			void GuiToolstripCommand::SetEnabled(bool value)
			{
				if(enabled!=value)
				{
					enabled=value;
					InvokeDescriptionChanged();
				}
			}
		}
	}
}