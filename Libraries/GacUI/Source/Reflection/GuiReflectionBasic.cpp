#include "GuiReflectionBasic.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
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
Type Declaration
***********************************************************************/

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