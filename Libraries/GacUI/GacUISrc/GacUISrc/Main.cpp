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

//#define GUI_GRAPHICS_RENDERER_GDI
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

#define PROJECT_CONTROLTEMPLATE

#ifndef PROJECT_CONTROLTEMPLATE

namespace demos
{
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
#define DEMO_TYPES(F)\
			F(demos::MainWindow)

			DEMO_TYPES(DECL_TYPE_INFO)
			DEMO_TYPES(IMPL_TYPE_INFO)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(), NO_PARAMETER)
			END_CLASS_MEMBER(demos::MainWindow)

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
		}
	}
}

void GuiMain()
{
	{
		List<WString> errors;
		auto resource = GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResource.xml", errors);
		GetInstanceLoaderManager()->SetResource(L"Demo", resource);
	}
	demos::MainWindow window;
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}

#else

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
	F(WindowTemplate, GuiWindowTemplate)\
	F(TabHeaderTemplate, GuiSelectableButtonTemplate)\
	F(TabDropdownTemplate, GuiButtonTemplate)\
	F(TabTemplate, GuiTabTemplate)\
	F(GroupBoxTemplate, GuiControlTemplate)\
	F(ToolstripTemplate, GuiControlTemplate)\
	F(ToolstripMenuTemplate, GuiMenuTemplate)\
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
	F(ProgressBarTemplate, GuiScrollTemplate)\
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
	F(MenuItemButtonTemplate, GuiToolstripButtonTemplate)\
	F(ComboBoxTemplate, GuiDateComboBoxTemplate)\
	F(DatePickerTemplate, GuiDatePickerTemplate)\
	F(ListViewColumnHeaderTemplate, GuiListViewColumnHeaderTemplate)\
	F(ListViewTemplate, GuiListViewTemplate)\
	F(ExpandingDecoratorTemplate, GuiSelectableButtonTemplate)\
	F(TreeViewTemplate, GuiTreeViewTemplate)\

	DEMO_TEMPLATES(CONTROL_TEMPLATE_DECL)

	class ScrollTrackerTemplate : public GuiScrollTemplate, public GuiInstancePartialClass<GuiScrollTemplate>
	{
	protected:
		GuiGraphicsComposition*				handle;
		Point								draggingStartLocation;
		bool								draggingHandle;
	
		virtual vint						Size(const Rect& value) = 0;
		virtual vint						Component(const Rect& value) = 0;
		virtual vint						Component(const Point& value) = 0;
		virtual vint						Component(const GuiMouseEventArgs& value) = 0;
	public:
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
				vint totalPixels = Size(handle->GetParent()->GetBounds());
				vint currentOffset = Component(handle->GetBounds());
				vint newOffset = currentOffset + (Component(arguments) - Component(draggingStartLocation));
				vint totalSize = GetTotalSize();
				double ratio = (double)newOffset / totalPixels;
				vint newPosition = (vint)(ratio * totalSize);

				vint offset1 = (vint)(((double)newPosition / totalSize) * totalPixels);
				vint offset2 = vint(((double)(newPosition + 1) / totalSize) * totalPixels);
				vint delta1 = abs((int)(offset1 - newOffset));
				vint delta2 = abs((int)(offset2 - newOffset));

				if(delta1 < delta2)
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
		ScrollTrackerTemplate(const WString& typeName)
			:GuiInstancePartialClass<GuiScrollTemplate>(typeName)
			, handle(0)
			, draggingHandle(false)
		{
		}
	};

	class HScrollTemplate : public ScrollTrackerTemplate, public Description<HScrollTemplate>
	{
	protected:
		vint Size(const Rect& value)override
		{
			return value.Width();
		}

		vint Component(const Rect& value)override
		{
			return value.Left();
		}

		vint Component(const Point& value)override
		{
			return value.x;
		}

		vint Component(const GuiMouseEventArgs& value)override
		{
			return value.x;
		}
	public:
		HScrollTemplate()
			:ScrollTrackerTemplate(L"demos::HScrollTemplate")
		{
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(handle);
			}
		}
	};

	class VScrollTemplate : public ScrollTrackerTemplate, public Description<VScrollTemplate>
	{
	protected:
		vint Size(const Rect& value)override
		{
			return value.Height();
		}

		vint Component(const Rect& value)override
		{
			return value.Top();
		}

		vint Component(const Point& value)override
		{
			return value.y;
		}

		vint Component(const GuiMouseEventArgs& value)override
		{
			return value.y;
		}
	public:
		VScrollTemplate()
			:ScrollTrackerTemplate(L"demos::VScrollTemplate")
		{
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(handle);
			}
		}
	};

	class HTrackerTemplate : public ScrollTrackerTemplate, public Description<HTrackerTemplate>
	{
	protected:
		vint Size(const Rect& value)override
		{
			return value.Width();
		}

		vint Component(const Rect& value)override
		{
			return value.Left();
		}

		vint Component(const Point& value)override
		{
			return value.x;
		}

		vint Component(const GuiMouseEventArgs& value)override
		{
			return value.x;
		}
	public:
		HTrackerTemplate()
			:ScrollTrackerTemplate(L"demos::HTrackerTemplate")
		{
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(handle);
			}
		}
	};

	class VTrackerTemplate : public ScrollTrackerTemplate, public Description<VTrackerTemplate>
	{
	protected:
		vint Size(const Rect& value)override
		{
			return value.Height();
		}

		vint Component(const Rect& value)override
		{
			return value.Top();
		}

		vint Component(const Point& value)override
		{
			return value.y;
		}

		vint Component(const GuiMouseEventArgs& value)override
		{
			return value.y;
		}
	public:
		VTrackerTemplate()
			:ScrollTrackerTemplate(L"demos::VTrackerTemplate")
		{
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(handle);
			}
		}
	};

	template<typename TImpl>
	class MainWindow_ : public GuiWindow, public GuiInstancePartialClass<GuiWindow>, public Description<TImpl>
	{
	protected:
		static GuiWindow::IStyleController* CreateStyle()
		{
			List<description::ITypeDescriptor*> types;
			types.Add(description::GetTypeDescriptor<demos::WindowTemplate>());
			auto factory = GuiTemplate::IFactory::CreateTemplateFactory(types);
			auto style = new GuiWindowTemplate_StyleProvider(factory);
			return style;
		}

		void InitializeComponents()
		{
			InitializeFromResource();
		}
	public:
		MainWindow_()
			:GuiWindow(CreateStyle())
			//:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,GuiInstancePartialClass<GuiWindow>(L"demos::MainWindow")
		{
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
			F(demos::WindowTemplate)\
			F(demos::TabHeaderTemplate)\
			F(demos::TabDropdownTemplate)\
			F(demos::TabTemplate)\
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
			F(demos::HTrackerTemplate)\
			F(demos::VTrackerTemplate)\
			F(demos::ProgressBarTemplate)\
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
			F(demos::MenuItemButtonTemplate)\
			F(demos::ComboBoxTemplate)\
			F(demos::DatePickerTemplate)\
			F(demos::ListViewColumnHeaderTemplate)\
			F(demos::ListViewTemplate)\
			F(demos::ExpandingDecoratorTemplate)\
			F(demos::TreeViewTemplate)\

#define DEFINE_SCROLL_TRACKER_TEMPLATE(CLASSNAME)\
			BEGIN_CLASS_MEMBER(CLASSNAME)\
				CLASS_MEMBER_BASE(GuiScrollTemplate)\
				CLASS_MEMBER_CONSTRUCTOR(CLASSNAME*(), NO_PARAMETER)\
				CLASS_MEMBER_GUIEVENT_HANDLER(OnHandleMouseDown, GuiMouseEventArgs)\
				CLASS_MEMBER_GUIEVENT_HANDLER(OnHandleMouseUp, GuiMouseEventArgs)\
				CLASS_MEMBER_GUIEVENT_HANDLER(OnHandleMouseMove, GuiMouseEventArgs)\
			END_CLASS_MEMBER(CLASSNAME)\

			DEMO_TYPES(DECL_TYPE_INFO)
			DEMO_TYPES(IMPL_TYPE_INFO)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(), NO_PARAMETER)
			END_CLASS_MEMBER(demos::MainWindow)

			DEFINE_SCROLL_TRACKER_TEMPLATE(demos::HScrollTemplate)
			DEFINE_SCROLL_TRACKER_TEMPLATE(demos::VScrollTemplate)
			DEFINE_SCROLL_TRACKER_TEMPLATE(demos::HTrackerTemplate)
			DEFINE_SCROLL_TRACKER_TEMPLATE(demos::VTrackerTemplate)

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

//#define RUN_GENERATE_METADATA
//#define RUN_GENERATE_PRECOMPILED_RESOURCE
//#define RUN_SHOW_WINDOW

void GuiMain()
{
#ifndef VCZH_DEBUG_NO_REFLECTION
#ifdef RUN_GENERATE_METADATA
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
#endif

	List<WString> errors;
	vint64_t loadTime = 0, desTime = 0;

#ifdef RUN_GENERATE_PRECOMPILED_RESOURCE
	{
		DateTime begin = DateTime::LocalTime();
		auto resource = GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResourceDataBinding.xml", errors);
		resource->Precompile(errors);
		GetInstanceLoaderManager()->SetResource(L"Demo", resource);
		DateTime end = DateTime::LocalTime();
		loadTime = end.totalMilliseconds - begin.totalMilliseconds;
		CHECK_ERROR(errors.Count() == 0, L"");
		
		{
			FileStream fileStream(L"Precompiled.xml", FileStream::WriteOnly);
			BomEncoder encoder(BomEncoder::Utf8);
			EncoderStream stream(fileStream, encoder);
			StreamWriter writer(stream);
			
			auto xml = resource->SaveToXml(true);
			XmlPrint(xml, writer);
		}
		{
			FileStream fileStream(L"Precompiled.binary", FileStream::WriteOnly);
			LzwEncoder encoder;
			EncoderStream stream(fileStream, encoder);
			resource->SavePrecompiledBinary(stream);
		}
		{
			FileStream fileStream(L"Precompiled.binary.uncompressed", FileStream::WriteOnly);
			resource->SavePrecompiledBinary(fileStream);
		}
		{
			auto testResource = GuiResource::LoadFromXml(L"Precompiled.xml", errors);
			CHECK_ERROR(errors.Count() == 0, L"");
		}
		{
			FileStream fileStream(L"Precompiled.binary", FileStream::ReadOnly);
			LzwDecoder decoder;
			DecoderStream stream(fileStream, decoder);

			auto testResource = GuiResource::LoadPrecompiledBinary(stream, errors);
			CHECK_ERROR(errors.Count() == 0, L"");
		}
	}
#else
	{
		DateTime begin = DateTime::LocalTime();
		FileStream fileStream(L"Precompiled.binary.uncompressed", FileStream::ReadOnly);
		//LzwDecoder decoder;
		//DecoderStream stream(fileStream, decoder);
		auto resource = GuiResource::LoadPrecompiledBinary(fileStream, errors);
		GetInstanceLoaderManager()->SetResource(L"Demo", resource);
		DateTime end = DateTime::LocalTime();
		loadTime = end.totalMilliseconds - begin.totalMilliseconds;
	}
#endif
	DateTime begin = DateTime::LocalTime();
	demos::MainWindow window;
	DateTime end = DateTime::LocalTime();

	desTime = end.totalMilliseconds - begin.totalMilliseconds;
	window.SetText(window.GetText() + L" " + i64tow(loadTime) + L", " + i64tow(desTime) + L" milliseconds");

	auto scope = window.GetScope().Obj();
	CopyFrom(errors, scope->errors, true);
	CHECK_ERROR(errors.Count() == 0, L"");

	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
#ifdef RUN_SHOW_WINDOW
	GetApplication()->Run(&window);
#endif
#endif
}

#endif