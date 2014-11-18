#include "GuiReflectionEvents.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			using namespace collections;

#ifndef VCZH_DEBUG_NO_REFLECTION

			GUIREFLECTIONELEMENT_TYPELIST(IMPL_TYPE_INFO)

/***********************************************************************
External Functions
***********************************************************************/

			template<typename T>
			Ptr<T> Element_Constructor()
			{
				return T::Create();
			}

			text::TextLines* GuiColorizedTextElement_GetLines(GuiColorizedTextElement* thisObject)
			{
				return &thisObject->GetLines();
			}

/***********************************************************************
Type Declaration
***********************************************************************/

#define _ ,

			BEGIN_CLASS_MEMBER(IGuiGraphicsParagraph)
				CLASS_MEMBER_BASE(IDescriptable)
			END_CLASS_MEMBER(IGuiGraphicsParagraph)

			BEGIN_ENUM_ITEM(IGuiGraphicsParagraph::CaretRelativePosition)
				ENUM_ITEM_NAMESPACE(IGuiGraphicsParagraph)
				ENUM_NAMESPACE_ITEM(CaretFirst)
				ENUM_NAMESPACE_ITEM(CaretLast)
				ENUM_NAMESPACE_ITEM(CaretLineFirst)
				ENUM_NAMESPACE_ITEM(CaretLineLast)
				ENUM_NAMESPACE_ITEM(CaretMoveLeft)
				ENUM_NAMESPACE_ITEM(CaretMoveRight)
				ENUM_NAMESPACE_ITEM(CaretMoveUp)
				ENUM_NAMESPACE_ITEM(CaretMoveDown)
			END_ENUM_ITEM(IGuiGraphicsParagraph::CaretRelativePosition)

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

				CLASS_MEMBER_METHOD_RENAME(GetPoints, GetPointsArray, NO_PARAMETER);
				CLASS_MEMBER_METHOD_RENAME(SetPoints, SetPointsArray, {L"points"});
				CLASS_MEMBER_PROPERTY(Points, GetPoints, SetPoints);
				CLASS_MEMBER_PROPERTY_FAST(Size)
				CLASS_MEMBER_PROPERTY_FAST(BorderColor)
				CLASS_MEMBER_PROPERTY_FAST(BackgroundColor)
			END_CLASS_MEMBER(GuiPolygonElement)

			BEGIN_CLASS_MEMBER(text::TextLines)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Count)
				CLASS_MEMBER_PROPERTY_FAST(TabSpaceCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RowHeight)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MaxWidth)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MaxHeight)
				CLASS_MEMBER_PROPERTY_FAST(PasswordChar)

				CLASS_MEMBER_METHOD_OVERLOAD(GetText, NO_PARAMETER, WString(text::TextLines::*)())
				CLASS_MEMBER_METHOD_OVERLOAD(GetText, {L"start" _ L"end"}, WString(text::TextLines::*)(TextPos _ TextPos))
				CLASS_MEMBER_METHOD(SetText, {L"value"})
				CLASS_MEMBER_METHOD(RemoveLines, {L"start" _ L"end"})
				CLASS_MEMBER_METHOD(IsAvailable, {L"pos"})
				CLASS_MEMBER_METHOD(Normalize, {L"pos"})
				CLASS_MEMBER_METHOD_OVERLOAD(Modify, {L"start" _ L"end" _ L"input"}, TextPos(text::TextLines::*)(TextPos _ TextPos _ const WString&))
				CLASS_MEMBER_METHOD(Clear, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ClearMeasurement, NO_PARAMETER)
				CLASS_MEMBER_METHOD(MeasureRow, {L"row"})
				CLASS_MEMBER_METHOD(GetRowWidth, {L"row"})
				CLASS_MEMBER_METHOD(GetTextPosFromPoint, {L"point"})
				CLASS_MEMBER_METHOD(GetPointFromTextPos, {L"pos"})
				CLASS_MEMBER_METHOD(GetRectFromTextPos, {L"pos"})
			END_CLASS_MEMBER(text::TextLines)

			BEGIN_STRUCT_MEMBER(text::ColorItem)
				STRUCT_MEMBER(text)
				STRUCT_MEMBER(background)
			END_STRUCT_MEMBER(text::ColorItem)

			BEGIN_STRUCT_MEMBER(text::ColorEntry)
				STRUCT_MEMBER(normal)
				STRUCT_MEMBER(selectedFocused)
				STRUCT_MEMBER(selectedUnfocused)
			END_STRUCT_MEMBER(text::ColorEntry)

			BEGIN_CLASS_MEMBER(GuiColorizedTextElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiColorizedTextElement>(), NO_PARAMETER, &Element_Constructor<GuiColorizedTextElement>)

				CLASS_MEMBER_PROPERTY_FAST(Font)
				CLASS_MEMBER_PROPERTY_FAST(PasswordChar)
				CLASS_MEMBER_PROPERTY_FAST(ViewPosition)
				CLASS_MEMBER_PROPERTY_FAST(VisuallyEnabled)
				CLASS_MEMBER_PROPERTY_FAST(Focused)
				CLASS_MEMBER_PROPERTY_FAST(CaretBegin)
				CLASS_MEMBER_PROPERTY_FAST(CaretEnd)
				CLASS_MEMBER_PROPERTY_FAST(CaretVisible)
				CLASS_MEMBER_PROPERTY_FAST(CaretColor)

				CLASS_MEMBER_EXTERNALMETHOD(GetLines, NO_PARAMETER, text::TextLines*(GuiColorizedTextElement::*)(), &GuiColorizedTextElement_GetLines)
				CLASS_MEMBER_PROPERTY_READONLY(Lines, GetLines)
				CLASS_MEMBER_PROPERTY_FAST(Colors)
			END_CLASS_MEMBER(GuiColorizedTextElement)

			BEGIN_CLASS_MEMBER(GuiDocumentElement)
				CLASS_MEMBER_BASE(IGuiGraphicsElement)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiDocumentElement>(), NO_PARAMETER, &Element_Constructor<GuiDocumentElement>)

				CLASS_MEMBER_PROPERTY_FAST(Document)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(CaretBegin)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(CaretEnd)
				CLASS_MEMBER_PROPERTY_FAST(CaretVisible)
				CLASS_MEMBER_PROPERTY_FAST(CaretColor)

				CLASS_MEMBER_METHOD(IsCaretEndPreferFrontSide, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetCaret, {L"begin" _ L"end" _ L"frontSide"})
				CLASS_MEMBER_METHOD(CalculateCaret, {L"comparingCaret" _ L"position" _ L"preferFrontSide"})
				CLASS_MEMBER_METHOD(CalculateCaretFromPoint, {L"point"})
				CLASS_MEMBER_METHOD(GetCaretBounds, {L"caret" _ L"frontSide"})
				CLASS_MEMBER_METHOD(NotifyParagraphUpdated, {L"index" _ L"oldCount" _ L"newCount" _ L"updatedText"})
				CLASS_MEMBER_METHOD(EditRun, {L"begin" _ L"end" _ L"model"})
				CLASS_MEMBER_METHOD(EditText, {L"begin" _ L"end" _ L"frontSide" _ L"text"})
				CLASS_MEMBER_METHOD(EditStyle, {L"begin" _ L"end" _ L"style"})
				CLASS_MEMBER_METHOD(EditImage, {L"begin" _ L"end" _ L"image"})
				CLASS_MEMBER_METHOD(EditImage, {L"paragraphIndex" _ L"begin" _ L"end" _ L"reference" _ L"normalStyleName" _ L"activeStyleName"})
				CLASS_MEMBER_METHOD(RemoveHyperlink, {L"paragraphIndex" _ L"begin" _ L"end"})
				CLASS_MEMBER_METHOD(EditStyleName, {L"begin" _ L"end" _ L"styleName"})
				CLASS_MEMBER_METHOD(RemoveStyleName, {L"begin" _ L"end" _ L"image"})
				CLASS_MEMBER_METHOD(RenameStyle, {L"oldStyleName" _ L"newStyleName"})
				CLASS_MEMBER_METHOD(ClearStyle, {L"begin" _ L"end"})
				CLASS_MEMBER_METHOD(SummarizeStyle, {L"begin" _ L"end"})
				CLASS_MEMBER_METHOD(SetParagraphAlignment, {L"begin" _ L"end" _ L"alignments"})
				CLASS_MEMBER_METHOD(GetHyperlinkFromPoint, {L"point"})
			END_CLASS_MEMBER(GuiDocumentElement)
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

#endif

			bool LoadGuiElementTypes()
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiElementTypeLoader;
					return manager->AddTypeLoader(loader);
				}
#endif
				return false;
			}
		}
	}
}