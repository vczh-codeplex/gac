/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITOOLSTRIPCOMMAND
#define VCZH_PRESENTATION_CONTROLS_GUITOOLSTRIPCOMMAND

#include "..\GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			/// <summary>A command for toolstrip controls.</summary>
			class GuiToolstripCommand : public GuiComponent, public Description<GuiToolstripCommand>
			{
			protected:
				Ptr<GuiImageData>						image;
				WString									text;
				compositions::IGuiShortcutKeyItem*		shortcutKeyItem;
				bool									enabled;

				void									InvokeDescriptionChanged();
			public:
				/// <summary>Create the command.</summary>
				GuiToolstripCommand();
				~GuiToolstripCommand();

				/// <summary>Executed event.</summary>
				compositions::GuiNotifyEvent			Executed;

				/// <summary>Description changed event, raised when any description property is modified.</summary>
				compositions::GuiNotifyEvent			DescriptionChanged;

				/// <summary>Get the image for this command.</summary>
				/// <returns>The image for this command.</returns>
				Ptr<GuiImageData>						GetImage();
				/// <summary>Set the image for this command.</summary>
				/// <param name="value">The image for this command.</param>
				void									SetImage(Ptr<GuiImageData> value);
				/// <summary>Get the text for this command.</summary>
				/// <returns>The text for this command.</returns>
				const WString&							GetText();
				/// <summary>Set the text for this command.</summary>
				/// <param name="value">The text for this command.</param>
				void									SetText(const WString& value);
				/// <summary>Get the shortcut key item for this command.</summary>
				/// <returns>The shortcut key item for this command.</returns>
				compositions::IGuiShortcutKeyItem*		GetShortcut();
				/// <summary>Set the shortcut key item for this command.</summary>
				/// <param name="value">The shortcut key item for this command.</param>
				void									SetShortcut(compositions::IGuiShortcutKeyItem* value);
				/// <summary>Get the enablility for this command.</summary>
				/// <returns>The enablility for this command.</returns>
				bool									GetEnabled();
				/// <summary>Set the enablility for this command.</summary>
				/// <param name="value">The enablility for this command.</param>
				void									SetEnabled(bool value);
			};
		}
	}
}

#endif