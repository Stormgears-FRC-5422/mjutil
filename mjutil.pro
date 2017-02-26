TEMPLATE = subdirs
SUBDIRS = mjcommon mjplayer mjviewer mjmkindex

mjplayer.depends = mjcommon
mjmkindex.depends = mjcommon
mjviewer.depends = mjcommon


