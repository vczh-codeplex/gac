#include "FpmacroRuntime.h"

namespace fpmacro
{
/***********************************************************************
RunningObject
***********************************************************************/

	RunningObject::Ref RunningObject::Run(RunningObject::Ref object, Environment::Ref environment)
	{
		return object->RunInternal(object, environment);
	}

	RunningObject::Ref RunningObject::Invoke(RunningObject::Ref object, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		return object->InvokeInternal(object, environment, arguments);
	}

/***********************************************************************
ReferenceObject
***********************************************************************/

	RunningObject::Ref ReferenceObject::RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		while(environment)
		{
			int index=environment->objects.Keys().IndexOf(name);
			if(index!=-1)
			{
				RunningObject::Ref result=Run(environment->objects.Values().Get(index), environment);
				return result;
			}
			else
			{
				environment=environment->previous;
			}
		}
		throw Exception(L"�Ҳ���"+name+L"��");
	}

	RunningObject::Ref ReferenceObject::InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		throw Exception(L"��������"+name+L"����һ���������������δ����ֵ��");
	}

/***********************************************************************
ExecutableObject
***********************************************************************/

	RunningObject::Ref ExecutableObject::RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return self;
	}

	RunningObject::Ref ExecutableObject::InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		if(parameters.Count()!=arguments.Count())
		{
			throw Exception(L"����������һ�¡�");
		}
		Environment::Ref newEnvironment=new Environment;
		newEnvironment->previous=environment;
		for(int i=0;i<parameters.Count();i++)
		{
			newEnvironment->objects.Add(parameters[i], Run(arguments.Get(i), environment));
		}
		CopyFrom(newEnvironment->objects, predefinedEnvironment->objects, true);
		return Run(runningObject, newEnvironment);
	}

/***********************************************************************
TextObject
***********************************************************************/

	RunningObject::Ref TextObject::RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return self;
	}

	RunningObject::Ref TextObject::InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		throw Exception(L"�ַ������ʽ����һ�����������ַ������ʽ����Ϊ��\""+text+L"\"��");
	}

/***********************************************************************
ConcatObject
***********************************************************************/

	RunningObject::Ref ConcatObject::RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		WString text;
		for(int i=0;i<objects.Count();i++)
		{
			RunningObject::Ref object=Run(objects[i], environment);
			TextObject* textObject=dynamic_cast<TextObject*>(object.Obj());
			if(textObject)
			{
				text+=textObject->text;
			}
			else
			{
				throw Exception(L"���ӱ��ʽ���ӵ�����Ķ����Ѿ����ӵ��ַ���Ϊ��\""+text+L"\"��");
			}
		}
		TextObject* object=new TextObject;
		object->text=text;
		return object;
	}

	RunningObject::Ref ConcatObject::InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		throw Exception(L"���ӱ��ʽ����һ��������");
	}

/***********************************************************************
ArrayObject
***********************************************************************/

	RunningObject::Ref ArrayObject::RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return self;
	}

	RunningObject::Ref ArrayObject::InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		throw Exception(L"������ʽ����һ��������");
	}

/***********************************************************************
InvokeObject
***********************************************************************/

	RunningObject::Ref InvokeObject::RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return Invoke(Run(target, environment), environment, arguments);
	}

	RunningObject::Ref InvokeObject::InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		return Invoke(Run(self, environment), environment, arguments);
	}

/***********************************************************************
PredefinedObject
***********************************************************************/

	int PredefinedObject::GetInt(RunningObject::Ref object, Environment::Ref environment)
	{
		object=Run(object, environment);
		TextObject* textObject=dynamic_cast<TextObject*>(object.Obj());
		if(textObject)
		{
			return wtoi(textObject->text);
		}
		else
		{
			throw Exception(L"������ת��Ϊ���ַ�������");
		}
	}

	WString PredefinedObject::GetStr(RunningObject::Ref object, Environment::Ref environment)
	{
		object=Run(object, environment);
		TextObject* textObject=dynamic_cast<TextObject*>(object.Obj());
		if(textObject)
		{
			return textObject->text;
		}
		else
		{
			throw Exception(L"������ת��Ϊ�ַ�����������");
		}
	}

	RunningObject::Ref PredefinedObject::GetObj(int number)
	{
		TextObject* textObject=new TextObject;
		textObject->text=itow(number);
		return textObject;
	}

	RunningObject::Ref PredefinedObject::GetObj(WString text)
	{
		TextObject* textObject=new TextObject;
		textObject->text=text;
		return textObject;
	}

	RunningObject::Ref PredefinedObject::RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return self;
	}

	RunningObject::Ref PredefinedObject::InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		switch(type)
		{
		case Add:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) + GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Add�����Ĳ�������������2��");
				}
			}
		case Sub:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) - GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Sub�����Ĳ�������������2��");
				}
			}
		case Mul:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) * GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Mul�����Ĳ�������������2��");
				}
			}
		case Div:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) / GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Div�����Ĳ�������������2��");
				}
			}
		case Mod:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) % GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Mod�����Ĳ�������������2��");
				}
			}
		case LE:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) <= GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Le�����Ĳ�������������2��");
				}
			}
		case LT:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) < GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Lt�����Ĳ�������������2��");
				}
			}
		case GE:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) >= GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Ge�����Ĳ�������������2��");
				}
			}
		case GT:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) > GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Gt�����Ĳ�������������2��");
				}
			}
		case EQ:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) == GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Eq�����Ĳ�������������2��");
				}
			}
		case NE:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) != GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Ne�����Ĳ�������������2��");
				}
			}
		case If:
			{
				if(arguments.Count()==3)
				{
					return Run(arguments.Get(GetInt(arguments.Get(0), environment)?1:2), environment);
				}
				else
				{
					throw Exception(L"If�����Ĳ�������������3��");
				}
			}
		case Loop:
			{
				if(arguments.Count()!=3)
				{
					throw Exception(L"Loop�����Ĳ�������������3��");
				}
				WString text;
				int count=GetInt(arguments.Get(0), environment);
				int start=GetInt(arguments.Get(1), environment);
				RunningObject::Ref function=Run(arguments.Get(2), environment);
				for(int i=0;i<count;i++)
				{
					List<RunningObject::Ref> arguments;
					arguments.Add(GetObj(i+start));
					text+=GetStr(Invoke(function, environment, arguments), environment);
				}
				return GetObj(text);
			}
		case LoopSep:
			{
				if(arguments.Count()!=4)
				{
					throw Exception(L"LoopSep�����Ĳ�������������4��");
				}
				WString text;
				int count=GetInt(arguments.Get(0), environment);
				int start=GetInt(arguments.Get(1), environment);
				RunningObject::Ref function=Run(arguments.Get(2), environment);
				WString sep=GetStr(arguments.Get(3), environment);
				for(int i=0;i<count;i++)
				{
					List<RunningObject::Ref> arguments;
					arguments.Add(GetObj(i+start));
					if(i)
					{
						text+=sep;
					}
					text+=GetStr(Invoke(function, environment, arguments), environment);
				}
				return GetObj(text);
			}
		case Head:
			{
				if(arguments.Count()!=1)
				{
					throw Exception(L"Head�����Ĳ�������������1��");
				}

				RunningObject::Ref arrayArgument=arguments.Get(0);
				arrayArgument=Run(arrayArgument, environment);
				ArrayObject* arrayObject=dynamic_cast<ArrayObject*>(arrayArgument.Obj());

				if(!arrayObject)
				{
					throw Exception(L"Head�����Ĳ������������顣");
				}
				if(arrayObject->objects.Count()==0)
				{
					throw Exception(L"Head�������ܶԿ�������ֵ��");
				}
				return arrayObject->objects[0];
			}
		case Tail:
			{
				if(arguments.Count()!=1)
				{
					throw Exception(L"Tail�����Ĳ�������������1��");
				}

				RunningObject::Ref arrayArgument=arguments.Get(0);
				arrayArgument=Run(arrayArgument, environment);
				ArrayObject* arrayObject=dynamic_cast<ArrayObject*>(arrayArgument.Obj());

				if(!arrayObject)
				{
					throw Exception(L"Tail�����Ĳ������������顣");
				}
				if(arrayObject->objects.Count()==0)
				{
					throw Exception(L"Tail�������ܶԿ�������ֵ��");
				}
				ArrayObject* newArray=new ArrayObject;
				for(int i=1;i<arrayObject->objects.Count();i++)
				{
					newArray->objects.Add(arrayObject->objects[i]);
				}
				return newArray;
			}
		case Length:
			{
				if(arguments.Count()!=1)
				{
					throw Exception(L"Length�����Ĳ�������������1��");
				}

				RunningObject::Ref arrayArgument=arguments.Get(0);
				arrayArgument=Run(arrayArgument, environment);
				ArrayObject* arrayObject=dynamic_cast<ArrayObject*>(arrayArgument.Obj());

				if(!arrayObject)
				{
					throw Exception(L"Length�����Ĳ������������顣");
				}
				return GetObj(arrayObject->objects.Count());
			}
		case Get:
			{
				if(arguments.Count()!=2)
				{
					throw Exception(L"Get�����Ĳ�������������2��");
				}

				RunningObject::Ref arrayArgument=arguments.Get(0);
				arrayArgument=Run(arrayArgument, environment);
				ArrayObject* arrayObject=dynamic_cast<ArrayObject*>(arrayArgument.Obj());

				if(!arrayObject)
				{
					throw Exception(L"Get�����ĵ�һ���������������顣");
				}
				int index=GetInt(arguments.Get(1), environment);
				if(index>=0 && index<arrayObject->objects.Count())
				{
					return Run(arrayObject->objects[index], environment);
				}
				else
				{
					throw Exception(L"Get�����ĵڶ�������\""+itow(index)+L"\"Խ�硣");
				}
			}
		default:
			throw Exception(L"δ֪����");
		}
	}
}