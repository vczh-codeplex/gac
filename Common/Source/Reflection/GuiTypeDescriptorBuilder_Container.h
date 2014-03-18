/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Reflection
	
Interfaces:
***********************************************************************/
 
#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER_CONTAINER
#define VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER_CONTAINER
 
#include "GuiTypeDescriptorBuilder.h"
 
namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
DetailTypeInfoRetriver<TContainer>
***********************************************************************/

			template<typename T>
			struct DetailTypeInfoRetriver<T, TypeFlags::EnumerableType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef IValueEnumerable										Type;
				typedef typename UpLevelRetriver::TempValueType					TempValueType;
				typedef typename UpLevelRetriver::ResultReferenceType			ResultReferenceType;
				typedef typename UpLevelRetriver::ResultNonReferenceType		ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					typedef typename DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>::Type		ContainerType;
					typedef typename ContainerType::ElementType										ElementType;

					Ptr<TypeInfoImpl> arrayType=new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					arrayType->SetTypeDescriptor(Description<IValueEnumerable>::GetAssociatedTypeDescriptor());

					Ptr<TypeInfoImpl> genericType=new TypeInfoImpl(ITypeInfo::Generic);
					genericType->SetElementType(arrayType);
					genericType->AddGenericArgument(TypeInfoRetriver<ElementType>::CreateTypeInfo());

					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::SharedPtr);
					type->SetElementType(genericType);
					return type;
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<T, TypeFlags::ReadonlyListType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef IValueReadonlyList										Type;
				typedef typename UpLevelRetriver::TempValueType					TempValueType;
				typedef typename UpLevelRetriver::ResultReferenceType			ResultReferenceType;
				typedef typename UpLevelRetriver::ResultNonReferenceType		ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					typedef typename DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>::Type		ContainerType;
					typedef typename ContainerType::ElementType										ElementType;

					Ptr<TypeInfoImpl> arrayType=new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					arrayType->SetTypeDescriptor(Description<IValueReadonlyList>::GetAssociatedTypeDescriptor());

					Ptr<TypeInfoImpl> genericType=new TypeInfoImpl(ITypeInfo::Generic);
					genericType->SetElementType(arrayType);
					genericType->AddGenericArgument(TypeInfoRetriver<ElementType>::CreateTypeInfo());

					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::SharedPtr);
					type->SetElementType(genericType);
					return type;
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<T, TypeFlags::ListType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef IValueList												Type;
				typedef typename UpLevelRetriver::TempValueType					TempValueType;
				typedef typename UpLevelRetriver::ResultReferenceType			ResultReferenceType;
				typedef typename UpLevelRetriver::ResultNonReferenceType		ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					typedef typename DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>::Type		ContainerType;
					typedef typename ContainerType::ElementType										ElementType;

					Ptr<TypeInfoImpl> arrayType=new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					arrayType->SetTypeDescriptor(Description<IValueList>::GetAssociatedTypeDescriptor());

					Ptr<TypeInfoImpl> genericType=new TypeInfoImpl(ITypeInfo::Generic);
					genericType->SetElementType(arrayType);
					genericType->AddGenericArgument(TypeInfoRetriver<ElementType>::CreateTypeInfo());

					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::SharedPtr);
					type->SetElementType(genericType);
					return type;
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<T, TypeFlags::ReadonlyDictionaryType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef IValueReadonlyList										Type;
				typedef typename UpLevelRetriver::TempValueType					TempValueType;
				typedef typename UpLevelRetriver::ResultReferenceType			ResultReferenceType;
				typedef typename UpLevelRetriver::ResultNonReferenceType		ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					typedef typename DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>::Type		ContainerType;
					typedef typename ContainerType::KeyContainer									KeyContainer;
					typedef typename ContainerType::ValueContainer									ValueContainer;
					typedef typename KeyContainer::ElementType										KeyType;
					typedef typename ValueContainer::ElementType									ValueType;

					Ptr<TypeInfoImpl> arrayType=new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					arrayType->SetTypeDescriptor(Description<IValueReadonlyDictionary>::GetAssociatedTypeDescriptor());

					Ptr<TypeInfoImpl> genericType=new TypeInfoImpl(ITypeInfo::Generic);
					genericType->SetElementType(arrayType);
					genericType->AddGenericArgument(TypeInfoRetriver<KeyType>::CreateTypeInfo());
					genericType->AddGenericArgument(TypeInfoRetriver<ValueType>::CreateTypeInfo());

					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::SharedPtr);
					type->SetElementType(genericType);
					return type;
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<T, TypeFlags::DictionaryType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef IValueReadonlyList										Type;
				typedef typename UpLevelRetriver::TempValueType					TempValueType;
				typedef typename UpLevelRetriver::ResultReferenceType			ResultReferenceType;
				typedef typename UpLevelRetriver::ResultNonReferenceType		ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					typedef typename DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>::Type		ContainerType;
					typedef typename ContainerType::KeyContainer									KeyContainer;
					typedef typename ContainerType::ValueContainer									ValueContainer;
					typedef typename KeyContainer::ElementType										KeyType;
					typedef typename ValueContainer::ElementType									ValueType;

					Ptr<TypeInfoImpl> arrayType=new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					arrayType->SetTypeDescriptor(Description<IValueDictionary>::GetAssociatedTypeDescriptor());

					Ptr<TypeInfoImpl> genericType=new TypeInfoImpl(ITypeInfo::Generic);
					genericType->SetElementType(arrayType);
					genericType->AddGenericArgument(TypeInfoRetriver<KeyType>::CreateTypeInfo());
					genericType->AddGenericArgument(TypeInfoRetriver<ValueType>::CreateTypeInfo());

					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::SharedPtr);
					type->SetElementType(genericType);
					return type;
				}
			};
 
/***********************************************************************
ParameterAccessor<TContainer>
***********************************************************************/

			template<typename T>
			struct ParameterAccessor<collections::LazyList<T>, TypeFlags::EnumerableType>
			{
				static Value BoxParameter(collections::LazyList<T>& object, ITypeDescriptor* typeDescriptor)
				{
					Ptr<IValueEnumerable> result=IValueEnumerable::Create(
						collections::From(object)
							.Select([](const T& item)
							{
								return BoxValue<T>(item);
							})
						);
					return BoxValue<Ptr<IValueEnumerable>>(result, Description<IValueEnumerable>::GetAssociatedTypeDescriptor());
				}

				static void UnboxParameter(const Value& value, collections::LazyList<T>& result, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					typedef typename T::ElementType ElementType;
					Ptr<IValueEnumerable> listProxy=UnboxValue<Ptr<IValueEnumerable>>(value, typeDescriptor, valueName);
					result=IValueEnumerable::GetLazyList(listProxy);
				}
			};

			template<typename T>
			struct ParameterAccessor<T, TypeFlags::ReadonlyListType>
			{
				static Value BoxParameter(T& object, ITypeDescriptor* typeDescriptor)
				{
					Ptr<IValueReadonlyList> result=new ValueReadonlyListWrapper<T*>(&object);
					return BoxValue<Ptr<IValueReadonlyList>>(result, Description<IValueReadonlyList>::GetAssociatedTypeDescriptor());
				}

				static void UnboxParameter(const Value& value, T& result, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					typedef typename T::ElementType ElementType;
					Ptr<IValueReadonlyList> listProxy=UnboxValue<Ptr<IValueReadonlyList>>(value, typeDescriptor, valueName);
					LazyList<ElementType> lazyList=listProxy->GetLazyList<ElementType>();
					collections::CopyFrom(result, lazyList);
				}
			};

			template<typename T>
			struct ParameterAccessor<T, TypeFlags::ListType>
			{
				static Value BoxParameter(T& object, ITypeDescriptor* typeDescriptor)
				{
					Ptr<IValueList> result=new ValueListWrapper<T*>(&object);
					return BoxValue<Ptr<IValueList>>(result, Description<IValueList>::GetAssociatedTypeDescriptor());
				}

				static void UnboxParameter(const Value& value, T& result, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					typedef typename T::ElementType ElementType;
					Ptr<IValueList> listProxy=UnboxValue<Ptr<IValueList>>(value, typeDescriptor, valueName);
					collections::LazyList<ElementType> lazyList=listProxy->GetLazyList<ElementType>();
					collections::CopyFrom(result, lazyList);
				}
			};

			template<typename T>
			struct ParameterAccessor<T, TypeFlags::ReadonlyDictionaryType>
			{
				static Value BoxParameter(T& object, ITypeDescriptor* typeDescriptor)
				{
					Ptr<IValueReadonlyDictionary> result=new ValueReadonlyDictionaryWrapper<T*>(&object);
					return BoxValue<Ptr<IValueReadonlyDictionary>>(result, Description<IValueReadonlyList>::GetAssociatedTypeDescriptor());
				}

				static void UnboxParameter(const Value& value, T& result, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					typedef typename T::KeyContainer					KeyContainer;
					typedef typename T::ValueContainer					ValueContainer;
					typedef typename KeyContainer::ElementType			KeyType;
					typedef typename ValueContainer::ElementType		ValueType;

					Ptr<IValueReadonlyDictionary> dictionaryProxy=UnboxValue<Ptr<IValueReadonlyDictionary>>(value, typeDescriptor, valueName);
					collections::LazyList<collections::Pair<KeyType, ValueType>> lazyList=dictionaryProxy->GetLazyList<KeyType, ValueType>();
					collections::CopyFrom(result, lazyList);
				}
			};

			template<typename T>
			struct ParameterAccessor<T, TypeFlags::DictionaryType>
			{
				static Value BoxParameter(T& object, ITypeDescriptor* typeDescriptor)
				{
					Ptr<IValueDictionary> result=new ValueDictionaryWrapper<T*>(&object);
					return BoxValue<Ptr<IValueDictionary>>(result, Description<IValueList>::GetAssociatedTypeDescriptor());
				}

				static void UnboxParameter(const Value& value, T& result, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					typedef typename T::KeyContainer					KeyContainer;
					typedef typename T::ValueContainer					ValueContainer;
					typedef typename KeyContainer::ElementType			KeyType;
					typedef typename ValueContainer::ElementType		ValueType;

					Ptr<IValueDictionary> dictionaryProxy=UnboxValue<Ptr<IValueDictionary>>(value, typeDescriptor, valueName);
					collections::LazyList<collections::Pair<KeyType, ValueType>> lazyList=dictionaryProxy->GetLazyList<KeyType, ValueType>();
					collections::CopyFrom(result, lazyList);
				}
			};
		}
	}
}

#endif