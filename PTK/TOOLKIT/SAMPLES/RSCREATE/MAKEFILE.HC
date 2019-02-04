rscreate.app: rscreate.obj
   link /SEGMENTS:1024 rscreate.obj, rscreate.exe, nul, bhcgem.lib hcbe.lib

rscreate.obj: rscreate.c
   hc rscreate -pr \toolkit\gemlib\gemlib.pro
