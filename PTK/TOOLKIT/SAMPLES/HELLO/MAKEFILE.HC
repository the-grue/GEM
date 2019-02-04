hello.app: hello.obj
   link /SEGMENTS:1024 hello.obj, hello.app, nul, bhcgem.lib hcbe.lib

hello.obj: hello.c
   hc hello -pr \toolkit\gemlib\gemlib.pro
