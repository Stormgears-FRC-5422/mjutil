#!/bin/bash

WD=$(dirname $0)
. /etc/os-release
dist=$ID$VERSION_ID

gitdescr=$(git describe)
gitversion=$(cut -f1 -d';' <<< ${gitdescr/-/;})
gitrelease=$(cut -f2 -d';' <<< ${gitdescr/-/;})
gitrelease=${gitrelease/-/_}_$dist

echo version $gitversion
echo release $gitrelease
