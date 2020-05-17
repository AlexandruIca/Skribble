![CI](https://github.com/AlexandruIca/Skribble/workflows/CI/badge.svg)
![WinDeploy](https://github.com/AlexandruIca/Skribble/workflows/WinDeploy/badge.svg)

# Skribble

Collaborative painting app made with Qt!

![Skribble GIF](media/Skribble.gif)

You can check the [features used](https://github.com/AlexandruIca/Skribble/blob/master/FEATURES_USED.md)
to see the requirements met as part of the homework.

# Documentation

Check it out [here](https://alexandruica.github.io/Skribble/).

# Build

First you must install [Qt](https://www.qt.io/download-qt-installer?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4)
and [CMake](https://cmake.org/download/).

For the general case:
```sh
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=path/to/Qt/5.x.x/lib/cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
And then you can run Skribble. Additionally on windows you also have to go where
`Skribble.exe` is and run:
```sh
path/to/Qt/5.x.x/bin/windeployqt --qmldir src/qml/ Skribble.exe
```

To build for development purpose, check out the documentation.
