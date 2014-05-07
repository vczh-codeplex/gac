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
				};

				/// <summary>Create a template.</summary>
				GuiTemplate();
				~GuiTemplate();
			};
		}
	}
}

#endif