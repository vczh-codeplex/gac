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
				,resolver(_resolver)
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
			bool									supportAssign;
			bool									supportSet;
			bool									tryParent;
			bool									multipleValues;
			bool									required;
			bool									constructorParameter;
			TypeDescriptorList						acceptableTypes;

			GuiInstancePropertyInfo();
			~GuiInstancePropertyInfo();

			static Ptr<GuiInstancePropertyInfo>		Unsupported();
			static Ptr<GuiInstancePropertyInfo>		Supported(description::ITypeDescriptor* typeDescriptor = 0);
			static Ptr<GuiInstancePropertyInfo>		SupportedWithParent(description::ITypeDescriptor* typeDescriptor = 0);
			static Ptr<GuiInstancePropertyInfo>		SupportedCollection(description::ITypeDescriptor* typeDescriptor = 0);
			static Ptr<GuiInstancePropertyInfo>		SupportedCollectionWithParent(description::ITypeDescriptor* typeDescriptor = 0);
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
					,typeDescriptor(_typeDescriptor)
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
					,propertyName(_propertyName)
				{
				}
			};

			struct PropertyValue : PropertyInfo
			{
				description::Value					instanceValue;
				description::Value					propertyValue;

				PropertyValue(){}
				PropertyValue(const TypeInfo& _typeInfo, const WString& _propertyName, description::Value _instanceValue, description::Value _propertyValue=description::Value())
					:PropertyInfo(_typeInfo, _propertyName)
					,instanceValue(_instanceValue)
					,propertyValue(_propertyValue)
				{
				}
			};

			virtual WString							GetTypeName()=0;
			virtual bool							IsDeserializable(const TypeInfo& typeInfo) = 0;
			virtual description::Value				Deserialize(const TypeInfo& typeInfo, const WString& text) = 0;
			virtual description::Value				CreateInstance(const TypeInfo& typeInfo)=0;
			virtual Ptr<GuiInstancePropertyInfo>	GetPropertyType(const PropertyInfo& propertyInfo)=0;
			virtual bool							GetPropertyValue(PropertyValue& propertyValue)=0;
			virtual bool							SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)=0;
		};

/***********************************************************************
Instance Binder
***********************************************************************/

		class IGuiInstanceBinder : public IDescriptable, public Description<IGuiInstanceBinder>
		{
		public:
			virtual WString							GetBindingName()=0;
			virtual void							GetExpectedValueTypes(collections::List<description::ITypeDescriptor*>& expectedTypes)=0;
			virtual bool							SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, IGuiInstanceLoader::PropertyValue& propertyValue, vint currentIndex=-1)=0;
		};

/***********************************************************************
Instance Loader Manager
***********************************************************************/

		class IGuiInstanceLoaderManager : public IDescriptable, public Description<IGuiInstanceLoaderManager>
		{
		public:
			virtual bool							AddInstanceBinder(Ptr<IGuiInstanceBinder> binder)=0;
			virtual IGuiInstanceBinder*				GetInstanceBinder(const WString& bindingName)=0;
			virtual bool							CreateVirtualType(const WString& typeName, const WString& parentType, Ptr<IGuiInstanceLoader> loader)=0;
			virtual bool							SetLoader(Ptr<IGuiInstanceLoader> loader)=0;
			virtual IGuiInstanceLoader*				GetLoader(const WString& typeName)=0;
			virtual IGuiInstanceLoader*				GetParentLoader(IGuiInstanceLoader* loader)=0;
			virtual description::ITypeDescriptor*	GetTypeDescriptorForType(const WString& typeName)=0;
		};

		struct InstanceLoadingSource
		{
			IGuiInstanceLoader*						loader;
			WString									typeName;
			Ptr<GuiInstanceContext>					context;

			InstanceLoadingSource():loader(0){}
			InstanceLoadingSource(IGuiInstanceLoader* _loader, const WString& _typeName):loader(_loader), typeName(_typeName){}
			InstanceLoadingSource(Ptr<GuiInstanceContext> _context):loader(0), context(_context){}

			operator bool()const
			{
				return loader!=0 || context;
			}
		};

		extern IGuiInstanceLoaderManager*			GetInstanceLoaderManager();
		extern Ptr<GuiInstanceContextScope>			LoadInstance(
														Ptr<GuiResource> resource,
														const WString& instancePath,
														description::ITypeDescriptor* expectedType=0
														);

/***********************************************************************
Instance Scope Wrapper
***********************************************************************/

		template<typename T>
		class GuiInstance : public Object
		{
		private:
			Ptr<GuiResource>						resource;
			Ptr<GuiInstanceContextScope>			scope;
			T*										instance;

		public:
			GuiInstance(Ptr<GuiResource> _resource, const WString& path)
				:resource(_resource)
				,instance(0)
			{
				if (scope = LoadInstance(resource, path))
				{
					instance = description::UnboxValue<T*>(scope->rootInstance);
				}
			}

			Ptr<GuiResource> GetResource()
			{
				return resource;
			}

			Ptr<GuiInstanceContextScope> GetScope()
			{
				return scope;
			}

			T* GetInstance()
			{
				return instance;
			}
		};

#define GUI_INSTANCE_REFERENCE(NAME) this->NAME=vl::reflection::description::UnboxValue<decltype(NAME)>(this->GetScope()->referenceValues[L#NAME])
	}
}

#endif