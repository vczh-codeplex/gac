#include "GuiWin8StylesCommon.h"

namespace vl
{
	namespace presentation
	{
		namespace win8
		{
			using namespace collections;
			using namespace elements;
			using namespace compositions;
			using namespace controls;

/***********************************************************************
Win8ButtonColors
***********************************************************************/

			void Win8ButtonColors::SetAlphaWithoutText(unsigned char a)
			{
				borderColor.a=a;
				g1.a=a;
				g2.a=a;
			}

			Win8ButtonColors Win8ButtonColors::Blend(const Win8ButtonColors& c1, const Win8ButtonColors& c2, int ratio, int total)
			{
				if(ratio<0) ratio=0;
				else if(ratio>total) ratio=total;

				Win8ButtonColors result;
				result.borderColor=BlendColor(c1.borderColor, c2.borderColor, ratio, total);
				result.g1=BlendColor(c1.g1, c2.g1, ratio, total);
				result.g2=BlendColor(c1.g2, c2.g2, ratio, total);
				result.textColor=BlendColor(c1.textColor, c2.textColor, ratio, total);
				result.bullet=BlendColor(c1.bullet, c2.bullet, ratio, total);
				return result;
			}

			//---------------------------------------------------------

			Win8ButtonColors Win8ButtonColors::ButtonNormal()
			{
				Win8ButtonColors colors=
				{
					Color(172, 172, 172),
					Color(239, 239, 239),
					Color(229, 229, 229),
					Win8GetSystemTextColor(true),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ButtonActive()
			{
				Win8ButtonColors colors=
				{
					Color(126, 180, 234),
					Color(235, 244, 252),
					Color(220, 236, 252),
					Win8GetSystemTextColor(true),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ButtonPressed()
			{
				Win8ButtonColors colors=
				{
					Color(86, 157, 229),
					Color(218, 236, 252),
					Color(196, 224, 252),
					Win8GetSystemTextColor(true),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ButtonDisabled()
			{
				Win8ButtonColors colors=
				{
					Color(173, 178, 181),
					Color(252, 252, 252),
					Color(252, 252, 252),
					Win8GetSystemTextColor(false),
				};
				return colors;
			}

			//---------------------------------------------------------

			Win8ButtonColors Win8ButtonColors::CheckedNormal(bool selected)
			{
				Win8ButtonColors colors=
				{
					Color(112, 112, 112),
					Color(255, 255, 255),
					Color(255, 255, 255),
					Win8GetSystemTextColor(true),
					Color(0, 0, 0),
				};
				if(!selected)
				{
					colors.bullet.a=0;
				}
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::CheckedActive(bool selected)
			{
				Win8ButtonColors colors=
				{
					Color(51, 153, 255),
					Color(243, 249, 255),
					Color(243, 249, 255),
					Win8GetSystemTextColor(true),
					Color(33, 33, 33),
				};
				if(!selected)
				{
					colors.bullet.a=0;
				}
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::CheckedPressed(bool selected)
			{
				Win8ButtonColors colors=
				{
					Color(0, 124, 222),
					Color(217, 236, 255),
					Color(217, 236, 255),
					Win8GetSystemTextColor(true),
					Color(0, 0, 0),
				};
				if(!selected)
				{
					colors.bullet.a=0;
				}
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::CheckedDisabled(bool selected)
			{
				Win8ButtonColors colors=
				{
					Color(188, 188, 188),
					Color(230, 230, 230),
					Color(230, 230, 230),
					Win8GetSystemTextColor(false),
					Color(112, 112, 112),
				};
				if(!selected)
				{
					colors.bullet.a=0;
				}
				return colors;
			}

			//---------------------------------------------------------

			Win8ButtonColors Win8ButtonColors::ScrollHandleNormal()
			{
				Win8ButtonColors colors=
				{
					Color(205, 205, 205),
					Color(205, 205, 205),
					Color(205, 205, 205),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ScrollHandleActive()
			{
				Win8ButtonColors colors=
				{
					Color(166, 166, 166),
					Color(166, 166, 166),
					Color(166, 166, 166),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ScrollHandlePressed()
			{
				Win8ButtonColors colors=
				{
					Color(166, 166, 166),
					Color(166, 166, 166),
					Color(166, 166, 166),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ScrollHandleDisabled()
			{
				Win8ButtonColors colors=
				{
					Win8GetSystemWindowColor(),
					Win8GetSystemWindowColor(),
					Win8GetSystemWindowColor(),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ScrollArrowNormal()
			{
				Win8ButtonColors colors=
				{
					Win8GetSystemWindowColor(),
					Win8GetSystemWindowColor(),
					Win8GetSystemWindowColor(),
					Color(96, 96, 96),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ScrollArrowActive()
			{
				Win8ButtonColors colors=
				{
					Color(218, 218, 218),
					Color(218, 218, 218),
					Color(218, 218, 218),
					Color(0, 0, 0),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ScrollArrowPressed()
			{
				Win8ButtonColors colors=
				{
					Color(96, 96, 96),
					Color(96, 96, 96),
					Color(96, 96, 96),
					Color(255, 255, 255),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ScrollArrowDisabled()
			{
				Win8ButtonColors colors=
				{
					Win8GetSystemWindowColor(),
					Win8GetSystemWindowColor(),
					Win8GetSystemWindowColor(),
					Color(191, 191, 191),
				};
				return colors;
			}

/***********************************************************************
Win8ButtonElements
***********************************************************************/

			Win8ButtonElements Win8ButtonElements::Create(Alignment::Type horizontal, Alignment::Type vertical)
			{
				Win8ButtonElements button;
				{
					button.mainComposition=new GuiBoundsComposition;
					button.mainComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				}
				{
					GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
					button.rectBorderElement=element;

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					button.mainComposition->AddChild(composition);
					composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					composition->SetOwnedElement(element);
				}
				{
					GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
					button.backgroundElement=element;
					element->SetDirection(GuiGradientBackgroundElement::Vertical);
					element->SetShape(ElementShape::Rectangle);

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					button.backgroundComposition=composition;
					button.mainComposition->AddChild(composition);
					composition->SetAlignmentToParent(Margin(1, 1, 1, 1));
					composition->SetOwnedElement(element);
				}
				{
					Win8CreateSolidLabelElement(button.textElement, button.textComposition, horizontal, vertical);
					button.mainComposition->AddChild(button.textComposition);
				}
				return button;
			}

			void Win8ButtonElements::Apply(const Win8ButtonColors& colors)
			{
				if(rectBorderElement)
				{
					rectBorderElement->SetColor(colors.borderColor);
				}
				backgroundElement->SetColors(colors.g1, colors.g2);
				textElement->SetColor(colors.textColor);
			}

/***********************************************************************
Win8CheckedButtonElements
***********************************************************************/

			Win8CheckedButtonElements Win8CheckedButtonElements::Create(elements::ElementShape::Type shape, bool backgroundVisible)
			{
				const int checkSize=13;
				const int checkPadding=2;

				Win8CheckedButtonElements button;
				{
					button.mainComposition=new GuiBoundsComposition;
					button.mainComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				}
				{
					GuiTableComposition* mainTable=new GuiTableComposition;
					button.mainComposition->AddChild(mainTable);
					if(backgroundVisible)
					{
						GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
						element->SetColor(Win8GetSystemWindowColor());
						mainTable->SetOwnedElement(element);
					}
					mainTable->SetRowsAndColumns(1, 2);
					mainTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
					mainTable->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
					mainTable->SetColumnOption(0, GuiCellOption::AbsoluteOption(checkSize+2*checkPadding));
					mainTable->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
					
					{
						GuiCellComposition* cell=new GuiCellComposition;
						mainTable->AddChild(cell);
						cell->SetSite(0, 0, 1, 1);

						GuiTableComposition* table=new GuiTableComposition;
						cell->AddChild(table);
						table->SetRowsAndColumns(3, 1);
						table->SetAlignmentToParent(Margin(0, 0, 0, 0));
						table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
						table->SetRowOption(1, GuiCellOption::MinSizeOption());
						table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));

						{
							GuiCellComposition* checkCell=new GuiCellComposition;
							table->AddChild(checkCell);
							checkCell->SetSite(1, 0, 1, 1);
							{
								GuiBoundsComposition* borderBounds=new GuiBoundsComposition;
								checkCell->AddChild(borderBounds);
								borderBounds->SetAlignmentToParent(Margin(checkPadding, -1, checkPadding, -1));
								borderBounds->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
								{
									GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
									button.bulletBorderElement=element;
									element->SetShape(shape);

									GuiBoundsComposition* bounds=new GuiBoundsComposition;
									borderBounds->AddChild(bounds);
									bounds->SetOwnedElement(element);
									bounds->SetAlignmentToParent(Margin(0, 0, 0, 0));
									bounds->SetBounds(Rect(Point(0, 0), Size(checkSize, checkSize)));
								}
								{
									GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
									button.bulletBackgroundElement=element;
									element->SetShape(shape);
									element->SetDirection(GuiGradientBackgroundElement::Vertical);

									GuiBoundsComposition* bounds=new GuiBoundsComposition;
									borderBounds->AddChild(bounds);
									bounds->SetOwnedElement(element);
									bounds->SetAlignmentToParent(Margin(1, 1, 1, 1));
								}
							}

							button.bulletCheckElement=0;
							button.bulletRadioElement=0;
							if(shape==ElementShape::Rectangle)
							{
								button.bulletCheckElement=GuiSolidLabelElement::Create();
								{
									FontProperties font;
									font.fontFamily=L"Wingdings 2";
									font.size=16;
									font.bold=true;
									button.bulletCheckElement->SetFont(font);
								}
								button.bulletCheckElement->SetText(L"P");
								button.bulletCheckElement->SetAlignments(Alignment::Center, Alignment::Center);

								GuiBoundsComposition* composition=new GuiBoundsComposition;
								composition->SetOwnedElement(button.bulletCheckElement);
								composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
								checkCell->AddChild(composition);
							}
							else
							{
								button.bulletRadioElement=GuiSolidBackgroundElement::Create();
								button.bulletRadioElement->SetShape(ElementShape::Ellipse);

								GuiBoundsComposition* composition=new GuiBoundsComposition;
								composition->SetOwnedElement(button.bulletRadioElement);
								composition->SetAlignmentToParent(Margin(checkPadding+3, 3, checkPadding+3, 3));
								checkCell->AddChild(composition);
							}
						}
					}

					{
						GuiCellComposition* textCell=new GuiCellComposition;
						mainTable->AddChild(textCell);
						textCell->SetSite(0, 1, 1, 1);
						{
							Win8CreateSolidLabelElement(button.textElement, button.textComposition, Alignment::Left, Alignment::Center);
							textCell->AddChild(button.textComposition);
						}
					}
				}
				return button;
			}

			void Win8CheckedButtonElements::Apply(const Win8ButtonColors& colors)
			{
				bulletBorderElement->SetColor(colors.borderColor);
				bulletBackgroundElement->SetColors(colors.g1, colors.g2);
				textElement->SetColor(colors.textColor);
				if(bulletCheckElement)
				{
					bulletCheckElement->SetColor(colors.bullet);
				}
				if(bulletRadioElement)
				{
					bulletRadioElement->SetColor(colors.bullet);
				}
			}

/***********************************************************************
Win8TextBoxColors
***********************************************************************/

			Win8TextBoxColors Win8TextBoxColors::Blend(const Win8TextBoxColors& c1, const Win8TextBoxColors& c2, int ratio, int total)
			{
				if(ratio<0) ratio=0;
				else if(ratio>total) ratio=total;

				Win8TextBoxColors result;
				result.borderColor=BlendColor(c1.borderColor, c2.borderColor, ratio, total);
				result.backgroundColor=BlendColor(c1.backgroundColor, c2.backgroundColor, ratio, total);
				return result;
			}
			
			Win8TextBoxColors Win8TextBoxColors::Normal()
			{
				Win8TextBoxColors result=
				{
					Color(171, 173, 179),
					Color(255, 255, 255),
				};
				return result;
			}

			Win8TextBoxColors Win8TextBoxColors::Active()
			{
				Win8TextBoxColors result=
				{
					Color(126, 180, 234),
					Color(255, 255, 255),
				};
				return result;
			}

			Win8TextBoxColors Win8TextBoxColors::Focused()
			{
				Win8TextBoxColors result=
				{
					Color(86, 157, 229),
					Color(255, 255, 255),
				};
				return result;
			}

			Win8TextBoxColors Win8TextBoxColors::Disabled()
			{
				Win8TextBoxColors result=
				{
					Color(217, 217, 217),
					Win8GetSystemWindowColor(),
				};
				return result;
			}

/***********************************************************************
Helpers
***********************************************************************/

			Color Win8GetSystemWindowColor()
			{
				return Color(240, 240, 240);
			}

			Color Win8GetSystemBorderColor()
			{
				return Color(171, 173, 179);
			}

			Color Win8GetSystemTextColor(bool enabled)
			{
				return enabled?Color(0, 0, 0):Color(131, 131, 131);
			}

			void Win8SetFont(GuiSolidLabelElement* element, GuiBoundsComposition* composition, const FontProperties& fontProperties)
			{
				int margin=3;
				element->SetFont(fontProperties);
				composition->SetMargin(Margin(margin, margin, margin, margin));
			}

			void Win8CreateSolidLabelElement(GuiSolidLabelElement*& element, GuiBoundsComposition*& composition, Alignment::Type horizontal, Alignment::Type vertical)
			{
				element=GuiSolidLabelElement::Create();
				element->SetAlignments(horizontal, vertical);

				composition=new GuiBoundsComposition;
				composition->SetOwnedElement(element);
				composition->SetMargin(Margin(0, 0, 0, 0));
				composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			}

			elements::text::ColorEntry Win8GetTextBoxTextColor()
			{
				elements::text::ColorEntry entry;
				entry.normal.text=Color(0, 0, 0);
				entry.normal.background=Color(0, 0, 0, 0);
				entry.selectedFocused.text=Color(255, 255, 255);
				entry.selectedFocused.background=Color(51, 153, 255);
				entry.selectedUnfocused.text=Color(255, 255, 255);
				entry.selectedUnfocused.background=Color(51, 153, 255);
				return entry;
			}
		}
	}
}