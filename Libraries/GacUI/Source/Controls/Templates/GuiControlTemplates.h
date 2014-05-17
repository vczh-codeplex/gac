/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Template System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_TEMPLATES_GUICONTROLTEMPLATES
#define VCZH_PRESENTATION_CONTROLS_TEMPLATES_GUICONTROLTEMPLATES

#include "..\Styles\GuiThemeStyleFactory.h"

namespace vl
{
	namespace presentation
	{
		namespace templates
		{

#define GUI_TEMPLATE_PROPERTY_DECL(CLASS, TYPE, NAME)\
			private:\
				TYPE NAME##_;\
			public:\
				TYPE Get##NAME();\
				void Set##NAME(TYPE const& value);\
				compositions::GuiNotifyEvent NAME##Changed;\

#define GUI_TEMPLATE_PROPERTY_IMPL(CLASS, TYPE, NAME)\
			TYPE CLASS::Get##NAME()\
			{\
				return NAME##_;\
			}\
			void CLASS::Set##NAME(TYPE const& value)\
			{\
				if (NAME##_ != value)\
				{\
					NAME##_ = value;\
					NAME##Changed.Execute(compositions::GuiEventArgs(this));\
				}\
			}\

#define GUI_TEMPLATE_PROPERTY_EVENT_INIT(CLASS, TYPE, NAME)\
			NAME##Changed.SetAssociatedComposition(this);

			/// <summary>Represents a user customizable template.</summary>
			class GuiTemplate : public compositions::GuiBoundsComposition, public controls::GuiInstanceRootObject, public Description<GuiTemplate>
			{
			public:
				/// <summary>Factory interface for creating <see cref="GuiTemplate"/> instances.</summary>
				class IFactory : public IDescriptable, public Description<IFactory>
				{
				public:
					/// <summary>Create a <see cref="GuiTemplate"/> instance.</summary>
					/// <returns>The created template.</returns>
					/// <param name="viewModel">The view model for binding.</param>
					virtual GuiTemplate*				CreateTemplate(const description::Value& viewModel) = 0;

					static Ptr<IFactory>				CreateTemplateFactory(const collections::List<description::ITypeDescriptor*>& types);
				};

				/// <summary>Create a template.</summary>
				GuiTemplate();
				~GuiTemplate();
				
#define GuiTemplate_PROPERTIES(F)\
				F(GuiTemplate, FontProperties, Font)\
				F(GuiTemplate, bool, VisuallyEnabled)\

				GuiTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_DECL)
			};

/***********************************************************************
Control Template
***********************************************************************/

			class GuiControlTemplate : public GuiTemplate, public Description<GuiControlTemplate>
			{
			public:
				GuiControlTemplate();
				~GuiControlTemplate();
				
#define GuiControlTemplate_PROPERTIES(F)\
				F(GuiControlTemplate, WString, Text)\
				F(GuiControlTemplate, compositions::GuiGraphicsComposition*, ContainerComposition)\
				F(GuiControlTemplate, compositions::GuiGraphicsComposition*, FocusableComposition)\

				GuiControlTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_DECL)
			};

			class GuiWindowTemplate : public GuiControlTemplate, public Description<GuiWindowTemplate>
			{
			public:
				GuiWindowTemplate();
				~GuiWindowTemplate();
			};

			class GuiButtonTemplate : public GuiControlTemplate, public Description<GuiButtonTemplate>
			{
			public:
				GuiButtonTemplate();
				~GuiButtonTemplate();

#define GuiButtonTemplate_PROPERTIES(F)\
				F(GuiButtonTemplate, controls::GuiButton::ControlState, State)\

				GuiButtonTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_DECL)
			};

			class GuiSelectableButtonTemplate : public GuiButtonTemplate, public Description<GuiSelectableButtonTemplate>
			{
			public:
				GuiSelectableButtonTemplate();
				~GuiSelectableButtonTemplate();

#define GuiSelectableButtonTemplate_PROPERTIES(F)\
				F(GuiSelectableButtonTemplate, bool, Selected)\

				GuiSelectableButtonTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_DECL)
			};

			class GuiToolstripButtonTemplate : public GuiSelectableButtonTemplate, public Description<GuiToolstripButtonTemplate>
			{
			public:
				GuiToolstripButtonTemplate();
				~GuiToolstripButtonTemplate();

#define GuiToolstripButtonTemplate_PROPERTIES(F)\
				F(GuiToolstripButtonTemplate, WString, SubMenuTemplate)\
				F(GuiToolstripButtonTemplate, bool, SubMenuExisting)\
				F(GuiToolstripButtonTemplate, bool, SubMenuOpening)\
				F(GuiToolstripButtonTemplate, controls::GuiButton*, SubMenuHost)\
				F(GuiToolstripButtonTemplate, Ptr<GuiImageData>, Image)\
				F(GuiToolstripButtonTemplate, WString, ShortcutText)\

				GuiToolstripButtonTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_DECL)
			};

/***********************************************************************
Item Template
***********************************************************************/

			class GuiListItemTemplate : public GuiTemplate, public Description<GuiListItemTemplate>
			{
			public:
				GuiListItemTemplate();
				~GuiListItemTemplate();
				
#define GuiListItemTemplate_PROPERTIES(F)\
				F(GuiListItemTemplate, bool, Selected)\
				F(GuiListItemTemplate, vint, Index)\

				GuiListItemTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_DECL)
			};

			class GuiTreeItemTemplate : public GuiListItemTemplate, public Description<GuiTreeItemTemplate>
			{
			public:
				GuiTreeItemTemplate();
				~GuiTreeItemTemplate();
				
#define GuiTreeItemTemplate_PROPERTIES(F)\
				F(GuiTreeItemTemplate, bool, Expanding)\

				GuiTreeItemTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_DECL)
			};
		}
	}
}

#endif