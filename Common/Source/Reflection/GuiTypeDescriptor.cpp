#include "GuiTypeDescriptorBuilder.h"

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

			bool Value::operator==(const Value& value)const
			{
				switch(valueType)
				{
				case Null:
					return value.IsNull();
				case Text:
					return GetTypeDescriptor()==value.GetTypeDescriptor() && GetText()==value.GetText();
				case RawPtr:
				case SharedPtr:
					return GetRawPtr()==value.GetRawPtr();
				}
				return false;
			}

			bool Value::operator!=(const Value& value)const
			{
				return !(*this==value);
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
				if(targetType==GetGlobalTypeManager()->GetRootType())
				{
					return true;
				}
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

			IMethodInfo* Value::SelectMethod(IMethodGroupInfo* methodGroup, collections::Array<Value>& arguments)
			{
				if(methodGroup->GetMethodCount()==1)
				{
					return methodGroup->GetMethod(0);
				}

				List<IMethodInfo*> methods;
				for(vint i=0;i<methodGroup->GetMethodCount();i++)
				{
					IMethodInfo* method=methodGroup->GetMethod(i);
					if(method->GetParameterCount()==arguments.Count())
					{
						methods.Add(method);
					}
				}

				if(methods.Count()==0)
				{
					throw ArgumentCountMismtatchException();
				}
				else if(methods.Count()==1)
				{
					return methods[0];
				}
				else
				{
					for(vint i=0;i<methods.Count();i++)
					{
						IMethodInfo* method=methods[i];
						try
						{
							method->CheckArguments(arguments);
							return method;
						}
						catch(const TypeDescriptorException&)
						{
						}
					}
					return methods[0];
				}
			}

			Value Value::Create(const WString& typeName)
			{
				Array<Value> arguments;
				return Create(typeName, arguments);
			}

			Value Value::Create(const WString& typeName, collections::Array<Value>& arguments)
			{
				ITypeDescriptor* type=vl::reflection::description::GetTypeDescriptor(typeName);
				if(!type) throw TypeNotExistsException(typeName);

				IMethodGroupInfo* methodGroup=type->GetConstructorGroup();
				if(!methodGroup) throw ConstructorNotExistsException();

				IMethodInfo* method=SelectMethod(methodGroup, arguments);
				return method->Invoke(Value(), arguments);
			}

			Value Value::InvokeStatic(const WString& typeName, const WString& name)
			{
				Array<Value> arguments;
				return InvokeStatic(typeName, name, arguments);
			}

			Value Value::InvokeStatic(const WString& typeName, const WString& name, collections::Array<Value>& arguments)
			{
				ITypeDescriptor* type=vl::reflection::description::GetTypeDescriptor(typeName);
				if(!type) throw TypeNotExistsException(typeName);

				IMethodGroupInfo* methodGroup=type->GetMethodGroupByName(name, true);
				if(!methodGroup) throw MemberNotExistsException(name);

				IMethodInfo* method=SelectMethod(methodGroup, arguments);
				return method->Invoke(Value(), arguments);
			}

			Value Value::GetProperty(const WString& name)const
			{
				ITypeDescriptor* type=GetTypeDescriptor();
				if(!type) throw ArgumentNullException(L"thisObject");

				IPropertyInfo* prop=type->GetPropertyByName(name, true);
				if(!prop) throw MemberNotExistsException(name);

				return prop->GetValue(*this);
			}

			void Value::SetProperty(const WString& name, const Value& newValue)
			{
				ITypeDescriptor* type=GetTypeDescriptor();
				if(!type) throw ArgumentNullException(L"thisObject");

				IPropertyInfo* prop=type->GetPropertyByName(name, true);
				if(!prop) throw MemberNotExistsException(name);

				prop->SetValue(*this, newValue);
			}

			Value Value::Invoke(const WString& name)const
			{
				Array<Value> arguments;
				return Invoke(name, arguments);
			}

			Value Value::Invoke(const WString& name, collections::Array<Value>& arguments)const
			{
				ITypeDescriptor* type=GetTypeDescriptor();
				if(!type) throw ArgumentNullException(L"thisObject");

				IMethodGroupInfo* methodGroup=type->GetMethodGroupByName(name, true);
				if(!methodGroup) throw MemberNotExistsException(name);

				IMethodInfo* method=SelectMethod(methodGroup, arguments);
				return method->Invoke(*this, arguments);
			}

/***********************************************************************
IValueList
***********************************************************************/

			Ptr<IValueList> IValueList::Create()
			{
				Ptr<List<Value>> list=new List<Value>;
				return new ValueListWrapper<Ptr<List<Value>>>(list);
			}

			Ptr<IValueList> IValueList::Create(Ptr<IValueReadonlyList> values)
			{
				Ptr<List<Value>> list=new List<Value>;
				CopyFrom(*list.Obj(), values->GetLazyList<Value>());
				return new ValueListWrapper<Ptr<List<Value>>>(list);
			}

			Ptr<IValueList> IValueList::Create(collections::LazyList<Value> values)
			{
				Ptr<List<Value>> list=new List<Value>;
				CopyFrom(*list.Obj(), values);
				return new ValueListWrapper<Ptr<List<Value>>>(list);
			}

/***********************************************************************
description::TypeManager
***********************************************************************/

			class TypeManager : public Object, public ITypeManager
			{
			protected:
				Dictionary<WString, Ptr<ITypeDescriptor>>		typeDescriptors;
				List<Ptr<ITypeLoader>>							typeLoaders;
				ITypeDescriptor*								rootType;
				bool											loaded;

			public:
				TypeManager()
					:rootType(0)
					,loaded(false)
				{
				}

				~TypeManager()
				{
					Unload();
				}

				vint GetTypeDescriptorCount()override
				{
					return typeDescriptors.Values().Count();
				}

				ITypeDescriptor* GetTypeDescriptor(vint index)override
				{
					return typeDescriptors.Values().Get(index).Obj();
				}

				ITypeDescriptor* GetTypeDescriptor(const WString& name)override
				{
					vint index=typeDescriptors.Keys().IndexOf(name);
					return index==-1?0:typeDescriptors.Values().Get(index).Obj();
				}

				bool SetTypeDescriptor(const WString& name, Ptr<ITypeDescriptor> typeDescriptor)override
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

				bool AddTypeLoader(Ptr<ITypeLoader> typeLoader)override
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

				bool RemoveTypeLoader(Ptr<ITypeLoader> typeLoader)override
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

				bool Load()override
				{
					if(!loaded)
					{
						loaded=true;
						for(vint i=0;i<typeLoaders.Count();i++)
						{
							typeLoaders[i]->Load(this);
						}
						rootType=description::GetTypeDescriptor<Value>();
						return true;
					}
					else
					{
						return false;
					}
				}

				bool Unload()override
				{
					if(loaded)
					{
						loaded=false;
						rootType=0;
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

				bool Reload()override
				{
					Unload();
					Load();
					return true;
				}

				bool IsLoaded()override
				{
					return loaded;
				}

				ITypeDescriptor* GetRootType()override
				{
					return rootType;
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
LogTypeManager (enum)
***********************************************************************/

			void LogTypeManager_Enum(stream::TextWriter& writer, ITypeDescriptor* type, IValueSerializer* serializer)
			{
				writer.WriteLine(L"enum "+type->GetTypeName()+(serializer->CanMergeCandidate()?L" flag":L""));
				writer.WriteLine(L"{");
				for(vint j=0;j<serializer->GetCandidateCount();j++)
				{
					writer.WriteLine(L"    "+serializer->GetCandidate(j)+L",");
				}
				writer.WriteLine(L"}");
			}

/***********************************************************************
LogTypeManager (struct)
***********************************************************************/

			void LogTypeManager_Struct(stream::TextWriter& writer, ITypeDescriptor* type)
			{
				writer.WriteLine(L"struct "+type->GetTypeName());
				writer.WriteLine(L"{");
				for(vint j=0;j<type->GetPropertyCount();j++)
				{
					IPropertyInfo* info=type->GetProperty(j);
					writer.WriteLine(L"    "+info->GetReturn()->GetTypeFriendlyName()+L" "+info->GetName()+L";");
				}
				writer.WriteLine(L"}");
			}

/***********************************************************************
LogTypeManager (data)
***********************************************************************/

			void LogTypeManager_Data(stream::TextWriter& writer, ITypeDescriptor* type)
			{
				writer.WriteLine(L"data "+type->GetTypeName()+L";");
			}

/***********************************************************************
LogTypeManager (class)
***********************************************************************/

			bool LogTypeManager_IsInterface(ITypeDescriptor* type)
			{
				if(IMethodGroupInfo* group=type->GetConstructorGroup())
				{
					if(group->GetMethodCount()==1)
					{
						if(IMethodInfo* info=group->GetMethod(0))
						{
							if(info->GetParameterCount()==1 && info->GetParameter(0)->GetValueTypeDescriptor()->GetTypeName()==L"InterfaceProxy")
							{
								return true;
							}
						}
					}
				}
				return false;
			}

			void LogTypeManager_PrintEvents(stream::TextWriter& writer, ITypeDescriptor* type)
			{
				bool printed=false;
				for(vint j=0;j<type->GetEventCount();j++)
				{
					printed=true;
					IEventInfo* info=type->GetEvent(j);
					writer.WriteString(L"    event "+info->GetName());
					if(info->GetObservingProperty())
					{
						writer.WriteString(L" observing "+info->GetObservingProperty()->GetName());
					}
					writer.WriteLine(L";");
				}
				if(printed)
				{
					writer.WriteLine(L"");
				}
			}

			void LogTypeManager_PrintProperties(stream::TextWriter& writer, ITypeDescriptor* type, List<IMethodInfo*>& propertyAccessors)
			{
				bool printed=false;
				for(vint j=0;j<type->GetPropertyCount();j++)
				{
					printed=true;
					IPropertyInfo* info=type->GetProperty(j);
					writer.WriteString(L"    property "+info->GetReturn()->GetTypeFriendlyName()+L" "+info->GetName()+L"{");
					if(info->GetGetter())
					{
						propertyAccessors.Add(info->GetGetter());
						writer.WriteString(L" getter "+info->GetGetter()->GetName()+L";");
					}
					if(info->GetSetter())
					{
						propertyAccessors.Add(info->GetSetter());
						writer.WriteString(L" setter "+info->GetSetter()->GetName()+L";");
					}
					if(info->GetValueChangedEvent())
					{
						writer.WriteString(L" raising "+info->GetValueChangedEvent()->GetName()+L";");
					}
					writer.WriteLine(L"}");
				}
				if(printed)
				{
					writer.WriteLine(L"");
				}
			}

			void LogTypeManager_PrintMethods(stream::TextWriter& writer, ITypeDescriptor* type, const List<IMethodInfo*>& propertyAccessors, bool isPropertyAccessor)
			{
				bool printed=false;
				for(vint j=0;j<type->GetMethodGroupCount();j++)
				{
					IMethodGroupInfo* group=type->GetMethodGroup(j);
					for(vint k=0;k<group->GetMethodCount();k++)
					{
						IMethodInfo* info=group->GetMethod(k);
						if(propertyAccessors.Contains(info)==isPropertyAccessor)
						{
							printed=true;
							if(info->GetReturn())
							{
								writer.WriteString(WString(L"    ")+(info->IsStatic()?L"static ":L"")+(isPropertyAccessor?L"accessor ":L"function ")+info->GetReturn()->GetTypeFriendlyName());
							}
							else
							{
								writer.WriteString(WString(L"    ")+(info->IsStatic()?L"static ":L"")+(isPropertyAccessor?L"accessor void":L"function void"));
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
				}
				if(printed)
				{
					writer.WriteLine(L"");
				}
			}

			void LogTypeManager_PrintConstructors(stream::TextWriter& writer, ITypeDescriptor* type)
			{
				if(IMethodGroupInfo* group=type->GetConstructorGroup())
				{
					for(vint k=0;k<group->GetMethodCount();k++)
					{
						IMethodInfo* info=group->GetMethod(k);
						writer.WriteString(L"    constructor "+info->GetReturn()->GetTypeFriendlyName());
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
			}

			void LogTypeManager_Class(stream::TextWriter& writer, ITypeDescriptor* type)
			{
				bool isInterface=false;

				writer.WriteString((isInterface?L"interface ":L"class ")+type->GetTypeName());
				for(vint j=0;j<type->GetBaseTypeDescriptorCount();j++)
				{
					writer.WriteString(j==0?L" : ":L", ");
					writer.WriteString(type->GetBaseTypeDescriptor(j)->GetTypeName());
				}
				writer.WriteLine(L"");
				writer.WriteLine(L"{");
				
				List<IMethodInfo*> propertyAccessors;
				LogTypeManager_PrintEvents(writer, type);
				LogTypeManager_PrintProperties(writer, type, propertyAccessors);
				LogTypeManager_PrintMethods(writer, type, propertyAccessors, false);
				LogTypeManager_PrintMethods(writer, type, propertyAccessors, true);
				LogTypeManager_PrintConstructors(writer, type);

				writer.WriteLine(L"}");
			}

/***********************************************************************
LogTypeManager
***********************************************************************/

			void LogTypeManager(stream::TextWriter& writer)
			{
				for(vint i=0;i<globalTypeManager->GetTypeDescriptorCount();i++)
				{
					ITypeDescriptor* type=globalTypeManager->GetTypeDescriptor(i);
					IValueSerializer* serializer=type->GetValueSerializer();
					if(serializer)
					{
						if(serializer->HasCandidate())
						{
							LogTypeManager_Enum(writer, type, serializer);
						}
						else if(type->GetPropertyCount()>0)
						{
							LogTypeManager_Struct(writer, type);
						}
						else
						{
							LogTypeManager_Data(writer, type);
						}
					}
					else
					{
						LogTypeManager_Class(writer, type);
					}
					writer.WriteLine(L"");
				}
			}
		}
	}
}