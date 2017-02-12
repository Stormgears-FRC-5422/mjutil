TEMPLATE = subdirs
SUBDIRS = mjcommon app mjmkindex

app.depends = mjcommon
mjmkindex.depends = mjcommon


