@echo OFF

cmake -G"Visual Studio" -Ax64 -DCMAKE_PREFIX_PATH=C:/Qt/5.14.1/msvc2017_64/lib/cmake ..

cmake --build .

REM windeployqt --qmldir ../src/qml_files/ Skribble.exe

pause