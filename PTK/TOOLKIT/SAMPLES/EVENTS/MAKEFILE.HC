.SUFFIXES .obj .c
.c.obj:
   hc $* -pr \toolkit\gemlib\gemlib.pro

OBJ = xmain.obj events.obj window.obj  meta_out.obj meta_t.obj \
      meta_r.obj meta_f.obj

events.app:   $(OBJ)
   link /SEG:1024 $(OBJ), events.app, nul, bhcgem.lib hcbe.lib
