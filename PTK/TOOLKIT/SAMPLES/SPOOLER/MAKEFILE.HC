spooler.app: spooler.obj
   link /SEGMENTS:1024 spooler.obj, spooler.app, nul, bhcgem.lib hcbe.lib

spooler.obj: spooler.c
   hc spooler -pr \toolkit\gemlib\gemlib.pro
