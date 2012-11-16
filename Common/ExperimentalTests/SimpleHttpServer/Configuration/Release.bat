mkdir _DeployServiceFolder
cd _DeployServiceFolder
del *.* /Q
cd..

mkdir _DeployServerBinaries
cd _DeployServerBinaries
del *.* /Q
cd..

mkdir _DeployClientBinaries
cd _DeployClientBinaries
del *.* /Q
cd..

cd _DeployServiceFolder
copy ..\ServiceFolder\ServiceConfiguration.xml ServiceConfiguration.xml
copy ..\..\HelloWorldTestServer\bin\Release\*.exe *.exe
copy ..\..\HelloWorldTestServer\bin\Release\*.dll *.dll
cd..

cd _DeployServerBinaries
copy ..\StorageConfiguration.xml StorageConfiguration.xml
copy ..\..\DeployServer\bin\Release\*.exe *.exe
copy ..\..\DeployServer\bin\Release\*.dll *.dll
cd..

cd _DeployClientBinaries
copy ..\StorageConfiguration.xml StorageConfiguration.xml
copy ..\..\DeployClient\bin\Release\*.exe *.exe
copy ..\..\DeployClient\bin\Release\*.dll *.dll
mkdir ServiceFolder
cd ServiceFolder
del *.* /Q
copy ..\..\_DeployServiceFolder\*.* *.*
cd..
cd..