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
			template<typename T>
			class CustomMethodInfoImpl{};
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					T7 p7 = UnboxValue<T7>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					T7 p7 = UnboxValue<T7>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					T8 p8 = UnboxValue<T8>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
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
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					T7 p7 = UnboxValue<T7>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					T8 p8 = UnboxValue<T8>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					T9 p9 = UnboxValue<T9>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
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
Method: void()
***********************************************************************/
			template< >
			class CustomMethodInfoImpl<void()> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{

					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
				{

				}
			};
/***********************************************************************
Method: R()
***********************************************************************/
			template<typename R >
			class CustomMethodInfoImpl<R()> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{

					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
				{

				}
			};
  
/***********************************************************************
Method: void(T0)
***********************************************************************/
			template< typename T0>
			class CustomMethodInfoImpl<void(T0)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 
				}
			};
/***********************************************************************
Method: R(T0)
***********************************************************************/
			template<typename R,typename T0>
			class CustomMethodInfoImpl<R(T0)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 
				}
			};
  
/***********************************************************************
Method: void(T0,T1)
***********************************************************************/
			template< typename T0,typename T1>
			class CustomMethodInfoImpl<void(T0,T1)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 
				}
			};
/***********************************************************************
Method: R(T0,T1)
***********************************************************************/
			template<typename R,typename T0,typename T1>
			class CustomMethodInfoImpl<R(T0,T1)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 
				}
			};
  
/***********************************************************************
Method: void(T0,T1,T2)
***********************************************************************/
			template< typename T0,typename T1,typename T2>
			class CustomMethodInfoImpl<void(T0,T1,T2)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 
				}
			};
/***********************************************************************
Method: R(T0,T1,T2)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2>
			class CustomMethodInfoImpl<R(T0,T1,T2)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 
				}
			};
  
/***********************************************************************
Method: void(T0,T1,T2,T3)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3>
			class CustomMethodInfoImpl<void(T0,T1,T2,T3)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 
				}
			};
/***********************************************************************
Method: R(T0,T1,T2,T3)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3>
			class CustomMethodInfoImpl<R(T0,T1,T2,T3)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 
				}
			};
  
/***********************************************************************
Method: void(T0,T1,T2,T3,T4)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4>
			class CustomMethodInfoImpl<void(T0,T1,T2,T3,T4)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 
				}
			};
/***********************************************************************
Method: R(T0,T1,T2,T3,T4)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
			class CustomMethodInfoImpl<R(T0,T1,T2,T3,T4)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], GetTypeDescriptor<typename ParameterTypeInfo<T0>::Type>(), ParameterTypeInfo<T0>::Decorator, ParameterTypeInfo<T0>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], GetTypeDescriptor<typename ParameterTypeInfo<T1>::Type>(), ParameterTypeInfo<T1>::Decorator, ParameterTypeInfo<T1>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], GetTypeDescriptor<typename ParameterTypeInfo<T2>::Type>(), ParameterTypeInfo<T2>::Decorator, ParameterTypeInfo<T2>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], GetTypeDescriptor<typename ParameterTypeInfo<T3>::Type>(), ParameterTypeInfo<T3>::Decorator, ParameterTypeInfo<T3>::Output));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], GetTypeDescriptor<typename ParameterTypeInfo<T4>::Type>(), ParameterTypeInfo<T4>::Decorator, ParameterTypeInfo<T4>::Output));
 
				}
			};
  
/***********************************************************************
Method: void(T0,T1,T2,T3,T4,T5)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
			class CustomMethodInfoImpl<void(T0,T1,T2,T3,T4,T5)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
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
Method: R(T0,T1,T2,T3,T4,T5)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
			class CustomMethodInfoImpl<R(T0,T1,T2,T3,T4,T5)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
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
Method: void(T0,T1,T2,T3,T4,T5,T6)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
			class CustomMethodInfoImpl<void(T0,T1,T2,T3,T4,T5,T6)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
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
Method: R(T0,T1,T2,T3,T4,T5,T6)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
			class CustomMethodInfoImpl<R(T0,T1,T2,T3,T4,T5,T6)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
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
Method: void(T0,T1,T2,T3,T4,T5,T6,T7)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
			class CustomMethodInfoImpl<void(T0,T1,T2,T3,T4,T5,T6,T7)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					T7 p7 = UnboxValue<T7>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
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
Method: R(T0,T1,T2,T3,T4,T5,T6,T7)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
			class CustomMethodInfoImpl<R(T0,T1,T2,T3,T4,T5,T6,T7)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					T7 p7 = UnboxValue<T7>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
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
Method: void(T0,T1,T2,T3,T4,T5,T6,T7,T8)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
			class CustomMethodInfoImpl<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					T7 p7 = UnboxValue<T7>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					T8 p8 = UnboxValue<T8>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
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
Method: R(T0,T1,T2,T3,T4,T5,T6,T7,T8)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
			class CustomMethodInfoImpl<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					T7 p7 = UnboxValue<T7>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					T8 p8 = UnboxValue<T8>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
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
Method: void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)
***********************************************************************/
			template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
			class CustomMethodInfoImpl<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					T7 p7 = UnboxValue<T7>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					T8 p8 = UnboxValue<T8>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					T9 p9 = UnboxValue<T9>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, 0, IParameterInfo::Text, _isStatic)
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
Method: R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)
***********************************************************************/
			template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
			class CustomMethodInfoImpl<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					T0 p0 = UnboxValue<T0>(arguments[0], GetParameter(0)->GetValueTypeDescriptor(), L"p0");
 					T1 p1 = UnboxValue<T1>(arguments[1], GetParameter(1)->GetValueTypeDescriptor(), L"p1");
 					T2 p2 = UnboxValue<T2>(arguments[2], GetParameter(2)->GetValueTypeDescriptor(), L"p2");
 					T3 p3 = UnboxValue<T3>(arguments[3], GetParameter(3)->GetValueTypeDescriptor(), L"p3");
 					T4 p4 = UnboxValue<T4>(arguments[4], GetParameter(4)->GetValueTypeDescriptor(), L"p4");
 					T5 p5 = UnboxValue<T5>(arguments[5], GetParameter(5)->GetValueTypeDescriptor(), L"p5");
 					T6 p6 = UnboxValue<T6>(arguments[6], GetParameter(6)->GetValueTypeDescriptor(), L"p6");
 					T7 p7 = UnboxValue<T7>(arguments[7], GetParameter(7)->GetValueTypeDescriptor(), L"p7");
 					T8 p8 = UnboxValue<T8>(arguments[8], GetParameter(8)->GetValueTypeDescriptor(), L"p8");
 					T9 p9 = UnboxValue<T9>(arguments[9], GetParameter(9)->GetValueTypeDescriptor(), L"p9");
 
					return Value();
				}
			public:
				CustomMethodInfoImpl(bool _isStatic, const wchar_t* parameterNames[])
					:MethodInfoImpl(0, GetTypeDescriptor<typename ParameterTypeInfo<R>::Type>(), ParameterTypeInfo<R>::Decorator, _isStatic)
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
 
		}
	}
}
 
#endif