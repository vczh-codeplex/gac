#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>

#include "TestFunctions.h"
#include "..\..\..\..\Common\Source\Console.h"

Ptr<ParsingTable> workflowTable;

Ptr<ParsingTable> GetWorkflowTable()
{
	if (!workflowTable)
	{
		workflowTable = WfLoadTable();
	}
	return workflowTable;
}

WString GetPath()
{
	wchar_t buffer[65536];
	GetModuleFileName(NULL, buffer, sizeof(buffer)/sizeof(*buffer));
	vint pos=-1;
	vint index=0;
	while(buffer[index])
	{
		if(buffer[index]==L'\\')
		{
			pos=index;
		}
		index++;
	}
#ifdef _WIN64
	return WString(buffer, pos+1)+L"..\\..\\TestFiles\\";
#else
	return WString(buffer, pos+1)+L"..\\TestFiles\\";
#endif
}

void LoadSampleIndex(const WString& sampleName, List<WString>& itemNames)
{
	FileStream fileStream(GetPath() + L"Samples\\Index" + sampleName + L".txt", FileStream::ReadOnly);
	BomDecoder decoder;
	DecoderStream decoderStream(fileStream, decoder);
	StreamReader reader(decoderStream);

	while (!reader.IsEnd())
	{
		itemNames.Add(reader.ReadLine());
	}
}

WString LoadSample(const WString& sampleName, const WString& itemName)
{
	FileStream fileStream(GetPath() + L"Samples\\" + sampleName + L"\\" + itemName + L".txt", FileStream::ReadOnly);
	BomDecoder decoder;
	DecoderStream decoderStream(fileStream, decoder);
	StreamReader reader(decoderStream);
	return reader.ReadToEnd();
}

void LogSampleParseResult(const WString& sampleName, const WString& itemName, const WString& sample, Ptr<ParsingTreeNode> node, WfLexicalScopeManager* manager)
{
	FileStream fileStream(GetPath() + L"Parsing." + sampleName + L"." + itemName + L".txt", FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream encoderStream(fileStream, encoder);
	StreamWriter writer(encoderStream);
	writer.WriteLine(sample);

	if (manager && manager->errors.Count() > 0)
	{
		writer.WriteLine(L"========================================================");
		FOREACH(Ptr<ParsingError>, error, manager->errors)
		{
			writer.WriteLine(L"Line: " + itow(error->codeRange.start.row + 1) + L", Column: " + itow(error->codeRange.start.column + 1) + L", Message: " + error->errorMessage);
		}
	}

	writer.WriteLine(L"========================================================");
	Log(node.Obj(), L"", writer);
}

void LogSampleCodegenResult(const WString& sampleName, const WString& itemName, Ptr<WfAssembly> assembly)
{
	FileStream fileStream(GetPath() + L"Assembly." + sampleName + L"." + itemName + L".txt", FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream encoderStream(fileStream, encoder);
	StreamWriter writer(encoderStream);
	
	writer.WriteLine(L"========================================================");
	writer.WriteLine(L"Global Variables:");
	writer.WriteLine(L"========================================================");
	FOREACH(WString, name, assembly->variableNames)
	{
		writer.WriteLine(name);
	}
	writer.WriteLine(L"");
	
	writer.WriteLine(L"========================================================");
	writer.WriteLine(L"Functions:");
	writer.WriteLine(L"========================================================");
	FOREACH(Ptr<WfAssemblyFunction>, function, assembly->functions)
	{
		writer.WriteLine(function->name + L" (" + itow(function->firstInstruction) + L" .. " + itow(function->lastInstruction) + L")");

		writer.WriteLine(L"    Arguments:");
		FOREACH(WString, name, function->argumentNames)
		{
			writer.WriteLine(L"        " + name);
		}

		writer.WriteLine(L"    Captured Variables:");
		FOREACH(WString, name, function->capturedVariableNames)
		{
			writer.WriteLine(L"        " + name);
		}

		writer.WriteLine(L"    Local Variables:");
		FOREACH(WString, name, function->localVariableNames)
		{
			writer.WriteLine(L"        " + name);
		}

		writer.WriteLine(L"");
	}
	
	writer.WriteLine(L"========================================================");
	writer.WriteLine(L"Instructions:");
	writer.WriteLine(L"========================================================");

	auto formatText = [](const WString& text, vint length)
	{
		WString result = text;
		while (result.Length() < length)
		{
			result += L" ";
		}
		return result;
	};

	auto formatFlag = [](Value::ValueType type)->WString
	{
		switch (type)
		{
		case Value::RawPtr:
			return L"RawPtr";
		case Value::SharedPtr:
			return L"SharedPtr";
		case Value::Text:
			return L"Text";
		default:
			return L"Null";
		}
	};

	auto formatType = [](WfInsType type)->WString
	{
		switch (type)
		{
		case WfInsType::Bool:
			return L"Bool";
		case WfInsType::I1:
			return L"I1";
		case WfInsType::I2:
			return L"I2";
		case WfInsType::I4:
			return L"I4";
		case WfInsType::I8:
			return L"I8";
		case WfInsType::U1:
			return L"U1";
		case WfInsType::U2:
			return L"U2";
		case WfInsType::U4:
			return L"U4";
		case WfInsType::U8:
			return L"U8";
		case WfInsType::F4:
			return L"F4";
		case WfInsType::F8:
			return L"F8";
		case WfInsType::String:
			return L"Bool";
		default:
			return L"Unknown";
		}
	};

#define LOG(NAME)						case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18)); break;
#define LOG_FUNCTION(NAME)				case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18) + L": func = " + itow(ins.indexParameter) + L"(" + assembly->functions[ins.indexParameter]->name + L")"); break;
#define LOG_FUNCTION_COUNT(NAME)		case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18) + L": func = " + itow(ins.indexParameter) + L"(" + assembly->functions[ins.indexParameter]->name + L"), stackPatternCount = " + itow(ins.countParameter)); break;
#define LOG_VARIABLE(NAME)				case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18) + L": var = " + itow(ins.indexParameter)); break;
#define LOG_COUNT(NAME)					case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18) + L": stackPatternCount = " + itow(ins.countParameter)); break;
#define LOG_FLAG_TYPEDESCRIPTOR(NAME)	case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18) + L": flag = " + formatFlag(ins.flagParameter) + L", typeDescriptor = " + ins.typeDescriptorParameter->GetTypeName()); break;
#define LOG_METHOD_COUNT(NAME)			case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18) + L": methodInfo = " + ins.methodParameter->GetName() + L"<" + ins.methodParameter->GetOwnerTypeDescriptor()->GetTypeName() + L">, stackPatternCount = " + itow(ins.countParameter)); break;
#define LOG_EVENT(NAME)					case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18) + L": eventInfo = " + ins.eventParameter->GetName() + L"<" + ins.eventParameter->GetOwnerTypeDescriptor()->GetTypeName() + L">"); break;
#define LOG_LABEL(NAME)					case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18) + L": label = " + itow(ins.indexParameter)); break;
#define LOG_TYPE(NAME)					case WfInsCode::NAME: writer.WriteLine(formatText(itow(index), 5) + L": " + formatText(L"    " L ## #NAME, 18) + L": type = " + formatType(ins.typeParameter)); break;

	FOREACH_INDEXER(WfInstruction, ins, index, assembly->instructions)
	{
		switch (ins.code)
		{
			LOG(Nop)
			LOG(LoadValue)
			LOG_FUNCTION(LoadFunction)
			LOG_FUNCTION_COUNT(LoadLambda)
			LOG(LoadException)
			LOG_VARIABLE(LoadLocalVar)
			LOG_VARIABLE(LoadGlobalVar)
			LOG_VARIABLE(StoreLocalVar)
			LOG_VARIABLE(StoreGlobalVar)
			LOG(Pop)
			LOG(Return)
			LOG_COUNT(CreateArray)
			LOG_COUNT(CreateMap)
			LOG_FLAG_TYPEDESCRIPTOR(ConvertToType)
			LOG_FLAG_TYPEDESCRIPTOR(AssertAsType)
			LOG_FLAG_TYPEDESCRIPTOR(TestType)
			LOG_LABEL(Jump)
			LOG_LABEL(JumpIf)
			LOG_FUNCTION_COUNT(Invoke)
			LOG_METHOD_COUNT(InvokeMethod)
			LOG_EVENT(AttachEvent)
			LOG(DetachEvent)
			LOG_LABEL(InstallTry)
			LOG_LABEL(UninstallTry)
			LOG(RaiseException)
			LOG_TYPE(CompareLiteral)
			LOG(CompareReference)
			LOG_TYPE(OpNot)
			LOG_TYPE(OpPositive)
			LOG_TYPE(OpNegative)
			LOG(OpConcat)
			LOG_TYPE(OpExp)
			LOG_TYPE(OpAdd)
			LOG_TYPE(OpSub)
			LOG_TYPE(OpMul)
			LOG_TYPE(OpDiv)
			LOG_TYPE(OpShl)
			LOG_TYPE(OpShr)
			LOG(OpXor)
			LOG(OpAnd)
			LOG(OpOr)
			LOG(OpLT)
			LOG(OpGT)
			LOG(OpLE)
			LOG(OpGE)
			LOG(OpEQ)
			LOG(OpNE)
		}
	}

#undef LOG
#undef LOG_FUNCTION
#undef LOG_FUNCTION_COUNT
#undef LOG_VARIABLE
#undef LOG_COUNT
#undef LOG_FLAG_TYPEDESCRIPTOR
#undef LOG_METHOD_COUNT
#undef LOG_EVENT
#undef LOG_LABEL
#undef LOG_TYPE
}

namespace test
{
	struct Point
	{
		int									x;
		int									y;
	};

	class ObservableValue : public Object, public Description<ObservableValue>
	{
	protected:
		vint								value;
		WString								name;
	public:
		Event<void(vint, vint)>				ValueChanged;

		ObservableValue()
		{
		}

		ObservableValue(vint _value)
			:value(_value)
		{
		}

		ObservableValue(vint _value, Nullable<double> x)
			:value(_value)
		{
		}

		ObservableValue(vint _value, Nullable<bool> x)
			:value(_value)
		{
		}

		vint GetValue()
		{
			return value;
		}

		void SetValue(vint newValue)
		{
			vint oldValue = value;
			value = newValue;
			ValueChanged(oldValue, newValue);
		}

		WString GetName()
		{
			return name;
		}

		void SetName(const WString& newName)
		{
			name = newName;
		}

		WString GetDisplayName()
		{
			return L"This is " + name;
		}
	};
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			using namespace test;

#define _ ,
#define UNITTEST_TYPELIST(F)\
			F(test::Point)\
			F(test::ObservableValue)\

			UNITTEST_TYPELIST(DECL_TYPE_INFO)
				UNITTEST_TYPELIST(IMPL_TYPE_INFO)

			BEGIN_STRUCT_MEMBER(Point)
				STRUCT_MEMBER(x)
				STRUCT_MEMBER(y)
			END_STRUCT_MEMBER(Point)

			BEGIN_CLASS_MEMBER(ObservableValue)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ObservableValue>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ObservableValue>(vint), { L"value" })
				CLASS_MEMBER_CONSTRUCTOR(ObservableValue*(vint, Nullable<double>), { L"value" _ L"x" })
				CLASS_MEMBER_CONSTRUCTOR(ObservableValue*(vint, Nullable<bool>), { L"value" _ L"x" })

				CLASS_MEMBER_EVENT(ValueChanged)
				CLASS_MEMBER_PROPERTY_EVENT_FAST(Value, ValueChanged)
				CLASS_MEMBER_PROPERTY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(DisplayName);
			END_CLASS_MEMBER(ObservableValue)

			class UnitTestTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					UNITTEST_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			bool LoadUnitTestTypes()
			{
				ITypeManager* manager = GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader = new UnitTestTypeLoader;
					return manager->AddTypeLoader(loader);
				}
				return false;
			}

#undef _
		}
	}
}

void LoadTypes()
{
	LoadPredefinedTypes();
	LoadParsingTypes();
	XmlLoadTypes();
	JsonLoadTypes();
	WfLoadTypes();
	LoadUnitTestTypes();
	TEST_ASSERT(GetGlobalTypeManager()->Load());
}

void UnloadTypes()
{
	TEST_ASSERT(GetGlobalTypeManager()->Unload());
	TEST_ASSERT(ResetGlobalTypeManager());
}

int wmain(int argc, wchar_t* argv[])
{
	workflowTable = 0;
	_CrtDumpMemoryLeaks();
	return 0;
}