#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include "..\..\..\Workflow\Source\Analyzer\WfAnalyzer.h"
#include "..\..\Source\Resources\GuiParserManager.h"
#include "..\..\Source\Reflection\GuiInstanceLoader.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionControls.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionEvents.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionTemplates.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::reflection::description;
using namespace vl::parsing;
using namespace vl::parsing::xml;
using namespace vl::workflow;
using namespace vl::workflow::analyzer;
using namespace vl::workflow::runtime;
using namespace vl::presentation::controls::list;
using namespace vl::presentation::templates;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result = SetupWindowsDirect2DRenderer();
#endif

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

extern void UnitTestInGuiMain();

/*
Data Binding: IValueObservableList/ItemTemplate
	GuiSelectableListControl
	GuiVirtualTextList
	GuiVirtualListView
	GuiVirtualTreeView
	GuiVirtualDataGrid
	GuiTab
	GuiComboBoxListControl

Features:
	View Model				: View model interface declaration
	Data Binding			: Data structure declaration
	Control Template		: Interface methods to events, or prepare predefined classes
	Workflow as Code Behind
	Data Service and Query
*/

#ifndef VCZH_DEBUG_NO_REFLECTION

#define CONTROL_TEMPLATE_DECL(IMPL, BASE)\
	class IMPL : public Template_<IMPL, BASE>\
	{\
	public:\
		IMPL()\
			:Template_<IMPL, BASE>(L"demos::" L#IMPL)\
		{\
		}\
	};\

#define CONTROL_TEMPLATE_IMPL(IMPL, BASE)\
	BEGIN_CLASS_MEMBER(demos::IMPL)\
		CLASS_MEMBER_BASE(BASE)\
		CLASS_MEMBER_CONSTRUCTOR(demos::IMPL*(), NO_PARAMETER)\
	END_CLASS_MEMBER(demos::IMPL)\

namespace demos
{
// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //

	template<typename TImpl>
	class MainWindow_ : public GuiWindow, public GuiInstancePartialClass<GuiWindow>, public Description<TImpl>
	{
	protected:

		void InitializeComponents()
		{
			InitializeFromResource();
		}
	public:
		MainWindow_()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,GuiInstancePartialClass<GuiWindow>(L"demos::MainWindow")
		{
		}
	};

	template<typename TImpl, typename TBase>
	class Template_ : public TBase, public GuiInstancePartialClass<TBase>, public Description<TImpl>
	{
	public:
		Template_(const WString& className)
			:GuiInstancePartialClass<TBase>(className)
		{
			InitializeFromResource();
		}
	};

#define DEMO_TEMPLATES(F)\
	F(GroupBoxTemplate, GuiControlTemplate)\
	F(ToolstripTemplate, GuiControlTemplate)\
	F(ToolstripMenuTemplate, GuiWindowTemplate)\
	F(ToolstripSplitterTemplate, GuiControlTemplate)\
	F(MenuSplitterTemplate, GuiControlTemplate)\
	F(ButtonTemplate, GuiButtonTemplate)\
	F(CheckBoxTemplate, GuiSelectableButtonTemplate)\
	F(RadioButtonTemplate, GuiSelectableButtonTemplate)\
	F(DocumentLabelTemplate, GuiControlTemplate)\
	F(LabelTemplate, GuiLabelTemplate)\
	F(SinglelineTextBoxTemplate, GuiSinglelineTextBoxTemplate)\
	F(MultilineTextBoxTemplate, GuiMultilineTextBoxTemplate)\
	F(LeftScrollButtonTemplate, GuiButtonTemplate)\
	F(RightScrollButtonTemplate, GuiButtonTemplate)\
	F(TopScrollButtonTemplate, GuiButtonTemplate)\
	F(BottomScrollButtonTemplate, GuiButtonTemplate)\
	F(HScrollHandleTemplate, GuiButtonTemplate)\
	F(VScrollHandleTemplate, GuiButtonTemplate)\
	F(ScrollViewTemplate, GuiScrollViewTemplate)\
	F(ItemBackgroundTemplate, GuiSelectableButtonTemplate)\
	F(CheckItemBackgroundTemplate, GuiSelectableButtonTemplate)\
	F(TextListTemplate, GuiTextListTemplate)\
	F(CheckTextListTemplate, GuiTextListTemplate)\
	F(RadioTextListTemplate, GuiTextListTemplate)\
	F(ToolstripButtonTemplate, GuiToolstripButtonTemplate)\
	F(ToolstripDropdownButtonTemplate, GuiToolstripButtonTemplate)\
	F(ToolstripSplitArrowTemplate, GuiSelectableButtonTemplate)\
	F(ToolstripSplitButtonTemplate, GuiToolstripButtonTemplate)\
	F(MenuBarButtonTemplate, GuiToolstripButtonTemplate)\
	F(ComboBoxTemplate, GuiDateComboBoxTemplate)\
	F(DatePickerTemplate, GuiDatePickerTemplate)\
	F(ListViewColumnHeaderTemplate, GuiListViewColumnHeaderTemplate)\
	F(ListViewTemplate, GuiListViewTemplate)\

	DEMO_TEMPLATES(CONTROL_TEMPLATE_DECL)

	class HScrollTemplate : public GuiScrollTemplate, public GuiInstancePartialClass<GuiScrollTemplate>, public Description<HScrollTemplate>
	{
		friend struct CustomTypeDescriptorSelector<HScrollTemplate>;
	protected:
		GuiPartialViewComposition*			handle;
		Point								draggingStartLocation;
		bool								draggingHandle;
		
		void OnHandleMouseDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
		{
			if (sender->GetRelatedControl()->GetVisuallyEnabled())
			{
				draggingHandle = true;
				draggingStartLocation = Point(arguments.x, arguments.y);
			}
		}

		void OnHandleMouseUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
		{
			draggingHandle = false;
		}

		void OnHandleMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
		{
			if (draggingHandle)
			{
				vint totalPixels = handle->GetParent()->GetBounds().Width();
				vint currentOffset = handle->GetBounds().Left();
				vint newOffset = currentOffset + (arguments.x - draggingStartLocation.x);
				vint totalSize = GetTotalSize();
				double ratio = (double)newOffset / totalPixels;
				vint newPosition = (vint)(ratio * totalSize);

				vint offset1 = (vint)(((double)newPosition / totalSize) * totalPixels);
				vint offset2 = vint(((double)(newPosition + 1) / totalSize) * totalPixels);
				vint delta1 = abs((int)(offset1 - newOffset));
				vint delta2 = abs((int)(offset2 - newOffset));

				if(delta1<delta2)
				{
					GetCommands()->SetPosition(newPosition);
				}
				else
				{
					GetCommands()->SetPosition(newPosition + 1);
				}
			}
		}
	public:
		HScrollTemplate()
			:GuiInstancePartialClass<GuiScrollTemplate>(L"demos::HScrollTemplate")
			, handle(0)
			, draggingHandle(false)
		{
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(handle);
			}
		}
	};

	class VScrollTemplate : public GuiScrollTemplate, public GuiInstancePartialClass<GuiScrollTemplate>, public Description<VScrollTemplate>
	{
		friend struct CustomTypeDescriptorSelector<VScrollTemplate>;
	protected:
		GuiPartialViewComposition*			handle;
		Point								draggingStartLocation;
		bool								draggingHandle;
		
		void OnHandleMouseDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
		{
			if (sender->GetRelatedControl()->GetVisuallyEnabled())
			{
				draggingHandle = true;
				draggingStartLocation = Point(arguments.x, arguments.y);
			}
		}

		void OnHandleMouseUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
		{
			draggingHandle = false;
		}

		void OnHandleMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
		{
			if (draggingHandle)
			{
				vint totalPixels = handle->GetParent()->GetBounds().Height();
				vint currentOffset = handle->GetBounds().Top();
				vint newOffset = currentOffset + (arguments.y - draggingStartLocation.y);
				vint totalSize = GetTotalSize();
				double ratio = (double)newOffset / totalPixels;
				vint newPosition = (vint)(ratio * totalSize);

				vint offset1 = (vint)(((double)newPosition / totalSize) * totalPixels);
				vint offset2 = vint(((double)(newPosition + 1) / totalSize) * totalPixels);
				vint delta1 = abs((int)(offset1 - newOffset));
				vint delta2 = abs((int)(offset2 - newOffset));

				if(delta1<delta2)
				{
					GetCommands()->SetPosition(newPosition);
				}
				else
				{
					GetCommands()->SetPosition(newPosition + 1);
				}
			}
		}
	public:
		VScrollTemplate()
			:GuiInstancePartialClass<GuiScrollTemplate>(L"demos::VScrollTemplate")
			, handle(0)
			, draggingHandle(false)
		{
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(handle);
			}
		}
	};

// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //
}

namespace demos
{
	class MainWindow : public MainWindow_<MainWindow>
	{
	public:
		MainWindow()
		{
			InitializeComponents();
		}
	};
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //

#define DEMO_TYPES(F)\
			F(demos::MainWindow)\
			F(demos::GroupBoxTemplate)\
			F(demos::ToolstripTemplate)\
			F(demos::ToolstripMenuTemplate)\
			F(demos::ToolstripSplitterTemplate)\
			F(demos::MenuSplitterTemplate)\
			F(demos::ButtonTemplate)\
			F(demos::CheckBoxTemplate)\
			F(demos::RadioButtonTemplate)\
			F(demos::DocumentLabelTemplate)\
			F(demos::LabelTemplate)\
			F(demos::SinglelineTextBoxTemplate)\
			F(demos::MultilineTextBoxTemplate)\
			F(demos::LeftScrollButtonTemplate)\
			F(demos::RightScrollButtonTemplate)\
			F(demos::TopScrollButtonTemplate)\
			F(demos::BottomScrollButtonTemplate)\
			F(demos::HScrollHandleTemplate)\
			F(demos::VScrollHandleTemplate)\
			F(demos::HScrollTemplate)\
			F(demos::VScrollTemplate)\
			F(demos::ScrollViewTemplate)\
			F(demos::ItemBackgroundTemplate)\
			F(demos::CheckItemBackgroundTemplate)\
			F(demos::TextListTemplate)\
			F(demos::CheckTextListTemplate)\
			F(demos::RadioTextListTemplate)\
			F(demos::ToolstripButtonTemplate)\
			F(demos::ToolstripDropdownButtonTemplate)\
			F(demos::ToolstripSplitArrowTemplate)\
			F(demos::ToolstripSplitButtonTemplate)\
			F(demos::MenuBarButtonTemplate)\
			F(demos::ComboBoxTemplate)\
			F(demos::DatePickerTemplate)\
			F(demos::ListViewColumnHeaderTemplate)\
			F(demos::ListViewTemplate)\

			DEMO_TYPES(DECL_TYPE_INFO)
			DEMO_TYPES(IMPL_TYPE_INFO)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(), NO_PARAMETER)
			END_CLASS_MEMBER(demos::MainWindow)
			
			BEGIN_CLASS_MEMBER(demos::HScrollTemplate)
				CLASS_MEMBER_BASE(GuiScrollTemplate)
				CLASS_MEMBER_CONSTRUCTOR(demos::HScrollTemplate*(), NO_PARAMETER)

				CLASS_MEMBER_GUIEVENT_HANDLER(OnHandleMouseDown, GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(OnHandleMouseUp, GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(OnHandleMouseMove, GuiMouseEventArgs)
			END_CLASS_MEMBER(demos::HScrollTemplate)
			
			BEGIN_CLASS_MEMBER(demos::VScrollTemplate)
				CLASS_MEMBER_BASE(GuiScrollTemplate)
				CLASS_MEMBER_CONSTRUCTOR(demos::VScrollTemplate*(), NO_PARAMETER)

				CLASS_MEMBER_GUIEVENT_HANDLER(OnHandleMouseDown, GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(OnHandleMouseUp, GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(OnHandleMouseMove, GuiMouseEventArgs)
			END_CLASS_MEMBER(demos::VScrollTemplate)

			DEMO_TEMPLATES(CONTROL_TEMPLATE_IMPL)


			class DemoResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					DEMO_TYPES(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class DemoResourcePlugin : public Object, public IGuiPlugin
			{
			public:
				void Load()override
				{
					GetGlobalTypeManager()->AddTypeLoader(new DemoResourceLoader);
				}

				void AfterLoad()override
				{
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(DemoResourcePlugin)

// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //
		}
	}
}

#endif

WString XmlToString(Ptr<XmlDocument> xml)
{
	MemoryStream stream;
	{
		StreamWriter writer(stream);
		XmlPrint(xml, writer);
	}
	stream.SeekFromBegin(0);
	{
		StreamReader reader(stream);
		return reader.ReadToEnd();
	}
}

void GuiMain()
{
#ifndef VCZH_DEBUG_NO_REFLECTION
	UnitTestInGuiMain();
	{
		FileStream fileStream(L"Reflection.txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);
		LogTypeManager(writer);
	}
	{
		FileStream fileStream(L"Instance.txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);
		LogInstanceLoaderManager(writer);
	}

	List<WString> errors;
	DateTime begin, end;
	List<vint64_t> timeSpans;
#define CLOCK\
	end = DateTime::LocalTime(); \
	timeSpans.Add(end.totalMilliseconds - begin.totalMilliseconds); \
	begin = end;\

	WString xmlText;
	{
		auto resource = GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResourceDataBinding.xml", errors);
		resource->Precompile(errors);
		auto xml = resource->SaveToXml(true);
		xmlText = XmlToString(xml);
	}
	
	begin = DateTime::LocalTime();
	{
		auto parser = GetParserManager()->GetParser<XmlDocument>(L"XML");
		auto xml = parser->TypedParse(xmlText, errors);
		CLOCK;
		auto resource = GuiResource::LoadFromXml(xml, L"<Unknown>", errors);
		CLOCK;
		GetInstanceLoaderManager()->SetResource(L"Demo", resource);
		CLOCK;
	}
	demos::MainWindow window;
	CLOCK;

	WString timeString;
	vint64_t totalTimeSpan = 0;
	FOREACH_INDEXER(vint64_t, timeSpan, index, timeSpans)
	{
		totalTimeSpan += timeSpan;
		if (index != 0) timeString += L", ";
		timeString += i64tow(timeSpan);
	}
	window.SetText(window.GetText() + L" " + i64tow(totalTimeSpan) + L": " + timeString + L" milliseconds");

	auto scope = window.GetScope().Obj();
	CopyFrom(errors, scope->errors, true);
	CHECK_ERROR(errors.Count() == 0, L"");

	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
#endif
}