#include "..\..\Public\Source\GacUI.h"

using namespace vl::stream;
using namespace vl::parsing;
using namespace vl::parsing::tabling;
using namespace vl::parsing::definitions;
using namespace vl::collections;

namespace colorization
{

/***********************************************************************
XmlGrammarColorizer
***********************************************************************/

	class XmlGrammarColorizer : public GuiGrammarColorizer
	{
	public:
		XmlGrammarColorizer()
			:GuiGrammarColorizer(CreateAutoRecoverParser(xml::XmlLoadTable()), L"XDocument")
		{
			SetColor(L"Boundary", Color(0, 0, 255));
			SetColor(L"Comment", Color(0, 128, 0));
			SetColor(L"TagName", Color(163, 21, 21));
			SetColor(L"AttName", Color(255, 0, 0));
			SetColor(L"AttValue", Color(128, 0, 255));
			EndSetColors();
		}
	};

/***********************************************************************
JsonGrammarColorizer
***********************************************************************/

	class JsonGrammarColorizer : public GuiGrammarColorizer
	{
	public:
		JsonGrammarColorizer()
			:GuiGrammarColorizer(CreateAutoRecoverParser(json::JsonLoadTable()), L"JRoot")
		{
			SetColor(L"Boundary", Color(255, 0, 0));
			SetColor(L"Keyword", Color(0, 0, 255));
			SetColor(L"AttName", Color(64, 64, 64));
			SetColor(L"Number", Color(128, 0, 255));
			SetColor(L"String", Color(163, 21, 2));
			EndSetColors();
		}
	};

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
Event Handlers
***********************************************************************/

	void textBox_SwitchLanguage(const WString& sampleCodePath, GuiGrammarColorizer* colorizer, GuiMultilineTextBox* textBox)
	{
		textBox->SetColorizer(colorizer);
		FileStream fileStream(sampleCodePath, FileStream::ReadOnly);
		BomDecoder decoder;
		DecoderStream decoderStream(fileStream, decoder);
		StreamReader reader(decoderStream);
		textBox->SetText(reader.ReadToEnd());
		textBox->Select(TextPos(), TextPos());
	}

	void radioGrammar_SelectedChanged(GuiSelectableButton* radio, GuiMultilineTextBox* textBox)
	{
		if(radio->GetSelected())
		{
			textBox_SwitchLanguage(L"Resources\\CalculatorDefinition.txt", new ParserGrammarColorizer, textBox);
		}
	}

	void radioXml_SelectedChanged(GuiSelectableButton* radio, GuiMultilineTextBox* textBox)
	{
		if(radio->GetSelected())
		{
			textBox_SwitchLanguage(L"Resources\\XmlResource.xml", new XmlGrammarColorizer, textBox);
		}
	}

	void radioJson_SelectedChanged(GuiSelectableButton* radio, GuiMultilineTextBox* textBox)
	{
		if(radio->GetSelected())
		{
			textBox_SwitchLanguage(L"Resources\\JsonSample.txt", new JsonGrammarColorizer, textBox);
		}
	}
}

using namespace colorization;

void SetupTextBoxWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	
	GuiMultilineTextBox* textBox=0;
	GuiSelectableButton* radioGrammar=0;
	GuiSelectableButton* radioXml=0;
	GuiSelectableButton* radioJson=0;
	GuiSelectableButton::MutexGroupController* controller=new GuiSelectableButton::MutexGroupController;
	controlHost->AddComponent(controller);

	GuiTableComposition* table=new GuiTableComposition;
	table->SetAlignmentToParent(Margin(0, 0, 0, 0));
	container->GetBoundsComposition()->AddChild(table);

	table->SetCellPadding(5);
	table->SetRowsAndColumns(2, 4);
	table->SetRowOption(0, GuiCellOption::MinSizeOption());
	table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
	table->SetColumnOption(0, GuiCellOption::MinSizeOption());
	table->SetColumnOption(1, GuiCellOption::MinSizeOption());
	table->SetColumnOption(2, GuiCellOption::MinSizeOption());
	table->SetColumnOption(3, GuiCellOption::PercentageOption(1.0));
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(1, 0, 1, 4);

		textBox=g::NewMultilineTextBox();
		textBox->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		cell->AddChild(textBox->GetBoundsComposition());
	}
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(0, 0, 1, 1);

		radioGrammar=g::NewRadioButton();
		radioGrammar->SetGroupController(controller);
		radioGrammar->SetText(L"Grammar");
		radioGrammar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		radioGrammar->SelectedChanged.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			radioGrammar_SelectedChanged(radioGrammar, textBox);
		});
		cell->AddChild(radioGrammar->GetBoundsComposition());
	}
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(0, 1, 1, 1);

		radioXml=g::NewRadioButton();
		radioXml->SetGroupController(controller);
		radioXml->SetText(L"Xml");
		radioXml->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		radioXml->SelectedChanged.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			radioXml_SelectedChanged(radioXml, textBox);
		});
		cell->AddChild(radioXml->GetBoundsComposition());
	}
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(0, 2, 1, 1);

		radioJson=g::NewRadioButton();
		radioJson->SetGroupController(controller);
		radioJson->SetText(L"Json");
		radioJson->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		radioJson->SelectedChanged.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			radioJson_SelectedChanged(radioJson, textBox);
		});
		cell->AddChild(radioJson->GetBoundsComposition());
	}
	radioGrammar->SetSelected(true);
}