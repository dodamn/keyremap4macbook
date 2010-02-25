#!/bin/sh

basedir=`dirname $0`
version=$(awk -F'-' '{print $1}' "$basedir/../version")

for f in `find $basedir/.. -name 'Info.plist.tmpl'`; do
    if [ -f "$f" ]; then
        sed "s|PKGVERSION|$version|g" $f > `dirname $f`/`basename $f .tmpl`
    fi
done
