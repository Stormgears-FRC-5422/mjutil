#!/bin/bash
# make icns files needed by Mac applications in this project

RESOURCES=../resources

mk_icns() {
  name=$1
  dir=$2

  rm -rf /tmp/${name}.iconset
  mkdir  /tmp/${name}.iconset

  sips -z 16 16     $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_16x16.png
  sips -z 32 32     $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_16x16@2x.png
  sips -z 32 32     $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_32x32.png
  sips -z 64 64     $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_32x32@2x.png
  sips -z 128 128   $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_128x128.png
  sips -z 256 256   $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_128x128@2x.png
  sips -z 256 256   $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_256x256.png
  sips -z 512 512   $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_256x256@2x.png
  sips -z 512 512   $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_512x512.png
  sips -z 1024 1024 $RESOURCES/${name}.png --out /tmp/${name}.iconset/icon_512x512@2x.png

  cp /tmp/${name}.iconset/icon_256x256.png $dir/icon.png

  iconutil -c icns /tmp/${name}.iconset

  mv /tmp/${name}.icns $dir
}

mk_icns mjplay ../mjplayer/images
mk_icns mjview ../mjviewer/images
mk_icns mjutil_install .
