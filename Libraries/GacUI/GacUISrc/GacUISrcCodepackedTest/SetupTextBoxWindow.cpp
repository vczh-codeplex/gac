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
			textBox_SwitchLanguage(L"Resources\\CalculatorDefinition.txt", new ParserGrammarColorizer(new ParserGrammarExecutor), textBox);
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