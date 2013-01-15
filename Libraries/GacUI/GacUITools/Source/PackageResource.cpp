#include "PackageResource.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
Helper Functions
***********************************************************************/

		struct EnumeratePackageResourceStruct
		{
			List<Ptr<XmlDocument>>&				packageResources;
			Ptr<ParsingTable>					xmlParserTable;

			EnumeratePackageResourceStruct(List<Ptr<XmlDocument>>& _packageResources)
				:packageResources(_packageResources)
			{
				xmlParserTable=XmlLoadTable();
			}
		};

		BOOL CALLBACK EnumeratePackageResourceProc(
		  _In_opt_  HMODULE hModule,
		  _In_      LPCTSTR lpszType,
		  _In_      LPTSTR lpszName,
		  _In_      LONG_PTR lParam
		)
		{
			EnumeratePackageResourceStruct* arguments=(EnumeratePackageResourceStruct*)lParam;

			WString resourceString;
			{
				HRSRC res=FindResource(hModule, lpszName, lpszType);
				DWORD size=SizeofResource(hModule, res);
				HGLOBAL hglobal=LoadResource(hModule, res);
				void* buffer=LockResource(hglobal);
				MemoryWrapperStream bufferStream(buffer, size);
				Utf8Decoder decoder;
				DecoderStream decoderStream(bufferStream, decoder);
				StreamReader reader(decoderStream);
				resourceString=reader.ReadToEnd();
				FreeResource(hglobal);
			}
			{
				Ptr<XmlDocument> document=XmlParseDocument(resourceString, arguments->xmlParserTable);
				if(document)
				{
					arguments->packageResources.Add(document);
				}
			}
			return TRUE;
		}

		BOOL CALLBACK EnumResTypeProc(
		  _In_opt_  HMODULE hModule,
		  _In_      LPTSTR lpszType,
		  _In_      LONG_PTR lParam
		)
		{
			return TRUE;
		}

		void EnumeratePackageResources(List<Ptr<XmlDocument>>& packageResources)
		{
			EnumeratePackageResourceStruct arguments(packageResources);
			EnumResourceTypes(NULL, &EnumResTypeProc, NULL);
			EnumResourceNames(NULL, L"GacUIToolsPackages", &EnumeratePackageResourceProc, (LONG_PTR)&arguments);
		}
	}
}