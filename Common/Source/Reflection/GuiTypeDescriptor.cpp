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
			Value::Value(DescriptableObject* value)
				:valueType(RawPtr)
				,rawPtr(value)
				,typeDescriptor(0)
			{
			}

			Value::Value(Ptr<DescriptableObject> value)
				:valueType(SharedPtr)
				,rawPtr(value.Obj())
				,sharedPtr(value)
				,typeDescriptor(0)
			{
			}

			Value::Value(const WString& value, ITypeDescriptor* associatedTypeDescriptor)
				:valueType(Text)
				,rawPtr(0)
				,text(value)
				,typeDescriptor(associatedTypeDescriptor)
			{
			}

			Value::Value()
				:valueType(Null)
				,rawPtr(0)
				,typeDescriptor(0)
			{
			}

			Value::Value(const Value& value)
				:valueType(value.valueType)
				,rawPtr(value.rawPtr)
				,sharedPtr(value.sharedPtr)
				,text(value.text)
				,typeDescriptor(value.typeDescriptor)
			{
			}

			Value& Value::operator=(const Value& value)
			{
				valueType=value.valueType;
				rawPtr=value.rawPtr;
				sharedPtr=value.sharedPtr;
				text=value.text;
				typeDescriptor=value.typeDescriptor;
				return *this;
			}

			Value::ValueType Value::GetValueType()const
			{
				return valueType;
			}

			DescriptableObject* Value::GetRawPtr()const
			{
				return rawPtr;
			}

			Ptr<DescriptableObject> Value::GetSharedPtr()const
			{
				return sharedPtr;
			}

			const WString& Value::GetText()const
			{
				return text;
			}

			ITypeDescriptor* Value::GetTypeDescriptor()const
			{
				switch(valueType)
				{
				case RawPtr:
				case SharedPtr:
					return rawPtr?rawPtr->GetTypeDescriptor():0;
				case Text:
					return typeDescriptor;
				}
				return 0;
			}

			WString Value::GetTypeFriendlyName()const
			{
				switch(valueType)
				{
				case RawPtr:
					return GetTypeDescriptor()->GetTypeName()+L"*";
				case SharedPtr:
					return L"Ptr<"+GetTypeDescriptor()->GetTypeName()+L">";
				case Text:
					return GetTypeDescriptor()->GetTypeName();
				default:
					return L"null";
				}
			}

			bool Value::IsNull()const
			{
				return valueType==Null;
			}

			bool Value::CanConvertTo(ITypeDescriptor* targetType, ValueType targetValueType)const
			{
				switch(valueType)
				{
				case Null:
					return targetValueType!=Text;
				case RawPtr:
					if(targetValueType!=RawPtr) return false;
					break;
				case SharedPtr:
					if(targetValueType!=RawPtr && targetValueType!=SharedPtr) return false;
					break;
				case Text:
					return targetValueType==Text && GetTypeDescriptor()==targetType;
				}
				return GetTypeDescriptor()->CanConvertTo(targetType);
			}

			bool Value::CanConvertTo(IParameterInfo* targetType)const
			{
				ValueType targetValueType=ValueType::Null;
				switch(targetType->GetDecorator())
				{
				case IParameterInfo::RawPtr:
					targetValueType=RawPtr;
					break;
				case IParameterInfo::SharedPtr:
					targetValueType=SharedPtr;
					break;
				case IParameterInfo::Text:
					targetValueType=Text;
					break;
				}
				return CanConvertTo(targetType->GetValueTypeDescriptor(), targetValueType);
			}

			Value Value::From(DescriptableObject* value)
			{
				return Value(value);
			}

			Value Value::From(Ptr<DescriptableObject> value)
			{
				return Value(value);
			}

			Value Value::From(const WString& value, ITypeDescriptor* type)
			{
				return Value(value, type);
			}

/***********************************************************************
description::TypeManager
***********************************************************************/

			class TypeManager : public Object, public ITypeManager
			{
			protected:
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

				vint GetTypeDescriptorCount()
				{
					return typeDescriptors.Values().Count();
				}

				ITypeDescriptor* GetTypeDescriptor(vint index)
				{
					return typeDescriptors.Values().Get(index).Obj();
				}

				ITypeDescriptor* GetTypeDescriptor(const WString& name)
				{
					vint index=typeDescriptors.Keys().IndexOf(name);
					return index==-1?0:typeDescriptors.Values().Get(index).Obj();
				}

				bool SetTypeDescriptor(const WString& name, Ptr<ITypeDescriptor> typeDescriptor)
				{
					if(typeDescriptor && name!=typeDescriptor->GetTypeName())
					{
						return false;
					}
					if(!typeDescriptors.Keys().Contains(name))
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

			bool ResetGlobalTypeManager()
			{
				if(!DestroyGlobalTypeManager()) return false;
				initializedGlobalTypeManager=false;
				return true;
			}

			IValueSerializer* GetValueSerializer(const WString& name)
			{
				ITypeDescriptor* descriptor=GetTypeDescriptor(name);
				return descriptor?descriptor->GetValueSerializer():0;
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

/***********************************************************************
LogTypeManager
***********************************************************************/

			void LogTypeManager(stream::TextWriter& writer)
			{
				for(vint i=0;i<globalTypeManager->GetTypeDescriptorCount();i++)
				{
					ITypeDescriptor* type=globalTypeManager->GetTypeDescriptor(i);
					if(type->GetValueSerializer())
					{
						writer.WriteLine(L"struct "+type->GetTypeName()+L";");
					}
					else
					{
						writer.WriteString(L"class "+type->GetTypeName());
						for(vint j=0;j<type->GetBaseTypeDescriptorCount();j++)
						{
							writer.WriteString(L" "+type->GetBaseTypeDescriptor(j)->GetTypeName());
						}
						writer.WriteLine(L"");
						writer.WriteLine(L"{");

						for(vint j=0;j<type->GetEventCount();j++)
						{
							IEventInfo* info=type->GetEvent(j);
							writer.WriteString(L"    event "+info->GetName());
							if(info->GetObservingProperty())
							{
								writer.WriteString(L" observing "+info->GetObservingProperty()->GetName());
							}
							writer.WriteLine(L";");
						}

						for(vint j=0;j<type->GetPropertyCount();j++)
						{
							IPropertyInfo* info=type->GetProperty(j);
							writer.WriteString(L"    "+info->GetReturn()->GetTypeFriendlyName()+L" "+info->GetName());
							if(info->GetGetter())
							{
								writer.WriteString(L" getter "+info->GetGetter()->GetName());
							}
							if(info->GetSetter())
							{
								writer.WriteString(L" setter "+info->GetSetter()->GetName());
							}
							if(info->GetValueChangedEvent())
							{
								writer.WriteString(L" raising "+info->GetValueChangedEvent()->GetName());
							}
							writer.WriteLine(L";");
						}

						for(vint j=0;j<type->GetMethodGroupCount();j++)
						{
							IMethodGroupInfo* group=type->GetMethodGroup(j);
							for(vint k=0;k<group->GetMethodCount();k++)
							{
								IMethodInfo* info=group->GetMethod(k);
								if(info->GetReturn())
								{
									writer.WriteString(L"    "+info->GetReturn()->GetTypeFriendlyName());
								}
								else
								{
									writer.WriteString(L"    void");
								}
								writer.WriteString(L" "+info->GetName()+L"(");
								for(vint l=0;l<info->GetParameterCount();l++)
								{
									if(l>0) writer.WriteString(L", ");
									IParameterInfo* parameter=info->GetParameter(l);
									writer.WriteString(parameter->GetTypeFriendlyName()+L" "+parameter->GetName());
								}
								writer.WriteLine(L");");
							}
						}

						writer.WriteLine(L"}");
					}
					writer.WriteLine(L"");
				}
			}
		}
	}
}