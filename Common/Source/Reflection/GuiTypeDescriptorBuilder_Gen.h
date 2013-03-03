/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::Reflection
	
本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/
 
#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER_GEN
#define VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER_GEN
 
#include "GuiTypeDescriptorBuilder.h"
 
namespace vl
{
	namespace reflection
	{
		namespace description
		{
			template<typename T>
			class CustomConstructorInfoImpl{};
			template<typename TClass, typename T>
			struct CustomMethodInfoImplSelector{};
 
/***********************************************************************
Constructor: R()
***********************************************************************/
			template<typename R >
			class CustomConstructorInfoImpl<R()> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{

					R result = new typename ParameterTypeInfo<R>::Type();
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{

				}
			};
  
/***********************************************************************
Constructor: R(T0)
***********************************************************************/
			template<typename R,typename T0>
			class CustomConstructorInfoImpl<R(T0)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 
				}
			};
  
/***********************************************************************
Constructor: R(T0,T1)
***********************************************************************/
			template<typename R,typename T0,typename T1>
			class CustomConstructorInfoImpl<R(T0,T1)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0,p1);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 
				}
			};
  
/***********************************************************************
Constructor: R(T0,T1,T2)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2>
			class CustomConstructorInfoImpl<R(T0,T1,T2)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0,p1,p2);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 
				}
			};
  
/***********************************************************************
Constructor: R(T0,T1,T2,T3)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3>
			class CustomConstructorInfoImpl<R(T0,T1,T2,T3)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0,p1,p2,p3);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 
				}
			};
  
/***********************************************************************
Constructor: R(T0,T1,T2,T3,T4)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
			class CustomConstructorInfoImpl<R(T0,T1,T2,T3,T4)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0,p1,p2,p3,p4);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 
				}
			};
  
/***********************************************************************
Constructor: R(T0,T1,T2,T3,T4,T5)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
			class CustomConstructorInfoImpl<R(T0,T1,T2,T3,T4,T5)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0,p1,p2,p3,p4,p5);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 
				}
			};
  
/***********************************************************************
Constructor: R(T0,T1,T2,T3,T4,T5,T6)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
			class CustomConstructorInfoImpl<R(T0,T1,T2,T3,T4,T5,T6)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0,p1,p2,p3,p4,p5,p6);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 
				}
			};
  
/***********************************************************************
Constructor: R(T0,T1,T2,T3,T4,T5,T6,T7)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
			class CustomConstructorInfoImpl<R(T0,T1,T2,T3,T4,T5,T6,T7)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0,p1,p2,p3,p4,p5,p6,p7);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 
				}
			};
  
/***********************************************************************
Constructor: R(T0,T1,T2,T3,T4,T5,T6,T7,T8)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
			class CustomConstructorInfoImpl<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 
				}
			};
  
/***********************************************************************
Constructor: R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
			class CustomConstructorInfoImpl<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					typename ParameterTypeInfo<T9>::TempValueType p9 = UnboxValue<typename ParameterTypeInfo<T9>::TempValueType>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
					R result = new typename ParameterTypeInfo<R>::Type(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return BoxValue<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], GetTypeDescriptor<typename ParameterTypeInfo<T9>::Type>(), ParameterTypeInfo<T9>::Decorator, ParameterTypeInfo<T9>::Output));
 
				}
			};
 
 
/***********************************************************************
Member Method: void()
***********************************************************************/
			template<typename TClass  >
			struct CustomMethodInfoImplSelector<TClass, void()>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)();
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");

					  (object->*method)();
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)())
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{

				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");

					  method(object);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{

				}
			};
			};
/***********************************************************************
Member Method: R()
***********************************************************************/
			template<typename TClass,typename R >
			struct CustomMethodInfoImplSelector<TClass, R()>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)();
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");

					R result =  (object->*method)();
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)())
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{

				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");

					R result =  method(object);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{

				}
			};
			};
  
/***********************************************************************
Member Method: void(T0)
***********************************************************************/
			template<typename TClass, typename T0>
			struct CustomMethodInfoImplSelector<TClass, void(T0)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
					  (object->*method)(p0);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
					  method(object, p0);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0)
***********************************************************************/
			template<typename TClass,typename R,typename T0>
			struct CustomMethodInfoImplSelector<TClass, R(T0)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
					R result =  (object->*method)(p0);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
					R result =  method(object, p0);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 
				}
			};
			};
  
/***********************************************************************
Member Method: void(T0,T1)
***********************************************************************/
			template<typename TClass, typename T0,typename T1>
			struct CustomMethodInfoImplSelector<TClass, void(T0,T1)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0,T1);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
					  (object->*method)(p0,p1);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
					  method(object, p0, p1);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0,T1)
***********************************************************************/
			template<typename TClass,typename R,typename T0,typename T1>
			struct CustomMethodInfoImplSelector<TClass, R(T0,T1)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0,T1);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
					R result =  (object->*method)(p0,p1);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
					R result =  method(object, p0, p1);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 
				}
			};
			};
  
/***********************************************************************
Member Method: void(T0,T1,T2)
***********************************************************************/
			template<typename TClass, typename T0,typename T1,typename T2>
			struct CustomMethodInfoImplSelector<TClass, void(T0,T1,T2)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0,T1,T2);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
					  (object->*method)(p0,p1,p2);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
					  method(object, p0, p1, p2);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0,T1,T2)
***********************************************************************/
			template<typename TClass,typename R,typename T0,typename T1,typename T2>
			struct CustomMethodInfoImplSelector<TClass, R(T0,T1,T2)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0,T1,T2);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
					R result =  (object->*method)(p0,p1,p2);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
					R result =  method(object, p0, p1, p2);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 
				}
			};
			};
  
/***********************************************************************
Member Method: void(T0,T1,T2,T3)
***********************************************************************/
			template<typename TClass, typename T0,typename T1,typename T2,typename T3>
			struct CustomMethodInfoImplSelector<TClass, void(T0,T1,T2,T3)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0,T1,T2,T3);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
					  (object->*method)(p0,p1,p2,p3);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
					  method(object, p0, p1, p2, p3);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0,T1,T2,T3)
***********************************************************************/
			template<typename TClass,typename R,typename T0,typename T1,typename T2,typename T3>
			struct CustomMethodInfoImplSelector<TClass, R(T0,T1,T2,T3)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0,T1,T2,T3);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
					R result =  (object->*method)(p0,p1,p2,p3);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
					R result =  method(object, p0, p1, p2, p3);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 
				}
			};
			};
  
/***********************************************************************
Member Method: void(T0,T1,T2,T3,T4)
***********************************************************************/
			template<typename TClass, typename T0,typename T1,typename T2,typename T3,typename T4>
			struct CustomMethodInfoImplSelector<TClass, void(T0,T1,T2,T3,T4)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0,T1,T2,T3,T4);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
					  (object->*method)(p0,p1,p2,p3,p4);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
					  method(object, p0, p1, p2, p3, p4);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0,T1,T2,T3,T4)
***********************************************************************/
			template<typename TClass,typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
			struct CustomMethodInfoImplSelector<TClass, R(T0,T1,T2,T3,T4)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0,T1,T2,T3,T4);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
					R result =  method(object, p0, p1, p2, p3, p4);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 
				}
			};
			};
  
/***********************************************************************
Member Method: void(T0,T1,T2,T3,T4,T5)
***********************************************************************/
			template<typename TClass, typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
			struct CustomMethodInfoImplSelector<TClass, void(T0,T1,T2,T3,T4,T5)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
					  method(object, p0, p1, p2, p3, p4, p5);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0,T1,T2,T3,T4,T5)
***********************************************************************/
			template<typename TClass,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
			struct CustomMethodInfoImplSelector<TClass, R(T0,T1,T2,T3,T4,T5)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 
				}
			};
			};
  
/***********************************************************************
Member Method: void(T0,T1,T2,T3,T4,T5,T6)
***********************************************************************/
			template<typename TClass, typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
			struct CustomMethodInfoImplSelector<TClass, void(T0,T1,T2,T3,T4,T5,T6)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5,T6);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5,p6);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
					  method(object, p0, p1, p2, p3, p4, p5, p6);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0,T1,T2,T3,T4,T5,T6)
***********************************************************************/
			template<typename TClass,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
			struct CustomMethodInfoImplSelector<TClass, R(T0,T1,T2,T3,T4,T5,T6)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5,T6);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5,p6);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5, p6);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 
				}
			};
			};
  
/***********************************************************************
Member Method: void(T0,T1,T2,T3,T4,T5,T6,T7)
***********************************************************************/
			template<typename TClass, typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
			struct CustomMethodInfoImplSelector<TClass, void(T0,T1,T2,T3,T4,T5,T6,T7)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5,T6,T7);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
					  method(object, p0, p1, p2, p3, p4, p5, p6, p7);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0,T1,T2,T3,T4,T5,T6,T7)
***********************************************************************/
			template<typename TClass,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
			struct CustomMethodInfoImplSelector<TClass, R(T0,T1,T2,T3,T4,T5,T6,T7)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5,T6,T7);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5, p6, p7);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 
				}
			};
			};
  
/***********************************************************************
Member Method: void(T0,T1,T2,T3,T4,T5,T6,T7,T8)
***********************************************************************/
			template<typename TClass, typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
			struct CustomMethodInfoImplSelector<TClass, void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5,T6,T7,T8);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
					  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0,T1,T2,T3,T4,T5,T6,T7,T8)
***********************************************************************/
			template<typename TClass,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
			struct CustomMethodInfoImplSelector<TClass, R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5,T6,T7,T8);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 
				}
			};
			};
  
/***********************************************************************
Member Method: void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)
***********************************************************************/
			template<typename TClass, typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
			struct CustomMethodInfoImplSelector<TClass, void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					typename ParameterTypeInfo<T9>::TempValueType p9 = UnboxValue<typename ParameterTypeInfo<T9>::TempValueType>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], GetTypeDescriptor<typename ParameterTypeInfo<T9>::Type>(), ParameterTypeInfo<T9>::Decorator, ParameterTypeInfo<T9>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					typename ParameterTypeInfo<T9>::TempValueType p9 = UnboxValue<typename ParameterTypeInfo<T9>::TempValueType>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
					  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], GetTypeDescriptor<typename ParameterTypeInfo<T9>::Type>(), ParameterTypeInfo<T9>::Decorator, ParameterTypeInfo<T9>::Output));
 
				}
			};
			};
/***********************************************************************
Member Method: R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)
***********************************************************************/
			template<typename TClass,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
			struct CustomMethodInfoImplSelector<TClass, R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(__thiscall TClass::* method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					typename ParameterTypeInfo<T9>::TempValueType p9 = UnboxValue<typename ParameterTypeInfo<T9>::TempValueType>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], GetTypeDescriptor<typename ParameterTypeInfo<T9>::Type>(), ParameterTypeInfo<T9>::Decorator, ParameterTypeInfo<T9>::Output));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					typename ParameterTypeInfo<T9>::TempValueType p9 = UnboxValue<typename ParameterTypeInfo<T9>::TempValueType>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], GetTypeDescriptor<typename ParameterTypeInfo<T9>::Type>(), ParameterTypeInfo<T9>::Decorator, ParameterTypeInfo<T9>::Output));
 
				}
			};
			};
 
 
/***********************************************************************
Static Method: void()
***********************************************************************/
			template< >
			struct CustomMethodInfoImplSelector<void, void()>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)();
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{

					  method();
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)())
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{

				}
			};
			};
/***********************************************************************
Static Method: R()
***********************************************************************/
			template<typename R >
			struct CustomMethodInfoImplSelector<void, R()>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)();
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{

					R result =  method();
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)())
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{

				}
			};
			};
  
/***********************************************************************
Static Method: void(T0)
***********************************************************************/
			template< typename T0>
			struct CustomMethodInfoImplSelector<void, void(T0)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
					  method(p0);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0)
***********************************************************************/
			template<typename R,typename T0>
			struct CustomMethodInfoImplSelector<void, R(T0)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
					R result =  method(p0);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 
				}
			};
			};
  
/***********************************************************************
Static Method: void(T0,T1)
***********************************************************************/
			template< typename T0,typename T1>
			struct CustomMethodInfoImplSelector<void, void(T0,T1)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0,T1);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
					  method(p0,p1);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0,T1)
***********************************************************************/
			template<typename R,typename T0,typename T1>
			struct CustomMethodInfoImplSelector<void, R(T0,T1)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0,T1);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
					R result =  method(p0,p1);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 
				}
			};
			};
  
/***********************************************************************
Static Method: void(T0,T1,T2)
***********************************************************************/
			template< typename T0,typename T1,typename T2>
			struct CustomMethodInfoImplSelector<void, void(T0,T1,T2)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0,T1,T2);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
					  method(p0,p1,p2);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0,T1,T2)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2>
			struct CustomMethodInfoImplSelector<void, R(T0,T1,T2)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0,T1,T2);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
					R result =  method(p0,p1,p2);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 
				}
			};
			};
  
/***********************************************************************
Static Method: void(T0,T1,T2,T3)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3>
			struct CustomMethodInfoImplSelector<void, void(T0,T1,T2,T3)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0,T1,T2,T3);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
					  method(p0,p1,p2,p3);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0,T1,T2,T3)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3>
			struct CustomMethodInfoImplSelector<void, R(T0,T1,T2,T3)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0,T1,T2,T3);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
					R result =  method(p0,p1,p2,p3);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 
				}
			};
			};
  
/***********************************************************************
Static Method: void(T0,T1,T2,T3,T4)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4>
			struct CustomMethodInfoImplSelector<void, void(T0,T1,T2,T3,T4)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0,T1,T2,T3,T4);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
					  method(p0,p1,p2,p3,p4);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0,T1,T2,T3,T4)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
			struct CustomMethodInfoImplSelector<void, R(T0,T1,T2,T3,T4)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0,T1,T2,T3,T4);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
					R result =  method(p0,p1,p2,p3,p4);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 
				}
			};
			};
  
/***********************************************************************
Static Method: void(T0,T1,T2,T3,T4,T5)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
			struct CustomMethodInfoImplSelector<void, void(T0,T1,T2,T3,T4,T5)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0,T1,T2,T3,T4,T5);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
					  method(p0,p1,p2,p3,p4,p5);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0,T1,T2,T3,T4,T5)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
			struct CustomMethodInfoImplSelector<void, R(T0,T1,T2,T3,T4,T5)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0,T1,T2,T3,T4,T5);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
					R result =  method(p0,p1,p2,p3,p4,p5);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 
				}
			};
			};
  
/***********************************************************************
Static Method: void(T0,T1,T2,T3,T4,T5,T6)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
			struct CustomMethodInfoImplSelector<void, void(T0,T1,T2,T3,T4,T5,T6)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0,T1,T2,T3,T4,T5,T6);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
					  method(p0,p1,p2,p3,p4,p5,p6);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5,T6))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0,T1,T2,T3,T4,T5,T6)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
			struct CustomMethodInfoImplSelector<void, R(T0,T1,T2,T3,T4,T5,T6)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0,T1,T2,T3,T4,T5,T6);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
					R result =  method(p0,p1,p2,p3,p4,p5,p6);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5,T6))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 
				}
			};
			};
  
/***********************************************************************
Static Method: void(T0,T1,T2,T3,T4,T5,T6,T7)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
			struct CustomMethodInfoImplSelector<void, void(T0,T1,T2,T3,T4,T5,T6,T7)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0,T1,T2,T3,T4,T5,T6,T7);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
					  method(p0,p1,p2,p3,p4,p5,p6,p7);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5,T6,T7))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0,T1,T2,T3,T4,T5,T6,T7)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
			struct CustomMethodInfoImplSelector<void, R(T0,T1,T2,T3,T4,T5,T6,T7)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0,T1,T2,T3,T4,T5,T6,T7);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
					R result =  method(p0,p1,p2,p3,p4,p5,p6,p7);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5,T6,T7))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 
				}
			};
			};
  
/***********************************************************************
Static Method: void(T0,T1,T2,T3,T4,T5,T6,T7,T8)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
			struct CustomMethodInfoImplSelector<void, void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0,T1,T2,T3,T4,T5,T6,T7,T8);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
					  method(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0,T1,T2,T3,T4,T5,T6,T7,T8)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
			struct CustomMethodInfoImplSelector<void, R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0,T1,T2,T3,T4,T5,T6,T7,T8);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
					R result =  method(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 
				}
			};
			};
  
/***********************************************************************
Static Method: void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
			struct CustomMethodInfoImplSelector<void, void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(* method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					typename ParameterTypeInfo<T9>::TempValueType p9 = UnboxValue<typename ParameterTypeInfo<T9>::TempValueType>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
					  method(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
					:MethodInfoImpl(0, 0, IParameterInfo::Text, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], GetTypeDescriptor<typename ParameterTypeInfo<T9>::Type>(), ParameterTypeInfo<T9>::Decorator, ParameterTypeInfo<T9>::Output));
 
				}
			};
			};
/***********************************************************************
Static Method: R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
			struct CustomMethodInfoImplSelector<void, R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
			{
			class CustomMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(* method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					typename ParameterTypeInfo<T0>::TempValueType p0 = UnboxValue<typename ParameterTypeInfo<T0>::TempValueType>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					typename ParameterTypeInfo<T1>::TempValueType p1 = UnboxValue<typename ParameterTypeInfo<T1>::TempValueType>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					typename ParameterTypeInfo<T2>::TempValueType p2 = UnboxValue<typename ParameterTypeInfo<T2>::TempValueType>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					typename ParameterTypeInfo<T3>::TempValueType p3 = UnboxValue<typename ParameterTypeInfo<T3>::TempValueType>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					typename ParameterTypeInfo<T4>::TempValueType p4 = UnboxValue<typename ParameterTypeInfo<T4>::TempValueType>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					typename ParameterTypeInfo<T5>::TempValueType p5 = UnboxValue<typename ParameterTypeInfo<T5>::TempValueType>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					typename ParameterTypeInfo<T6>::TempValueType p6 = UnboxValue<typename ParameterTypeInfo<T6>::TempValueType>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					typename ParameterTypeInfo<T7>::TempValueType p7 = UnboxValue<typename ParameterTypeInfo<T7>::TempValueType>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					typename ParameterTypeInfo<T8>::TempValueType p8 = UnboxValue<typename ParameterTypeInfo<T8>::TempValueType>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					typename ParameterTypeInfo<T9>::TempValueType p9 = UnboxValue<typename ParameterTypeInfo<T9>::TempValueType>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
					R result =  method(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return BoxValue(result, GetReturn()->GetValueTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], GetTypeDescriptor<typename ParameterTypeInfo<T5>::Type>(), ParameterTypeInfo<T5>::Decorator, ParameterTypeInfo<T5>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], GetTypeDescriptor<typename ParameterTypeInfo<T6>::Type>(), ParameterTypeInfo<T6>::Decorator, ParameterTypeInfo<T6>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], GetTypeDescriptor<typename ParameterTypeInfo<T7>::Type>(), ParameterTypeInfo<T7>::Decorator, ParameterTypeInfo<T7>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], GetTypeDescriptor<typename ParameterTypeInfo<T8>::Type>(), ParameterTypeInfo<T8>::Decorator, ParameterTypeInfo<T8>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], GetTypeDescriptor<typename ParameterTypeInfo<T9>::Type>(), ParameterTypeInfo<T9>::Decorator, ParameterTypeInfo<T9>::Output));
 
				}
			};
			};
 
		}
	}
}
 
#endif