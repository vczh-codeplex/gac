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