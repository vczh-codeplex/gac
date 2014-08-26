#include "../../../Libraries/GacUI/Public/Source/Vlpp.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;

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

int main()
{
	return 0;
}
