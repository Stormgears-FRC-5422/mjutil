TEMPLATE = subdirs
SUBDIRS = mjcommon mjplayer mjviewer mjmkindex mjframes mjcut

mjplayer.depends = mjcommon
mjmkindex.depends = mjcommon
mjviewer.depends = mjcommon
mjframes.depends = mjcommon
mjcut.depends = mjcommon


