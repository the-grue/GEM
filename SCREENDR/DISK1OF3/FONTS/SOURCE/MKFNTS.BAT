vfntcnv 8x8.ful 8x8ful.hdr 8x8.a86
vfntcnv 8x14.ful 8x14ful.hdr 8x14.a86
vfntcnv 8x16.ful 8x16ful.hdr 8x16.a86
rasm86 8x8 $pzsz
copy 8x8.obj ..\obj
del 8x8.obj
rasm86 8x14 $pzsz
copy 8x14.obj ..\obj
del 8x14.obj
rasm86 8x16 $pzsz
copy 8x16.obj ..\obj
del 8x16.obj
