@echo OFF

cmake -G"Visual Studio" -Ax64 -DCMAKE_PREFIX_PATH=C:/Qt/5.14.1/ ..

cmake --build .

REM windeployqt --qmldir ../src/qml_files/ Skribble.exe
