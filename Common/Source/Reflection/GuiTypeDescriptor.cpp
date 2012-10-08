#include "GuiTypeDescriptor.h"
#include "..\Collections\List.h"
#include "..\Collections\Dictionary.h"

namespace vl
{
	using namespace collections;

	namespace reflection
	{

/***********************************************************************
DescriptableObject
***********************************************************************/

		DescriptableObject::DescriptableObject()
			:objectSize(0)
			,typeDescriptor(0)
		{
		}

		DescriptableObject::~DescriptableObject()
		{
		}

		description::ITypeDescriptor* DescriptableObject::GetTypeDescriptor()
		{
			return typeDescriptor?*typeDescriptor:0;
		}

/***********************************************************************
description::Value
***********************************************************************/

		namespace description
		{
			Value::Value()
				:valueType(Null)
				,descriptableObjectRef(0)
				,typeDescriptor(0)
			{
			}

			Value::Value(DescriptableObject* value)
				:valueType(DescriptableObjectRef)
				,descriptableObjectRef(value)
				,typeDescriptor(0)
			{
			}

			Value::Value(Ptr<DescriptableObject> value)
				:valueType(DescriptableObjectPtr)
				,descriptableObjectRef(value.Obj())
				,descriptableObjectPtr(value)
				,typeDescriptor(0)
			{
			}

			Value::Value(const WString& value, ITypeDescriptor* associatedTypeDescriptor)
				:valueType(Null)
				,descriptableObjectRef(0)
				,text(value)
				,typeDescriptor(associatedTypeDescriptor)
			{
			}

			Value::Value(const Value& value)
				:valueType(value.valueType)
				,descriptableObjectRef(value.descriptableObjectRef)
				,descriptableObjectPtr(value.descriptableObjectPtr)
				,text(value.text)
				,typeDescriptor(value.typeDescriptor)
			{
			}

			Value& Value::operator=(const Value& value)
			{
				valueType=value.valueType;
				descriptableObjectRef=value.descriptableObjectRef;
				descriptableObjectPtr=value.descriptableObjectPtr;
				text=value.text;
				typeDescriptor=value.typeDescriptor;
				return *this;
			}

			Value::ValueType Value::GetValueType()const
			{
				return valueType;
			}

			DescriptableObject* Value::GetDescriptableObjectRef()const
			{
				return descriptableObjectRef;
			}

			Ptr<DescriptableObject> Value::GetDescriptableObjectPtr()const
			{
				return descriptableObjectPtr;
			}

			const WString& Value::GetText()const
			{
				return text;
			}

			ITypeDescriptor* Value::GetTypeDescriptor()const
			{
				switch(valueType)
				{
				case DescriptableObjectRef:
					return descriptableObjectRef?descriptableObjectRef->GetTypeDescriptor():0;
				case DescriptableObjectPtr:
					return descriptableObjectPtr?descriptableObjectPtr->GetTypeDescriptor():0;
				case Text:
					return typeDescriptor;
				}
				return 0;
			}

/***********************************************************************
description::TypeManager
***********************************************************************/

			class TypeManager : public Object, public ITypeManager
			{
			protected:
				Dictionary<WString, Ptr<IValueSerializer>>		valueSerializers;
				Dictionary<WString, Ptr<ITypeDescriptor>>		typeDescriptors;
				List<Ptr<ITypeLoader>>							typeLoaders;
				bool											loaded;

			public:
				TypeManager()
					:loaded(false)
				{
				}

				~TypeManager()
				{
					Unload();
				}

				IValueSerializer* GetValueSerializer(const WString& name)
				{
					vint index=valueSerializers.Keys().IndexOf(name);
					return index==-1?0:valueSerializers.Values()[index].Obj();
				}

				bool SetValueSerializer(const WString& name, Ptr<IValueSerializer> valueSerializer)
				{
					if(valueSerializers.Keys().Contains(name))
					{
						if(valueSerializer)
						{
							valueSerializers.Add(name, valueSerializer);
							return true;
						}
					}
					else
					{
						if(!valueSerializer)
						{
							valueSerializers.Remove(name);
							return true;
						}
					}
					return false;
				}

				ITypeDescriptor* GetTypeDescriptor(const WString& name)
				{
					vint index=typeDescriptors.Keys().IndexOf(name);
					return index==-1?0:typeDescriptors.Values()[index].Obj();
				}

				bool SetTypeDescriptor(const WString& name, Ptr<ITypeDescriptor> typeDescriptor)
				{
					if(typeDescriptors.Keys().Contains(name))
					{
						if(typeDescriptor)
						{
							typeDescriptors.Add(name, typeDescriptor);
							return true;
						}
					}
					else
					{
						if(!typeDescriptor)
						{
							typeDescriptors.Remove(name);
							return true;
						}
					}
					return false;
				}

				bool AddTypeLoader(Ptr<ITypeLoader> typeLoader)
				{
					vint index=typeLoaders.IndexOf(typeLoader.Obj());
					if(index==-1)
					{
						typeLoaders.Add(typeLoader);
						if(loaded)
						{
							typeLoader->Load(this);
						}
						return true;
					}
					else
					{
						return false;
					}
				}

				bool RemoveTypeLoader(Ptr<ITypeLoader> typeLoader)
				{
					vint index=typeLoaders.IndexOf(typeLoader.Obj());
					if(index!=-1)
					{
						if(loaded)
						{
							typeLoader->Unload(this);
						}
						typeLoaders.RemoveAt(index);
						return true;
					}
					else
					{
						return false;
					}
				}

				bool Load()
				{
					if(!loaded)
					{
						loaded=true;
						for(vint i=0;i<typeLoaders.Count();i++)
						{
							typeLoaders[i]->Load(this);
						}
						return true;
					}
					else
					{
						return false;
					}
				}

				bool Unload()
				{
					if(loaded)
					{
						loaded=false;
						for(vint i=0;i<typeLoaders.Count();i++)
						{
							typeLoaders[i]->Unload(this);
						}
						valueSerializers.Clear();
						typeDescriptors.Clear();
						return true;
					}
					else
					{
						return false;
					}
				}

				bool Reload()
				{
					Unload();
					Load();
					return true;
				}

				bool IsLoaded()
				{
					return loaded;
				}
			};

/***********************************************************************
description::TypeManager helper functions
***********************************************************************/

			ITypeManager* globalTypeManager=0;
			bool initializedGlobalTypeManager=false;

			ITypeManager* GetGlobalTypeManager()
			{
				if(!initializedGlobalTypeManager)
				{
					initializedGlobalTypeManager=true;
					globalTypeManager=new TypeManager;
				}
				return globalTypeManager;
			}

			bool DestroyGlobalTypeManager()
			{
				if(initializedGlobalTypeManager && globalTypeManager)
				{
					delete globalTypeManager;
					globalTypeManager=0;
					return true;
				}
				else
				{
					return false;
				}
			}

			IValueSerializer* GetValueSerializer(const WString& name)
			{
				if(globalTypeManager)
				{
					if(!globalTypeManager->IsLoaded())
					{
						globalTypeManager->Load();
					}
					return globalTypeManager->GetValueSerializer(name);
				}
				return 0;
			}

			ITypeDescriptor* GetTypeDescriptor(const WString& name)
			{
				if(globalTypeManager)
				{
					if(!globalTypeManager->IsLoaded())
					{
						globalTypeManager->Load();
					}
					return globalTypeManager->GetTypeDescriptor(name);
				}
				return 0;
			}
		}
	}
}