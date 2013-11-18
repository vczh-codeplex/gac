#include "ParserGen.h"

/***********************************************************************
WriteVisitorImpl
***********************************************************************/

void WriteVisitorImpl(ParsingSymbolManager* manager, ParsingSymbol* scope, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	if(scope->GetType()==ParsingSymbol::ClassType)
	{
		ParsingSymbol* parent=scope->GetDescriptorSymbol();
		if(parent)
		{
			writer.WriteString(prefix);
			writer.WriteString(L"void ");
			PrintType(scope, codeClassPrefix, writer);
			writer.WriteString(L"::Accept(");
			PrintType(parent, codeClassPrefix, writer);
			writer.WriteLine(L"::IVisitor* visitor)");
			writer.WriteString(prefix);
			writer.WriteLine(L"{");
			writer.WriteString(prefix);
			writer.WriteLine(L"\tvisitor->Visit(this);");
			writer.WriteString(prefix);
			writer.WriteLine(L"}");
			writer.WriteLine(L"");
		}
	}
	for(vint i=0;i<scope->GetSubSymbolCount();i++)
	{
		WriteVisitorImpl(manager, scope->GetSubSymbol(i), prefix, codeClassPrefix, writer);
	}
}