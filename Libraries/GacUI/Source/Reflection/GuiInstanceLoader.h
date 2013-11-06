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

		class IGuiInstanceLoader : public IDescriptable, public Description<IGuiInstanceLoader>
		{
		public:
			enum PropertyType
			{
				UnsupportedProperty		=0,
				SupportedProperty		=1<<0,
				HandleByParentLoader	=1<<1,
			};

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
			virtual description::Value				CreateInstance(Ptr<GuiInstanceEnvironment> env, Ptr<GuiConstructorRepr> ctor, const TypeInfo& typeInfo)=0;
			virtual PropertyType					GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)=0;
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
	}
}

#endif