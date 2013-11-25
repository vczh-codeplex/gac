/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Instance Loader

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONINSTANCELOADER
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONINSTANCELOADER

#include "..\Controls\GuiApplication.h"
#include "..\Controls\Styles\GuiThemeStyleFactory.h"
#include "GuiInstanceRepresentation.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

/***********************************************************************
Instance Environment
***********************************************************************/

		class GuiInstanceContextScope : public Object, public Description<GuiInstanceContextScope>
		{
			typedef collections::Dictionary<WString, description::Value>		ValueMap;
		public:
			WString									typeName;
			description::Value						rootInstance;
			ValueMap								referenceValues;
		};

		class GuiInstanceEnvironment : public Object, public Description<GuiInstanceEnvironment>
		{
		public:
			Ptr<GuiInstanceContext>					context;
			Ptr<GuiResourcePathResolver>			resolver;
			Ptr<GuiInstanceContextScope>			scope;

			GuiInstanceEnvironment(Ptr<GuiInstanceContext> _context, Ptr<GuiResourcePathResolver> _resolver)
				:context(_context)
				, resolver(_resolver)
			{
				scope = new GuiInstanceContextScope;
			}
		};

/***********************************************************************
Instance Loader
***********************************************************************/

		class GuiInstancePropertyInfo : public IDescriptable, public Description<GuiInstancePropertyInfo>
		{
			typedef collections::List<description::ITypeDescriptor*>		TypeDescriptorList;
		public:
			enum Support
			{
				NotSupport,
				SupportAssign,
				SupportArray,
				SupportCollection,
				SupportSet,
			};

			Support									support;
			bool									tryParent;
			bool									required;
			bool									constructorParameter;
			TypeDescriptorList						acceptableTypes;

			GuiInstancePropertyInfo();
			~GuiInstancePropertyInfo();

			static Ptr<GuiInstancePropertyInfo>		Unsupported();
			static Ptr<GuiInstancePropertyInfo>		Assign(description::ITypeDescriptor* typeDescriptor = 0);
			static Ptr<GuiInstancePropertyInfo>		AssignWithParent(description::ITypeDescriptor* typeDescriptor = 0);
			static Ptr<GuiInstancePropertyInfo>		Collection(description::ITypeDescriptor* typeDescriptor = 0);
			static Ptr<GuiInstancePropertyInfo>		CollectionWithParent(description::ITypeDescriptor* typeDescriptor = 0);
			static Ptr<GuiInstancePropertyInfo>		Set(description::ITypeDescriptor* typeDescriptor = 0);
			static Ptr<GuiInstancePropertyInfo>		Array(description::ITypeDescriptor* typeDescriptor = 0);
		};

		class GuiInstanceEventInfo : public IDescriptable, public Description<GuiInstanceEventInfo>
		{
			typedef collections::List<description::ITypeDescriptor*>		TypeDescriptorList;
		public:
			enum Support
			{
				NotSupport,
				SupportAssign,
			};

			Support									support;
			description::ITypeDescriptor*			argumentType;

			GuiInstanceEventInfo();
			~GuiInstanceEventInfo();

			static Ptr<GuiInstanceEventInfo>		Unsupported();
			static Ptr<GuiInstanceEventInfo>		Assign(description::ITypeDescriptor* typeDescriptor);
		};

		class IGuiInstanceLoader : public IDescriptable, public Description<IGuiInstanceLoader>
		{
		public:
			struct TypeInfo
			{
				WString								typeName;
				description::ITypeDescriptor*		typeDescriptor;

				TypeInfo() :typeDescriptor(0){}
				TypeInfo(const WString& _typeName, description::ITypeDescriptor* _typeDescriptor)
					:typeName(_typeName)
					, typeDescriptor(_typeDescriptor)
				{
				}
			};

			struct PropertyInfo
			{
				TypeInfo							typeInfo;
				WString								propertyName;

				PropertyInfo(){}
				PropertyInfo(const TypeInfo& _typeInfo, const WString& _propertyName)
					:typeInfo(_typeInfo)
					, propertyName(_propertyName)
				{
				}
			};

			struct PropertyValue : PropertyInfo
			{
				description::Value					instanceValue;
				description::Value					propertyValue;

				PropertyValue(){}
				PropertyValue(const TypeInfo& _typeInfo, const WString& _propertyName, description::Value _instanceValue, description::Value _propertyValue = description::Value())
					:PropertyInfo(_typeInfo, _propertyName)
					, instanceValue(_instanceValue)
					, propertyValue(_propertyValue)
				{
				}
			};

			virtual WString							GetTypeName() = 0;

			virtual bool							IsDeserializable(const TypeInfo& typeInfo);
			virtual description::Value				Deserialize(const TypeInfo& typeInfo, const WString& text);
			virtual bool							IsCreatable(const TypeInfo& typeInfo);
			virtual description::Value				CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments);
			virtual bool							IsInitializable(const TypeInfo& typeInfo);
			virtual Ptr<GuiInstanceContextScope>	InitializeInstance(const TypeInfo& typeInfo, description::Value instance);

			virtual void							GetPropertyNames(const TypeInfo& typeInfo, collections::List<WString>& propertyNames);
			virtual void							GetConstructorParameters(const TypeInfo& typeInfo, collections::List<WString>& propertyNames);
			virtual Ptr<GuiInstancePropertyInfo>	GetPropertyType(const PropertyInfo& propertyInfo);
			virtual bool							GetPropertyValue(PropertyValue& propertyValue);
			virtual bool							SetPropertyValue(PropertyValue& propertyValue);

			virtual void							GetEventNames(const TypeInfo& typeInfo, collections::List<WString>& eventNames);
			virtual Ptr<GuiInstanceEventInfo>		GetEventType(const PropertyInfo& eventInfo);
			virtual bool							SetEventValue(PropertyValue& propertyValue);
		};

/***********************************************************************
Instance Binder
***********************************************************************/

		class IGuiInstanceBinder : public IDescriptable, public Description<IGuiInstanceBinder>
		{
		public:
			virtual WString							GetBindingName() = 0;
			virtual void							GetExpectedValueTypes(collections::List<description::ITypeDescriptor*>& expectedTypes) = 0;
			virtual bool							SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, IGuiInstanceLoader::PropertyValue& propertyValue) = 0;
		};

/***********************************************************************
Instance Loader Manager
***********************************************************************/

		class IGuiInstanceLoaderManager : public IDescriptable, public Description<IGuiInstanceLoaderManager>
		{
		public:
			virtual bool							AddInstanceBinder(Ptr<IGuiInstanceBinder> binder) = 0;
			virtual IGuiInstanceBinder*				GetInstanceBinder(const WString& bindingName) = 0;
			virtual bool							CreateVirtualType(const WString& parentType, Ptr<IGuiInstanceLoader> loader) = 0;
			virtual bool							SetLoader(Ptr<IGuiInstanceLoader> loader) = 0;
			virtual IGuiInstanceLoader*				GetLoader(const WString& typeName) = 0;
			virtual IGuiInstanceLoader*				GetParentLoader(IGuiInstanceLoader* loader) = 0;
			virtual description::ITypeDescriptor*	GetTypeDescriptorForType(const WString& typeName) = 0;
			virtual void							GetVirtualTypes(collections::List<WString>& typeNames) = 0;
			virtual WString							GetParentTypeForVirtualType(const WString& virtualType) = 0;
			virtual bool							SetResource(const WString& name, Ptr<GuiResource> resource) = 0;
			virtual Ptr<GuiResource>				GetResource(const WString& name) = 0;
		};

		struct InstanceLoadingSource
		{
			IGuiInstanceLoader*						loader;
			WString									typeName;
			Ptr<GuiInstanceContext>					context;

			InstanceLoadingSource() :loader(0){}
			InstanceLoadingSource(IGuiInstanceLoader* _loader, const WString& _typeName) :loader(_loader), typeName(_typeName){}
			InstanceLoadingSource(Ptr<GuiInstanceContext> _context) :loader(0), context(_context){}

			operator bool()const
			{
				return loader != 0 || context;
			}
		};

		extern IGuiInstanceLoaderManager*			GetInstanceLoaderManager();
		extern InstanceLoadingSource				FindInstanceLoadingSource(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor
			);
		Ptr<GuiInstanceContextScope>				LoadInstanceFromContext(
			Ptr<GuiInstanceContext> context,
			Ptr<GuiResourcePathResolver> resolver,
			description::ITypeDescriptor* expectedType = 0
			);
		extern Ptr<GuiInstanceContextScope>			LoadInstance(
			Ptr<GuiResource> resource,
			const WString& instancePath,
			description::ITypeDescriptor* expectedType = 0
			);
		extern Ptr<GuiInstanceContextScope>			InitializeInstanceFromContext(
			Ptr<GuiInstanceContext> context,
			Ptr<GuiResourcePathResolver> resolver,
			description::Value instance
			);
		extern Ptr<GuiInstanceContextScope>			InitializeInstance(
			Ptr<GuiResource> resource,
			const WString& instancePath,
			description::Value instance
			);
		extern void									LogInstanceLoaderManager(stream::TextWriter& writer);

/***********************************************************************
Instance Scope Wrapper
***********************************************************************/

		template<typename T>
		class GuiInstancePartialClass
		{
			template<typename T>
			friend class GuiInstance;

			typedef collections::Dictionary<WString, description::Value>	ValueMap;
		private:
			WString									className;
			Ptr<GuiInstanceContextScope>			scope;

		protected:
			bool InitializeFromResource()
			{
				if (scope) return false;
				if (auto loader = GetInstanceLoaderManager()->GetLoader(className))
				{
					IGuiInstanceLoader::TypeInfo typeInfo(className, description::GetTypeDescriptor<T>());
					if (loader->IsInitializable(typeInfo))
					{
						auto value = description::Value::From(dynamic_cast<T*>(this));
						if (scope = loader->InitializeInstance(typeInfo, value))
						{
							return true;
						}
					}
				}
				return false;
			}
		public:
			GuiInstancePartialClass(const WString& _className)
				:className(_className)
			{
			}

			virtual ~GuiInstancePartialClass()
			{
			}

			Ptr<GuiInstanceContextScope> GetScope()
			{
				return scope;
			}
		};

		template<typename T>
		class GuiInstance : public Object, public GuiInstancePartialClass<T>
		{
		public:
			GuiInstance(Ptr<GuiResource> _resource, const WString& path) :GuiInstancePartialClass<T>(_resource)
			{
				LoadFromResource(path);
			}
		};

#define GUI_INSTANCE_REFERENCE(NAME) this->NAME=vl::reflection::description::UnboxValue<decltype(NAME)>(this->GetScope()->referenceValues[L#NAME])
	}
}

#endif