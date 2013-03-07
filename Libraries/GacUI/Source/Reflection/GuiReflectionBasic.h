/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Basic

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONBASIC
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONBASIC

#include "..\GacUI.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
Type List
***********************************************************************/

#define GUIREFLECTIONBASIC_TYPELIST(F)\
			F(presentation::Color)\
			F(presentation::Alignment)\
			F(presentation::TextPos)\
			F(presentation::Point)\
			F(presentation::Size)\
			F(presentation::Rect)\
			F(presentation::Margin)\
			F(presentation::FontProperties)\
			F(presentation::INativeImageFrame)\
			F(presentation::INativeImage)\
			F(presentation::INativeImage::FormatType)\
			F(presentation::INativeCursor)\
			F(presentation::INativeCursor::SystemCursorType)\
			F(presentation::INativeWindow)\
			F(presentation::INativeWindow::WindowSizeState)\
			F(presentation::INativeDelay)\
			F(presentation::INativeDelay::ExecuteStatus)\
			F(presentation::GuiImageData)\
			F(presentation::DocumentModel)\
			F(presentation::GuiResource)\
			F(presentation::elements::IGuiGraphicsElement)\
			F(presentation::compositions::GuiGraphicsComposition)\
			F(presentation::compositions::GuiGraphicsComposition::MinSizeLimitation)\
			F(presentation::INativeWindowListener::HitTestResult)\
			F(presentation::compositions::GuiGraphicsSite)\
			F(presentation::compositions::GuiWindowComposition)\
			F(presentation::compositions::GuiBoundsComposition)\
			F(presentation::controls::GuiControl)\
			F(presentation::controls::GuiControl::IStyleController)\
			F(presentation::controls::GuiControl::IStyleProvider)\
			F(presentation::controls::GuiComponent)\
			F(presentation::controls::GuiControlHost)\

			GUIREFLECTIONBASIC_TYPELIST(DECL_TYPE_INFO)

/***********************************************************************
Type Declaration
***********************************************************************/

			template<>
			struct TypedValueSerializerProvider<Color>
			{
				static bool Serialize(const Color& input, WString& output);
				static bool Deserialize(const WString& input, Color& output);
			};

			template<>
			struct CustomTypeDescriptorSelector<Color>
			{
			public:
				typedef SerializableTypeDescriptor<TypedValueSerializer<Color>> CustomTypeDescriptorImpl;
			};

/***********************************************************************
Interface Proxy
***********************************************************************/

			namespace interface_proxy
			{
				class GuiControl_IStyleController : public Object, public virtual GuiControl::IStyleController
				{
				protected:
					Ptr<IValueInterfaceProxy>		proxy;

				public:
					GuiControl_IStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static GuiControl::IStyleController* Create(Ptr<IValueInterfaceProxy> _proxy)
					{
						return new GuiControl_IStyleController(_proxy);
					}

					compositions::GuiBoundsComposition* GetBoundsComposition()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GuiBoundsComposition*, GetBoundsComposition);
					}

					compositions::GuiGraphicsComposition* GetContainerComposition()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GuiGraphicsComposition*, GetContainerComposition);
					}

					void SetFocusableComposition(compositions::GuiGraphicsComposition* value)override
					{
						INVOKE_INTERFACE_PROXY(SetFocusableComposition, value);
					}

					void SetText(const WString& value)override
					{
						INVOKE_INTERFACE_PROXY(SetText, value);
					}

					void SetFont(const FontProperties& value)override
					{
						INVOKE_INTERFACE_PROXY(SetFont, value);
					}

					void SetVisuallyEnabled(bool value)override
					{
						INVOKE_INTERFACE_PROXY(SetVisuallyEnabled, value);
					}
				};
				
				class GuiControl_IStyleProvider : public Object, public virtual GuiControl::IStyleProvider
				{
				protected:
					Ptr<IValueInterfaceProxy>		proxy;

				public:
					GuiControl_IStyleProvider(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static GuiControl::IStyleProvider* Create(Ptr<IValueInterfaceProxy> _proxy)
					{
						return new GuiControl_IStyleProvider(_proxy);
					}

					void AssociateStyleController(GuiControl::IStyleController* controller)override
					{
						INVOKE_INTERFACE_PROXY(AssociateStyleController, controller);
					}

					void SetFocusableComposition(compositions::GuiGraphicsComposition* value)override
					{
						INVOKE_INTERFACE_PROXY(SetFocusableComposition, value);
					}

					void SetText(const WString& value)override
					{
						INVOKE_INTERFACE_PROXY(SetText, value);
					}

					void SetFont(const FontProperties& value)override
					{
						INVOKE_INTERFACE_PROXY(SetFont, value);
					}

					void SetVisuallyEnabled(bool value)override
					{
						INVOKE_INTERFACE_PROXY(SetVisuallyEnabled, value);
					}
				};
			}

/***********************************************************************
Type Loader
***********************************************************************/

			extern bool						LoadGuiBasicTypes();
		}
	}
}

#endif