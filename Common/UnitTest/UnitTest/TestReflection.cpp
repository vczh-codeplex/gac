#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Reflection\GuiTypeDescriptorMacros.h"
#include "..\..\Source\Stream\FileStream.h"
#include "..\..\Source\Stream\Accessor.h"
#include "..\..\Source\Stream\CharFormat.h"
#include "..\..\Source\Collections\Operation.h"

#include <limits>

using namespace vl;
using namespace vl::collections;
using namespace vl::reflection;
using namespace vl::reflection::description;
using namespace vl::stream;

extern WString GetPath();

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
	TEST_ASSERT(ResetGlobalTypeManager());
}

namespace test
{
	enum Season
	{
		Spring,
		Summer,
		Autumn,
		Winter,
	};

	enum ResetOption
	{
		ResetNone=0,
		ResetA=1,
		ResetB=2,
	};

	struct Point
	{
		int x;
		int y;
	};

	struct Size
	{
		int cx;
		int cy;
	};

	struct Rect
	{
		Point point;
		Size size;
	};

	class Base : public Description<Base>
	{
	public:
		int a;
		Season season;
		Base():a(0), season(Spring){}
		Base(int _a):a(_a){}
		static Ptr<Base> Create(int _a, int _b){return new Base(_a+_b);}
	};

	class Derived : public Base, public Description<Derived>
	{
	private:
		int b;
	public:
		Derived():b(0){}
		Derived(int _a, int _b):Base(_a),b(_b){}
		static Ptr<Derived> Create(){return new Derived();}
		static Ptr<Derived> Create(int _a, int _b){return new Derived(_a, _b);}

		int GetB(){return b;}
		void SetB(int value){b=value;}
		void Reset(){a=0; b=0;}
		void Reset(int _a, int _b){a=_a; b=_b;}
		void Reset(ResetOption opt){if(opt&ResetA) a=0; if(opt&ResetB) b=0;}
		void Reset(Derived* derived){a=derived->a; b=derived->b;}
	};

	class BaseSummer : public Description<BaseSummer>
	{
	protected:
		Array<Ptr<Base>>		bases;
	public:
		const Array<Ptr<Base>>& GetBases()
		{
			return bases;
		}

		void SetBases(const Array<Ptr<Base>>& _bases)
		{
			CopyFrom(bases, _bases);
		}

		int Sum()
		{
			return From(bases)
				.Select([](const Ptr<Base>& base){return base->a;})
				.Aggregate(0, [](int a, int b){return a+b;});
		}
	};
}
using namespace test;

#define _ ,

#define TYPE_LIST(F)\
	F(test::Season)\
	F(test::ResetOption)\
	F(test::Base)\
	F(test::Derived)\
	F(test::BaseSummer)\
	F(test::Point)\
	F(test::Size)\
	F(test::Rect)\

BEGIN_TYPE_INFO_NAMESPACE

	TYPE_LIST(DECL_TYPE_INFO)
	TYPE_LIST(IMPL_TYPE_INFO)

	BEGIN_ENUM_ITEM(test::Season)
		ENUM_ITEM(Spring)
		ENUM_ITEM(Summer)
		ENUM_ITEM(Autumn)
		ENUM_ITEM(Winter)
	END_ENUM_ITEM(test::Season)

	BEGIN_ENUM_ITEM_MERGABLE(test::ResetOption)
		ENUM_ITEM(ResetNone)
		ENUM_ITEM(ResetA)
		ENUM_ITEM(ResetB)
	END_ENUM_ITEM(test::ResetOption)

	BEGIN_CLASS_MEMBER(test::Base)
		CLASS_MEMBER_FIELD(a)
		CLASS_MEMBER_FIELD(season)
		CLASS_MEMBER_CONSTRUCTOR(Ptr<Base>(), NO_PARAMETER)
		CLASS_MEMBER_CONSTRUCTOR(Ptr<Base>(int), {L"_a"})
		CLASS_MEMBER_EXTERNALCTOR(Ptr<Base>(int, int), {L"_a"_ L"_b"}, &Base::Create)
	END_CLASS_MEMBER(test::Base)

	BEGIN_CLASS_MEMBER(test::Derived)
		CLASS_MEMBER_BASE(Base)
		CLASS_MEMBER_CONSTRUCTOR(Ptr<Derived>(), NO_PARAMETER)
		CLASS_MEMBER_CONSTRUCTOR(Ptr<Derived>(int _ int), {L"_a" _ L"_b"})

		CLASS_MEMBER_STATIC_METHOD_OVERLOAD(Create, NO_PARAMETER, Ptr<Derived>(*)())
		CLASS_MEMBER_STATIC_METHOD_OVERLOAD(Create, {L"_a" _ L"_b"}, Ptr<Derived>(*)(int _ int))

		CLASS_MEMBER_METHOD(GetB, NO_PARAMETER)
		CLASS_MEMBER_METHOD(SetB, {L"value"})
		CLASS_MEMBER_PROPERTY(b, GetB, SetB)

		CLASS_MEMBER_METHOD_OVERLOAD(Reset, NO_PARAMETER, void(Derived::*)())
		CLASS_MEMBER_METHOD_OVERLOAD(Reset, {L"_a" _ L"_b"}, void(Derived::*)(int _ int))
		CLASS_MEMBER_METHOD_OVERLOAD(Reset, {L"opt"}, void(Derived::*)(ResetOption))
		CLASS_MEMBER_METHOD_OVERLOAD(Reset, {L"derived"}, void(Derived::*)(Derived*))
	END_CLASS_MEMBER(test::Derived)

	BEGIN_STRUCT_MEMBER(test::Point)
		STRUCT_MEMBER(x)
		STRUCT_MEMBER(y)
	END_STRUCT_MEMBER(test::Point)

	BEGIN_STRUCT_MEMBER(test::Size)
		STRUCT_MEMBER(cx)
		STRUCT_MEMBER(cy)
	END_STRUCT_MEMBER(test::Size)

	BEGIN_STRUCT_MEMBER(test::Rect)
		STRUCT_MEMBER(point)
		STRUCT_MEMBER(size)
	END_STRUCT_MEMBER(test::Rect)

	Ptr<IValueReadonlyList> BaseSummer_GetBases(BaseSummer* thisObject)
	{
		return new ValueRreadonlyListWrapper<const Array<Ptr<Base>>*>(&thisObject->GetBases());
	}

	void BaseSummer_SetBases(BaseSummer* thisObject, Ptr<IValueReadonlyList> bases)
	{
		Array<Ptr<Base>> baseArray;
		CopyFrom(baseArray, bases->GetLazyList<Ptr<Base>>());
		thisObject->SetBases(baseArray);
	}

	BEGIN_CLASS_MEMBER(BaseSummer)
		CLASS_MEMBER_CONSTRUCTOR(Ptr<BaseSummer>(), NO_PARAMETER)
		CLASS_MEMBER_METHOD(Sum, NO_PARAMETER)
		CLASS_MEMBER_EXTERNALMETHOD(GetBases, NO_PARAMETER, Ptr<IValueReadonlyList>(BaseSummer::*)(), &BaseSummer_GetBases)
		CLASS_MEMBER_EXTERNALMETHOD(SetBases, {L"bases"}, void(BaseSummer::*)(Ptr<IValueReadonlyList>), &BaseSummer_SetBases)
	END_CLASS_MEMBER(BaseSummer)

	class TestTypeLoader : public Object, public ITypeLoader
	{
	public:
		void Load(ITypeManager* manager)override
		{
			TYPE_LIST(ADD_TYPE_INFO)
		}
		
		void Unload(ITypeManager* manager)override
		{
		}
	};

END_TYPE_INFO_NAMESPACE

#undef TYPE_LIST

TEST_CASE(TestReflectionBuilder)
{
	TEST_ASSERT(LoadPredefinedTypes());
	TEST_ASSERT(GetGlobalTypeManager()->AddTypeLoader(new TestTypeLoader));
	TEST_ASSERT(GetGlobalTypeManager()->Load());
	{
		FileStream fileStream(GetPath()+L"Reflection.txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);
		LogTypeManager(writer);
	}
	TEST_ASSERT(ResetGlobalTypeManager());
}

TEST_CASE(TestReflectionInvoke)
{
	TEST_ASSERT(LoadPredefinedTypes());
	TEST_ASSERT(GetGlobalTypeManager()->AddTypeLoader(new TestTypeLoader));
	TEST_ASSERT(GetGlobalTypeManager()->Load());
	{
		Value base=Value::Create(L"test::Base");
		TEST_ASSERT(base.GetTypeDescriptor());
		TEST_ASSERT(base.GetTypeDescriptor()->GetTypeName()==L"test::Base");
		TEST_ASSERT(UnboxValue<int>(base.GetProperty(L"a"))==0);

		base.SetProperty(L"a", BoxValue<vint>(100));
		TEST_ASSERT(UnboxValue<int>(base.GetProperty(L"a"))==100);

		Value base2=Value::Create(L"test::Base", (Value::xs(), 200));
		TEST_ASSERT(base2.GetTypeDescriptor());
		TEST_ASSERT(base2.GetTypeDescriptor()->GetTypeName()==L"test::Base");
		TEST_ASSERT(UnboxValue<int>(base2.GetProperty(L"a"))==200);

		Value base3=Value::Create(L"test::Base", (Value::xs(), 100, 200));
		TEST_ASSERT(base3.GetTypeDescriptor());
		TEST_ASSERT(base3.GetTypeDescriptor()->GetTypeName()==L"test::Base");
		TEST_ASSERT(UnboxValue<int>(base3.GetProperty(L"a"))==300);
	}
	{
		Value derived=Value::Create(L"test::Derived");
		TEST_ASSERT(derived.GetTypeDescriptor());
		TEST_ASSERT(derived.GetTypeDescriptor()->GetTypeName()==L"test::Derived");
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==0);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==0);

		derived.SetProperty(L"a", BoxValue<vint>(100));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==100);
		derived.SetProperty(L"b", BoxValue<vint>(200));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==200);
	}
	{
		Value derived=Value::Create(L"test::Derived", (Value::xs(), 10, 20));
		TEST_ASSERT(derived.GetTypeDescriptor());
		TEST_ASSERT(derived.GetTypeDescriptor()->GetTypeName()==L"test::Derived");
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==10);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==20);

		derived.Invoke(L"Reset");
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==0);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==0);

		derived.Invoke(L"Reset", (Value::xs(), 30, 40));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==30);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==40);

		Ptr<Derived> d=UnboxValue<Ptr<Derived>>(derived);
		TEST_ASSERT(d->a==30);
		TEST_ASSERT(d->GetB()==40);
		
		Value derived2=Value::Create(L"test::Derived", (Value::xs(), 10, 20));
		derived2.Invoke(L"Reset", (Value::xs(), derived));
		TEST_ASSERT(UnboxValue<int>(derived2.GetProperty(L"a"))==30);
		TEST_ASSERT(UnboxValue<int>(derived2.GetProperty(L"b"))==40);
	}
	TEST_ASSERT(ResetGlobalTypeManager());
}

TEST_CASE(TestReflectionEnum)
{
	TEST_ASSERT(LoadPredefinedTypes());
	TEST_ASSERT(GetGlobalTypeManager()->AddTypeLoader(new TestTypeLoader));
	TEST_ASSERT(GetGlobalTypeManager()->Load());
	{
		Value base=Value::Create(L"test::Base");
		TEST_ASSERT(UnboxValue<Season>(base.GetProperty(L"season"))==Spring);

		base.SetProperty(L"season", BoxValue<Season>(Winter));
		TEST_ASSERT(UnboxValue<Season>(base.GetProperty(L"season"))==Winter);
	}
	{
		Value derived=Value::Create(L"test::Derived", (Value::xs(), 10, 20));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==10);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==20);

		derived.Invoke(L"Reset", (Value::xs(), ResetNone));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==10);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==20);
	}
	{
		Value derived=Value::Create(L"test::Derived", (Value::xs(), 10, 20));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==10);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==20);

		derived.Invoke(L"Reset", (Value::xs(), ResetA));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==0);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==20);
	}
	{
		Value derived=Value::Create(L"test::Derived", (Value::xs(), 10, 20));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==10);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==20);

		derived.Invoke(L"Reset", (Value::xs(), ResetB));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==10);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==0);
	}
	{
		Value derived=Value::Create(L"test::Derived", (Value::xs(), 10, 20));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==10);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==20);

		derived.Invoke(L"Reset", (Value::xs(), (ResetOption)(ResetA|ResetB)));
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"a"))==0);
		TEST_ASSERT(UnboxValue<int>(derived.GetProperty(L"b"))==0);
	}
	TEST_ASSERT(ResetGlobalTypeManager());
}

TEST_CASE(TestReflectionStruct)
{
	TEST_ASSERT(LoadPredefinedTypes());
	TEST_ASSERT(GetGlobalTypeManager()->AddTypeLoader(new TestTypeLoader));
	TEST_ASSERT(GetGlobalTypeManager()->Load());
	{
		Point point={10, 20};
		Value value=BoxValue<Point>(point);
		TEST_ASSERT(value.GetText()==L"x:10 y:20");

		point=UnboxValue<Point>(value);
		TEST_ASSERT(point.x==10);
		TEST_ASSERT(point.y==20);
	}
	{
		Size size={10, 20};
		Value value=BoxValue<Size>(size);
		TEST_ASSERT(value.GetText()==L"cx:10 cy:20");

		size=UnboxValue<Size>(value);
		TEST_ASSERT(size.cx==10);
		TEST_ASSERT(size.cy==20);
	}
	{
		Rect rect={{10, 20}, {30, 40}};
		Value value=BoxValue<Rect>(rect);
		TEST_ASSERT(value.GetText()==L"point:{x:10 y:20} size:{cx:30 cy:40}");

		rect=UnboxValue<Rect>(value);
		TEST_ASSERT(rect.point.x==10);
		TEST_ASSERT(rect.point.y==20);
		TEST_ASSERT(rect.size.cx==30);
		TEST_ASSERT(rect.size.cy==40);
	}
	TEST_ASSERT(ResetGlobalTypeManager());
}

TEST_CASE(TestReflectionList)
{
	TEST_ASSERT(LoadPredefinedTypes());
	TEST_ASSERT(GetGlobalTypeManager()->AddTypeLoader(new TestTypeLoader));
	TEST_ASSERT(GetGlobalTypeManager()->Load());
	{
		Value bases=Value::Create(L"List");
		bases.Invoke(L"Add", (Value::xs(), Value::Create(L"test::Base", (Value::xs(), 1))));
		bases.Invoke(L"Add", (Value::xs(), Value::Create(L"test::Base", (Value::xs(), 2))));
		bases.Invoke(L"Add", (Value::xs(), Value::Create(L"test::Base", (Value::xs(), 3))));
		bases.Invoke(L"Add", (Value::xs(), Value::Create(L"test::Base", (Value::xs(), 4))));

		Value baseSummer=Value::Create(L"test::BaseSummer");
		baseSummer.Invoke(L"SetBases", (Value::xs(), bases));
		TEST_ASSERT(UnboxValue<int>(baseSummer.Invoke(L"Sum"))==10);

		Value baseArray=baseSummer.Invoke(L"GetBases");
		TEST_ASSERT(UnboxValue<int>(baseArray.Invoke(L"Count"))==4);
		TEST_ASSERT(UnboxValue<int>(baseArray.Invoke(L"Get", (Value::xs(), 0)).GetProperty(L"a"))==1);
		TEST_ASSERT(UnboxValue<int>(baseArray.Invoke(L"Get", (Value::xs(), 1)).GetProperty(L"a"))==2);
		TEST_ASSERT(UnboxValue<int>(baseArray.Invoke(L"Get", (Value::xs(), 2)).GetProperty(L"a"))==3);
		TEST_ASSERT(UnboxValue<int>(baseArray.Invoke(L"Get", (Value::xs(), 3)).GetProperty(L"a"))==4);
	}
	TEST_ASSERT(ResetGlobalTypeManager());
}