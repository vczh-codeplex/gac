cd ..\..\..\Common\Tools
call BuildTools.bat
cd ..\..\Libraries\GacUI\Public
copy ..\..\..\Common\Tools\GacGen.exe Source\GacGen.exe