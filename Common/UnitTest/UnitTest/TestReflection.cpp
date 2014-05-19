#include "../../Source/UnitTest/UnitTest.h"
#include "../../Source/Reflection/GuiTypeDescriptorMacros.h"
#include "../../Source/Stream/FileStream.h"
#include "../../Source/Stream/Accessor.h"
#include "../../Source/Stream/CharFormat.h"
#include "../../Source/Collections/Operation.h"
#include <limits>
#include <float.h>

using namespace vl;
using namespace vl::collections;
using namespace vl::reflection;
using namespace vl::reflection::description;
using namespace vl::stream;

extern WString GetPath();

namespace test
{
	template<typename T, typename TValue, vint Count, WString(*ToString)(TValue)>
	void TestNumber(TValue(&values)[Count], TValue min, TValue max, bool testGetText)
	{
		ITypeDescriptor* type=GetTypeDescriptor<T>();
		ITypedValueSerializer<T>* serializer=GetValueSerializer<T>();
		TEST_ASSERT(type);
		TEST_ASSERT(serializer);
		TEST_ASSERT(type->GetValueSerializer()==serializer);
		TEST_ASSERT(serializer->GetOwnerTypeDescriptor()==type);
#ifdef VCZH_GCC
		TEST_PRINT(L"\tValidate Serializer of: " + type->GetTypeName());
#endif
		for(auto i:values)
		{
#ifdef VCZH_GCC
			TEST_PRINT(L"\tParsing: " + ToString(i) + L" using " + type->GetTypeName());
#endif
			if(min<=i && i<=max)
			{
				Value value;
				TEST_ASSERT(serializer->Validate(ToString(i))==true);
				{
					TEST_ASSERT(serializer->Parse(ToString(i), value));
#ifdef VCZH_GCC
					TEST_PRINT(L"\tPrinting: " + value.GetText());
#endif
					TEST_ASSERT(value.GetValueType()==Value::Text);
					TEST_ASSERT(value.GetTypeDescriptor()==type);
					TEST_ASSERT(value.GetRawPtr()==0);
					TEST_ASSERT(value.GetSharedPtr().Obj()==0);
					if (testGetText)
					{
						TEST_ASSERT(value.GetText()==ToString(i));
					}
				}
				{
					TEST_ASSERT(serializer->Serialize((T)i, value));
#ifdef VCZH_GCC
					TEST_PRINT(L"\tPrinting: " + value.GetText());
#endif
					TEST_ASSERT(value.GetValueType()==Value::Text);
					TEST_ASSERT(value.GetTypeDescriptor()==type);
					TEST_ASSERT(value.GetRawPtr()==0);
					TEST_ASSERT(value.GetSharedPtr().Obj()==0);
					if (testGetText)
					{
						TEST_ASSERT(value.GetText()==ToString(i));
					}
				}
				{
					T n=2;
					TEST_ASSERT(serializer->Deserialize(value, n));
#ifdef VCZH_MSVC
					TEST_ASSERT(n==(T)i);
#endif
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

	template<typename T, vint LegalCount, vint IllegalCount>
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
#ifdef VCZH_GCC
			TEST_PRINT(L"\tParsing: " + i + L" using " + type->GetTypeName());
#endif
			T j=legals[x];
			Value value;
			TEST_ASSERT(serializer->Validate(i)==true);
			{
				TEST_ASSERT(serializer->Parse(i, value));
#ifdef VCZH_GCC
			TEST_PRINT(L"\tPrinting: " + value.GetText());
#endif
				TEST_ASSERT(value.GetValueType()==Value::Text);
				TEST_ASSERT(value.GetTypeDescriptor()==type);
				TEST_ASSERT(value.GetRawPtr()==0);
				TEST_ASSERT(value.GetSharedPtr().Obj()==0);
				TEST_ASSERT(value.GetText()==i);
			}
			{
				TEST_ASSERT(serializer->Serialize(j, value));
#ifdef VCZH_GCC
			TEST_PRINT(L"\tPrinting: " + value.GetText());
#endif
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
		vint64_t values[]=
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
		TestNumber<T, vint64_t, sizeof(values)/sizeof(*values), &i64tow>(values, std::numeric_limits<T>::min(), std::numeric_limits<T>::max(), true);
	}
	
	template<typename T>
	void TestUInt()
	{
		vuint64_t values[]=
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
		TestNumber<T, vuint64_t, sizeof(values)/sizeof(*values), &u64tow>(values, std::numeric_limits<T>::min(), std::numeric_limits<T>::max(), true);
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
		TestNumber<T, double, sizeof(values)/sizeof(*values), &ftow>(values, -std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), false);
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

	void TestDateTime()
	{
		WString legalsText[]={L"2000-01-01 00:00:00.000", L"2012-12-21 13:30:45.123", L"2013-06-11 14:30:00.000"};
		WString illegalsText[]={L"2000-01-01", L"00:00:00.000", L"2000-01-01 00:00:00"};
		DateTime legals[]={DateTime::FromDateTime(2000, 1, 1), DateTime::FromDateTime(2012, 12, 21, 13, 30, 45, 123), DateTime::FromDateTime(2013, 6, 11, 14, 30, 0)};
		TestLiteral<DateTime, sizeof(legalsText)/sizeof(*legalsText), sizeof(illegalsText)/sizeof(*illegalsText)>(legalsText, illegalsText, legals);
	}

	void TestLocale()
	{
		WString legalsText[]={L"zh-CN", L"en-US"};
		Locale legals[]={Locale(L"zh-CN"), Locale(L"en-US")};
		TestLiteral<Locale, sizeof(legalsText)/sizeof(*legalsText), 0>(legalsText, 0, legals);
	}
}
using namespace test;

TEST_CASE(TestReflectionPredefinedType)
{
	TEST_ASSERT(LoadPredefinedTypes());
	TestInt<vint8_t>();
	TestInt<vint16_t>();
	TestInt<vint32_t>();
	TestInt<vint64_t>();
	TestUInt<vuint8_t>();
	TestUInt<vuint16_t>();
	TestUInt<vuint32_t>();
	TestUInt<vuint64_t>();
	TestFloat<float>();
	TestFloat<double>();
	TestBool();
	TestString();
	TestDateTime();
	TestLocale();
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
		vint x;
		vint y;
	};

	struct Size
	{
		vint cx;
		vint cy;
	};

	struct Rect
	{
		Point point;
		Size size;
	};

	class Base : public Object, public Description<Base>
	{
	public:
		vint a;
		Season season;
		Base():a(0), season(Spring){}
		Base(vint _a):a(_a){}
		static Ptr<Base> Create(vint _a, vint _b){return new Base(_a+_b);}
	};

	class Derived : public Base, public Description<Derived>
	{
	private:
		vint b;
	public:
		Derived():b(0){}
		Derived(vint _a, vint _b):Base(_a),b(_b){}
		static Ptr<Derived> Create(){return new Derived();}
		static Ptr<Derived> Create(vint _a, vint _b){return new Derived(_a, _b);}

		vint GetB(){return b;}
		void SetB(vint value){b=value;}
		void Reset(){a=0; b=0;}
		void Reset(vint _a, vint _b){a=_a; b=_b;}
		void Reset(ResetOption opt){if(opt&ResetA) a=0; if(opt&ResetB) b=0;}
		void Reset(Derived* derived){a=derived->a; b=derived->b;}
		
		Nullable<WString> c;
		Nullable<WString> GetC(){return c;}
		void SetC(Nullable<WString> value){c=value;}
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

		vint Sum()
		{
			return From(bases)
				.Select([](const Ptr<Base>& base){return base->a;})
				.Aggregate((vint)0, [](vint a, vint b){return a+b;});
		}

		List<Ptr<Base>>			bases3;

		List<Ptr<Base>>& GetBases2()
		{
			return bases3;
		}

		void SetBases2(List<Ptr<Base>>& _bases)
		{
			CopyFrom(bases3, _bases);
		}

		vint Sum2()
		{
			return From(bases3)
				.Select([](const Ptr<Base>& base){return base->a;})
				.Aggregate((vint)0, [](vint a, vint b){return a+b;});
		}

		Func<vint(vint)> Sum3(Func<vint(vint)> f1, Func<vint(vint)> f2)
		{
			return [=](vint i){return f1(i)+f2(i);};
		}
	};

	class DictionaryHolder : public Description<DictionaryHolder>
	{
	public:
		Dictionary<vint, WString>				maps;
		Dictionary<Ptr<Base>, Ptr<Base>>		maps2;

		const Dictionary<vint, WString>& GetMaps()
		{ 
			return maps;
		}

		void SetMaps(const Dictionary<vint, WString>& value)
		{
			CopyFrom(maps, value);
		}
	};

	class EventRaiser : public Description<EventRaiser>
	{
	protected:
		vint									value;
	public:
		Event<void(vint, vint)>					ValueChanged;

		EventRaiser()
			:value(0)
		{
		}

		vint GetValue()
		{
			return value;
		}

		void SetValue(vint newValue)
		{
			vint oldValue = value;
			value = newValue;
			ValueChanged(oldValue, value);
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
	F(test::DictionaryHolder)\
	F(test::EventRaiser)\
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
		CLASS_MEMBER_CONSTRUCTOR(Ptr<Base>(vint), {L"_a"})
		CLASS_MEMBER_EXTERNALCTOR(Ptr<Base>(vint, vint), {L"_a" _ L"_b"}, &Base::Create)
	END_CLASS_MEMBER(test::Base)

	BEGIN_CLASS_MEMBER(test::Derived)
		CLASS_MEMBER_BASE(Base)
		CLASS_MEMBER_CONSTRUCTOR(Ptr<Derived>(), NO_PARAMETER)
		CLASS_MEMBER_CONSTRUCTOR(Ptr<Derived>(vint _ vint), {L"_a" _ L"_b"})

		CLASS_MEMBER_STATIC_METHOD_OVERLOAD(Create, NO_PARAMETER, Ptr<Derived>(*)())
		CLASS_MEMBER_STATIC_METHOD_OVERLOAD(Create, {L"_a" _ L"_b"}, Ptr<Derived>(*)(vint _ vint))

		CLASS_MEMBER_METHOD(GetB, NO_PARAMETER)
		CLASS_MEMBER_METHOD(SetB, {L"value"})
		CLASS_MEMBER_PROPERTY(b, GetB, SetB)

		CLASS_MEMBER_METHOD_OVERLOAD(Reset, NO_PARAMETER, void(Derived::*)())
		CLASS_MEMBER_METHOD_OVERLOAD(Reset, {L"_a" _ L"_b"}, void(Derived::*)(vint _ vint))
		CLASS_MEMBER_METHOD_OVERLOAD(Reset, {L"opt"}, void(Derived::*)(ResetOption))
		CLASS_MEMBER_METHOD_OVERLOAD(Reset, {L"derived"}, void(Derived::*)(Derived*))

		CLASS_MEMBER_FIELD(c)
		CLASS_MEMBER_PROPERTY_FAST(C)
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
		return new ValueReadonlyListWrapper<const Array<Ptr<Base>>*>(&thisObject->GetBases());
	}

	void BaseSummer_SetBases(BaseSummer* thisObject, Ptr<IValueReadonlyList> bases)
	{
		Array<Ptr<Base>> baseArray;
		CopyFrom(baseArray, GetLazyList<Ptr<Base>>(bases));
		thisObject->SetBases(baseArray);
	}

	BEGIN_CLASS_MEMBER(BaseSummer)
		CLASS_MEMBER_CONSTRUCTOR(Ptr<BaseSummer>(), NO_PARAMETER)
		CLASS_MEMBER_METHOD(Sum, NO_PARAMETER)
		CLASS_MEMBER_METHOD(Sum2, NO_PARAMETER)
		CLASS_MEMBER_METHOD(Sum3, {L"f1" _ L"f2"})
		CLASS_MEMBER_METHOD(GetBases, NO_PARAMETER)
		CLASS_MEMBER_METHOD(SetBases, {L"bases"})
		CLASS_MEMBER_METHOD(GetBases2, NO_PARAMETER)
		CLASS_MEMBER_METHOD(SetBases2, {L"bases"})
		CLASS_MEMBER_FIELD(bases3)
	END_CLASS_MEMBER(BaseSummer)

	BEGIN_CLASS_MEMBER(DictionaryHolder)
		CLASS_MEMBER_FIELD(maps)
		CLASS_MEMBER_FIELD(maps2)
		CLASS_MEMBER_PROPERTY_FAST(Maps)
	END_CLASS_MEMBER(DictionaryHolder)

	BEGIN_CLASS_MEMBER(EventRaiser)
		CLASS_MEMBER_CONSTRUCTOR(Ptr<EventRaiser>(), NO_PARAMETER)

		CLASS_MEMBER_EVENT(ValueChanged)
		CLASS_MEMBER_PROPERTY_EVENT_FAST(Value, ValueChanged)
	END_CLASS_MEMBER(EventRaiser)

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

TEST_CASE(TestDescriptableObjectReferenceCounterOperator)
{
	TEST_ASSERT((!AcceptValue<typename RequiresConvertable<vint, DescriptableObject>::YesNoType>::Result));
	TEST_ASSERT((AcceptValue<typename RequiresConvertable<DescriptableObject, DescriptableObject>::YesNoType>::Result));
	TEST_ASSERT((AcceptValue<typename RequiresConvertable<IDescriptable, DescriptableObject>::YesNoType>::Result));
	TEST_ASSERT((AcceptValue<typename RequiresConvertable<Base, DescriptableObject>::YesNoType>::Result));
	TEST_ASSERT((AcceptValue<typename RequiresConvertable<Derived, DescriptableObject>::YesNoType>::Result));

	Base* raw=new Base;
	volatile vint* counter=ReferenceCounterOperator<Base>::CreateCounter(raw);
	TEST_ASSERT(0==*counter);
	{
		Ptr<Base> ptr1=raw;
		TEST_ASSERT(1==*counter);
		{
			Ptr<Base> ptr2=raw;
			TEST_ASSERT(2==*counter);
			{
				Ptr<Base> ptr3=raw;
				TEST_ASSERT(3==*counter);
			}
			TEST_ASSERT(2==*counter);
		}
		TEST_ASSERT(1==*counter);
	}
}

namespace reflection_test
{
	void TestReflectionBuilder()
	{
		FileStream fileStream(GetPath()+L"Reflection.txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);
		LogTypeManager(writer);
	}

	int MyFunc(int a, int b)
	{
		return a + b;
	}

	void TestReflectionInvoke()
	{
		{
			Value base=Value::Create(L"test::Base");
			TEST_ASSERT(base.GetTypeDescriptor());
			TEST_ASSERT(base.GetTypeDescriptor()->GetTypeName()==L"test::Base");
			TEST_ASSERT(UnboxValue<vint>(base.GetProperty(L"a"))==0);

			base.SetProperty(L"a", BoxValue<vint>(100));
			TEST_ASSERT(UnboxValue<vint>(base.GetProperty(L"a"))==100);

			Value base2=Value::Create(L"test::Base", (Value_xs(), 200));
			TEST_ASSERT(base2.GetTypeDescriptor());
			TEST_ASSERT(base2.GetTypeDescriptor()->GetTypeName()==L"test::Base");
			TEST_ASSERT(UnboxValue<vint>(base2.GetProperty(L"a"))==200);

			Value base3=Value::Create(L"test::Base", (Value_xs(), 100, 200));
			TEST_ASSERT(base3.GetTypeDescriptor());
			TEST_ASSERT(base3.GetTypeDescriptor()->GetTypeName()==L"test::Base");
			TEST_ASSERT(UnboxValue<vint>(base3.GetProperty(L"a"))==300);
		}
		{
			Value derived=Value::Create(L"test::Derived");
			TEST_ASSERT(derived.GetTypeDescriptor());
			TEST_ASSERT(derived.GetTypeDescriptor()->GetTypeName()==L"test::Derived");
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==0);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==0);

			derived.SetProperty(L"a", BoxValue<vint>(100));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==100);
			derived.SetProperty(L"b", BoxValue<vint>(200));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==200);
		}
		{
			Value derived=Value::Create(L"test::Derived", (Value_xs(), 10, 20));
			TEST_ASSERT(derived.GetTypeDescriptor());
			TEST_ASSERT(derived.GetTypeDescriptor()->GetTypeName()==L"test::Derived");
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==10);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==20);

			derived.Invoke(L"Reset");
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==0);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==0);

			derived.Invoke(L"Reset", (Value_xs(), 30, 40));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==30);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==40);

			Ptr<Derived> d=UnboxValue<Ptr<Derived>>(derived);
			TEST_ASSERT(d->a==30);
			TEST_ASSERT(d->GetB()==40);
		
			Value derived2=Value::InvokeStatic(L"test::Derived", L"Create", (Value_xs(), 10, 20));
			derived2.Invoke(L"Reset", (Value_xs(), derived));
			TEST_ASSERT(UnboxValue<vint>(derived2.GetProperty(L"a"))==30);
			TEST_ASSERT(UnboxValue<vint>(derived2.GetProperty(L"b"))==40);
		}
	}

	void TestReflectionInvokeIndirect()
	{
		Value derived;
		{
			auto type = GetTypeDescriptor<Derived>();
			auto ctors = type->GetConstructorGroup();
			for (vint i = 0; i < ctors->GetMethodCount(); i++)
			{
				auto ctor = ctors->GetMethod(i);
				if (ctor->GetParameterCount() == 2)
				{
					auto proxy = ctor->CreateFunctionProxy(Value());

					auto xs = IValueList::Create();
					xs->Add(BoxValue<vint>(1));
					xs->Add(BoxValue<vint>(2));
					derived = proxy.Invoke(L"Invoke", (Value_xs(), xs));

					TEST_ASSERT(!derived.IsNull());
					TEST_ASSERT(UnboxValue<vint>(derived.Invoke(L"GetB", (Value_xs()))) == 2);
					break;
				}
			}
		}
		{
			auto proxy = derived.GetTypeDescriptor()->GetMethodGroupByName(L"SetB", true)->GetMethod(0)->CreateFunctionProxy(derived);
			{
				auto xs = IValueList::Create();
				xs->Add(BoxValue<vint>(3));
				proxy.Invoke(L"Invoke", (Value_xs(), xs));
			}
			TEST_ASSERT(UnboxValue<vint>(derived.Invoke(L"GetB", (Value_xs()))) == 3);
		}
	}

	void TestReflectionEvent()
	{
		Value eventRaiser = Value::Create(L"test::EventRaiser");
		vint oldValue = 0;
		vint newValue = 0;
		auto eventHandler = eventRaiser.AttachEvent(
			L"ValueChanged",
			BoxParameter<Func<void(vint, vint)>>(LAMBDA([&](vint _oldValue, vint _newValue)
			{
				oldValue = _oldValue;
				newValue = _newValue;
			})));
		TEST_ASSERT(eventHandler->GetOwnerObject().GetRawPtr() == eventRaiser.GetRawPtr());
		TEST_ASSERT(eventHandler->GetOwnerEvent()->GetName() == L"ValueChanged");

		TEST_ASSERT(UnboxValue<vint>(eventRaiser.GetProperty(L"Value")) == 0);
		TEST_ASSERT(oldValue == 0);
		TEST_ASSERT(newValue == 0);

		eventRaiser.SetProperty(L"Value", BoxValue<vint>(100));
		TEST_ASSERT(UnboxValue<vint>(eventRaiser.GetProperty(L"Value")) == 100);
		TEST_ASSERT(oldValue == 0);
		TEST_ASSERT(newValue == 100);

		eventRaiser.SetProperty(L"Value", BoxValue<vint>(200));
		TEST_ASSERT(UnboxValue<vint>(eventRaiser.GetProperty(L"Value")) == 200);
		TEST_ASSERT(oldValue == 100);
		TEST_ASSERT(newValue == 200);

		TEST_ASSERT(eventHandler->Detach() == true);

		eventRaiser.SetProperty(L"Value", BoxValue<vint>(300));
		TEST_ASSERT(UnboxValue<vint>(eventRaiser.GetProperty(L"Value")) == 300);
		TEST_ASSERT(oldValue == 100);
		TEST_ASSERT(newValue == 200);

		TEST_ASSERT(eventHandler->Detach() == false);
	}

	void TestReflectionEnum()
	{
		{
			Value base=Value::Create(L"test::Base");
			TEST_ASSERT(UnboxValue<Season>(base.GetProperty(L"season"))==Spring);

			base.SetProperty(L"season", BoxValue<Season>(Winter));
			TEST_ASSERT(UnboxValue<Season>(base.GetProperty(L"season"))==Winter);
		}
		{
			Value derived=Value::Create(L"test::Derived", (Value_xs(), 10, 20));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==10);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==20);

			derived.Invoke(L"Reset", (Value_xs(), ResetNone));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==10);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==20);
		}
		{
			Value derived=Value::Create(L"test::Derived", (Value_xs(), 10, 20));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==10);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==20);

			derived.Invoke(L"Reset", (Value_xs(), ResetA));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==0);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==20);
		}
		{
			Value derived=Value::Create(L"test::Derived", (Value_xs(), 10, 20));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==10);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==20);

			derived.Invoke(L"Reset", (Value_xs(), ResetB));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==10);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==0);
		}
		{
			Value derived=Value::Create(L"test::Derived", (Value_xs(), 10, 20));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==10);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==20);

			derived.Invoke(L"Reset", (Value_xs(), (ResetOption)(ResetA|ResetB)));
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"a"))==0);
			TEST_ASSERT(UnboxValue<vint>(derived.GetProperty(L"b"))==0);
		} 
	}

	void TestReflectionNullable()
	{
		{
			Value derived=Value::Create(L"test::Derived");
			TEST_ASSERT(UnboxValue<Nullable<WString>>(derived.GetProperty(L"c"))==false);
			TEST_ASSERT(UnboxValue<Nullable<WString>>(derived.GetProperty(L"C"))==false);

			derived.SetProperty(L"c", BoxValue(Nullable<WString>(L"vczh")));
			TEST_ASSERT(UnboxValue<Nullable<WString>>(derived.GetProperty(L"c")).Value()==L"vczh");
			TEST_ASSERT(UnboxValue<Nullable<WString>>(derived.GetProperty(L"C")).Value()==L"vczh");

			derived.SetProperty(L"C", BoxValue(Nullable<WString>()));
			TEST_ASSERT(UnboxValue<Nullable<WString>>(derived.GetProperty(L"c"))==false);
			TEST_ASSERT(UnboxValue<Nullable<WString>>(derived.GetProperty(L"C"))==false);
		}
	}

	void TestReflectionStruct()
	{
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
	}

	void TestReflectionList()
	{
		{
			Value bases=Value::Create(L"system::List");
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 1))));
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 2))));
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 3))));
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 4))));

			Value baseSummer=Value::Create(L"test::BaseSummer");
			baseSummer.Invoke(L"SetBases", (Value_xs(), bases));
			TEST_ASSERT(UnboxValue<vint>(baseSummer.Invoke(L"Sum"))==10);

			Value baseArray=baseSummer.Invoke(L"GetBases");
			TEST_ASSERT(UnboxValue<vint>(baseArray.GetProperty(L"Count"))==4);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 0)).GetProperty(L"a"))==1);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 1)).GetProperty(L"a"))==2);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 2)).GetProperty(L"a"))==3);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 3)).GetProperty(L"a"))==4);
		}
		{
			Value bases=Value::Create(L"system::List");
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 1))));
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 2))));
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 3))));
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 4))));

			Value baseSummer=Value::Create(L"test::BaseSummer");
			baseSummer.Invoke(L"SetBases2", (Value_xs(), bases));
			TEST_ASSERT(UnboxValue<vint>(baseSummer.Invoke(L"Sum2"))==10);

			Value baseArray=baseSummer.Invoke(L"GetBases2");
			TEST_ASSERT(UnboxValue<vint>(baseArray.GetProperty(L"Count"))==4);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 0)).GetProperty(L"a"))==1);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 1)).GetProperty(L"a"))==2);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 2)).GetProperty(L"a"))==3);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 3)).GetProperty(L"a"))==4);
		}
		{
			Value bases=Value::Create(L"system::List");
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 1))));
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 2))));
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 3))));
			bases.Invoke(L"Add", (Value_xs(), Value::Create(L"test::Base", (Value_xs(), 4))));

			Value baseSummer=Value::Create(L"test::BaseSummer");
			baseSummer.SetProperty(L"bases3", bases);
			TEST_ASSERT(UnboxValue<vint>(baseSummer.Invoke(L"Sum2"))==10);

			Value baseArray=baseSummer.GetProperty(L"bases3");
			TEST_ASSERT(UnboxValue<vint>(baseArray.GetProperty(L"Count"))==4);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 0)).GetProperty(L"a"))==1);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 1)).GetProperty(L"a"))==2);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 2)).GetProperty(L"a"))==3);
			TEST_ASSERT(UnboxValue<vint>(baseArray.Invoke(L"Get", (Value_xs(), 3)).GetProperty(L"a"))==4);
		}
		{
			Value baseSummer=Value::Create(L"test::BaseSummer");
			Value f1=BoxParameter<Func<vint(vint)>>(LAMBDA([](vint i){return i+1;}));
			Value f2=BoxParameter<Func<vint(vint)>>(LAMBDA([](vint i){return i+2;}));
			Value f=baseSummer.Invoke(L"Sum3", (Value_xs(), f1, f2));
			Func<vint(vint)> fx;
			UnboxParameter<Func<vint(vint)>>(f, fx);
			TEST_ASSERT(fx(10)==23);
		}
	}

	void TestReflectionDictionary()
	{
		Value map=Value::Create(L"system::Dictionary");
		map.Invoke(L"Set", (Value_xs(), 1, 1));
		map.Invoke(L"Set", (Value_xs(), 2, 4));
		map.Invoke(L"Set", (Value_xs(), 3, 9));

		TEST_ASSERT(3==UnboxValue<vint>(map.GetProperty(L"Count")));
		TEST_ASSERT(1==UnboxValue<vint>(map.GetProperty(L"Keys").Invoke(L"Get", (Value_xs(), 0))));
		TEST_ASSERT(2==UnboxValue<vint>(map.GetProperty(L"Keys").Invoke(L"Get", (Value_xs(), 1))));
		TEST_ASSERT(3==UnboxValue<vint>(map.GetProperty(L"Keys").Invoke(L"Get", (Value_xs(), 2))));
		TEST_ASSERT(1==UnboxValue<vint>(map.GetProperty(L"Values").Invoke(L"Get", (Value_xs(), 0))));
		TEST_ASSERT(4==UnboxValue<vint>(map.GetProperty(L"Values").Invoke(L"Get", (Value_xs(), 1))));
		TEST_ASSERT(9==UnboxValue<vint>(map.GetProperty(L"Values").Invoke(L"Get", (Value_xs(), 2))));

		map.Invoke(L"Remove", (Value_xs(), 2));
		TEST_ASSERT(2==UnboxValue<vint>(map.GetProperty(L"Count")));
		TEST_ASSERT(1==UnboxValue<vint>(map.GetProperty(L"Keys").Invoke(L"Get", (Value_xs(), 0))));
		TEST_ASSERT(3==UnboxValue<vint>(map.GetProperty(L"Keys").Invoke(L"Get", (Value_xs(), 1))));
		TEST_ASSERT(1==UnboxValue<vint>(map.GetProperty(L"Values").Invoke(L"Get", (Value_xs(), 0))));
		TEST_ASSERT(9==UnboxValue<vint>(map.GetProperty(L"Values").Invoke(L"Get", (Value_xs(), 1))));

		map.Invoke(L"Clear", Value_xs());
		TEST_ASSERT(0==UnboxValue<vint>(map.GetProperty(L"Count")));
	}

	void TestSharedRawPtrConverting()
	{
		Base* b1=new Base;
		volatile vint* rc=ReferenceCounterOperator<Base>::CreateCounter(b1);
		TEST_ASSERT(*rc==0);

		Ptr<Base> b2=b1;
		TEST_ASSERT(*rc==1);

		Value v1=BoxValue(b1);
		TEST_ASSERT(v1.GetValueType()==Value::RawPtr);
		TEST_ASSERT(*rc==1);

		Value v2=BoxValue(b2);
		TEST_ASSERT(v2.GetValueType()==Value::SharedPtr);
		TEST_ASSERT(*rc==2);

		Base* b3=UnboxValue<Base*>(v2);
		TEST_ASSERT(b3==b1);
		TEST_ASSERT(*rc==2);

		Ptr<Base> b4=UnboxValue<Ptr<Base>>(v1);
		TEST_ASSERT(b4==b1);
		TEST_ASSERT(*rc==3);
	}

	void TestSharedRawPtrDestructing()
	{
		{
			Ptr<Base> b=new Base;
			Ptr<Object> o=b;
			b=0;
			o=0;
		}
		{
			//Base* b=new Base;
			//volatile vint* rc=ReferenceCounterOperator<Base>::CreateCounter(b);
			//TEST_ASSERT(*rc==0);

			//Ptr<Object> o=b;
			//TEST_ASSERT(*rc==1);
		}
	}
}
using namespace reflection_test;

#define TEST_CASE_REFLECTION(NAME)\
	TEST_CASE(NAME)\
	{\
		TEST_ASSERT(LoadPredefinedTypes());\
		TEST_ASSERT(GetGlobalTypeManager()->AddTypeLoader(new TestTypeLoader));\
		TEST_ASSERT(GetGlobalTypeManager()->Load());\
		{\
			NAME();\
		}\
		TEST_ASSERT(ResetGlobalTypeManager());\
	}\

TEST_CASE_REFLECTION(TestReflectionBuilder)
TEST_CASE_REFLECTION(TestReflectionInvoke)
TEST_CASE_REFLECTION(TestReflectionInvokeIndirect)
TEST_CASE_REFLECTION(TestReflectionEvent)
TEST_CASE_REFLECTION(TestReflectionEnum)
TEST_CASE_REFLECTION(TestReflectionNullable)
TEST_CASE_REFLECTION(TestReflectionStruct)
TEST_CASE_REFLECTION(TestReflectionList)
TEST_CASE_REFLECTION(TestReflectionDictionary)
TEST_CASE_REFLECTION(TestSharedRawPtrConverting)
TEST_CASE_REFLECTION(TestSharedRawPtrDestructing)
