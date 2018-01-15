#!/bin/bash

PKG=mjutil

WD=$(dirname $0)
. /etc/os-release
dist=$ID$VERSION_ID
codename=$VERSION_CODENAME
[ -z "$codename" ] && codename=$(echo $VERSION | sed 's/.*(\(.*\)).*/\1/')

gitdescr=$(git describe)
gitversion=$(cut -f1 -d';' <<< ${gitdescr/-/;})
if [ ${gitversion} == ${gitdescr} ]; then
  gitrelease=1
else
  gitrelease=$(cut -f2 -d';' <<< ${gitdescr/-/;})
fi
gitrelease=${gitrelease/-/.}.$dist
gitlog=$(git log -n 1 --oneline)
gitname=$(git config --get user.name)
gitemail=$(git config --get user.email)
gitdate=$(date -R)

# copy source tree with version in directory name for deb build
rm -rf $WD/../../${PKG}-*
mkdir -p $WD/../../${PKG}-${gitversion}/debian
rsync -aP $WD/../../${PKG}/ $WD/../../${PKG}-${gitversion}/

cat > $WD/../../${PKG}-${gitversion}/debian/changelog <<EOF
$PKG (${gitversion}-${gitrelease}) $codename; urgency=low

  * ${gitlog}

 -- ${gitname} <${gitemail}>  ${gitdate}
EOF

cp -r $WD/deb/*.ex $WD/deb/{compat,control,copyright,debhelper-build-stamp,docs,rules,source} $WD/../../${PKG}-${gitversion}/debian

# update files to make deb build process with source files happy
$WD/../mjcommon/update_version.sh $WD/../mjcommon
tar -C $WD/../.. -czf $WD/../../${PKG}_${gitversion}.orig.tar.gz ${PKG}

cd $WD/../../${PKG}-${gitversion}
dpkg-buildpackage -S

