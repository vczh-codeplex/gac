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
			F(Color)\
			F(Alignment)\
			F(TextPos)\
			F(Point)\
			F(Size)\
			F(Rect)\
			F(Margin)\
			F(FontProperties)\
			F(INativeImageFrame)\
			F(INativeImage)\
			F(INativeImage::FormatType)\
			F(INativeCursor)\
			F(INativeCursor::SystemCursorType)\
			F(GuiImageData)\
			F(DocumentModel)\
			F(GuiResource)\
			F(IGuiGraphicsElement)\
			F(GuiGraphicsComposition)\
			F(GuiGraphicsComposition::MinSizeLimitation)\
			F(INativeWindowListener::HitTestResult)\
			F(GuiGraphicsSite)\
			F(GuiWindowComposition)\
			F(GuiBoundsComposition)\
			F(GuiControl)\
			F(GuiControl::IStyleController)\
			F(GuiControl::IStyleProvider)\
			F(GuiComponent)\
			F(GuiControlHost)\

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
				class GuiControl_IStyleController : public Object, public GuiControl::IStyleController
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
				
				class GuiControl_IStyleProvider : public Object, public GuiControl::IStyleProvider
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