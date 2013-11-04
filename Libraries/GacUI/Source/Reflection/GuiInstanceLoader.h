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
Instance Loader
***********************************************************************/

		class IGuiInstanceLoader : public IDescriptable, public Description<IGuiInstanceLoader>
		{
		public:
			enum PropertyType
			{
				ValueProperty,
				CollectionProperty,
				UnsupportedProperty,
				HandleByParentLoader,
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
			virtual description::Value				CreateInstance(Ptr<GuiInstanceContext> context, Ptr<GuiConstructorRepr> ctor, Ptr<GuiResourcePathResolver> resolver, const TypeInfo& typeInfo)=0;
			virtual PropertyType					GetPropertyType(const PropertyInfo& propertyInfo, description::ITypeDescriptor*& elementType, bool &nullable)=0;
			virtual bool							GetPropertyValue(PropertyValue& propertyValue)=0;
			virtual bool							SetPropertyValue(PropertyValue& propertyValue)=0;
			virtual bool							SetPropertyCollection(PropertyValue& propertyValue, vint currentIndex)=0;
		};

/***********************************************************************
Instance Binder
***********************************************************************/

		class IGuiInstanceBinder : public IDescriptable, public Description<IGuiInstanceBinder>
		{
		public:
			virtual WString							GetBindingName()=0;
			virtual description::ITypeDescriptor*	GetExpectedValueType()=0;
			virtual bool							SetPropertyValue(IGuiInstanceLoader* loader, Ptr<GuiResourcePathResolver> resolver, IGuiInstanceLoader::PropertyValue& propertyValue, vint currentIndex=-1)=0;
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
		extern InstanceLoadingSource				FindInstanceLoadingSource(
														Ptr<GuiInstanceContext> context,
														GuiConstructorRepr* ctor,
														Ptr<GuiResourcePathResolver> resolver
														);
		extern description::Value					LoadInstance(
														Ptr<GuiInstanceContext> context,
														Ptr<GuiResourcePathResolver> resolver
														);
		extern description::Value					LoadInstance(
														Ptr<GuiInstanceContext> context,
														GuiConstructorRepr* ctor,
														Ptr<GuiResourcePathResolver> resolver,
														WString& typeName,
														description::ITypeDescriptor*& typeDescriptor
														);
	}
}

#endif