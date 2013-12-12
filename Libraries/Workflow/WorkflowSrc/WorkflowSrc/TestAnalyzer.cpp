#include "..\..\..\..\Common\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Analyzer\WfAnalyzer.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::unittest;
using namespace vl::parsing;
using namespace vl::parsing::tabling;
using namespace vl::workflow;
using namespace vl::workflow::analyzer;
using namespace vl::reflection;
using namespace vl::reflection::description;

extern Ptr<ParsingTable>		GetWorkflowTable();
extern WString					GetPath();

void LoadTypes()
{
	LoadPredefinedTypes();
	LoadParsingTypes();
	XmlLoadTypes();
	JsonLoadTypes();
	WfLoadTypes();
	TEST_ASSERT(GetGlobalTypeManager()->Load());
}

void UnloadTypes()
{
	TEST_ASSERT(GetGlobalTypeManager()->Unload());
	TEST_ASSERT(DestroyGlobalTypeManager());
}

TEST_CASE(TestBuildGlobalName)
{
	LoadTypes();
	WfLexicalScopeManager manager;
	manager.BuildGlobalName();
	UnloadTypes();
}