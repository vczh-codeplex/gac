..\..\..\Common\Tools\Codepack.exe CodegenConfig.xml
mkdir Temp
cd Temp
del *.* /Q
mkdir StaticMetaDoc
mkdir StaticHtmlDoc
copy ..\..\GacUISrc\DebugNoReflection\GacUISrc.xml GacUISrc.xml
copy ..\..\GacUISrc\DebugNoReflection\GacUISrc.pdb GacUISrc.pdb
..\..\..\..\Common\Tools\PdbDump.exe GacUISrc.pdb GacUISrcPdb.xml
..\..\..\..\Common\Tools\LinkPdbXml.exe GacUISrcPdb.xml GacUISrc.xml GacUISrcDoc.xml
..\..\..\..\Common\Tools\GenXmlDocRef.exe GacUISrcDoc.xml . reference:gacui
cd ..
..\..\..\Common\Tools\GenDocIndex.exe meta Temp\StaticMetaDoc Temp DocumentRawItems
..\..\..\Common\Tools\GenDocIndex.exe html Temp\StaticHtmlDoc Temp DocumentRawItems
..\..\..\Common\Tools\Recurcpy.exe Source										Temp\ReleaseFolder\Public\Source		include cpp h
..\..\..\Common\Tools\Recurcpy.exe Temp\StaticHtmlDoc							Temp\ReleaseFolder\Public\Document		include html
..\..\..\Common\Tools\Recurcpy.exe ..\GaclibServices\GaclibWebPage\Content\		Temp\ReleaseFolder\Public\Content		include css gif
..\..\..\Common\Tools\Recurcpy.exe ..\GacUIDemo									Temp\ReleaseFolder\GacUIDemo			include sln vssscc vcxproj filters vspscc cpp h bmp jpg gif png ico txt xml
pause