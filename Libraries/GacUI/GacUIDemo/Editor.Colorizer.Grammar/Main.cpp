#include "..\..\Public\Source\GacUI.h"
#include "..\..\Public\Source\Vlpp.h"
#include <Windows.h>

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::tabling;
using namespace vl::parsing::definitions;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

/***********************************************************************
SymbolLookup
***********************************************************************/

class GrammarSymbol : public ParsingScopeSymbol
{
public:
	GrammarSymbol(Ptr<ParsingTreeObject> node)
		:ParsingScopeSymbol(node->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue())
	{
		SetNode(node.Obj());
	}
};

class EnumFieldSymbol : public GrammarSymbol
{
public:
	EnumFieldSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class ClassFieldSymbol : public GrammarSymbol
{
public:
	ClassFieldSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class TypeSymbol : public GrammarSymbol
{
public:
	TypeSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class EnumSymbol : public TypeSymbol
{
public:
	EnumSymbol(Ptr<ParsingTreeObject> node)
		:TypeSymbol(node)
	{
		CreateScope();
		if(Ptr<ParsingTreeArray> members=node->GetMember(L"members").Cast<ParsingTreeArray>())
		{
			FOREACH(Ptr<ParsingTreeNode>, node, members->GetItems())
			{
				if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
				{
					GetScope()->AddSymbol(new EnumFieldSymbol(obj));
				}
			}
		}
	}
};

class ClassSymbol : public TypeSymbol
{
public:
	ClassSymbol(Ptr<ParsingTreeObject> node)
		:TypeSymbol(node)
	{
		CreateScope();
		if(Ptr<ParsingTreeArray> members=node->GetMember(L"members").Cast<ParsingTreeArray>())
		{
			FOREACH(Ptr<ParsingTreeNode>, node, members->GetItems())
			{
				if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
				{
					GetScope()->AddSymbol(new ClassFieldSymbol(obj));
				}
			}
		}
		if(Ptr<ParsingTreeArray> members=node->GetMember(L"subTypes").Cast<ParsingTreeArray>())
		{
			FOREACH(Ptr<ParsingTreeNode>, node, members->GetItems())
			{
				Ptr<ParsingTreeObject> type=node.Cast<ParsingTreeObject>();
				if(type->GetType()==L"EnumTypeDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new EnumSymbol(obj));
					}
				}
				else if(type->GetType()==L"ClassTypeDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new ClassSymbol(obj));
					}
				}
			}
		}
	}
};

class TokenSymbol : public GrammarSymbol
{
public:
	TokenSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class RuleSymbol : public GrammarSymbol
{
public:
	RuleSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class ParserDefScope : public ParsingScopeRoot
{
public:
	ParserDefScope(Ptr<ParsingTreeObject> node)
	{
		SetNode(node.Obj());
		CreateScope();
		if(Ptr<ParsingTreeArray> members=node->GetMember(L"definitions").Cast<ParsingTreeArray>())
		{
			FOREACH(Ptr<ParsingTreeNode>, node, members->GetItems())
			{
				Ptr<ParsingTreeObject> type=node.Cast<ParsingTreeObject>();
				if(type->GetType()==L"EnumTypeDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new EnumSymbol(obj));
					}
				}
				else if(type->GetType()==L"ClassTypeDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new ClassSymbol(obj));
					}
				}
				else if(type->GetType()==L"TokenDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new TokenSymbol(obj));
					}
				}
				else if(type->GetType()==L"RuleDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new RuleSymbol(obj));
					}
				}
			}
		}
		InitializeQueryCache();
	}
};

/***********************************************************************
ParserGrammarExecutor
***********************************************************************/

class ParserGrammarExecutor : public RepeatingParsingExecutor
{
protected:

	void OnContextFinishedAsync(RepeatingParsingOutput& context)override
	{
		context.semanticContext=new ParserDefScope(context.node);
	}
public:
	ParserGrammarExecutor()
		:RepeatingParsingExecutor(CreateBootstrapAutoRecoverParser(), L"ParserDecl")
	{
	}
};

/***********************************************************************
ParserGrammarColorizer
***********************************************************************/

class ParserGrammarColorizer : public GuiGrammarColorizer
{
protected:
	vint									tokenIdType;
	vint									tokenIdToken;
	vint									tokenIdRule;
	vint									semanticType;
	vint									semanticGrammar;

	void OnSemanticColorize(SemanticColorizeContext& context)override
	{
		if(Ptr<ParserDefScope> parserDef=context.semanticContext.Cast<ParserDefScope>())
		{
			if(context.semantic==semanticType)
			{
				List<WString> names;
				{
					ParsingTreeObject* type=context.tokenParent;
					while(type)
					{
						if(type->GetType()==L"PrimitiveTypeObj")
						{
							names.Add(type->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue());
						}
						if(type->GetType()==L"SubTypeObj")
						{
							names.Add(type->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue());
							type=dynamic_cast<ParsingTreeObject*>(type->GetMember(L"parentType").Obj());
						}
						else
						{
							break;
						}
					}
				}

				ParsingScope* scope=parserDef->GetScopeFromNode(context.foundToken);
				Ptr<TypeSymbol> type=0;
				for(vint i=names.Count()-1;i>=0;i--)
				{
					if(type)
					{
						const List<Ptr<ParsingScopeSymbol>>& symbols=type->GetScope()->GetSymbols(names[i]);
						if(symbols.Count()>0)
						{
							type=symbols[0].Cast<TypeSymbol>();
						}
						else
						{
							type=0;
						}
					}
					else if(i==names.Count()-1)
					{
						const List<Ptr<ParsingScopeSymbol>>& symbols=scope->GetSymbolsRecursively(names[i]);
						if(symbols.Count()>0)
						{
							type=symbols[0].Cast<TypeSymbol>();
						}
					}
					else
					{
						break;
					}
				}

				if(type)
				{
					context.token=tokenIdType;
				}
			}
			else if(context.semantic==semanticGrammar)
			{
				WString name=context.foundToken->GetValue();
				const List<Ptr<ParsingScopeSymbol>>& symbols=parserDef->GetScope()->GetSymbols(name);
				FOREACH(Ptr<ParsingScopeSymbol>, symbol, symbols)
				{
					if(symbol.Cast<TokenSymbol>())
					{
						context.token=tokenIdToken;
						break;
					}
					else if(symbol.Cast<RuleSymbol>())
					{
						context.token=tokenIdRule;
						break;
					}
				}
			}
		}
	}
public:
	ParserGrammarColorizer(Ptr<ParserGrammarExecutor> executor)
		:GuiGrammarColorizer(executor)
	{
		SetColor(L"Keyword", Color(0, 0, 255));
		SetColor(L"Attribute", Color(0, 0, 255));
		SetColor(L"String", Color(163, 21, 21));
		SetColor(L"Type", Color(43, 145, 175));
		SetColor(L"Token", Color(163, 73, 164));
		SetColor(L"Rule", Color(255, 127, 39));
		EndSetColors();

		tokenIdType=GetTokenId(L"Type");
		tokenIdToken=GetTokenId(L"Token");
		tokenIdRule=GetTokenId(L"Rule");
		semanticType=GetSemanticId(L"Type");
		semanticGrammar=GetSemanticId(L"Grammar");
	}

	~ParserGrammarColorizer()
	{
		EnsureColorizerFinished();
	}
};

/***********************************************************************
TextBoxColorizerWindow
***********************************************************************/

class TextBoxColorizerWindow : public GuiWindow
{
protected:
	GuiMultilineTextBox*					textBoxEditor;

public:
	TextBoxColorizerWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Editor.Colorizer.Grammar");
		SetClientSize(Size(800, 600));

		textBoxEditor=g::NewMultilineTextBox();
		textBoxEditor->SetVerticalAlwaysVisible(false);
		textBoxEditor->SetHorizontalAlwaysVisible(false);
		textBoxEditor->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		textBoxEditor->SetColorizer(new ParserGrammarColorizer(new ParserGrammarExecutor));
		this->GetBoundsComposition()->AddChild(textBoxEditor->GetBoundsComposition());

		{
			FileStream fileStream(L"..\\Resources\\CalculatorDefinition.txt", FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			textBoxEditor->SetText(reader.ReadToEnd());
			textBoxEditor->Select(TextPos(), TextPos());
		}

		// set the preferred minimum client 600
		this->GetBoundsComposition()->SetPreferredMinSize(Size(800, 600));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~TextBoxColorizerWindow()
	{
	}
};

/***********************************************************************
GuiMain
***********************************************************************/

void GuiMain()
{
	TextBoxColorizerWindow window;
	GetApplication()->Run(&window);
}