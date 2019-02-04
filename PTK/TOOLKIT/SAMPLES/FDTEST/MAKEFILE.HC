fdtest.app: fdtest.obj
   link /SEGMENTS:1024 fdtest.obj, fdtest.app, nul, bhcgem.lib hcbe.lib

fdtest.obj: fdtest.c
   hc fdtest -pr \toolkit\gemlib\gemlib.pro
