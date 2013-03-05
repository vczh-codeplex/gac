#include "GuiReflectionCompositions.h"

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

			GUIREFLECTIONCOMPOSITION_TYPELIST(IMPL_TYPE_INFO)

/***********************************************************************
External Functions
***********************************************************************/

			Ptr<IValueReadonlyList> GuiStackComposition_GetStackItems(GuiStackComposition* thisObject)
			{
				return new ValueReadonlyListWrapper<const List<GuiStackItemComposition*>*>(&thisObject->GetStackItems());
			}

			void GuiTableComposition_SetRows(GuiTableComposition* thisObject, vint value)
			{
				int columns=thisObject->GetColumns();
				if(columns<=0) columns=1;
				thisObject->SetRowsAndColumns(value, columns);
			}

			void GuiTableComposition_SetColumns(GuiTableComposition* thisObject, vint value)
			{
				int row=thisObject->GetRows();
				if(row<=0) row=1;
				thisObject->SetRowsAndColumns(row, value);
			}

/***********************************************************************
Type Declaration
***********************************************************************/

#define _ ,

			BEGIN_CLASS_MEMBER(GuiStackComposition)
				CLASS_MEMBER_BASE(GuiBoundsComposition)
				CLASS_MEMBER_CONSTRUCTOR(GuiStackComposition*(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(GetStackItems, NO_PARAMETER, Ptr<IValueReadonlyList>(GuiStackComposition::*)(), &GuiStackComposition_GetStackItems)
				CLASS_MEMBER_PROPERTY_READONLY(StackItems, GetStackItems)

				CLASS_MEMBER_PROPERTY_FAST(Direction)
				CLASS_MEMBER_PROPERTY_FAST(Padding)
				CLASS_MEMBER_PROPERTY_FAST(ExtraMargin)

				CLASS_MEMBER_METHOD(InsertStackItem, {L"index" _ L"item"})
				CLASS_MEMBER_METHOD(IsStackItemClipped, NO_PARAMETER)
			END_CLASS_MEMBER(GuiStackComposition)

			BEGIN_ENUM_ITEM(GuiStackComposition::Direction)
				ENUM_ITEM_NAMESPACE(GuiStackComposition)
				ENUM_NAMESPACE_ITEM(Horizontal)
				ENUM_NAMESPACE_ITEM(Vertical)
			END_ENUM_ITEM(GuiStackComposition::Direction)

			BEGIN_CLASS_MEMBER(GuiStackItemComposition)
				CLASS_MEMBER_BASE(GuiGraphicsSite)
				CLASS_MEMBER_CONSTRUCTOR(GuiStackItemComposition*(), NO_PARAMETER)

				CLASS_MEMBER_PROPERTY_FAST(Bounds)
				CLASS_MEMBER_PROPERTY_FAST(ExtraMargin)
			END_CLASS_MEMBER(GuiStackItemComposition)

			BEGIN_STRUCT_MEMBER(GuiCellOption)
				STRUCT_MEMBER(composeType)
				STRUCT_MEMBER(absolute)
				STRUCT_MEMBER(percentage)
			END_STRUCT_MEMBER(GuiCellOption)

			BEGIN_ENUM_ITEM(GuiCellOption::ComposeType)
				ENUM_ITEM_NAMESPACE(GuiCellOption)
				ENUM_NAMESPACE_ITEM(Absolute)
				ENUM_NAMESPACE_ITEM(Percentage)
				ENUM_NAMESPACE_ITEM(MinSize)
			END_ENUM_ITEM(GuiCellOption::ComposeType)

			BEGIN_CLASS_MEMBER(GuiTableComposition)
				CLASS_MEMBER_BASE(GuiBoundsComposition)
				CLASS_MEMBER_CONSTRUCTOR(GuiTableComposition*(), NO_PARAMETER)

				CLASS_MEMBER_PROPERTY_FAST(CellPadding)

				CLASS_MEMBER_METHOD(GetRows, NO_PARAMETER)
				CLASS_MEMBER_EXTERNALMETHOD(SetRows, {L"value"}, void(GuiTableComposition::*)(vint), &GuiTableComposition_SetRows)
				CLASS_MEMBER_PROPERTY(Rows, GetRows, SetRows)
				CLASS_MEMBER_METHOD(GetColumns, NO_PARAMETER)
				CLASS_MEMBER_EXTERNALMETHOD(SetColumns, {L"value"}, void(GuiTableComposition::*)(vint), &GuiTableComposition_SetColumns)
				CLASS_MEMBER_PROPERTY(Columns, GetColumns, SetColumns)
				CLASS_MEMBER_METHOD(SetRowsAndColumns, {L"rows" _ L"columns"})

				CLASS_MEMBER_METHOD(GetSitedCell, {L"rows" _ L"columns"})
				CLASS_MEMBER_METHOD(GetRowOption, {L"row"})
				CLASS_MEMBER_METHOD(SetRowOption, {L"row" _ L"option"})
				CLASS_MEMBER_METHOD(GetColumnOption, {L"column"})
				CLASS_MEMBER_METHOD(SetColumnOption, {L"column" _ L"option"})
				CLASS_MEMBER_METHOD(GetCellArea, NO_PARAMETER)
				CLASS_MEMBER_METHOD(UpdateCellBounds, NO_PARAMETER)
			END_CLASS_MEMBER(GuiTableComposition)

			BEGIN_CLASS_MEMBER(GuiCellComposition)
				CLASS_MEMBER_BASE(GuiGraphicsSite)
				CLASS_MEMBER_CONSTRUCTOR(GuiCellComposition*(), NO_PARAMETER)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(TableParent)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Row)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RowSpan)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Column)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ColumnSpan)

				CLASS_MEMBER_METHOD(SetSite, {L"row" _ L"column" _ L"rowSpan" _ L"columnSpan"})
			END_CLASS_MEMBER(GuiCellComposition)

			BEGIN_CLASS_MEMBER(GuiSideAlignedComposition)
				CLASS_MEMBER_BASE(GuiGraphicsSite)
				CLASS_MEMBER_CONSTRUCTOR(GuiSideAlignedComposition*(), NO_PARAMETER)
				
				CLASS_MEMBER_PROPERTY_FAST(Direction)
				CLASS_MEMBER_PROPERTY_FAST(MaxLength)
				CLASS_MEMBER_PROPERTY_FAST(MaxRatio)
			END_CLASS_MEMBER(GuiSideAlignedComposition)

			BEGIN_ENUM_ITEM(GuiSideAlignedComposition::Direction)
				ENUM_ITEM_NAMESPACE(GuiSideAlignedComposition)
				ENUM_NAMESPACE_ITEM(Left)
				ENUM_NAMESPACE_ITEM(Top)
				ENUM_NAMESPACE_ITEM(Right)
				ENUM_NAMESPACE_ITEM(Bottom)
			END_ENUM_ITEM(GuiSideAlignedComposition::Direction)

			BEGIN_CLASS_MEMBER(GuiPartialViewComposition)
				CLASS_MEMBER_BASE(GuiGraphicsSite)
				CLASS_MEMBER_CONSTRUCTOR(GuiPartialViewComposition*(), NO_PARAMETER)
				
				CLASS_MEMBER_PROPERTY_FAST(WidthRatio)
				CLASS_MEMBER_PROPERTY_FAST(WidthPageSize)
				CLASS_MEMBER_PROPERTY_FAST(HeightRatio)
				CLASS_MEMBER_PROPERTY_FAST(HeightPageSize)
			END_CLASS_MEMBER(GuiPartialViewComposition)

#undef _

/***********************************************************************
Type Loader
***********************************************************************/

			class GuiCompositionTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					GUIREFLECTIONCOMPOSITION_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			bool LoadGuiCompositionTypes()
			{
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiCompositionTypeLoader;
					return manager->AddTypeLoader(loader);
				}
				return false;
			}
		}
	}
}