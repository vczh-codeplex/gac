/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Reflection

Classes:
***********************************************************************/

#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER
#define VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER

#include "GuiTypeDescriptor.h"
#include "..\Collections\List.h"
#include "..\Collections\Dictionary.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
Exceptions
***********************************************************************/

			class TypeDescriptorBuilderException : public Exception
			{
			public:
				TypeDescriptorBuilderException(const WString& message)
					:Exception(message)
				{
				}
			};

			class PropertyAlreadyExistsException : public TypeDescriptorBuilderException
			{
			public:
				PropertyAlreadyExistsException(ITypeDescriptor* typeDescriptor, const WString& propertyName)
					:TypeDescriptorBuilderException(L"Property \""+propertyName+L"\" already exists in type \""+typeDescriptor->GetTypeName()+L"\".")
				{
				}
			};

			class EventAlreadyExistsException : public TypeDescriptorBuilderException
			{
			public:
				EventAlreadyExistsException(ITypeDescriptor* typeDescriptor, const WString& eventName)
					:TypeDescriptorBuilderException(L"Event \""+eventName+L"\" already exists in type \""+typeDescriptor->GetTypeName()+L"\".")
				{
				}
			};

			class ParameterAlreadyExistsException : public TypeDescriptorBuilderException
			{
			public:
				ParameterAlreadyExistsException(IMethodInfo* method,  const WString& parameterName)
					:TypeDescriptorBuilderException(L"Parameter \""+parameterName+L"\" already exists in method \""+method->GetName()+L"\" in type \""+method->GetOwnerTypeDescriptor()->GetTypeName()+L"\".")
				{
				}
			};

/***********************************************************************
GeneralTypeDescriptor
***********************************************************************/

			class GeneralTypeDescriptor : public Object, public ITypeDescriptor
			{
			public:
				class PropertyGroup
				{
					friend class GeneralTypeDescriptor;
				protected:
					bool														loaded;
					Func<void(PropertyGroup*)>									loaderProcedure;
					ITypeDescriptor*											ownerTypeDescriptor;
					
					WString														typeName;
					IValueSerializer*											valueSerializer;
					collections::List<ITypeDescriptor*>							baseTypeDescriptors;
					collections::Dictionary<WString, Ptr<IPropertyInfo>>		properties;
					collections::Dictionary<WString, Ptr<IEventInfo>>			events;
					collections::Dictionary<WString, Ptr<IMethodGroupInfo>>		methodGroups;
					Ptr<IMethodGroupInfo>										contructorGroup;

				public:
					PropertyGroup();
					~PropertyGroup();

					void						Prepare();

					//----------------------------------------------------

					class MethodBuilder
					{
					protected:
						PropertyGroup&			propertyGroup;
						Ptr<IMethodGroupInfo>	buildingMethodGroup;
						Ptr<IMethodInfo>		buildingMethod;
					public:
						MethodBuilder(PropertyGroup& _propertyGroup, const WString& _name);

						MethodBuilder&			Parameter(
													const WString&									_name,
													ITypeDescriptor*								_type,
													bool											_nullable,
													bool											_canOutput
													);
						MethodBuilder&			Return(
													ITypeDescriptor*								_type,
													bool											_nullable
													);
						MethodBuilder&			Invoker(
													const Func<Value(const Value&, collections::IArray<Value>&)>&	_invoker
													);
						PropertyGroup&			Done();
					};

					//----------------------------------------------------

					class EventBuilder
					{
					protected:
						PropertyGroup&			propertyGroup;
						Ptr<IEventInfo>			buildingEvent;
					public:
						EventBuilder(PropertyGroup& _propertyGroup, const WString& _name);

						EventBuilder&			Attacher(
													const Func<void(DescriptableObject*, IEventHandler*)>&	_attacher
													);
						EventBuilder&			Detacher(
													const Func<void(DescriptableObject*, IEventHandler*)>&	_detacher
													);
						EventBuilder&			Invoker(
													const Func<Value(const Value&, Value&)>&		_invoker
													);
						PropertyGroup&			Done();
					};

					//----------------------------------------------------

					PropertyGroup&				TypeName(
													const WString&									_typeName
													);
					PropertyGroup&				Property(
													const WString&									_name,
													ITypeDescriptor*								_type,
													bool											_nullable,
													const Func<Value(const Value&)>&				_getter,
													const Func<void(const Value&, const Value&)>&	_setter,
													const WString&									_valueChangedEventName
													);
					MethodBuilder				Method(
													const WString&									_name
													);
					EventBuilder				Event(
													const WString&									_name
													);
				};
			protected:
				PropertyGroup				propertyGroup;

			public:
				GeneralTypeDescriptor(const Func<void(PropertyGroup*)>& loaderProcedure);
				~GeneralTypeDescriptor();

				PropertyGroup&				Operations();

				const WString&				GetTypeName()override;
				IValueSerializer*			GetValueSerializer()override;
				vint						GetBaseTypeDescriptorCount()override;
				ITypeDescriptor*			GetBaseTypeDescriptor(vint index)override;

				vint						GetPropertyCount()override;
				IPropertyInfo*				GetProperty(vint index)override;
				bool						IsPropertyExists(const WString& name, bool inheritable)override;
				IPropertyInfo*				GetPropertyByName(const WString& name, bool inheritable)override;

				vint						GetEventCount()override;
				IEventInfo*					GetEvent(vint index)override;
				bool						IsEventExists(const WString& name, bool inheritable)override;
				IEventInfo*					GetEventByName(const WString& name, bool inheritable)override;

				vint						GetMethodGroupCount()override;
				IMethodGroupInfo*			GetMethodGroup(vint index)override;
				bool						IsMethodGroupExists(const WString& name, bool inheritable)override;
				IMethodGroupInfo*			GetMethodGroupByName(const WString& name, bool inheritable)override;
				IMethodGroupInfo*			GetConstructorGroup()override;
			};
		}
	}
}

#endif