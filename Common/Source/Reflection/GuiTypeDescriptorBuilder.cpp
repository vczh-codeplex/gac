#include "GuiTypeDescriptorBuilder.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
description::GeneralTypeDescriptor::PropertyGroup
***********************************************************************/

			GeneralTypeDescriptor::PropertyGroup::PropertyGroup()
				:valueSerializer(0)
				,loaded(false)
				,ownerTypeDescriptor(0)
			{
			}

			GeneralTypeDescriptor::PropertyGroup::~PropertyGroup()
			{
			}

			void GeneralTypeDescriptor::PropertyGroup::Prepare()
			{
				if(!loaded)
				{
					loaded=true;
					loaderProcedure(this);
				}
			}

/***********************************************************************
description::GeneralTypeDescriptor
***********************************************************************/

			GeneralTypeDescriptor::GeneralTypeDescriptor(const Func<void(PropertyGroup*)>& loaderProcedure)
			{
				propertyGroup.loaderProcedure=loaderProcedure;
				propertyGroup.ownerTypeDescriptor=this;
			}

			GeneralTypeDescriptor::~GeneralTypeDescriptor()
			{
			}

			const WString& GeneralTypeDescriptor::GetTypeName()
			{
				propertyGroup.Prepare();
				return propertyGroup.typeName;
			}

			IValueSerializer* GeneralTypeDescriptor::GetValueSerializer()
			{
				propertyGroup.Prepare();
				return propertyGroup.valueSerializer;
			}

			vint GeneralTypeDescriptor::GetBaseTypeDescriptorCount()
			{
				propertyGroup.Prepare();
				return propertyGroup.baseTypeDescriptors.Count();
			}

			ITypeDescriptor* GeneralTypeDescriptor::GetBaseTypeDescriptor(vint index)
			{
				propertyGroup.Prepare();
				if(0<=index && index<propertyGroup.baseTypeDescriptors.Count())
				{
					return propertyGroup.baseTypeDescriptors[index];
				}
				else
				{
					return 0;
				}
			}

			vint GeneralTypeDescriptor::GetPropertyCount()
			{
				propertyGroup.Prepare();
				return propertyGroup.properties.Count();
			}

			IPropertyInfo* GeneralTypeDescriptor::GetProperty(vint index)
			{
				propertyGroup.Prepare();
				if(0<=index && index<propertyGroup.properties.Count())
				{
					return propertyGroup.properties.Values()[index].Obj();
				}
				else
				{
					return 0;
				}
			}

			bool GeneralTypeDescriptor::IsPropertyExists(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				if(propertyGroup.properties.Keys().Contains(name))
				{
					return true;
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						if(propertyGroup.baseTypeDescriptors[i]->IsPropertyExists(name, true))
						{
							return true;
						}
					}
				}
				return false;
			}

			IPropertyInfo* GeneralTypeDescriptor::GetPropertyByName(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				vint index=propertyGroup.properties.Keys().IndexOf(name);
				if(index!=-1)
				{
					return propertyGroup.properties.Values()[index].Obj();
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						IPropertyInfo* result=propertyGroup.baseTypeDescriptors[i]->GetPropertyByName(name, true);
						if(result)
						{
							return result;
						}
					}
				}
				return 0;
			}

			vint GeneralTypeDescriptor::GetEventCount()
			{
				propertyGroup.Prepare();
				return propertyGroup.events.Count();
			}

			IEventInfo* GeneralTypeDescriptor::GetEvent(vint index)
			{
				propertyGroup.Prepare();
				if(0<=index && index<propertyGroup.events.Count())
				{
					return propertyGroup.events.Values()[index].Obj();
				}
				else
				{
					return 0;
				}
			}

			bool GeneralTypeDescriptor::IsEventExists(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				if(propertyGroup.events.Keys().Contains(name))
				{
					return true;
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						if(propertyGroup.baseTypeDescriptors[i]->IsEventExists(name, true))
						{
							return true;
						}
					}
				}
				return false;
			}

			IEventInfo* GeneralTypeDescriptor::GetEventByName(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				vint index=propertyGroup.events.Keys().IndexOf(name);
				if(index!=-1)
				{
					return propertyGroup.events.Values()[index].Obj();
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						IEventInfo* result=propertyGroup.baseTypeDescriptors[i]->GetEventByName(name, true);
						if(result)
						{
							return result;
						}
					}
				}
				return 0;
			}

			vint GeneralTypeDescriptor::GetMethodGroupCount()
			{
				propertyGroup.Prepare();
				return propertyGroup.methodGroups.Count();
			}

			IMethodGroupInfo* GeneralTypeDescriptor::GetMethodGroup(vint index)
			{
				propertyGroup.Prepare();
				if(0<=index && index<propertyGroup.methodGroups.Count())
				{
					return propertyGroup.methodGroups.Values()[index].Obj();
				}
				else
				{
					return 0;
				}
			}

			bool GeneralTypeDescriptor::IsMethodGroupExists(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				if(propertyGroup.methodGroups.Keys().Contains(name))
				{
					return true;
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						if(propertyGroup.baseTypeDescriptors[i]->IsMethodGroupExists(name, true))
						{
							return true;
						}
					}
				}
				return false;
			}

			IMethodGroupInfo* GeneralTypeDescriptor::GetMethodGroupByName(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				vint index=propertyGroup.methodGroups.Keys().IndexOf(name);
				if(index!=-1)
				{
					return propertyGroup.methodGroups.Values()[index].Obj();
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						IMethodGroupInfo* result=propertyGroup.baseTypeDescriptors[i]->GetMethodGroupByName(name, true);
						if(result)
						{
							return result;
						}
					}
				}
				return 0;
			}

			IMethodGroupInfo* GeneralTypeDescriptor::GetConstructorGroup()
			{
				propertyGroup.Prepare();
				return propertyGroup.contructorGroup.Obj();
			}
		}
	}
}