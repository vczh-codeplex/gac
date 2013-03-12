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

					R result = new typename TypeInfoRetriver<R>::Type();
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0,p1);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0,p1,p2);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0,p1,p2,p3);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0,p1,p2,p3,p4);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0,p1,p2,p3,p4,p5);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0,p1,p2,p3,p4,p5,p6);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0,p1,p2,p3,p4,p5,p6,p7);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 					typename TypeInfoRetriver<T9>::TempValueType p9;
					UnboxParameter<typename TypeInfoRetriver<T9>::TempValueType>(arguments[9], p9, GetParameter(9)->GetType()->GetTypeDescriptor(), L"p9");
 
					R result = new typename TypeInfoRetriver<R>::Type(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return BoxParameter<R>(result, GetOwnerTypeDescriptor());
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], TypeInfoRetriver<T9>::CreateTypeInfo()));
 
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
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
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
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
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
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)())
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
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
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 
					  (object->*method)(p0);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 
					  method(object, p0);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 
					R result =  (object->*method)(p0);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 
					R result =  method(object, p0);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 
					  (object->*method)(p0,p1);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 
					  method(object, p0, p1);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 
					R result =  (object->*method)(p0,p1);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 
					R result =  method(object, p0, p1);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 
					  (object->*method)(p0,p1,p2);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 
					  method(object, p0, p1, p2);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 
					R result =  (object->*method)(p0,p1,p2);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 
					R result =  method(object, p0, p1, p2);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 
					  (object->*method)(p0,p1,p2,p3);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 
					  method(object, p0, p1, p2, p3);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 
					R result =  (object->*method)(p0,p1,p2,p3);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 
					R result =  method(object, p0, p1, p2, p3);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 
					  (object->*method)(p0,p1,p2,p3,p4);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 
					  method(object, p0, p1, p2, p3, p4);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 
					R result =  method(object, p0, p1, p2, p3, p4);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 
					  method(object, p0, p1, p2, p3, p4, p5);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5,p6);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 
					  method(object, p0, p1, p2, p3, p4, p5, p6);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5,p6);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5, p6);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 
					  method(object, p0, p1, p2, p3, p4, p5, p6, p7);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5, p6, p7);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 
					  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 					typename TypeInfoRetriver<T9>::TempValueType p9;
					UnboxParameter<typename TypeInfoRetriver<T9>::TempValueType>(arguments[9], p9, GetParameter(9)->GetType()->GetTypeDescriptor(), L"p9");
 
					  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], TypeInfoRetriver<T9>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				void(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 					typename TypeInfoRetriver<T9>::TempValueType p9;
					UnboxParameter<typename TypeInfoRetriver<T9>::TempValueType>(arguments[9], p9, GetParameter(9)->GetType()->GetTypeDescriptor(), L"p9");
 
					  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
					return Value();
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], void(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], TypeInfoRetriver<T9>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 					typename TypeInfoRetriver<T9>::TempValueType p9;
					UnboxParameter<typename TypeInfoRetriver<T9>::TempValueType>(arguments[9], p9, GetParameter(9)->GetType()->GetTypeDescriptor(), L"p9");
 
					R result =  (object->*method)(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(__thiscall TClass::* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], TypeInfoRetriver<T9>::CreateTypeInfo()));
 
				}
			};
 
			class ExternalMethodInfoImpl : public MethodInfoImpl
			{
			protected:
				R(*method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
 
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 					typename TypeInfoRetriver<T9>::TempValueType p9;
					UnboxParameter<typename TypeInfoRetriver<T9>::TempValueType>(arguments[9], p9, GetParameter(9)->GetType()->GetTypeDescriptor(), L"p9");
 
					R result =  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				ExternalMethodInfoImpl(const wchar_t* parameterNames[], R(*_method)(TClass*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), false)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], TypeInfoRetriver<T9>::CreateTypeInfo()));
 
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
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
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
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)())
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 
					  method(p0);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 
					R result =  method(p0);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 
					  method(p0,p1);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 
					R result =  method(p0,p1);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 
					  method(p0,p1,p2);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 
					R result =  method(p0,p1,p2);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 
					  method(p0,p1,p2,p3);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 
					R result =  method(p0,p1,p2,p3);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 
					  method(p0,p1,p2,p3,p4);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 
					R result =  method(p0,p1,p2,p3,p4);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 
					  method(p0,p1,p2,p3,p4,p5);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 
					R result =  method(p0,p1,p2,p3,p4,p5);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 
					  method(p0,p1,p2,p3,p4,p5,p6);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5,T6))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 
					R result =  method(p0,p1,p2,p3,p4,p5,p6);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5,T6))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 
					  method(p0,p1,p2,p3,p4,p5,p6,p7);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5,T6,T7))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 
					R result =  method(p0,p1,p2,p3,p4,p5,p6,p7);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5,T6,T7))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 
					  method(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 
					R result =  method(p0,p1,p2,p3,p4,p5,p6,p7,p8);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 					typename TypeInfoRetriver<T9>::TempValueType p9;
					UnboxParameter<typename TypeInfoRetriver<T9>::TempValueType>(arguments[9], p9, GetParameter(9)->GetType()->GetTypeDescriptor(), L"p9");
 
					  method(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return Value();
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], void(* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
					:MethodInfoImpl(0, TypeInfoRetriver<void>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], TypeInfoRetriver<T9>::CreateTypeInfo()));
 
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
					typename TypeInfoRetriver<T0>::TempValueType p0;
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[0], p0, GetParameter(0)->GetType()->GetTypeDescriptor(), L"p0");
 					typename TypeInfoRetriver<T1>::TempValueType p1;
					UnboxParameter<typename TypeInfoRetriver<T1>::TempValueType>(arguments[1], p1, GetParameter(1)->GetType()->GetTypeDescriptor(), L"p1");
 					typename TypeInfoRetriver<T2>::TempValueType p2;
					UnboxParameter<typename TypeInfoRetriver<T2>::TempValueType>(arguments[2], p2, GetParameter(2)->GetType()->GetTypeDescriptor(), L"p2");
 					typename TypeInfoRetriver<T3>::TempValueType p3;
					UnboxParameter<typename TypeInfoRetriver<T3>::TempValueType>(arguments[3], p3, GetParameter(3)->GetType()->GetTypeDescriptor(), L"p3");
 					typename TypeInfoRetriver<T4>::TempValueType p4;
					UnboxParameter<typename TypeInfoRetriver<T4>::TempValueType>(arguments[4], p4, GetParameter(4)->GetType()->GetTypeDescriptor(), L"p4");
 					typename TypeInfoRetriver<T5>::TempValueType p5;
					UnboxParameter<typename TypeInfoRetriver<T5>::TempValueType>(arguments[5], p5, GetParameter(5)->GetType()->GetTypeDescriptor(), L"p5");
 					typename TypeInfoRetriver<T6>::TempValueType p6;
					UnboxParameter<typename TypeInfoRetriver<T6>::TempValueType>(arguments[6], p6, GetParameter(6)->GetType()->GetTypeDescriptor(), L"p6");
 					typename TypeInfoRetriver<T7>::TempValueType p7;
					UnboxParameter<typename TypeInfoRetriver<T7>::TempValueType>(arguments[7], p7, GetParameter(7)->GetType()->GetTypeDescriptor(), L"p7");
 					typename TypeInfoRetriver<T8>::TempValueType p8;
					UnboxParameter<typename TypeInfoRetriver<T8>::TempValueType>(arguments[8], p8, GetParameter(8)->GetType()->GetTypeDescriptor(), L"p8");
 					typename TypeInfoRetriver<T9>::TempValueType p9;
					UnboxParameter<typename TypeInfoRetriver<T9>::TempValueType>(arguments[9], p9, GetParameter(9)->GetType()->GetTypeDescriptor(), L"p9");
 
					R result =  method(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
					return BoxParameter<R>(result, GetReturn()->GetTypeDescriptor());
				}
			public:
				CustomMethodInfoImpl(const wchar_t* parameterNames[], R(* _method)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
					,method(_method)
				{
					AddParameter(new ParameterInfoImpl(this, parameterNames[0], TypeInfoRetriver<T0>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[1], TypeInfoRetriver<T1>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[2], TypeInfoRetriver<T2>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[3], TypeInfoRetriver<T3>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[4], TypeInfoRetriver<T4>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[5], TypeInfoRetriver<T5>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[6], TypeInfoRetriver<T6>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[7], TypeInfoRetriver<T7>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[8], TypeInfoRetriver<T8>::CreateTypeInfo()));
 					AddParameter(new ParameterInfoImpl(this, parameterNames[9], TypeInfoRetriver<T9>::CreateTypeInfo()));
 
				}
			};
			};
 
		}
	}
}
 
#endif