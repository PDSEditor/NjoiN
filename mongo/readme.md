# Installing
1. install mongo-c-driver using visual studio 16 2019 adding 
	* -DBUILD_VERSION=1.16.2
2. aprire ALL_BUILD.vcxproj in visual studio. build->build solution
3. aprire INSTALL.vcxproj e fare lo stesso
4. cmake -G "Visual Studio 16 2019" -DCMAKE_INSTALL_PREFIX=C:\mongo-c-driver" -DCMAKE_PREFIX_PATH=C:\mongo-c-driver" -DBUILD_VERSION=1.16.2 ..