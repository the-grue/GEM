source\vfntcnv.exe:	.\source\vfntcnv.obj \
			.\source\hcportab.h
	cd source
	  hc vfntcnv
	  plink86 @vfntcnv.lnk
	cd ..

obj\8x8.obj:	.\source\8x8.ful\
		.\source\6x6.ful\
		.\source\8x8ful.hdr\
		.\source\vfntcnv.exe
	cd source
	  vfntcnv 8x8.ful 8x8ful.hdr 8x8.a86
	  rasm86 8x8 $pzsz
	  copy 8x8.obj ..\obj
	  del 8x8.obj
	  del 8x8.a86
	cd ..

obj\8x14.obj:	.\source\8x14.ful\
		.\source\6x6.ful\
		.\source\8x14ful.hdr\
		.\source\vfntcnv.exe
	cd source
	  vfntcnv 8x14.ful 8x14ful.hdr 8x14.a86
	  rasm86 8x14 $pzsz
	  copy 8x14.obj ..\obj
	  del 8x14.obj
	  del 8x14.a86
	cd ..

obj\8x16.obj:	.\source\8x16.ful\
		.\source\6x6.ful\
		.\source\8x16ful.hdr\
		.\source\vfntcnv.exe
	cd source
	  vfntcnv 8x16.ful 8x16ful.hdr 8x16.a86
	  rasm86 8x16 $pzsz
	  copy 8x16.obj ..\obj
	  del 8x16.obj
	  del 8x16.a86
	cd ..
