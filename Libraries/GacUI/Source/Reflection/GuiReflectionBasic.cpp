#include "GuiReflectionBasic.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			using namespace collections;
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace parsing::xml;
			using namespace stream;

			GUIREFLECTIONBASIC_TYPELIST(IMPL_TYPE_INFO)

			bool TypedValueSerializerProvider<Color>::Serialize(const Color& input, WString& output)
			{
				output=input.ToString();
				return true;
			}

			bool TypedValueSerializerProvider<Color>::Deserialize(const WString& input, Color& output)
			{
				output=Color::Parse(input);
				return true;
			}

/***********************************************************************
External Functions
***********************************************************************/

			Ptr<INativeImage> INativeImage_Constructor(const WString& path)
			{
				return GetCurrentController()->ImageService()->CreateImageFromFile(path);
			}

			INativeCursor* INativeCursor_Constructor1()
			{
				return GetCurrentController()->ResourceService()->GetDefaultSystemCursor();
			}

			INativeCursor* INativeCursor_Constructor2(INativeCursor::SystemCursorType type)
			{
				return GetCurrentController()->ResourceService()->GetSystemCursor(type);
			}

			Ptr<DocumentModel> DocumentModel_Constructor(const WString& path)
			{
				FileStream fileStream(path, FileStream::ReadOnly);
				if(!fileStream.IsAvailable()) return 0;

				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader reader(decoderStream);
				WString xmlText=reader.ReadToEnd();

				Ptr<ParsingTable> table=XmlLoadTable();
				Ptr<XmlDocument> xml=XmlParseDocument(xmlText, table);
				if(!xml) return 0;

				return DocumentModel::LoadFromXml(xml, GetFolderPath(path));
			}

			Ptr<IValueReadonlyList> GuiGraphicsComposition_Children(GuiGraphicsComposition* thisObject)
			{
				return new ValueReadonlyListWrapper<const List<GuiGraphicsComposition*>*>(&thisObject->Children());
			}

/***********************************************************************
Type Declaration
***********************************************************************/

#define _ ,

			BEGIN_ENUM_ITEM(Alignment)
				ENUM_CLASS_ITEM(Left)
				ENUM_CLASS_ITEM(Top)
				ENUM_CLASS_ITEM(Center)
				ENUM_CLASS_ITEM(Right)
				ENUM_CLASS_ITEM(Bottom)
			END_ENUM_ITEM(Alignment)

			BEGIN_STRUCT_MEMBER(TextPos)
				STRUCT_MEMBER(row)
				STRUCT_MEMBER(column)
			END_STRUCT_MEMBER(TextPos)

			BEGIN_STRUCT_MEMBER(Point)
				STRUCT_MEMBER(x)
				STRUCT_MEMBER(y)
			END_STRUCT_MEMBER(Point)

			BEGIN_STRUCT_MEMBER(Size)
				STRUCT_MEMBER(x)
				STRUCT_MEMBER(y)
			END_STRUCT_MEMBER(Size)

			BEGIN_STRUCT_MEMBER(Rect)
				STRUCT_MEMBER(x1)
				STRUCT_MEMBER(y1)
				STRUCT_MEMBER(x2)
				STRUCT_MEMBER(y2)
			END_STRUCT_MEMBER(Rect)

			BEGIN_STRUCT_MEMBER(Margin)
				STRUCT_MEMBER(left)
				STRUCT_MEMBER(top)
				STRUCT_MEMBER(right)
				STRUCT_MEMBER(bottom)
			END_STRUCT_MEMBER(Margin)

			BEGIN_STRUCT_MEMBER(FontProperties)
				STRUCT_MEMBER(fontFamily)
				STRUCT_MEMBER(size)
				STRUCT_MEMBER(bold)
				STRUCT_MEMBER(italic)
				STRUCT_MEMBER(underline)
				STRUCT_MEMBER(strikeline)
				STRUCT_MEMBER(antialias)
				STRUCT_MEMBER(verticalAntialias)
			END_STRUCT_MEMBER(FontProperties)

			BEGIN_CLASS_MEMBER(INativeImageFrame)
				CLASS_MEMBER_METHOD(GetImage, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetSize, NO_PARAMETER)
			END_CLASS_MEMBER(INativeImageFrame)

			BEGIN_CLASS_MEMBER(INativeImage)
				CLASS_MEMBER_METHOD(GetFormat, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetFrameCount, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetFrame, {L"index"})
				CLASS_MEMBER_EXTERNALCTOR(Ptr<INativeImage>(const WString&), {L"filePath"}, &INativeImage_Constructor)
			END_CLASS_MEMBER(INativeImage)

			BEGIN_ENUM_ITEM(INativeImage::FormatType)
				ENUM_ITEM_NAMESPACE(INativeImage)
				ENUM_NAMESPACE_ITEM(Bmp)
				ENUM_NAMESPACE_ITEM(Gif)
				ENUM_NAMESPACE_ITEM(Icon)
				ENUM_NAMESPACE_ITEM(Jpeg)
				ENUM_NAMESPACE_ITEM(Png)
				ENUM_NAMESPACE_ITEM(Tiff)
				ENUM_NAMESPACE_ITEM(Wmp)
				ENUM_NAMESPACE_ITEM(Unknown)
			END_ENUM_ITEM(INativeImage::FormatType)

			BEGIN_CLASS_MEMBER(INativeCursor)
				CLASS_MEMBER_METHOD(IsSystemCursor, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetSystemCursorType, NO_PARAMETER)
				CLASS_MEMBER_EXTERNALCTOR(INativeCursor*(), NO_PARAMETER, &INativeCursor_Constructor1)
				CLASS_MEMBER_EXTERNALCTOR(INativeCursor*(INativeCursor::SystemCursorType), NO_PARAMETER, &INativeCursor_Constructor2)
			END_CLASS_MEMBER(INativeCursor)

			BEGIN_ENUM_ITEM(INativeCursor::SystemCursorType)
				ENUM_ITEM_NAMESPACE(INativeCursor)
				ENUM_NAMESPACE_ITEM(SmallWaiting)
				ENUM_NAMESPACE_ITEM(LargeWaiting)
				ENUM_NAMESPACE_ITEM(Arrow)
				ENUM_NAMESPACE_ITEM(Cross)
				ENUM_NAMESPACE_ITEM(Hand)
				ENUM_NAMESPACE_ITEM(Help)
				ENUM_NAMESPACE_ITEM(IBeam)
				ENUM_NAMESPACE_ITEM(SizeAll)
				ENUM_NAMESPACE_ITEM(SizeNESW)
				ENUM_NAMESPACE_ITEM(SizeNS)
				ENUM_NAMESPACE_ITEM(SizeNWSE)
				ENUM_NAMESPACE_ITEM(SizeWE)
			END_ENUM_ITEM(INativeCursor::FormatType)

			BEGIN_CLASS_MEMBER(GuiImageData)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiImageData>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiImageData>(Ptr<INativeImage>, vint), {L"image" _ L"frameIndex"})

				CLASS_MEMBER_METHOD(GetImage, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetFrameIndex, NO_PARAMETER)

				CLASS_MEMBER_PROPERTY_READONLY(Image, GetImage)
				CLASS_MEMBER_PROPERTY_READONLY(FrameIndex, GetFrameIndex)
			END_CLASS_MEMBER(GuiImageData)

			BEGIN_CLASS_MEMBER(DocumentModel)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<DocumentModel>(const WString&), {L"filePath"}, &DocumentModel_Constructor)
			END_CLASS_MEMBER(DocumentModel)

			BEGIN_CLASS_MEMBER(GuiResource)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiResource>(), NO_PARAMETER)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiResource>(const WString&), {L"filePath"}, &GuiResource::LoadFromXml);

				CLASS_MEMBER_METHOD(GetDocumentByPath, {L"path"})
				CLASS_MEMBER_METHOD(GetImageByPath, {L"path"})
				CLASS_MEMBER_METHOD(GetStringByPath, {L"path"})
			END_CLASS_MEMBER(GuiResource)

			BEGIN_CLASS_MEMBER(IGuiGraphicsElement)
			END_CLASS_MEMBER(IGuiGraphicsElement)

			BEGIN_CLASS_MEMBER(GuiGraphicsComposition)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Parent)
				CLASS_MEMBER_PROPERTY_FAST(OwnedElement)
				CLASS_MEMBER_PROPERTY_FAST(Visible)
				CLASS_MEMBER_PROPERTY_FAST(MinSizeLimitation)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(GlobalBounds)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(AssociatedControl)
				CLASS_MEMBER_PROPERTY_FAST(AssociatedCursor)
				CLASS_MEMBER_PROPERTY_FAST(AssociatedHitTestResult)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RelatedControl)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RelatedControlHost)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RelatedCursor)
				CLASS_MEMBER_PROPERTY_FAST(Margin)
				CLASS_MEMBER_PROPERTY_FAST(InternalMargin)
				CLASS_MEMBER_PROPERTY_FAST(PreferredMinSize)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ClientArea)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MinPreferredClientSize)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(PreferredBounds)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Bounds)

				CLASS_MEMBER_EXTERNALMETHOD(GetChildren, NO_PARAMETER, Ptr<IValueReadonlyList>(GuiGraphicsComposition::*)(), &GuiGraphicsComposition_Children)
				CLASS_MEMBER_PROPERTY_READONLY(Children, GetChildren)

				CLASS_MEMBER_METHOD(AddChild, {L"child"})
				CLASS_MEMBER_METHOD(InsertChild, {L"index" _ L"child"})
				CLASS_MEMBER_METHOD(RemoveChild, {L"child"})
				CLASS_MEMBER_METHOD(MoveChild, {L"child" _ L"newIndex"})
				CLASS_MEMBER_METHOD(Render, {L"size"})
				CLASS_MEMBER_METHOD(FindComposition, {L"location"})
				CLASS_MEMBER_METHOD(ForceCalculateSizeImmediately, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsSizeAffectParent, NO_PARAMETER)
			END_CLASS_MEMBER(GuiGraphicsComposition)

			BEGIN_ENUM_ITEM(GuiGraphicsComposition::MinSizeLimitation)
				ENUM_ITEM_NAMESPACE(GuiGraphicsComposition)
				ENUM_NAMESPACE_ITEM(NoLimit)
				ENUM_NAMESPACE_ITEM(LimitToElement)
				ENUM_NAMESPACE_ITEM(LimitToElementAndChildren)
			END_ENUM_ITEM(GuiGraphicsComposition::MinSizeLimitation)

			BEGIN_ENUM_ITEM(INativeWindowListener::HitTestResult)
				ENUM_ITEM_NAMESPACE(INativeWindowListener)
				ENUM_NAMESPACE_ITEM(BorderNoSizing)
				ENUM_NAMESPACE_ITEM(BorderLeft)
				ENUM_NAMESPACE_ITEM(BorderRight)
				ENUM_NAMESPACE_ITEM(BorderTop)
				ENUM_NAMESPACE_ITEM(BorderBottom)
				ENUM_NAMESPACE_ITEM(BorderLeftTop)
				ENUM_NAMESPACE_ITEM(BorderRightTop)
				ENUM_NAMESPACE_ITEM(BorderLeftBottom)
				ENUM_NAMESPACE_ITEM(BorderRightBottom)
				ENUM_NAMESPACE_ITEM(Title)
				ENUM_NAMESPACE_ITEM(ButtonMinimum)
				ENUM_NAMESPACE_ITEM(ButtonMaximum)
				ENUM_NAMESPACE_ITEM(ButtonClose)
				ENUM_NAMESPACE_ITEM(Client)
				ENUM_NAMESPACE_ITEM(Icon)
				ENUM_NAMESPACE_ITEM(NoDecision)
			END_ENUM_ITEM(INativeWindowListener::HitTestResult)

			BEGIN_CLASS_MEMBER(GuiGraphicsSite)
				CLASS_MEMBER_BASE(GuiGraphicsComposition)
			END_CLASS_MEMBER(GuiGraphicsSite)

			BEGIN_CLASS_MEMBER(GuiWindowComposition)
				CLASS_MEMBER_BASE(GuiGraphicsSite)
				CLASS_MEMBER_CONSTRUCTOR(GuiWindowComposition*(), NO_PARAMETER)
			END_CLASS_MEMBER(GuiWindowComposition)

			BEGIN_CLASS_MEMBER(GuiBoundsComposition)
				CLASS_MEMBER_BASE(GuiGraphicsSite)

				CLASS_MEMBER_PROPERTY_FAST(Bounds)
				
				CLASS_MEMBER_METHOD(ClearAlignmentToParent, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetAlignmentToParent, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetAlignmentToParent, {L"value"})
				CLASS_MEMBER_METHOD(IsAlignedToParent, NO_PARAMETER)
			END_CLASS_MEMBER(GuiBoundsComposition)

			BEGIN_CLASS_MEMBER(GuiControl)
				CLASS_MEMBER_CONSTRUCTOR(GuiControl*(GuiControl::IStyleController*), {L"styleController"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(StyleController)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(BoundsComposition)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ContainerComposition)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FocusableComposition)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Parent)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ChildrenCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RelatedControlHost)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(VisuallyEnabled)
				CLASS_MEMBER_PROPERTY_FAST(Enabled)
				CLASS_MEMBER_PROPERTY_FAST(Visible)
				CLASS_MEMBER_PROPERTY_FAST(Text)
				CLASS_MEMBER_PROPERTY_FAST(Font)
				CLASS_MEMBER_PROPERTY_FAST(TooltipControl)
				CLASS_MEMBER_PROPERTY_FAST(TooltipWidth)

				CLASS_MEMBER_METHOD(GetChild, {L"index"})
				CLASS_MEMBER_METHOD(AddChild, {L"control"})
				CLASS_MEMBER_METHOD(HasChild, {L"control"})
				CLASS_MEMBER_METHOD(SetFocus, NO_PARAMETER)
				CLASS_MEMBER_METHOD(DisplayTooltip, {L"location"})
				CLASS_MEMBER_METHOD(CloseTooltip, NO_PARAMETER)
				CLASS_MEMBER_METHOD_OVERLOAD(QueryService, {L"identifier"}, IDescriptable*(GuiControl::*)(const WString&))
			END_CLASS_MEMBER(GuiControl)

			BEGIN_CLASS_MEMBER(GuiControl::IStyleController)
				CLASS_MEMBER_METHOD(GetBoundsComposition, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetContainerComposition, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetFocusableComposition, {L"value"})
				CLASS_MEMBER_METHOD(SetText, {L"value"})
				CLASS_MEMBER_METHOD(SetFont, {L"value"})
				CLASS_MEMBER_METHOD(SetVisuallyEnabled, {L"value"})
			END_CLASS_MEMBER(GuiControl::IStyleController)

			BEGIN_CLASS_MEMBER(GuiControl::IStyleProvider)
				CLASS_MEMBER_METHOD(AssociateStyleController, {L"controller"})
				CLASS_MEMBER_METHOD(SetFocusableComposition, {L"value"})
				CLASS_MEMBER_METHOD(SetText, {L"value"})
				CLASS_MEMBER_METHOD(SetFont, {L"value"})
				CLASS_MEMBER_METHOD(SetVisuallyEnabled, {L"value"})
			END_CLASS_MEMBER(GuiControl::IStyleProvider)

			BEGIN_CLASS_MEMBER(GuiComponent)
			END_CLASS_MEMBER(GuiComponent)

			BEGIN_CLASS_MEMBER(GuiControlHost)
				CLASS_MEMBER_BASE(GuiControl)
				CLASS_MEMBER_CONSTRUCTOR(GuiControlHost*(GuiControl::IStyleController*), {L"styleController"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(MainComposition)
				CLASS_MEMBER_PROPERTY_FAST(ShowInTaskBar)
				CLASS_MEMBER_PROPERTY_FAST(EnabledActivate)
				CLASS_MEMBER_PROPERTY_FAST(TopMost)
				CLASS_MEMBER_PROPERTY_FAST(ClientSize)
				CLASS_MEMBER_PROPERTY_FAST(Bounds)

				CLASS_MEMBER_METHOD(ForceCalculateSizeImmediately, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Render, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetFocused, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetFocused, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetActivated, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetActivated, NO_PARAMETER)
				CLASS_MEMBER_METHOD(AddComponent, {L"component"})
				CLASS_MEMBER_METHOD(RemoveComponent, {L"component"})
				CLASS_MEMBER_METHOD(ContainsComponent, {L"component"})
				CLASS_MEMBER_METHOD(Show, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowDeactivated, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowRestored, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowMaximized, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowMinimized, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Hide, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Close, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetOpening, NO_PARAMETER)
			END_CLASS_MEMBER(GuiControlHost)

#undef _

/***********************************************************************
Type Loader
***********************************************************************/

			class GuiBasicTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					GUIREFLECTIONBASIC_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			bool LoadGuiBasicTypes()
			{
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiBasicTypeLoader;
					return manager->AddTypeLoader(loader);
				}
				return false;
			}
		}
	}
}