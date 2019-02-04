memgraf.app: memgraf.obj
   link /SEGMENTS:1024 memgraf.obj, memgraf.app, nul, bhcgem.lib hcbe.lib

memgraf.obj: memgraf.c
   hc memgraf -pr \toolkit\gemlib\gemlib.pro
