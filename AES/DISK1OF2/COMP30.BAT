hc GEMAPLIB
hc GEMASYNC
hc GEMBASE
hc GEMCTRL
hc GEMDISP
hc GEMDOS
hc GEMEVLIB
hc GEMFLAG 
hc GEMFMALT
hc GEMFMLIB
hc GEMFSLIB
hc GEMGLOBE
hc GEMGRAF 
hc GEMGRLIB 
hc GEMGSXIF 
hc GEMINIT -tpages 150 
hc GEMINPUT 
hc GEMMNLIB 
hc GEMOBED  
hc GEMOBLIB 
hc GEMOBJOP
hc GEMPD
hc GEMQUEUE
hc GEMRSLIB 
hc GEMSCLIB -tpages 150 
hc GEMSHLIB 
hc GEMSUPER -tpages 150 
hc GEMWMLIB 
hc GEMWRECT 
hc OPTIMIZE 
hc HCSMALL
rasm86 gemasm $pzsz
rasm86 gemdosif $pzsz
rasm86 gemgmain $pzsz
rasm86 gemstart $pzsz
rasm86 gsx2 $pzsz
rasm86 large $pzsz
rasm86 optimopt $pzsz
rasm86 proend $pzsz
rasm86 rtlasm $pz sz nc
link86 gem30[i
