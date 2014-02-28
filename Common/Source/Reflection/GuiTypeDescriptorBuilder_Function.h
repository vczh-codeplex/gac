/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Reflection
	
Interfaces:
***********************************************************************/
 
#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER_FUNCTION
#define VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER_FUNCTION
 
#include "GuiTypeDescriptorBuilder.h"
 
namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
DetailTypeInfoRetriver<Func<R(TArgs...)>>
***********************************************************************/

			namespace internal_helper
			{
				template<typename T>
				struct GenericArgumentAdder
				{
					static void Add(Ptr<TypeInfoImpl> genericType)
					{
					}
				};

				template<typename T0, typename ...TNextArgs>
				struct GenericArgumentAdder<TypeTuple<T0, TNextArgs...>>
				{
					static void Add(Ptr<TypeInfoImpl> genericType)
					{
						genericType->AddGenericArgument(TypeInfoRetriver<T0>::CreateTypeInfo());
						GenericArgumentAdder<TypeTuple<TNextArgs...>>::Add(genericType);
					}
				};
			}

			template<typename R, typename ...TArgs>
			struct DetailTypeInfoRetriver<Func<R(TArgs...)>, TypeFlags::FunctionType>
			{
				typedef DetailTypeInfoRetriver<Func<R(TArgs...)>, TypeFlags::NonGenericType>	UpLevelRetriver;
 
				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef IValueList												Type;
				typedef typename UpLevelRetriver::TempValueType					TempValueType;
				typedef typename UpLevelRetriver::ResultReferenceType			ResultReferenceType;
				typedef typename UpLevelRetriver::ResultNonReferenceType		ResultNonReferenceType;
 
				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					Ptr<TypeInfoImpl> functionType=new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					functionType->SetTypeDescriptor(Description<IValueFunctionProxy>::GetAssociatedTypeDescriptor());
 
					Ptr<TypeInfoImpl> genericType=new TypeInfoImpl(ITypeInfo::Generic);
					genericType->SetElementType(functionType);
					genericType->AddGenericArgument(TypeInfoRetriver<R>::CreateTypeInfo());
					internal_helper::GenericArgumentAdder<TypeTuple<TArgs...>>::Add(genericType);
 
					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::SharedPtr);
					type->SetElementType(genericType);
					return type;
				}
			};
 
/***********************************************************************
ValueFunctionProxyWrapper<Func<R(TArgs...)>>
***********************************************************************/

			template<typename T>
			class ValueFunctionProxyWrapper
			{
			};

			namespace internal_helper
			{
				extern void UnboxSpecifiedParameter(Ptr<IValueList> arguments, vint index);

				template<typename T0, typename ...TArgs>
				void UnboxSpecifiedParameter(Ptr<IValueList> arguments, vint index, T0& p0, TArgs& ...args)
				{
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments->Get(index), p0, 0, L"nth-argument");
					UnboxSpecifiedParameter(arguments, index + 1, args...);
				}

				template<typename R, typename ...TArgs>
				struct BoxedFunctionInvoker
				{
					static Value Invoke(const Func<R(TArgs...)>& function, Ptr<IValueList> arguments, typename RemoveCVR<TArgs>::Type&& ...args)
					{
						UnboxSpecifiedParameter(arguments, 0, args...);
						R result = function(args...);
						return BoxParameter<R>(result);
					}
				};

				template<typename ...TArgs>
				struct BoxedFunctionInvoker<void, TArgs...>
				{
					static Value Invoke(const Func<void(TArgs...)>& function, Ptr<IValueList> arguments, typename RemoveCVR<TArgs>::Type&& ...args)
					{
						UnboxSpecifiedParameter(arguments, 0, args...);
						function(args...);
						return Value();
					}
				};
			}

			template<typename R, typename ...TArgs>
			class ValueFunctionProxyWrapper<R(TArgs...)> : public Object, public virtual IValueFunctionProxy
			{
				typedef Func<R(TArgs...)>					FunctionType;
			protected:
				FunctionType			function;

			public:
				ValueFunctionProxyWrapper(const FunctionType& _function)
					:function(_function)
				{
				}
 
				FunctionType GetFunction()
				{
					return function;
				}
 
				Value Invoke(Ptr<IValueList> arguments)override
				{
					if(!arguments || arguments->GetCount()!=sizeof...(TArgs)) throw ArgumentCountMismtatchException();
					return internal_helper::BoxedFunctionInvoker<R, TArgs...>::Invoke(function, arguments, typename RemoveCVR<TArgs>::Type()...);
				}
			};
 
/***********************************************************************
ParameterAccessor<Func<R(TArgs...)>>
***********************************************************************/

			namespace internal_helper
			{
				extern void AddValueToList(Ptr<IValueList> arguments);

				template<typename T0, typename ...TArgs>
				void AddValueToList(Ptr<IValueList> arguments, T0&& p0, TArgs&& ...args)
				{
					arguments->Add(description::BoxParameter<T0>(p0));
					AddValueToList(arguments, args...);
				}
			}
 
			template<typename R, typename ...TArgs>
			struct ParameterAccessor<Func<R(TArgs...)>, TypeFlags::FunctionType>
			{
				static Value BoxParameter(const Func<R(TArgs...)>& object, ITypeDescriptor* typeDescriptor)
				{
					typedef R(RawFunctionType)(TArgs...);
					Ptr<IValueFunctionProxy> result=new ValueFunctionProxyWrapper<RawFunctionType>(object);
					return BoxValue<Ptr<IValueFunctionProxy>>(result, Description<IValueFunctionProxy>::GetAssociatedTypeDescriptor());
				}
 
				static void UnboxParameter(const Value& value, Func<R(TArgs...)>& result, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					typedef R(RawFunctionType)(TArgs...);
					typedef ValueFunctionProxyWrapper<RawFunctionType> ProxyType;
					Ptr<IValueFunctionProxy> functionProxy=UnboxValue<Ptr<IValueFunctionProxy>>(value, typeDescriptor, valueName);
					if(functionProxy)
					{
						if(Ptr<ProxyType> proxy=functionProxy.Cast<ProxyType>())
						{
							result=proxy->GetFunction();
						}
						else
						{
							result=[functionProxy](TArgs ...args)
							{
								Ptr<IValueList> arguments=IValueList::Create();
								internal_helper::AddValueToList(arguments, ForwardValue<TArgs>(args)...);
								TypeInfoRetriver<R>::TempValueType proxyResult;description::UnboxParameter<R>(functionProxy->Invoke(arguments),proxyResult);return proxyResult;
							};
						}
					}
				}
			};
 
/***********************************************************************
CustomConstructorInfoImpl<R(TArgs...)>
***********************************************************************/
 
			template<typename T>
			class CustomConstructorInfoImpl{};

			namespace internal_helper
			{
				extern void UnboxSpecifiedParameter(MethodInfoImpl* methodInfo, collections::Array<Value>& arguments, vint index);

				template<typename T0, typename ...TArgs>
				void UnboxSpecifiedParameter(MethodInfoImpl* methodInfo, collections::Array<Value>& arguments, vint index, T0& p0, TArgs& ...args)
				{
					UnboxParameter<typename TypeInfoRetriver<T0>::TempValueType>(arguments[index], p0, methodInfo->GetParameter(index)->GetType()->GetTypeDescriptor(), L"nth-argument");
					UnboxSpecifiedParameter(methodInfo, arguments, index + 1, args...);
				}

				template<typename R, typename ...TArgs>
				static Value InvokeBoxedConstructor(MethodInfoImpl* methodInfo, collections::Array<Value>& arguments, typename RemoveCVR<TArgs>::Type&& ...args)
				{
					UnboxSpecifiedParameter(methodInfo, arguments, 0, args...);
					R result = new typename TypeInfoRetriver<R>::Type(args...);
					return BoxParameter<R>(result);
				}

				template<typename T>
				struct ConstructorArgumentAdder
				{
					static void Add(MethodInfoImpl* methodInfo, const wchar_t* parameterNames[], vint index)
					{
					}
				};

				template<typename T0, typename ...TNextArgs>
				struct ConstructorArgumentAdder<TypeTuple<T0, TNextArgs...>>
				{
					static void Add(MethodInfoImpl* methodInfo, const wchar_t* parameterNames[], vint index)
					{
						methodInfo->AddParameter(new ParameterInfoImpl(methodInfo, parameterNames[index], TypeInfoRetriver<T0>::CreateTypeInfo()));
						ConstructorArgumentAdder<TypeTuple<TNextArgs...>>::Add(methodInfo, parameterNames, index + 1);
					}
				};
			}

			template<typename R, typename ...TArgs>
			class CustomConstructorInfoImpl<R(TArgs...)> : public MethodInfoImpl
			{
			protected:
				Value InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)override
				{
					return internal_helper::InvokeBoxedConstructor<R, TArgs...>(this, arguments, typename RemoveCVR<TArgs>::Type()...);
				}
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(TArgs...)> proxy(
						LAMBDA([](TArgs ...args)->R
						{
							R result = new TypeInfoRetriver<R>::Type(args...);
							return result;
						})
					);
					return BoxParameter<Func<R(TArgs...)>>(proxy);
				}
			public:
				CustomConstructorInfoImpl(const wchar_t* parameterNames[])
					:MethodInfoImpl(0, TypeInfoRetriver<R>::CreateTypeInfo(), true)
				{
					internal_helper::ConstructorArgumentAdder<TypeTuple<TArgs...>>::Add(this, parameterNames, 0);
				}
			};

			template<typename TClass, typename T>
			struct CustomMethodInfoImplSelector{};
 
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void()> proxy(object, method);
					return BoxParameter<Func<void()>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void()> proxy(
						LAMBDA([object, this]()->void
						{
							  method(object);
							 
						})
					);
					return BoxParameter<Func<void()>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R()> proxy(object, method);
					return BoxParameter<Func<R()>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R()> proxy(
						LAMBDA([object, this]()->R
						{
							R result =  method(object);
							return result;
						})
					);
					return BoxParameter<Func<R()>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0)> proxy(object, method);
					return BoxParameter<Func<void(T0)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0)> proxy(
						LAMBDA([object, this](T0 p0)->void
						{
							  method(object, p0);
							 
						})
					);
					return BoxParameter<Func<void(T0)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0)> proxy(object, method);
					return BoxParameter<Func<R(T0)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0)> proxy(
						LAMBDA([object, this](T0 p0)->R
						{
							R result =  method(object, p0);
							return result;
						})
					);
					return BoxParameter<Func<R(T0)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1)> proxy(object, method);
					return BoxParameter<Func<void(T0,T1)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1)->void
						{
							  method(object, p0, p1);
							 
						})
					);
					return BoxParameter<Func<void(T0,T1)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1)> proxy(object, method);
					return BoxParameter<Func<R(T0,T1)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1)->R
						{
							R result =  method(object, p0, p1);
							return result;
						})
					);
					return BoxParameter<Func<R(T0,T1)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2)> proxy(object, method);
					return BoxParameter<Func<void(T0,T1,T2)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2)->void
						{
							  method(object, p0, p1, p2);
							 
						})
					);
					return BoxParameter<Func<void(T0,T1,T2)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2)> proxy(object, method);
					return BoxParameter<Func<R(T0,T1,T2)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2)->R
						{
							R result =  method(object, p0, p1, p2);
							return result;
						})
					);
					return BoxParameter<Func<R(T0,T1,T2)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3)> proxy(object, method);
					return BoxParameter<Func<void(T0,T1,T2,T3)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3)->void
						{
							  method(object, p0, p1, p2, p3);
							 
						})
					);
					return BoxParameter<Func<void(T0,T1,T2,T3)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3)> proxy(object, method);
					return BoxParameter<Func<R(T0,T1,T2,T3)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3)->R
						{
							R result =  method(object, p0, p1, p2, p3);
							return result;
						})
					);
					return BoxParameter<Func<R(T0,T1,T2,T3)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4)> proxy(object, method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)->void
						{
							  method(object, p0, p1, p2, p3, p4);
							 
						})
					);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4)> proxy(object, method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)->R
						{
							R result =  method(object, p0, p1, p2, p3, p4);
							return result;
						})
					);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5)> proxy(object, method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)->void
						{
							  method(object, p0, p1, p2, p3, p4, p5);
							 
						})
					);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5)> proxy(object, method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)->R
						{
							R result =  method(object, p0, p1, p2, p3, p4, p5);
							return result;
						})
					);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5,T6)> proxy(object, method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5,T6)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)->void
						{
							  method(object, p0, p1, p2, p3, p4, p5, p6);
							 
						})
					);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5,T6)> proxy(object, method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5,T6)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)->R
						{
							R result =  method(object, p0, p1, p2, p3, p4, p5, p6);
							return result;
						})
					);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5,T6,T7)> proxy(object, method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5,T6,T7)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)->void
						{
							  method(object, p0, p1, p2, p3, p4, p5, p6, p7);
							 
						})
					);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5,T6,T7)> proxy(object, method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5,T6,T7)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)->R
						{
							R result =  method(object, p0, p1, p2, p3, p4, p5, p6, p7);
							return result;
						})
					);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)> proxy(object, method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)->void
						{
							  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8);
							 
						})
					);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)> proxy(object, method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)->R
						{
							R result =  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8);
							return result;
						})
					);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> proxy(object, method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)->void
						{
							  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
							 
						})
					);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> proxy(object, method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
					Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> proxy(
						LAMBDA([object, this](T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)->R
						{
							R result =  method(object, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
							return result;
						})
					);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void()> proxy(method);
					return BoxParameter<Func<void()>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R()> proxy(method);
					return BoxParameter<Func<R()>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0)> proxy(method);
					return BoxParameter<Func<void(T0)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0)> proxy(method);
					return BoxParameter<Func<R(T0)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0,T1)> proxy(method);
					return BoxParameter<Func<void(T0,T1)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0,T1)> proxy(method);
					return BoxParameter<Func<R(T0,T1)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0,T1,T2)> proxy(method);
					return BoxParameter<Func<void(T0,T1,T2)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0,T1,T2)> proxy(method);
					return BoxParameter<Func<R(T0,T1,T2)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0,T1,T2,T3)> proxy(method);
					return BoxParameter<Func<void(T0,T1,T2,T3)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0,T1,T2,T3)> proxy(method);
					return BoxParameter<Func<R(T0,T1,T2,T3)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0,T1,T2,T3,T4)> proxy(method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0,T1,T2,T3,T4)> proxy(method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0,T1,T2,T3,T4,T5)> proxy(method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0,T1,T2,T3,T4,T5)> proxy(method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0,T1,T2,T3,T4,T5,T6)> proxy(method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0,T1,T2,T3,T4,T5,T6)> proxy(method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0,T1,T2,T3,T4,T5,T6,T7)> proxy(method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0,T1,T2,T3,T4,T5,T6,T7)> proxy(method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)> proxy(method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)> proxy(method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> proxy(method);
					return BoxParameter<Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>>(proxy);
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
 
				Value CreateFunctionProxyInternal(const Value& thisObject)override
				{
					Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> proxy(method);
					return BoxParameter<Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>>(proxy);
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