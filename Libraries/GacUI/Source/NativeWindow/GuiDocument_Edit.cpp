#include "GuiResource.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

/***********************************************************************
DocumentModel
***********************************************************************/

		vint DocumentModel::EditText(TextPos begin, TextPos end, bool frontSide, const collections::Array<WString>& text)
		{
			return -1;
		}

		bool DocumentModel::EditStyle(TextPos begin, TextPos end, Ptr<DocumentStyleProperties> style)
		{
			return false;
		}
	}
}