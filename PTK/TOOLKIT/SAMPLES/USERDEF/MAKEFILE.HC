userdef.app: userdef.obj
   link /SEGMENTS:1024 userdef.obj, userdef.app, nul, bhcgem.lib hcbe.lib

userdef.obj: userdef.c
   hc userdef -pr \toolkit\gemlib\gemlib.pro
