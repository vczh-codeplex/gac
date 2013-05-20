/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIDATAGRIDCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIDATAGRIDCONTROLS

#include "GuiListViewControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
DataGrid Control
***********************************************************************/

			/// <summary>Data grid control in virtual mode.</summary>
			class GuiVirtualDataGrid : public GuiVirtualListView, public Description<GuiVirtualDataGrid>
			{
			public:
				class IDataProvider : public virtual IDescriptable, public Description<IDataProvider>
				{
				};

			protected:
				Ptr<IDataProvider>						dataProvider;
			public:
				/// <summary>Create a data grid control in virtual mode.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_dataProvider">The data provider for this control.</param>
				GuiVirtualDataGrid(IStyleProvider* _styleProvider, IDataProvider* _dataProvider);
				~GuiVirtualDataGrid();
			};

/***********************************************************************
DataSource Extensions
***********************************************************************/

/***********************************************************************
Visualizer Extensions
***********************************************************************/

/***********************************************************************
Editor Extensions
***********************************************************************/
		}
	}
}

#endif