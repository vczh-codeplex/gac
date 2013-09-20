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
				FontProperties									fontStyle;
				Color											fontColor;
				WString											text;
				Ptr<WinFont>									fontObject;
				vint											interactionId;

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
			};

/***********************************************************************
UniscribeItem
***********************************************************************/

			class UniscribeItem : public Object
			{
			public:
				SCRIPT_ITEM						scriptItem;
				vint							start;
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
					vint						start;
					vint						length;
					Rect						bounds;
				};

				UniscribeFragment*				documentFragment;
				UniscribeItem*					scriptItem;
				vint							start;
				vint							length;
				const wchar_t*					runText;
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
				SCRIPT_CACHE					scriptCache;
				vint							advance;
				UniscribeGlyphData				wholeGlyph;

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
UniscribeLine
***********************************************************************/

			class UniscribeLine : public Object
			{
			public:
				List<Ptr<UniscribeFragment>>	documentFragments;

				WString							lineText;
				List<Ptr<UniscribeItem>>		scriptItems;
				List<Ptr<UniscribeRun>>			scriptRuns;
				Rect							bounds;

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
				List<Ptr<UniscribeFragment>>	documentFragments;
				bool							built;

				List<Ptr<UniscribeLine>>		lines;
				vint							lastAvailableWidth;
				Alignment						paragraphAlignment;
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
			};
		}
	}
}

#endif