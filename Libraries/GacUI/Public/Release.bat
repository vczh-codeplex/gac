..\..\..\Common\Tools\Codepack.exe CodegenConfig.xml
mkdir Temp
cd Temp
del *.* /Q
copy ..\..\GacUISrc\Debug\GacUISrc.xml GacUISrc.xml
copy ..\..\GacUISrc\Debug\GacUISrc.pdb GacUISrc.pdb
..\..\..\..\Common\Tools\PdbDump.exe GacUISrc.pdb GacUISrcPdb.xml
..\..\..\..\Common\Tools\LinkPdbXml.exe GacUISrcPdb.xml GacUISrc.xml GacUISrcDoc.xml
..\..\..\..\Common\Tools\GenXmlDocRef.exe GacUISrcDoc.xml . reference:gacui
pause