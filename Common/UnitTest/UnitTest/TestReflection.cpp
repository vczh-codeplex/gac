#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Reflection\GuiTypeDescriptor.h"
#include "..\..\Source\Reflection\GuiTypeDescriptorPredefined.h"
#include "..\..\Source\Reflection\GuiTypeDescriptorBuilder.h"

#include <limits>

using namespace vl;
using namespace vl::collections;
using namespace vl::reflection;
using namespace vl::reflection::description;

namespace test
{
	template<typename T, typename TValue, int Count, WString(*ToString)(TValue)>
	void TestNumber(TValue(&values)[Count], TValue min, TValue max)
	{
		ITypeDescriptor* type=GetTypeDescriptor<T>();
		ITypedValueSerializer<T>* serializer=GetValueSerializer<T>();
		TEST_ASSERT(type);
		TEST_ASSERT(serializer);
		TEST_ASSERT(type->GetValueSerializer()==serializer);
		TEST_ASSERT(serializer->GetOwnerTypeDescriptor()==type);
		for(auto i:values)
		{
			if(min<=i && i<=max)
			{
				Value value;
				TEST_ASSERT(serializer->Validate(ToString(i))==true);
				{
					TEST_ASSERT(serializer->Parse(ToString(i), value));
					TEST_ASSERT(value.GetValueType()==Value::Text);
					TEST_ASSERT(value.GetTypeDescriptor()==type);
					TEST_ASSERT(value.GetRawPtr()==0);
					TEST_ASSERT(value.GetSharedPtr().Obj()==0);
					TEST_ASSERT(value.GetText()==ToString(i));
				}
				{
					TEST_ASSERT(serializer->Serialize((T)i, value));
					TEST_ASSERT(value.GetValueType()==Value::Text);
					TEST_ASSERT(value.GetTypeDescriptor()==type);
					TEST_ASSERT(value.GetRawPtr()==0);
					TEST_ASSERT(value.GetSharedPtr().Obj()==0);
					TEST_ASSERT(value.GetText()==ToString((T)i));
				}
				{
					T n=2;
					TEST_ASSERT(serializer->Deserialize(value, n));
					TEST_ASSERT(n==(T)i);
				}
			}
			else
			{
				Value value;
				TEST_ASSERT(serializer->Validate(ToString(i))==false);
				{
					TEST_ASSERT(serializer->Parse(ToString(i), value)==false);
					TEST_ASSERT(value.GetValueType()==Value::Null);
					TEST_ASSERT(value.GetTypeDescriptor()==0);
					TEST_ASSERT(value.GetRawPtr()==0);
					TEST_ASSERT(value.GetSharedPtr().Obj()==0);
					TEST_ASSERT(value.GetText()==L"");
				}
				{
					T n=2;
					TEST_ASSERT(serializer->Deserialize(value, n)==false);
					TEST_ASSERT(n==2);
				}
			}
		}
	}

	template<typename T, int LegalCount, int IllegalCount>
	void TestLiteral(WString legalsText[], WString illegalsText[], T legals[])
	{
		ITypeDescriptor* type=GetTypeDescriptor<T>();
		ITypedValueSerializer<T>* serializer=GetValueSerializer<T>();
		TEST_ASSERT(type);
		TEST_ASSERT(serializer);
		TEST_ASSERT(type->GetValueSerializer()==serializer);
		TEST_ASSERT(serializer->GetOwnerTypeDescriptor()==type);

		for(vint x=0;x<LegalCount;x++)
		{
			WString i=legalsText[x];
			T j=legals[x];
			Value value;
			TEST_ASSERT(serializer->Validate(i)==true);
			{
				TEST_ASSERT(serializer->Parse(i, value));
				TEST_ASSERT(value.GetValueType()==Value::Text);
				TEST_ASSERT(value.GetTypeDescriptor()==type);
				TEST_ASSERT(value.GetRawPtr()==0);
				TEST_ASSERT(value.GetSharedPtr().Obj()==0);
				TEST_ASSERT(value.GetText()==i);
			}
			{
				TEST_ASSERT(serializer->Serialize(j, value));
				TEST_ASSERT(value.GetValueType()==Value::Text);
				TEST_ASSERT(value.GetTypeDescriptor()==type);
				TEST_ASSERT(value.GetRawPtr()==0);
				TEST_ASSERT(value.GetSharedPtr().Obj()==0);
				TEST_ASSERT(value.GetText()==i);
			}
			{
				T n;
				TEST_ASSERT(serializer->Deserialize(value, n));
				TEST_ASSERT(n==j);
			}
		}
		
		for(vint x=0;x<IllegalCount;x++)
		{
			WString i=illegalsText[x];
			Value value;
			TEST_ASSERT(serializer->Validate(i)==false);
			{
				TEST_ASSERT(serializer->Parse(i, value)==false);
				TEST_ASSERT(value.GetValueType()==Value::Null);
				TEST_ASSERT(value.GetTypeDescriptor()==0);
				TEST_ASSERT(value.GetRawPtr()==0);
				TEST_ASSERT(value.GetSharedPtr().Obj()==0);
				TEST_ASSERT(value.GetText()==L"");
			}
			{
				T n;
				TEST_ASSERT(serializer->Deserialize(value, n)==false);
			}
		}
	}

	template<typename T>
	void TestInt()
	{
		signed __int64 values[]=
		{
			_I64_MIN,
			_I64_MIN+1,
			_I32_MIN,
			_I32_MIN+1,
			_I16_MIN,
			_I16_MIN+1,
			_I8_MIN,
			_I8_MIN+1,
			-1,
			0,
			1,
			_I8_MAX-1,
			_I8_MAX,
			_I16_MAX-1,
			_I16_MAX,
			_I32_MAX-1,
			_I32_MAX,
			_I64_MAX-1,
			_I64_MAX,
		};
		TestNumber<T, signed __int64, sizeof(values)/sizeof(*values), &i64tow>(values, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
	}
	
	template<typename T>
	void TestUInt()
	{
		unsigned __int64 values[]=
		{
			0,
			1,
			_UI8_MAX-1,
			_UI8_MAX,
			_UI16_MAX-1,
			_UI16_MAX,
			_UI32_MAX-1,
			_UI32_MAX,
			_UI64_MAX-1,
			_UI64_MAX,
		};
		TestNumber<T, unsigned __int64, sizeof(values)/sizeof(*values), &u64tow>(values, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
	}

	template<typename T>
	void TestFloat()
	{
		double values[]=
		{
			-DBL_MAX,
			-FLT_MAX,
			-FLT_MIN,
			-DBL_MIN,
			-1,
			0,
			1,
			DBL_MIN,
			FLT_MIN,
			FLT_MAX,
			DBL_MAX,
		};
		TestNumber<T, double, sizeof(values)/sizeof(*values), &ftow>(values, -std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
	}

	void TestBool()
	{
		WString legalsText[]={L"true", L"false"};
		WString illegalsText[]={L"TRUE", L"True", L"FALSE", L"False", L""};
		bool legals[]={true, false};
		TestLiteral<bool, sizeof(legalsText)/sizeof(*legalsText), sizeof(illegalsText)/sizeof(*illegalsText)>(legalsText, illegalsText, legals);
	}

	void TestString()
	{
		WString legals[]={L"a", L"b", L"c"};
		TestLiteral<WString, sizeof(legals)/sizeof(*legals), 0>(legals, 0, legals);
	}
}
using namespace test;

TEST_CASE(TestReflectionPredefinedType)
{
	TEST_ASSERT(LoadPredefinedTypes());
	TestInt<signed __int8>();
	TestInt<signed __int16>();
	TestInt<signed __int32>();
	TestInt<signed __int64>();
	TestUInt<unsigned __int8>();
	TestUInt<unsigned __int16>();
	TestUInt<unsigned __int32>();
	TestUInt<unsigned __int64>();
	TestFloat<float>();
	TestFloat<double>();
	TestBool();
	TestString();
}