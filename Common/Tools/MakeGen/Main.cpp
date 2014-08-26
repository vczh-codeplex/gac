#include "../../../Libraries/GacUI/Public/Source/Vlpp.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;
using namespace vl::console;

Regex regexInclude(LR"r(^include$)r");
Regex regexIncludeBody(LR"r(^\t(<path>/.*)$)r");
Regex regexTargets(LR"r(^targets$)r");
Regex regexTargetsBody(LR"r(^\t(<name>/w+)/s*=/s*(<path>/.*)$)r");
Regex regexFolder(LR"r(^folder/s+(<name>/w+)/s*=/s*(<path>/.*)$)r");
Regex regexFolderBody(LR"r(^\t(<category>/w+)/s*=/s*(<pattern>/.*)$)r");
Regex regexGroup(LR"r(^group/s+(<name>/w+)/s*=(/s*(<folder>[^ ]+))+$)r");
Regex regexDependency(LR"r(^dependency$)r");
Regex regexDependencyBody(LR"r(^\t(<folder1>/w+):(<category1>/w+)/s*/<(/s*(<folder2>/w+):(<category2>/w+))+$)r");
Regex regexMap(LR"r(^map(/s+(<category>/w+))+/s*:/s*(<pattern>/s*)$)r");
Regex regexLink(LR"r(^link(/s+(<category>/w+))+/s*:/s*(<pattern>/s*)$)r");
Regex regexOutput(LR"r(^\t/>/s*(<category>/w+)/s*:/s*(<target>/w+)/s+(<pattern>/.*)$)r");
Regex regexCommand(LR"r(^\t(<command>/.*)$)r");

WString GetFolderName(const WString& fileName)
{
	int index=fileName.Length()-1;
	while(index>=0)
	{
		if (fileName[index]==L'/') break;
		index--;
	}
	return index==-1 ? L"" : fileName.Left(index+1);
}

WString ConcatFolder(const WString& first, const WString& second)
{
	if (second.Length()>0 && second[0]==L'/')
	{
		return second;
	}
	else
	{
		return first;
	}
}

WString GetFileName(const WString& fileName)
{
	int index=fileName.Length()-1;
	while(index>=0)
	{
		if (fileName[index]==L'/') break;
		index--;
	}
	return index==-1 ? fileName : fileName.Right(fileName.Length()-index-1);
}

class FolderConfig
{
public:
	WString									name;
	List<WString>							subFolders;
	Group<WString, WString>					categories;
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
	List<WString>								inputCategories;
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

bool LoadMakeGen(Ptr<MakeGenConfig> config, const WString& fileName, SortedList<DependencyItem>& usedDependencies)
{
	FileStream fileStream(fileName, FileStream::ReadOnly);
	Utf8Decoder decoder;
	DecoderStream stream(fileStream, decoder);
	StreamReader reader(stream);

	WString line=reader.ReadLine();
	do
	{
		if(line==L"" || line[0]==L'#')
		{
			if (!reader.IsEnd())
			{
				line=reader.ReadLine();
			}
			continue;
		}

		Ptr<RegexMatch> match;
		if(match = regexInclude.MatchHead(line))
		{
			// include
			//     <path>
			while(!reader.IsEnd())
			{
				line=reader.ReadLine();
				if(line.Length()>0 && line[0]==L'\t')
				{
					if(match = regexIncludeBody.MatchHead(line))
					{
						auto path=match->Groups()[L"path"][0].Value();
						auto includeFileName=ConcatFolder(GetFolderName(fileName), path);
						if (!LoadMakeGen(config, includeFileName, usedDependencies))
						{
							return false;
						}
					}
					else
					{
						Console::WriteLine(L"Failed to read line \""+line+L"\" in file \""+GetFileName(fileName)+L"\".");
						return false;
					}
				}
				else
				{
					break;
				}
			}	
		}
		else if(match = regexTargets.MatchHead(line))
		{
			// targets
			//     <name> = <path>
			//     ...
			while(!reader.IsEnd())
			{
				line=reader.ReadLine();
				if(line.Length()>0 && line[0]==L'\t')
				{
					if(match = regexTargetsBody.MatchHead(line))
					{
						auto name=match->Groups()[L"name"][0].Value();
						auto path=match->Groups()[L"name"][0].Value();
						if(config->targets.Keys().Contains(name))
						{
							Console::WriteLine(L"Found duplicated target \""+name+L"\" in file \""+GetFileName(fileName)+L"\".");
							return false;
						}
						else
						{
							config->targets.Add(name, path);
						}
					}
					else
					{
						Console::WriteLine(L"Failed to read line \""+line+L"\" in file \""+GetFileName(fileName)+L"\".");
						return false;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if(match = regexFolder.MatchHead(line))
		{
			// folder <name> = <path>
			//     <category> = <pattern>
			//     ...
			auto name=match->Groups()[L"name"][0].Value();
			if(config->folders.Keys().Contains(name))
			{
				Console::WriteLine(L"Found duplicated folder \""+name+L"\" in file \""+GetFileName(fileName)+L"\".");
				return false;
			}

			auto folderConfig=MakePtr<FolderConfig>();
			folderConfig->name=name;
			config->folders.Add(name, folderConfig);

			while(!reader.IsEnd())
			{
				line=reader.ReadLine();
				if(line.Length()>0 && line[0]==L'\t')
				{
					if(match = regexFolderBody.MatchHead(line))
					{
						auto category=match->Groups()[L"category"][0].Value();
						auto pattern=match->Groups()[L"pattern"][0].Value();
						if(folderConfig->categories.Keys().Contains(category))
						{
							Console::WriteLine(L"Found duplicated category \""+category+L"\" in folder \""+name+L"\" in file \""+GetFileName(fileName)+L"\".");
							return false;
						}
						else
						{
							folderConfig->categories.Add(category, pattern);
						}
					}
					else
					{
						Console::WriteLine(L"Failed to read line \""+line+L"\" in file \""+GetFileName(fileName)+L"\".");
						return false;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if(match = regexGroup.MatchHead(line))
		{
			// group <name> = <folder> ...
			auto name=match->Groups()[L"name"][0].Value();
			if(config->folders.Keys().Contains(name))
			{
				Console::WriteLine(L"Found duplicated folder \""+name+L"\" in file \""+GetFileName(fileName)+L"\".");
				return false;
			}

			auto folderConfig=MakePtr<FolderConfig>();
			folderConfig->name=name;
			config->folders.Add(name, folderConfig);

			FOREACH(RegexString, folder, match->Groups()[L"folder"])
			{
				folderConfig->subFolders.Add(folder.Value());
			}

			if(!reader.IsEnd())
			{
				line=reader.ReadLine();
			}
		}
		else if(match = regexDependency.MatchHead(line))
		{
			// dependencies
			//     <folder1>:<category1> < <folder2>:<category2>
			while(!reader.IsEnd())
			{
				line=reader.ReadLine();
				if(line.Length()>0 && line[0]==L'\t')
				{
					if(match = regexDependencyBody.MatchHead(line))
					{
						DependencyItem di1, di2;
						di1.folder=match->Groups()[L"folder1"][0].Value();
						di1.category=match->Groups()[L"category1"][0].Value();

						if(usedDependencies.Contains(di1))
						{
							Console::WriteLine(L"Dependency items should be listed in partial order, \""+di1.folder+L":"+di1.category+L"\" has already been dependented before, so it's dependencies cannot be sepcified here, in file \""+GetFileName(fileName)+L"\".");
							return false;
						}
						else if(config->dependencies.Keys().Contains(di1))
						{
							Console::WriteLine(L"Found duplicated dependency specification \""+di1.folder+L":"+di1.category+L"\" in file \""+GetFileName(fileName)+L"\".");
							return false;
						}

						vint count=match->Groups()[L"folder2"].Count();
						for(vint i=0;i<count;i++)
						{
							di2.folder=match->Groups()[L"folder2"][i].Value();
							di2.category=match->Groups()[L"category2"][i].Value();
	
							config->dependencies.Add(di1, di2);
							if(!usedDependencies.Contains(di2))
							{
								usedDependencies.Add(di2);
							}
						}
					}	
					else
					{
						Console::WriteLine(L"Failed to read line \""+line+L"\" in file \""+GetFileName(fileName)+L"\".");
						return false;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if((match = regexMap.MatchHead(line)) || (match = regexLink.MatchHead(line)))
		{
			// map/link <category> ... : <pattern>
			//     > <category> : <target> <pattern>
			//     <command>
			//     ...

			auto buildingConfig=MakePtr<BuildingConfig>();
			config->buildings.Add(buildingConfig);
			buildingConfig->aggregation=line[0]==L'l';
			FOREACH(RegexString, category, match->Groups()[L"category"])
			{
				auto inputCategory=category.Value();
				if(!buildingConfig->inputCategories.Contains(inputCategory))
				{
					buildingConfig->inputCategories.Add(inputCategory);
				}
			}
			buildingConfig->inputPattern=match->Groups()[L"pattern"][0].Value();

			while(!reader.IsEnd())
			{
				line=reader.ReadLine();
				if(line.Length()>0 && line[0]==L'\t')
				{
					if(match = regexOutput.MatchHead(line))
					{
						auto category=match->Groups()[L"category"][0].Value();
						auto target=match->Groups()[L"target"][0].Value();
						auto pattern=match->Groups()[L"pattern"][0].Value();

						auto outputConfig=MakePtr<BuildingOutputConfig>();
						outputConfig->outputCategory=category;
						outputConfig->target=target;
						outputConfig->outputPattern=pattern;
						buildingConfig->outputs.Add(outputConfig);
					}
					else if(match = regexCommand.MatchHead(line))
					{
						auto command=match->Groups()[L"command"][0].Value();
						buildingConfig->commands.Add(command);
					}
					else
					{
						Console::WriteLine(L"Failed to read line \""+line+L"\" in file \""+GetFileName(fileName)+L"\".");
						return false;
					}
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			Console::WriteLine(L"Failed to read line \""+line+L"\" in file \""+GetFileName(fileName)+L"\".");
			return false;
		}
	}while(!reader.IsEnd() || line!=L"");
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

	auto config=MakePtr<MakeGenConfig>();
	SortedList<DependencyItem> usedDependencies;
	if (LoadMakeGen(config, input, usedDependencies))
	{
	}
	return 0;
}
