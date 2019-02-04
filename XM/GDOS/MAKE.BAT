rasm86 gdos
lc1 ch -s
lc2 ch -scode -v
rasm86 chasm
lc1 sw -s
lc2 sw -scode -v
lc1 mn -s
lc2 mn -scode -v
rasm86 cutasm
lc1 doscut -s
lc2 doscut -scode -v
lc1 dospaste -s
lc2 dospaste -scode -v
lc1 tblparse -s
lc2 tblparse -scode -v
rasm86 stealkbd
lc1 gemdos -s
lc2 gemdos -scode -v
rasm86 dosasm
rasm86 large
rasm86 cxc33
rasm86 ints
rasm86 load
link86 gemvdi[nop]=gdos,ch,chasm,sw,mn,cutasm,doscut,dospaste,tblparse,stealkbd,gemdos,dosasm,large,cxc33,ints,load

