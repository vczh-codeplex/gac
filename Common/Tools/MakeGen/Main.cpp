#include "../../../Libraries/GacUI/Public/Source/Vlpp.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;
using namespace vl::console;

Regex regexInclude(LR"r(^include$)r");
Regex regexIncludeBody(LR"r(^\t(<arg>/.*)$)r");
Regex regexTargets(LR"r(^targets$)r");
Regex regexTargetsBody(LR"r(^\t(<name>/w+)/s+=/s+(<arg>/.*)$)r");
Regex regexFolder(LR"r(^folder/s+(<arg>/.*)$)r");
Regex regexFolderBody(LR"r(^\t(<name>/w+)/s+=/s+(<arg>/.*)$)r");
Regex regexGroup(LR"r(^group/s+(<arg>/.*)$)r");
Regex regexDependency(LR"r(^dependency$)r");
Regex regexDependencyBody(LR"r(^\t(<folder1>/w+):(<category1>/w+)/s+/</s+(<folder2>/w+):(<category2>/w+)$)r");
Regex regexMap(LR"r(^map/s+(<category>/w+)/s+:/s+(<pattern>/s*)$)r");
Regex regexLink(LR"r(^link/s+(<category>/w+)/s+:/s+(<pattern>/s*)$)r");
Regex regexOutput(LR"r(^\t/>/s+(<category>/w+)/s+:/s+(<target>/w+)/s+(<pattern>/.*)$)r");
Regex regexCommand(LR"r(^\t(<command>/.*)$)r");

WString GetFolderName(const WString& fileName)
{
	int index=fileName.Length()-1;
	while(index>=0)
	{
		if (fileName[index]==L'/') break;
	}
	return index==-1 ? L"" : fileName.Left(index+1);
}

WString GetFileName(const WString& fileName)
{
	int index=fileName.Length()-1;
	while(index>=0)
	{
		if (fileName[index]==L'/') break;
	}
	return index==-1 ? fileName : fileName.Right(fileName.Length()-index-1);
}

class FolderConfig
{
public:
	WString									name;
	List<WString>							subFolders;
	Dictionary<WString, WString>			categories;
};

struct DependencyItem
{
	WString									folder;
	WString									category;

	static vint Compare(const DependencyItem& a, const DependencyItem& b)
	{
		vint result = WString::Compare(a.folder, b.folder);
		if (result!=0) return result;
		result = WString::Compare(a.category, b.category);
		return result;
	}

	bool operator == (const DependencyItem& i)const{return Compare(*this, i) == 0;}
	bool operator != (const DependencyItem& i)const{return Compare(*this, i) != 0;}
	bool operator <  (const DependencyItem& i)const{return Compare(*this, i) <  0;}
	bool operator <= (const DependencyItem& i)const{return Compare(*this, i) <= 0;}
	bool operator >  (const DependencyItem& i)const{return Compare(*this, i) >  0;}
	bool operator >= (const DependencyItem& i)const{return Compare(*this, i) >= 0;}
};

class BuildingOutputConfig
{
public:
	WString										outputCategory;
	WString										target;
	WString										outputPattern;
};

class BuildingConfig
{
public:
	bool										aggregation = false;
	List<WString>								inputCatregories;
	WString										inputPattern;
	List<WString>								commands;
	List<Ptr<BuildingOutputConfig>>				outputs;
};

class MakeGenConfig
{
public:
	Dictionary<WString, WString>				targets;
	Dictionary<WString, Ptr<FolderConfig>>		folders;
	Dictionary<DependencyItem, DependencyItem>	dependencies;
	List<Ptr<BuildingConfig>>					buildings;
};

bool LoadMakeGen(Ptr<MakeGenConfig> config, const WString& fileName)
{
}

int main(int argc, char* argv[])
{
	if (argc < 2 || argc > 3)
	{
		Console::WriteLine(L"MakeGen <input-makegen>");
		Console::WriteLine(L"\tConvert \"makefile.<input-makegen>.makegen\" to \"makefile\".");
		Console::WriteLine(L"MakeGen <input-makegen> <output-makefile>");
		Console::WriteLine(L"\tConvert \"makefile.<input-makegen>.makegen\" to \"<output-makefile>\".");
		return 1;
	}
	WString input = L"./makefile."+atow(argv[1])+L".makegen";
	WString output = argc==3 ? atow(argv[2]) : L"./makefile";
	Console::WriteLine(L"Converting \""+input+L"\" to \""+output+L"\" ...");
	return 0;
}
