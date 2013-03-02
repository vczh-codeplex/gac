#include "GuiReflectionBasic.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
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
Type Constructors
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