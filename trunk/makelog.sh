svn log -r head:$1 -v --xml | svn2og.py -p /trunk -o Log
