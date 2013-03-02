#include "GuiReflectionElements.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

			GUIREFLECTIONELEMENT_TYPELIST(IMPL_TYPE_INFO)

/***********************************************************************
Type Constructors
***********************************************************************/

			template<typename T>
			Ptr<T> Element_Constructor()
			{
				return T::Create();
			}

/***********************************************************************
Type Declaration
***********************************************************************/

#define _ ,

			BEGIN_ENUM_ITEM(ElementShape)
				ENUM_CLASS_ITEM(Rectangle)
				ENUM_CLASS_ITEM(Ellipse)
			END_ENUM_ITEM(ElementShape)

			BEGIN_CLASS_MEMBER(GuiSolidBorderElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiSolidBorderElement>(), NO_PARAMETER, &Element_Constructor<GuiSolidBorderElement>)

				CLASS_MEMBER_PROPERTY_FAST(Color)
				CLASS_MEMBER_PROPERTY_FAST(Shape)
			END_CLASS_MEMBER(GuiSolidBorderElement)

			BEGIN_CLASS_MEMBER(GuiRoundBorderElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiRoundBorderElement>(), NO_PARAMETER, &Element_Constructor<GuiRoundBorderElement>)
				
				CLASS_MEMBER_PROPERTY_FAST(Color)
				CLASS_MEMBER_PROPERTY_FAST(Radius)
			END_CLASS_MEMBER(GuiRoundBorderElement)

			BEGIN_CLASS_MEMBER(Gui3DBorderElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<Gui3DBorderElement>(), NO_PARAMETER, &Element_Constructor<Gui3DBorderElement>)
				
				CLASS_MEMBER_METHOD(SetColors, {L"value1" _ L"value2"})

				CLASS_MEMBER_PROPERTY_FAST(Color1)
				CLASS_MEMBER_PROPERTY_FAST(Color2)
			END_CLASS_MEMBER(Gui3DBorderElement)

			BEGIN_CLASS_MEMBER(Gui3DSplitterElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<Gui3DSplitterElement>(), NO_PARAMETER, &Element_Constructor<Gui3DSplitterElement>)
				
				CLASS_MEMBER_METHOD(SetColors, {L"value1" _ L"value2"})

				CLASS_MEMBER_PROPERTY_FAST(Color1)
				CLASS_MEMBER_PROPERTY_FAST(Color2)
				CLASS_MEMBER_PROPERTY_FAST(Direction)
			END_CLASS_MEMBER(Gui3DSplitterElement)

			BEGIN_ENUM_ITEM(Gui3DSplitterElement::Direction)
				ENUM_ITEM_NAMESPACE(Gui3DSplitterElement)
				ENUM_NAMESPACE_ITEM(Horizontal)
				ENUM_NAMESPACE_ITEM(Vertical)
			END_ENUM_ITEM(Gui3DSplitterElement::Direction)

			BEGIN_CLASS_MEMBER(GuiSolidBackgroundElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiSolidBackgroundElement>(), NO_PARAMETER, &Element_Constructor<GuiSolidBackgroundElement>)
				
				CLASS_MEMBER_PROPERTY_FAST(Color)
				CLASS_MEMBER_PROPERTY_FAST(Shape)
			END_CLASS_MEMBER(GuiSolidBackgroundElement)

			BEGIN_CLASS_MEMBER(GuiGradientBackgroundElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiGradientBackgroundElement>(), NO_PARAMETER, &Element_Constructor<GuiGradientBackgroundElement>)
				
				CLASS_MEMBER_METHOD(SetColors, {L"value1" _ L"value2"})

				CLASS_MEMBER_PROPERTY_FAST(Color1)
				CLASS_MEMBER_PROPERTY_FAST(Color2)
				CLASS_MEMBER_PROPERTY_FAST(Direction)
				CLASS_MEMBER_PROPERTY_FAST(Shape)
			END_CLASS_MEMBER(GuiGradientBackgroundElement)

			BEGIN_ENUM_ITEM(GuiGradientBackgroundElement::Direction)
				ENUM_ITEM_NAMESPACE(GuiGradientBackgroundElement)
				ENUM_NAMESPACE_ITEM(Horizontal)
				ENUM_NAMESPACE_ITEM(Vertical)
			END_ENUM_ITEM(GuiGradientBackgroundElement::Direction)

			BEGIN_CLASS_MEMBER(GuiSolidLabelElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiSolidLabelElement>(), NO_PARAMETER, &Element_Constructor<GuiSolidLabelElement>)
				
				CLASS_MEMBER_METHOD(SetAlignments, {L"horizontal" _ L"vertical"})

				CLASS_MEMBER_PROPERTY_FAST(Color)
				CLASS_MEMBER_PROPERTY_FAST(Font)
				CLASS_MEMBER_PROPERTY_FAST(Text)
				CLASS_MEMBER_PROPERTY_FAST(HorizontalAlignment)
				CLASS_MEMBER_PROPERTY_FAST(VerticalAlignment)
				CLASS_MEMBER_PROPERTY_FAST(WrapLine)
				CLASS_MEMBER_PROPERTY_FAST(Ellipse)
				CLASS_MEMBER_PROPERTY_FAST(Multiline)
				CLASS_MEMBER_PROPERTY_FAST(WrapLineHeightCalculation)
			END_CLASS_MEMBER(GuiSolidLabelElement)

			BEGIN_CLASS_MEMBER(GuiImageFrameElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiImageFrameElement>(), NO_PARAMETER, &Element_Constructor<GuiImageFrameElement>)

				CLASS_MEMBER_METHOD(GetImage, NO_PARAMETER)
				CLASS_MEMBER_METHOD_OVERLOAD(SetImage, {L"value"}, void(GuiImageFrameElement::*)(Ptr<INativeImage>))
				CLASS_MEMBER_METHOD_OVERLOAD(SetImage, {L"image" _  L"frameIndex"}, void(GuiImageFrameElement::*)(Ptr<INativeImage> _ vint))
				CLASS_MEMBER_PROPERTY(Image, GetImage, SetImage)

				CLASS_MEMBER_PROPERTY_FAST(FrameIndex)
				CLASS_MEMBER_PROPERTY_FAST(HorizontalAlignment)
				CLASS_MEMBER_PROPERTY_FAST(VerticalAlignment)
				CLASS_MEMBER_PROPERTY_FAST(Stretch)
				CLASS_MEMBER_PROPERTY_FAST(Enabled)
			END_CLASS_MEMBER(GuiImageFrameElement)

			BEGIN_CLASS_MEMBER(GuiPolygonElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiPolygonElement>(), NO_PARAMETER, &Element_Constructor<GuiPolygonElement>)

				CLASS_MEMBER_PROPERTY_FAST(Size)
				CLASS_MEMBER_PROPERTY_FAST(BorderColor)
				CLASS_MEMBER_PROPERTY_FAST(BackgroundColor)
			END_CLASS_MEMBER(GuiPolygonElement)

#undef _

/***********************************************************************
Type Loader
***********************************************************************/

			class GuiElementTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					GUIREFLECTIONELEMENT_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			bool LoadGuiElementTypes()
			{
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiElementTypeLoader;
					return manager->AddTypeLoader(loader);
				}
				return false;
			}
		}
	}
}