/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::GDI Provider for Windows Implementation::Renderer

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSUNISCRIBE
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSUNISCRIBE

#include "GuiGraphicsLayoutProviderWindowsGDI.h"
#include "GuiGraphicsWindowsGDI.h"
#include <usp10.h>

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_gdi
		{
			using namespace elements;
			using namespace collections;
			using namespace windows;

/***********************************************************************
UniscribeFragment
***********************************************************************/

			struct UniscribeFragment
			{
				//***************************** Document Data (Text)
				FontProperties									fontStyle;
				Color											fontColor;
				WString											text;
				Ptr<WinFont>									fontObject;
				vint											interactionId;
				//***************************** Document Data (Element)
				Ptr<IGuiGraphicsElement>						element;
				IGuiGraphicsParagraph::InlineObjectProperties	inlineObjectProperties;
				List<Ptr<UniscribeFragment>>					cachedTextFragment;

				UniscribeFragment();

				WString							GetFingerprint();
				Ptr<UniscribeFragment>			Copy();
			};

/***********************************************************************
UniscribeGlyphData
***********************************************************************/

			struct UniscribeGlyphData
			{
				//***************************** Uniscribe Data
				Array<WORD>						glyphs;
				Array<SCRIPT_VISATTR>			glyphVisattrs;
				Array<int>						glyphAdvances;
				Array<GOFFSET>					glyphOffsets;
				Array<WORD>						charCluster;
				ABC								runAbc;
				SCRIPT_ANALYSIS					sa;

				UniscribeGlyphData();

				void							ClearUniscribeData(vint glyphCount, vint length);
				bool							BuildUniscribeData(WinDC* dc, SCRIPT_ITEM* scriptItem, SCRIPT_CACHE& scriptCache, const wchar_t* runText, vint length, List<vint>& breakings, List<bool>& breakingAvailabilities);
				void							BuildUniscribeData(SCRIPT_STRING_ANALYSIS ssa, SCRIPT_ITEM* scriptItem);
			};

/***********************************************************************
UniscribeItem
***********************************************************************/

			class UniscribeItem : public Object
			{
			public:
				//***************************** Uniscribe Data
				SCRIPT_ITEM						scriptItem;
				vint							start;			// start from line
				vint							length;
				const wchar_t*					itemText;
				Array<SCRIPT_LOGATTR>			charLogattrs;

				UniscribeItem();
				~UniscribeItem();

				void							ClearUniscribeData();
				bool							BuildUniscribeData();
				bool							IsRightToLeft();
			};

/***********************************************************************
UniscribeRun
***********************************************************************/

			class UniscribeRun : public Object
			{
			public:
				struct RunFragmentBounds
				{
					vint						start;			// start from run
					vint						length;
					Rect						bounds;
				};
				
				//***************************** Document Data
				UniscribeFragment*				documentFragment;
				UniscribeItem*					scriptItem;
				//***************************** Uniscribe Data
				vint							start;			// start from line
				vint							length;
				const wchar_t*					runText;
				//***************************** Layout Data
				List<RunFragmentBounds>			fragmentBounds;

				UniscribeRun();
				~UniscribeRun();

				virtual bool					BuildUniscribeData(WinDC* dc, List<vint>& breakings)=0;
				virtual vint					SumWidth(vint charStart, vint charLength)=0;
				virtual vint					SumHeight()=0;
				virtual void					SearchForLineBreak(vint tempStart, vint maxWidth, bool firstRun, vint& charLength, vint& charAdvances)=0;
				virtual void					Render(WinDC* dc, vint fragmentBoundsIndex, vint offsetX, vint offsetY)=0;
				virtual bool					HitTestPoint(Point point, vint& start, vint& length, vint& interactionId);
			};

/***********************************************************************
UniscribeTextRun
***********************************************************************/

			class UniscribeTextRun : public UniscribeRun
			{
			public:
				//***************************** Uniscribe Data
				SCRIPT_CACHE					scriptCache;
				vint							advance;
				UniscribeGlyphData				wholeGlyph;
				//***************************** Uniscribe Data (Font Fallback)
				SCRIPT_STRING_ANALYSIS			ssa;

				UniscribeTextRun();
				~UniscribeTextRun();

				void							ClearUniscribeData();
				void							SearchGlyphCluster(vint charStart, vint charLength, vint& cluster, vint& nextCluster);
				bool							BuildUniscribeData(WinDC* dc, List<vint>& breakings)override;
				vint							SumWidth(vint charStart, vint charLength)override;
				vint							SumHeight()override;
				void							SearchForLineBreak(vint tempStart, vint maxWidth, bool firstRun, vint& charLength, vint& charAdvances)override;
				void							Render(WinDC* dc, vint fragmentBoundsIndex, vint offsetX, vint offsetY)override;
			};

/***********************************************************************
UniscribeElementRun
***********************************************************************/

			class UniscribeElementRun : public UniscribeRun
			{
			public:
				//***************************** Document Data
				Ptr<IGuiGraphicsElement>						element;
				IGuiGraphicsParagraph::InlineObjectProperties	properties;

				UniscribeElementRun();
				~UniscribeElementRun();

				bool							BuildUniscribeData(WinDC* dc, List<vint>& breakings)override;
				vint							SumWidth(vint charStart, vint charLength)override;
				vint							SumHeight()override;
				void							SearchForLineBreak(vint tempStart, vint maxWidth, bool firstRun, vint& charLength, vint& charAdvances)override;
				void							Render(WinDC* dc, vint fragmentBoundsIndex, vint offsetX, vint offsetY)override;
			};

/***********************************************************************
UniscribeVirtualLine
***********************************************************************/

			class UniscribeVirtualLine : Object
			{
			public:
				//***************************** Document Data
				vint							start;			// start from line
				vint							length;
				const wchar_t*					runText;
				//***************************** Layout Data
				vint							firstRunIndex;
				vint							firstRunBoundsIndex;
				vint							lastRunIndex;
				vint							lastRunBoundsIndex;
				Rect							bounds;

				UniscribeVirtualLine();
			};

/***********************************************************************
UniscribeLine
***********************************************************************/

			class UniscribeLine : public Object
			{
			public:
				//***************************** Document Data
				List<Ptr<UniscribeFragment>>	documentFragments;
				vint							start;			// start from paragraph
				WString							lineText;
				//***************************** Uniscribe Data
				List<Ptr<UniscribeItem>>		scriptItems;
				List<Ptr<UniscribeRun>>			scriptRuns;
				//***************************** Layout Data
				List<Ptr<UniscribeVirtualLine>>	virtualLines;
				Rect							bounds;

				UniscribeLine();

				void							ClearUniscribeData();
				bool							BuildUniscribeData(WinDC* dc);
				void							Layout(vint availableWidth, Alignment alignment, vint top, vint& totalHeight);
				void							Render(WinDC* dc, vint offsetX, vint offsetY);
			};

/***********************************************************************
UniscribeParagraph
***********************************************************************/

			class UniscribeParagraph : public Object
			{
			public:
				//***************************** Document Data
				List<Ptr<UniscribeFragment>>	documentFragments;
				Alignment						paragraphAlignment;
				WString							paragraphText;
				bool							built;
				//***************************** Uniscribe Data
				List<Ptr<UniscribeLine>>		lines;
				//***************************** Layout Data
				vint							lastAvailableWidth;
				Rect							bounds;

				UniscribeParagraph();
				~UniscribeParagraph();

				void							ClearUniscribeData();
				void							BuildUniscribeData(WinDC* dc);
				void							Layout(vint availableWidth, Alignment alignment);
				void							Render(WinDC* dc, vint offsetX, vint offsetY);

				void							SearchFragment(vint start, vint length, vint& fs, vint& ss, vint& fe, vint& se);
				bool							CutFragment(vint fs, vint ss, vint fe, vint se, vint& f1, vint& f2);
				bool							SetFont(vint start, vint length, const WString& value);
				bool							SetSize(vint start, vint length, vint value);
				bool							SetStyle(vint start, vint length, bool bold, bool italic, bool underline, bool strikeline);
				bool							SetColor(vint start, vint length, Color value);
				bool							SetInlineObject(vint start, vint length, const IGuiGraphicsParagraph::InlineObjectProperties& properties, Ptr<IGuiGraphicsElement> value);
				Ptr<IGuiGraphicsElement>		ResetInlineObject(vint start, vint length);
				bool							SetInteractionId(vint start, vint length, vint value);
				bool							HitTestPoint(Point point, vint& start, vint& length, vint& interactionId);

				void							GetLineIndexFromTextPos(vint textPos, vint& frontLine, vint& backLine);
				void							GetVirtualLineIndexFromTextPos(vint textPos, vint lineIndex, vint& frontLine, vint& backLine);
				void							GetItemIndexFromTextPos(vint textPos, vint lineIndex, vint& frontItem, vint& backItem);
				Rect							GetCaretBoundsWithLine(vint caret, vint lineIndex, vint virtualLineIndex, bool frontSide);
				vint							GetCaretFromXWithLine(vint x, vint lineIndex, vint virtualLineIndex);
				vint							GetLineIndexFromY(vint y);
				vint							GetVirtualLineIndexFromY(vint y, vint lineIndex);

				vint							GetCaret(vint comparingCaret, IGuiGraphicsParagraph::CaretRelativePosition position, bool preferFrontSide);
				Rect							GetCaretBounds(vint caret, bool frontSide);
				vint							GetCaretFromPoint(Point point);
				vint							GetNearestCaretFromTextPos(vint textPos, bool frontSide);
				bool							IsValidCaret(vint caret);
				bool							IsValidTextPos(vint textPos);
			};
		}
	}
}

#endif