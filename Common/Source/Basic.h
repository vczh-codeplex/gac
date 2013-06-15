/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Framework::Basic

Classes:
	NotCopyable									�����ɸ��ƶ���
	Error										���ڲ����󣬼�鵽�˲��ɳ��ֵı�����������״̬����
	Object										���������

Macros:
	CHECK_ERROR(CONDITION,DESCRIPTION)			������ڲ�����
***********************************************************************/

#ifndef VCZH_BASIC
#define VCZH_BASIC

namespace vl
{

/***********************************************************************
32λ/64λ����
***********************************************************************/

#ifdef _WIN64
#define VCZH_64
#endif

#ifdef VCZH_64
	typedef __int64					vint;
	typedef signed __int64			vsint;
	typedef unsigned __int64		vuint;
#else
	typedef __int32					vint;
	typedef signed __int32			vsint;
	typedef unsigned __int32		vuint;
#endif

typedef signed __int64	pos_t;

#ifdef VCZH_64
#define ITOA_S		_i64toa_s
#define ITOW_S		_i64tow_s
#define I64TOA_S	_i64toa_s
#define I64TOW_S	_i64tow_s
#define UITOA_S		_ui64toa_s
#define UITOW_S		_ui64tow_s
#define UI64TOA_S	_ui64toa_s
#define UI64TOW_S	_ui64tow_s
#else
#define ITOA_S		_itoa_s
#define ITOW_S		_itow_s
#define I64TOA_S	_i64toa_s
#define I64TOW_S	_i64tow_s
#define UITOA_S		_ui64toa_s
#define UITOW_S		_ui64tow_s
#define UI64TOA_S	_ui64toa_s
#define UI64TOW_S	_ui64tow_s
#endif

#ifndef _MSC_VER
#define override
#define abstract
#endif

/***********************************************************************
����
***********************************************************************/

	class NotCopyable
	{
	private:
		NotCopyable(const NotCopyable&);
		NotCopyable& operator=(const NotCopyable&);
	public:
		NotCopyable();
	};

	class Error
	{
	private:
		wchar_t*			description;
	public:
		Error(wchar_t* _description);

		wchar_t*			Description()const;
	};

#ifdef _DEBUG
	#define CHECK_ERROR(CONDITION,DESCRIPTION) do{if(!(CONDITION))throw Error(DESCRIPTION);}while(0)
#endif
#ifdef NDEBUG
	#define CHECK_ERROR(CONDITION,DESCRIPTION)
#endif

#define CHECK_FAIL(DESCRIPTION) do{throw Error(DESCRIPTION);}while(0)

	class Object
	{
	public:
		virtual ~Object();
	};

	template<typename T>
	class ObjectBox : public Object
	{
	private:
		T					object;
	public:
		ObjectBox(const T& _object)
		{
			object=_object;
		}

		const T& Unbox()
		{
			return object;
		}
	};

	template<typename T>
	class Nullable
	{
	private:
		T*					object;
	public:
		Nullable()
			:object(0)
		{
		}

		Nullable(const T& value)
			:object(new T(value))
		{
		}

		Nullable(const Nullable<T>& nullable)
			:object(nullable.object?new T(*nullable.object):0)
		{
		}

		Nullable(Nullable<T>&& nullable)
			:object(nullable.object)
		{
			nullable.object=0;
		}

		~Nullable()
		{
			if(object)
			{
				delete object;
				object=0;
			}
		}

		Nullable<T>& operator=(const T& value)
		{
			if(object)
			{
				delete object;
				object=0;
			}
			object=new T(value);
			return *this;
		}

		Nullable<T>& operator=(const Nullable<T>& nullable)
		{
			if(object)
			{
				delete object;
				object=0;
			}
			if(nullable.object)
			{
				object=new T(*nullable.object);
			}
			return *this;
		}

		Nullable<T>& operator=(Nullable<T>&& nullable)
		{
			if(object)
			{
				delete object;
				object=0;
			}
			object=nullable.object;
			nullable.object=0;
			return *this;
		}

		static bool Equals(const Nullable<T>& a, const Nullable<T>& b)
		{
			return
				a.object
				?b.object
					?*a.object==*b.object
					:false
				:b.object
					?false
					:true;
		}

		static vint Compare(const Nullable<T>& a, const Nullable<T>& b)
		{
			return
				a.object
				?b.object
					?(*a.object==*b.object?0:*a.object<*b.object?-1:1)
					:1
				:b.object
					?-1
					:0;
		}

		bool operator==(const Nullable<T>& nullable)const
		{
			return Equals(*this, nullable);
		}

		bool operator!=(const Nullable<T>& nullable)const
		{
			return !Equals(*this, nullable);
		}

		bool operator<(const Nullable<T>& nullable)const
		{
			return Compare(*this, nullable)<0;
		}

		bool operator<=(const Nullable<T>& nullable)const
		{
			return Compare(*this, nullable)<=0;
		}

		bool operator>(const Nullable<T>& nullable)const
		{
			return Compare(*this, nullable)>0;
		}

		bool operator>=(const Nullable<T>& nullable)const
		{
			return Compare(*this, nullable)>=0;
		}

		operator bool()const
		{
			return object!=0;
		}

		const T& Value()const
		{
			return *object;
		}
	};

	template<typename T, size_t minSize>
	union BinaryRetriver
	{
		T t;
		char binary[sizeof(T)>minSize?sizeof(T):minSize];
	};

/***********************************************************************
����
***********************************************************************/

	template<typename T>
	struct KeyType
	{
	public:
		typedef T Type;

		static T GetKeyValue(const T& value)
		{
			return value;
		}
	};

	template<typename T>
	struct POD
	{
		static const bool Result=false;
	};

	template<>struct POD<bool>{static const bool Result=true;};
	template<>struct POD<unsigned __int8>{static const bool Result=true;};
	template<>struct POD<signed __int8>{static const bool Result=true;};
	template<>struct POD<unsigned __int16>{static const bool Result=true;};
	template<>struct POD<signed __int16>{static const bool Result=true;};
	template<>struct POD<unsigned __int32>{static const bool Result=true;};
	template<>struct POD<signed __int32>{static const bool Result=true;};
	template<>struct POD<unsigned __int64>{static const bool Result=true;};
	template<>struct POD<signed __int64>{static const bool Result=true;};
	template<>struct POD<char>{static const bool Result=true;};
	template<>struct POD<wchar_t>{static const bool Result=true;};
	template<typename T>struct POD<T*>{static const bool Result=true;};
	template<typename T>struct POD<T&>{static const bool Result=true;};
	template<typename T, typename C>struct POD<T C::*>{static const bool Result=true;};
	template<typename T, vint _Size>struct POD<T[_Size]>{static const bool Result=POD<T>::Result;};
	template<typename T>struct POD<const T>{static const bool Result=POD<T>::Result;};
	template<typename T>struct POD<volatile T>{static const bool Result=POD<T>::Result;};
	template<typename T>struct POD<const volatile T>{static const bool Result=POD<T>::Result;};

/***********************************************************************
ʱ��
***********************************************************************/

	struct DateTime
	{
		vint				year;
		vint				month;
		vint				dayOfWeek;
		vint				day;
		vint				hour;
		vint				minute;
		vint				second;
		vint				milliseconds;

		unsigned __int64	totalMilliseconds;
		unsigned __int64	filetime;

		static DateTime		LocalTime();
		static DateTime		UtcTime();
		static DateTime		FromDateTime(vint _year, vint _month, vint _day, vint _hour=0, vint _minute=0, vint _second=0, vint _milliseconds=0);
		static DateTime		FromFileTime(unsigned __int64 filetime);

		DateTime();

		DateTime			ToLocalTime();
		DateTime			ToUtcTime();
		DateTime			Forward(unsigned __int64 milliseconds);
		DateTime			Backward(unsigned __int64 milliseconds);

		bool operator==(const DateTime& value)const { return filetime==value.filetime; }
		bool operator!=(const DateTime& value)const { return filetime!=value.filetime; }
		bool operator<(const DateTime& value)const { return filetime<value.filetime; }
		bool operator<=(const DateTime& value)const { return filetime<=value.filetime; }
		bool operator>(const DateTime& value)const { return filetime>value.filetime; }
		bool operator>=(const DateTime& value)const { return filetime>=value.filetime; }
	};

/***********************************************************************
�ӿ�
***********************************************************************/

	class Interface : private NotCopyable
	{
	public:
		virtual ~Interface();
	};
}

#endif