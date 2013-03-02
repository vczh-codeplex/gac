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
Type Loader
***********************************************************************/

			extern bool						LoadGuiBasicTypes();
		}
	}
}

#endif