/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Workflow::Analyzer

Interfaces:
**********************************************************************/

#include "..\Expression\WfExpression.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			class WfLexicalScope : public Object
			{
				typedef collections::Dictionary<WString, Ptr<WfType>>		TypeMap;
			public:
				// the following field is nullable and inheritable
				Ptr<WfModule>				ownerModule;
				Ptr<WfDeclaration>			ownerDeclaration;
				Ptr<WfStatement>			ownerStatement;
				Ptr<WfExpression>			ownerExpression;

				Ptr<WfLexicalScope>			parentScope;
				TypeMap						symbols;

				WfLexicalScope(Ptr<WfLexicalScope> _parentScope);
				~WfLexicalScope();
			};
		}
	}
}