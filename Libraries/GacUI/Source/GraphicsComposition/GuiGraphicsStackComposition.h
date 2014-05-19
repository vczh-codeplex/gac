/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Composition System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSSTACKCOMPOSITION
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSSTACKCOMPOSITION

#include "GuiGraphicsBasicComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace compositions
		{

/***********************************************************************
Stack Compositions
***********************************************************************/

			class GuiStackComposition;
			class GuiStackItemComposition;

			/// <summary>
			/// Represents a stack composition.
			/// </summary>
			class GuiStackComposition : public GuiBoundsComposition, public Description<GuiStackComposition>
			{
				friend class GuiStackItemComposition;

				typedef collections::List<GuiStackItemComposition*>				ItemCompositionList;
			public:
				/// <summary>Stack item layout direction.</summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.compositions.GuiStackComposition.Direction]Stack items is layouted horizontally.</summary>
					Horizontal,
					/// <summary>[T:vl.presentation.compositions.GuiStackComposition.Direction]Stack items is layouted vertically.</summary>
					Vertical,
				};
			protected:
				Direction							direction;
				ItemCompositionList					stackItems;
				collections::Array<Rect>			stackItemBounds;
				Size								stackItemTotalSize;
				vint								padding;
				Rect								previousBounds;
				Margin								extraMargin;
				GuiStackItemComposition*			ensuringVisibleStackItem;

				void								UpdateStackItemBounds();
				void								FixStackItemSizes();
				void								OnBoundsChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
				void								OnChildInserted(GuiGraphicsComposition* child)override;
				void								OnChildRemoved(GuiGraphicsComposition* child)override;
			public:
				GuiStackComposition();
				~GuiStackComposition();

				/// <summary>Get all stack items inside the stack composition.</summary>
				/// <returns>All stack items inside the stack composition.</returns>
				const ItemCompositionList&			GetStackItems();
				/// <summary>Insert an stack item at a specified position.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="index">The position.</param>
				/// <param name="item">The statck item to insert.</param>
				bool								InsertStackItem(vint index, GuiStackItemComposition* item);
				
				/// <summary>Get the stack item layout direction.</summary>
				/// <returns>The stack item layout direction.</returns>
				Direction							GetDirection();
				/// <summary>Set the stack item layout direction.</summary>
				/// <param name="value">The stack item layout direction.</param>
				void								SetDirection(Direction value);
				/// <summary>Get the stack item padding.</summary>
				/// <returns>The stack item padding.</returns>
				vint								GetPadding();
				/// <summary>Set the stack item padding.</summary>
				/// <param name="value">The stack item padding.</param>
				void								SetPadding(vint value);

				Size								GetMinPreferredClientSize()override;
				Rect								GetBounds()override;
				
				/// <summary>Get the extra margin inside the stack composition.</summary>
				/// <returns>The extra margin inside the stack composition.</returns>
				Margin								GetExtraMargin();
				/// <summary>Set the extra margin inside the stack composition.</summary>
				/// <param name="value">The extra margin inside the stack composition.</param>
				void								SetExtraMargin(Margin value);
				/// <summary>Test is any stack item clipped in the stack direction.</summary>
				/// <returns>Returns true if any stack item is clipped.</returns>
				bool								IsStackItemClipped();
				/// <summary>Make an item visible as complete as possible.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="index">The index of the item.</param>
				bool								EnsureVisible(vint index);
			};
			
			/// <summary>
			/// Represents a stack item composition of a <see cref="GuiStackComposition"/>.
			/// </summary>
			class GuiStackItemComposition : public GuiGraphicsSite, public Description<GuiStackItemComposition>
			{
				friend class GuiStackComposition;
			protected:
				GuiStackComposition*				stackParent;
				Rect								bounds;
				Margin								extraMargin;

				void								OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent)override;
				Size								GetMinSize();
			public:
				GuiStackItemComposition();
				~GuiStackItemComposition();
				
				bool								IsSizeAffectParent()override;
				Rect								GetBounds()override;
				/// <summary>Set the expected bounds of a stack item. In most of the cases only the size of the bounds is used.</summary>
				/// <param name="value">The expected bounds of a stack item.</param>
				void								SetBounds(Rect value);
				
				/// <summary>Get the extra margin for this stack item. An extra margin is used to enlarge the bounds of the stack item, but only the non-extra part will be used for decide the stack item layout.</summary>
				/// <returns>The extra margin for this stack item.</returns>
				Margin								GetExtraMargin();
				/// <summary>Set the extra margin for this stack item. An extra margin is used to enlarge the bounds of the stack item, but only the non-extra part will be used for decide the stack item layout.</summary>
				/// <param name="value">The extra margin for this stack item.</param>
				void								SetExtraMargin(Margin value);
			};
		}
	}
}

#endif