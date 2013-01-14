#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Pointer.h"
#include "..\..\Source\Tuple.h"
#include "..\..\Source\Lazy.h"
#include "..\..\Source\Locale.h"
#include "..\..\Source\Stream\Accessor.h"
#include "..\..\Source\Stream\CharFormat.h"
#include "..\..\Source\Stream\FileStream.h"
#include "..\..\Source\Collections\OperationForEach.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;

extern WString GetPath();

namespace ObjectsForTestAutoPointer
{
	class Base : public Object
	{
	public:
		vint number;

		Base(vint _number)
		{
			number=_number;
		}
	};

	class Derived1 : public Base
	{
	public:
		Derived1(vint _number)
			:Base(_number)
		{
		}
	};

	class Derived2 : public Base
	{
	public:
		Derived2(vint _number)
			:Base(_number)
		{
		}
	};
}

using namespace ObjectsForTestAutoPointer;

TEST_CASE(TestAutoPointer)
{
	Base* b=new Base(1);
	Base* d1=new Derived1(2);
	Base* d2=new Derived2(3);

	Base* bs[]={b, d1, d2};
	Ptr<Base> ps[]={b, d1, d2};

	for(vint i=0;i<sizeof(ps)/sizeof(*ps);i++)
	{
		TEST_ASSERT(ps[i].Obj()==bs[i]);
		TEST_ASSERT(ps[i].operator->()==bs[i]);
		TEST_ASSERT((bool)ps[i]==true);
	}

	for(vint i=0;i<sizeof(ps)/sizeof(*ps);i++)
	{
		for(vint j=0;j<sizeof(ps)/sizeof(*ps);j++)
		{
			TEST_ASSERT((ps[i]==ps[j]) == (bs[i]==bs[j]));
			TEST_ASSERT((ps[i]!=ps[j]) == (bs[i]!=bs[j]));
			TEST_ASSERT((ps[i]>ps[j])  == (bs[i]>bs[j]));
			TEST_ASSERT((ps[i]>=ps[j]) == (bs[i]>=bs[j]));
			TEST_ASSERT((ps[i]<ps[j])  == (bs[i]<bs[j]));
			TEST_ASSERT((ps[i]<=ps[j]) == (bs[i]<=bs[j]));
			
			TEST_ASSERT((ps[i]==bs[j]) == (bs[i]==bs[j]));
			TEST_ASSERT((ps[i]!=bs[j]) == (bs[i]!=bs[j]));
			TEST_ASSERT((ps[i]>bs[j])  == (bs[i]>bs[j]));
			TEST_ASSERT((ps[i]>=bs[j]) == (bs[i]>=bs[j]));
			TEST_ASSERT((ps[i]<bs[j])  == (bs[i]<bs[j]));
			TEST_ASSERT((ps[i]<=bs[j]) == (bs[i]<=bs[j]));
		}
	}

	Ptr<Derived1> p1=ps[1].Cast<Derived1>();
	Ptr<Derived2> p2=ps[1].Cast<Derived2>();
	TEST_ASSERT((bool)p1==true);
	TEST_ASSERT(p1->number==2);
	TEST_ASSERT((bool)p2==false);

	p1=ps[2].Cast<Derived1>();
	p2=ps[2].Cast<Derived2>();
	TEST_ASSERT((bool)p1==false);
	TEST_ASSERT((bool)p2==true);
	TEST_ASSERT(p2->number==3);

	p1=new Derived1(4);
	p2=new Derived2(5);
	TEST_ASSERT((bool)p1==true);
	TEST_ASSERT(p1->number==4);
	TEST_ASSERT((bool)p2==true);
	TEST_ASSERT(p2->number==5);
}

vint GetLazyValue1()
{
	return 100;
}

vint GetLazyValue2()
{
	return 200;
}

TEST_CASE(TestLazy)
{
	Lazy<vint> a=GetLazyValue1;
	Lazy<vint> b=a;
	TEST_ASSERT(a.IsEvaluated()==false);
	TEST_ASSERT(b.IsEvaluated()==false);
	TEST_ASSERT(a.Value()==100);
	TEST_ASSERT(a.IsEvaluated()==true);
	TEST_ASSERT(b.IsEvaluated()==true);
	TEST_ASSERT(b.Value()==100);

	b=GetLazyValue2;
	TEST_ASSERT(b.IsEvaluated()==false);
	TEST_ASSERT(a.IsEvaluated()==true);
	a=b;
	TEST_ASSERT(a.IsEvaluated()==false);
	TEST_ASSERT(b.IsEvaluated()==false);
	TEST_ASSERT(a.Value()==200);
	TEST_ASSERT(a.IsEvaluated()==true);
	TEST_ASSERT(b.IsEvaluated()==true);
	TEST_ASSERT(b.Value()==200);

	a=300;
	b=Lazy<vint>(400);
	TEST_ASSERT(a.Value()==300);
	TEST_ASSERT(a.IsEvaluated()==true);
	TEST_ASSERT(b.IsEvaluated()==true);
	TEST_ASSERT(b.Value()==400);

	TEST_ASSERT(a.IsAvailable()==true);
	a=Lazy<vint>();
	TEST_ASSERT(a.IsAvailable()==false);
}

TEST_CASE(TestLocale)
{
	FileStream fileStream(GetPath()+L"Locale.txt", FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream encoderStream(fileStream, encoder);
	StreamWriter writer(encoderStream);

	writer.WriteLine(L"Invariant locale: "+Locale::Invariant().GetName());
	writer.WriteLine(L"User default locale: "+Locale::UserDefault().GetName());
	writer.WriteLine(L"System default locale: "+Locale::SystemDefault().GetName());

	List<Locale> locales;
	Locale::Enumerate(locales);
	FOREACH(Locale, locale, locales)
	{
		writer.WriteLine(L"========================================");
		writer.WriteLine(L"Locale: "+locale.GetName());
	}
}