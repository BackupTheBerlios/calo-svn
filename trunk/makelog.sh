svn log -r head:$1 -v --xml | svn2log.py -p /trunk -o Log
