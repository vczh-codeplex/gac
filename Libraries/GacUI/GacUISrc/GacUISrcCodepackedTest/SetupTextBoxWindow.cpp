#include "..\..\Public\Source\GacUIIncludes.h"

int textBoxTextChangedCounter=0;
int textBoxSelectionChangedCounter=0;

class CommentColorizer : public GuiTextBoxColorizerBase
{
private:
	collections::Array<text::ColorEntry>	colors;
public:
	CommentColorizer()
	{
		text::ColorEntry defaultColor=GetCurrentTheme()->GetDefaultTextBoxColorEntry();
		colors.Resize(2);
		colors[0]=defaultColor;
		defaultColor.normal.text=Color(0, 192, 0);
		colors[1]=defaultColor;
	}

	int GetLexerStartState()override
	{
		return 0;
	}

	int GetContextStartState()override
	{
		return 0;
	}

	void ColorizeLineWithCRLF(const wchar_t* text, unsigned __int32* colors, int length, int& lexerState, int& contextState)override
	{
		if(length>0)
		{
			unsigned __int32 color=text[0]==L'#'?1:0;
			for(int i=0;i<length;i++)
			{
				colors[i]=color;
			}
		}
	}

	const collections::Array<text::ColorEntry>& GetColors()
	{
		return colors;
	}
};

class CommentColorizer2 : public GuiTextBoxRegexColorizer
{
public:
	CommentColorizer2()
	{
		text::ColorEntry defaultColor=GetCurrentTheme()->GetDefaultTextBoxColorEntry();
		SetDefaultColor(defaultColor);

		defaultColor.normal.text=Color(0, 192, 0);
		AddToken(L"#[^\r\n]*", defaultColor);

		Setup();
	}
};

void SetupTextBoxWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiMultilineTextBox* textBox=g::NewMultilineTextBox();
	textBox->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
	textBox->GetBoundsComposition()->SetBounds(Rect(0, 0, 300, 200));
	container->GetBoundsComposition()->AddChild(textBox->GetBoundsComposition());

	//FontProperties font=textBox->GetFont();
	//font.fontFamily=L"·½Õý¾§ËÎ";
	//font.size=16;
	//textBox->SetFont(font);

	textBox->TextChanged.AttachLambda([controlHost](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		textBoxTextChangedCounter++;
		controlHost->GetNativeWindow()->SetTitle(L"TextChanged["+itow(textBoxTextChangedCounter)+L"], SelectionChanged["+itow(textBoxSelectionChangedCounter)+L"]");
	});

	textBox->SelectionChanged.AttachLambda([controlHost](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		textBoxSelectionChangedCounter++;
		controlHost->GetNativeWindow()->SetTitle(L"TextChanged["+itow(textBoxTextChangedCounter)+L"], SelectionChanged["+itow(textBoxSelectionChangedCounter)+L"]");
	});

	Ptr<GuiTextBoxColorizerBase> colorizer=new CommentColorizer2;
	textBox->SetColorizer(colorizer);
	textBox->SetText(L"# Comment\r\nNormal\r\n");
}