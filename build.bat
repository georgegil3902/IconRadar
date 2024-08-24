@echo off

rem Create build directory if it doesnt exist yet
mkdir build

rem Navigate to the build directory
cd build

rem Run CMake to configure the project
cmake .. -DCMAKE_INSTALL_PREFIX=Z:/Magna/IconRadar/build/install

rem Build the project
cmake --build .

rem Install the project (copies files to the bin directory)
cmake --install . --config Debug

rem Run the application
.\install\bin\IconRadar.exe
