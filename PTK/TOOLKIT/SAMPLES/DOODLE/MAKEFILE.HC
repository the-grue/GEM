doodle.app: doodle.obj
   link /SEGMENTS:1024 doodle.obj, doodle.app, nul, bhcgem.lib hcbe.lib

doodle.obj: doodle.c
   hc doodle -pr \toolkit\gemlib\gemlib.pro
