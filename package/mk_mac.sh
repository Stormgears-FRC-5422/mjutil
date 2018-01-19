#!/bin/bash

PKG=mjutil

WD=$(dirname $0)

gitdescr=$(git describe)
gitversion=$(cut -f1 -d';' <<< ${gitdescr/-/;})

# go to top-level build directory
cd $WD/..
rm -rf app/mjutil.app
rm -f app/*.dmg
make clean
qmake ${PKG}.pro
make -j4
cd app
macdeployqt ${PKG}.app -verbose=3
install -m 0755 ../mjcommon/libmjcommon.1.dylib ${PKG}.app/Contents/Frameworks/
install_name_tool -change libmjcommon.1.dylib @executable_path/../Frameworks/libmjcommon.1.dylib ${PKG}.app/Contents/MacOS/${PKG}
macdeployqt ${PKG}.app -dmg -verbose=3
mv ${PKG}.dmg ${PKG}-${gitversion}.dmg

