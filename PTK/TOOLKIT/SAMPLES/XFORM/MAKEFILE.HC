xform.app: xform.obj
   link /SEGMENTS:1024 xform.obj, xform.app, nul, bhcgem.lib hcbe.lib

xform.obj: xform.c
   hc xform -pr \toolkit\gemlib\gemlib.pro
