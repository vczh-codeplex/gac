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

class TypeSymbol : public Object
{
public:
	WString									typeName;
	Dictionary<WString, Ptr<TypeSymbol>>	subTypes;
	TypeSymbol*								parent;

	TypeSymbol()
		:parent(0)
	{
	}

	void CollectSubTypes(Ptr<ParsingTreeArray> types, Dictionary<ParsingTreeNode*, TypeSymbol*>& nodeTypeMap)
	{
		if(types)
		{
			for(int i=0;i<types->Count();i++)
			{
				Ptr<ParsingTreeObject> type=types->GetItem(i).Cast<ParsingTreeObject>();
				if(type)
				{
					CollectSubType(type, nodeTypeMap);
				}
			}
		}
	}

	void CollectSubType(Ptr<ParsingTreeObject> type, Dictionary<ParsingTreeNode*, TypeSymbol*>& nodeTypeMap)
	{
		Ptr<ParsingTreeToken> name=type->GetMember(L"name").Cast<ParsingTreeToken>();
		if(name && !subTypes.Keys().Contains(name->GetValue()))
		{
			Ptr<TypeSymbol> symbol=new TypeSymbol;
			symbol->typeName=name->GetValue();
			symbol->parent=this;
			subTypes.Add(symbol->typeName, symbol);
			symbol->CollectSubTypes(type->GetMember(L"subTypes").Cast<ParsingTreeArray>(), nodeTypeMap);
			nodeTypeMap.Add(type.Obj(), symbol.Obj());
		}
	}
};

class ParserDecl : public TypeSymbol
{
public:
	SortedList<WString>							tokens;
	SortedList<WString>							rules;
	Dictionary<ParsingTreeNode*, TypeSymbol*>	nodeTypeMap;

	ParserDecl(Ptr<ParsingTreeObject> parserDecl)
	{
		nodeTypeMap.Add(parserDecl.Obj(), this);
		Ptr<ParsingTreeArray> defs=parserDecl->GetMember(L"definitions").Cast<ParsingTreeArray>();
		if(defs)
		{
			vint count=defs->Count();
			for(vint i=0;i<count;i++)
			{
				Ptr<ParsingTreeObject> defObject=defs->GetItem(i).Cast<ParsingTreeObject>();
				if(defObject)
				{
					if(defObject->GetType()==L"TokenDef")
					{
						Ptr<ParsingTreeToken> name=defObject->GetMember(L"name").Cast<ParsingTreeToken>();
						if(name)
						{
							tokens.Add(name->GetValue());
						}
					}
					else if(defObject->GetType()==L"RuleDef")
					{
						Ptr<ParsingTreeToken> name=defObject->GetMember(L"name").Cast<ParsingTreeToken>();
						if(name)
						{
							rules.Add(name->GetValue());
						}
					}
					else
					{
						CollectSubType(defObject, nodeTypeMap);
					}
				}
			}
		}
	}
};

/***********************************************************************
ParserGrammarColorizer
***********************************************************************/

class ParserGrammarColorizer : public GuiGrammarColorizer
{
protected:
	Ptr<ParserDecl>							parsingTreeDecl;
	vint									tokenIdType;
	vint									tokenIdToken;
	vint									tokenIdRule;
	vint									semanticType;
	vint									semanticGrammar;

	TypeSymbol* FindScope(ParsingTreeNode* node)
	{
		if(!node) return 0;
		int index=parsingTreeDecl->nodeTypeMap.Keys().IndexOf(node);
		return index==-1?FindScope(node->GetParent()):parsingTreeDecl->nodeTypeMap.Values().Get(index);
	}

	TypeSymbol* FindType(TypeSymbol* scope, const WString& name)
	{
		if(!scope) return 0;
		if(name==L"") return 0;
		int index=scope->subTypes.Keys().IndexOf(name);
		if(index!=-1) return scope->subTypes.Values().Get(index).Obj();
		return FindType(scope->parent, name);
	}

	TypeSymbol* FindType(TypeSymbol* scope, ParsingTreeObject* object)
	{
		if(scope && object)
		{
			Ptr<ParsingTreeToken> name=object->GetMember(L"name").Cast<ParsingTreeToken>();
			if(name)
			{
				WString typeName=name->GetValue();
				if(object->GetType()==L"PrimitiveTypeObj")
				{
					return FindType(scope, typeName);
				}
				else if(object->GetType()==L"SubTypeObj")
				{
					TypeSymbol* type=FindType(scope, object->GetMember(L"parentType").Cast<ParsingTreeObject>().Obj());
					if(type)
					{
						int index=type->subTypes.Keys().IndexOf(typeName);
						if(index!=-1) return type->subTypes.Values().Get(index).Obj();
					}
				}
			}
		}
		return 0;
	}

	void OnParsingFinished(bool generatedNewNode, RepeatingParsingExecutor* parsingExecutor)override
	{
		if(generatedNewNode)
		{
			Ptr<ParsingTreeObject> node=parsingExecutor->ThreadSafeGetTreeNode();
			parsingTreeDecl=new ParserDecl(node);
			node=0;
			parsingExecutor->ThreadSafeReturnTreeNode();
		}
		GuiGrammarColorizer::OnParsingFinished(generatedNewNode, parsingExecutor);
	}

	void OnSemanticColorize(ParsingTreeToken* foundToken, ParsingTreeObject* tokenParent, const WString& type, const WString& field, vint semantic, vint& token)override
	{
		if(semantic==semanticType)
		{
			TypeSymbol* scope=FindScope(tokenParent);
			if(FindType(scope, tokenParent))
			{
				token=tokenIdType;
			}
		}
		else if(semantic==semanticGrammar)
		{
			WString name=foundToken->GetValue();
			if(parsingTreeDecl->tokens.Contains(name))
			{
				token=tokenIdToken;
			}
			else if(parsingTreeDecl->rules.Contains(name))
			{
				token=tokenIdRule;
			}
		}
	}
public:
	ParserGrammarColorizer()
		:GuiGrammarColorizer(CreateBootstrapAutoRecoverParser(), L"ParserDecl")
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
ColorizerConfigurationGrid
***********************************************************************/

class ColorizerConfigurationGrid : public GuiVirtualDataGrid
{
public:
	struct ColorItem
	{
		WString								name;
		Color								color;
	};

	class ColorColumnProvider : public list::StrongTypedFieldColumnProvider<ColorItem, Color>
	{
	public:
		ColorColumnProvider(list::StrongTypedDataProvider<ColorItem>* _dataProvider)
			:list::StrongTypedFieldColumnProvider<ColorItem, Color>(_dataProvider, &ColorItem::color)
		{
		}

		void VisualizeCell(vint row, list::IDataVisualizer* dataVisualizer)override
		{
			list::StrongTypedFieldColumnProvider<ColorItem, Color>::VisualizeCell(row, dataVisualizer);
			list::ListViewSubColumnDataVisualizer* visualizer=dataVisualizer->GetVisualizer<list::ListViewSubColumnDataVisualizer>();
			if(visualizer)
			{
				ColorItem rowData;
				Color cellData;
				dataProvider->GetRowData(row, rowData);
				GetCellData(rowData, cellData);
				visualizer->GetTextElement()->SetColor(cellData);
			}
		}
	};

	class ColorItemProvider : public list::StrongTypedDataProvider<ColorItem>
	{
	protected:
		List<ColorItem>						items;
	public:
		ColorItemProvider()
		{
			AddSortableFieldColumn(L"Configuration", &ColorItem::name)
				->SetVisualizerFactory(new list::CellBorderDataVisualizer::Factory(new list::ListViewMainColumnDataVisualizer::Factory))
				->SetSize(160);
			AddStrongTypedColumn<Color>(L"Color", new ColorColumnProvider(this))
				->SetVisualizerFactory(new list::CellBorderDataVisualizer::Factory(new list::ListViewSubColumnDataVisualizer::Factory))
				->SetSize(120);
		}

		vint GetRowCount()override
		{
			return items.Count();
		}

		void GetRowData(vint row, ColorItem& rowData)override
		{
			rowData=items[row];
		}

		void ReadConfigurationFromColorizer(GuiGrammarColorizer* colorizer)
		{
			vint oldCount=items.Count();
			items.Clear();
			FOREACH(WString, name, colorizer->GetColorNames())
			{
				ColorItem item;
				item.name=name;
				item.color=colorizer->GetColor(name).normal.text;
				items.Add(item);
			}
			commandExecutor->OnDataProviderItemModified(0, oldCount, items.Count());
		}
	};

protected:
	ColorItemProvider*						dataProvider;
public:
	ColorizerConfigurationGrid()
		:GuiVirtualDataGrid(GetCurrentTheme()->CreateListViewStyle(), dataProvider=new ColorItemProvider)
	{
	}

	ColorItemProvider* GetDataProvider()
	{
		return dataProvider;
	}
};

/***********************************************************************
TextBoxColorizerWindow
***********************************************************************/

class TextBoxColorizerWindow : public GuiWindow
{
protected:
	GuiTab*									tabIntellisense;
	GuiMultilineTextBox*					textBoxEditor;
	GuiMultilineTextBox*					textBoxGrammar;
	GuiGrammarColorizer*					colorizer;
	ColorizerConfigurationGrid*				gridConfiguration;

public:
	TextBoxColorizerWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Editor.Colorizer.Grammar");
		SetClientSize(Size(800, 600));

		GuiSelectableButton::MutexGroupController* controller=new GuiSelectableButton::MutexGroupController;
		AddComponent(controller);

		tabIntellisense=g::NewTab();
		tabIntellisense->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		GetBoundsComposition()->AddChild(tabIntellisense->GetBoundsComposition());
		{
			GuiTabPage* page=tabIntellisense->CreatePage();
			page->SetText(L"Code Editor");

			textBoxEditor=g::NewMultilineTextBox();
			textBoxEditor->SetVerticalAlwaysVisible(false);
			textBoxEditor->SetHorizontalAlwaysVisible(false);
			textBoxEditor->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			page->GetContainer()->GetBoundsComposition()->AddChild(textBoxEditor->GetBoundsComposition());

			colorizer=new ParserGrammarColorizer;
			textBoxEditor->SetColorizer(colorizer);
		}
		{
			GuiTabPage* page=tabIntellisense->CreatePage();
			page->SetText(L"Grammar");

			textBoxGrammar=g::NewMultilineTextBox();
			textBoxGrammar->SetReadonly(true);
			textBoxGrammar->SetVerticalAlwaysVisible(false);
			textBoxGrammar->SetHorizontalAlwaysVisible(false);
			textBoxGrammar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			page->GetContainer()->GetBoundsComposition()->AddChild(textBoxGrammar->GetBoundsComposition());
			textBoxGrammar->SetColorizer(new ParserGrammarColorizer);
		}
		{
			GuiTabPage* page=tabIntellisense->CreatePage();
			page->SetText(L"Configuration");

			gridConfiguration=new ColorizerConfigurationGrid;
			gridConfiguration->SetVerticalAlwaysVisible(false);
			gridConfiguration->SetHorizontalAlwaysVisible(false);
			gridConfiguration->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			page->GetContainer()->GetBoundsComposition()->AddChild(gridConfiguration->GetBoundsComposition());
			gridConfiguration->GetDataProvider()->ReadConfigurationFromColorizer(colorizer);
		}
		{
			FileStream fileStream(L"..\\Resources\\CalculatorDefinition.txt", FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			textBoxEditor->SetText(reader.ReadToEnd());
			textBoxEditor->Select(TextPos(), TextPos());
		}
		{
			Ptr<ParsingDefinition> definition=CreateParserDefinition();
			MemoryStream stream;
			{
				StreamWriter writer(stream);
				Log(definition, writer);
			}
			stream.SeekFromBegin(0);
			StreamReader reader(stream);
			textBoxGrammar->SetText(reader.ReadToEnd());
			textBoxGrammar->Select(TextPos(), TextPos());
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