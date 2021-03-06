﻿/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Partial Classes

This file is generated by: Vczh GacUI Resource Code Generator
************************************************************************
DO NOT MODIFY
***********************************************************************/

#include "DataGridModel.h"

namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	BoldTextTemplate::BoldTextTemplate()
	{
		InitializeComponents();
	}
}


namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	CellBorderTemplate::CellBorderTemplate()
	{
		InitializeComponents();
	}
}


namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	NameEditor::NameEditor(Ptr<demos::IDataGridModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}


namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	NormalTextTemplate::NormalTextTemplate()
	{
		InitializeComponents();
	}
}


namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	TitleEditor::TitleEditor(Ptr<demos::IDataGridModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}


namespace vl
{
	namespace reflection
	{
		namespace description
		{
			#define _ ,
			IMPL_CPP_TYPE_INFO(demos::DataGridModelItem)
			IMPL_CPP_TYPE_INFO(demos::IDataGridModel)
			IMPL_CPP_TYPE_INFO(demos::BoldTextTemplate)
			IMPL_CPP_TYPE_INFO(demos::CellBorderTemplate)
			IMPL_CPP_TYPE_INFO(demos::MainWindow)
			IMPL_CPP_TYPE_INFO(demos::NameEditor)
			IMPL_CPP_TYPE_INFO(demos::NormalTextTemplate)
			IMPL_CPP_TYPE_INFO(demos::TitleEditor)

			BEGIN_CLASS_MEMBER(demos::DataGridModelItem)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<demos::DataGridModelItem>(), NO_PARAMETER)
				CLASS_MEMBER_FIELD(name)
				CLASS_MEMBER_FIELD(organization)
				CLASS_MEMBER_FIELD(title)
			END_CLASS_MEMBER(demos::DataGridModelItem)

			BEGIN_CLASS_MEMBER(demos::IDataGridModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Titles)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(People)
			END_CLASS_MEMBER(demos::IDataGridModel)

			BEGIN_CLASS_MEMBER(demos::BoldTextTemplate)
				CLASS_MEMBER_BASE(vl::presentation::templates::GuiGridVisualizerTemplate)
				CLASS_MEMBER_CONSTRUCTOR(demos::BoldTextTemplate*(), NO_PARAMETER)
			END_CLASS_MEMBER(demos::BoldTextTemplate)

			BEGIN_CLASS_MEMBER(demos::CellBorderTemplate)
				CLASS_MEMBER_BASE(vl::presentation::templates::GuiGridVisualizerTemplate)
				CLASS_MEMBER_CONSTRUCTOR(demos::CellBorderTemplate*(), NO_PARAMETER)
			END_CLASS_MEMBER(demos::CellBorderTemplate)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(Ptr<demos::IDataGridModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::MainWindow)

			BEGIN_CLASS_MEMBER(demos::NameEditor)
				CLASS_MEMBER_BASE(vl::presentation::templates::GuiGridEditorTemplate)
				CLASS_MEMBER_CONSTRUCTOR(demos::NameEditor*(Ptr<demos::IDataGridModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::NameEditor)

			BEGIN_CLASS_MEMBER(demos::NormalTextTemplate)
				CLASS_MEMBER_BASE(vl::presentation::templates::GuiGridVisualizerTemplate)
				CLASS_MEMBER_CONSTRUCTOR(demos::NormalTextTemplate*(), NO_PARAMETER)
			END_CLASS_MEMBER(demos::NormalTextTemplate)

			BEGIN_CLASS_MEMBER(demos::TitleEditor)
				CLASS_MEMBER_BASE(vl::presentation::templates::GuiGridEditorTemplate)
				CLASS_MEMBER_CONSTRUCTOR(demos::TitleEditor*(Ptr<demos::IDataGridModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::TitleEditor)

			#undef _

			class DataGridModelResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(demos::DataGridModelItem)
					ADD_TYPE_INFO(demos::IDataGridModel)
					ADD_TYPE_INFO(demos::BoldTextTemplate)
					ADD_TYPE_INFO(demos::CellBorderTemplate)
					ADD_TYPE_INFO(demos::MainWindow)
					ADD_TYPE_INFO(demos::NameEditor)
					ADD_TYPE_INFO(demos::NormalTextTemplate)
					ADD_TYPE_INFO(demos::TitleEditor)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class DataGridModelResourcePlugin : public Object, public vl::presentation::controls::IGuiPlugin
			{
			public:
				void Load()override
				{
					GetGlobalTypeManager()->AddTypeLoader(new DataGridModelResourceLoader);
				}

				void AfterLoad()override
				{
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(DataGridModelResourcePlugin)
		}
	}
}

