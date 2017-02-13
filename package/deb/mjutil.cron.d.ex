#
# Regular cron jobs for the mjutil package
#
0 4	* * *	root	[ -x /usr/bin/mjutil_maintenance ] && /usr/bin/mjutil_maintenance
