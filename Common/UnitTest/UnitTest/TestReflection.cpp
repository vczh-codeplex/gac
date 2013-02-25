#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Reflection\GuiTypeDescriptor.h"
#include "..\..\Source\Reflection\GuiTypeDescriptorPredefined.h"
#include "..\..\Source\Reflection\GuiTypeDescriptorBuilder.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::reflection;
using namespace vl::reflection::description;

TEST_CASE(TestReflectionPredefinedType)
{
	TEST_ASSERT(LoadPredefinedTypes());
}