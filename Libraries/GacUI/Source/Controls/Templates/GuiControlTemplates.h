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
			};

#define GUI_TEMPLATE_PROPERTY_DECL(CLASS, TYPE, NAME)\
			private:\
				TYPE NAME##_;\
			public:\
				TYPE Get##NAME();\
				void Set##NAME(const TYPE& value);\
				compositions::GuiNotifyEvent NAME##Changed;\

#define GUI_TEMPLATE_PROPERTY_IMPL(CLASS, TYPE, NAME)\
			TYPE CLASS::Get##NAME()\
			{\
				return NAME##_;\
			}\
			void CLASS::Set##NAME(const TYPE& value)\
			{\
				if (NAME##_ != value)\
				{\
					NAME##_ = value;\
					NAME##Changed.Execute(compositions::GuiEventArgs(this));\
				}\
			}\

#define GUI_TEMPLATE_PROPERTY_EVENT_INIT(CLASS, TYPE, NAME)\
			NAME##Changed.SetAssociatedComposition(this);

/***********************************************************************
Control Template
***********************************************************************/

/***********************************************************************
Item Template
***********************************************************************/

			class GuiListItemTemplate : public GuiTemplate
			{
			public:
				GuiListItemTemplate();
				~GuiListItemTemplate();
				
#define GuiListItemTemplate_PROPERTIES(F)\
				F(GuiListItemTemplate, bool, Selected)\
				F(GuiListItemTemplate, vint, Index)\

				GuiListItemTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_DECL)
			};
		}
	}
}

#endif