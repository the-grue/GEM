clock.app: clock.obj
   link /SEGMENTS:1024 clock.obj, clock.app, nul, bhcgem.lib hcbe.lib

clock.obj: clock.c
   hc clock -pr \toolkit\gemlib\gemlib.pro
