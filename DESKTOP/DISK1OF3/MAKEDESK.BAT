rem ************************************************************
rem ** This file uses RASM86 and the MetaWare High-C compiler
rem ** to build Desktop v3.0.
rem ************************************************************
pause
rasm86 deskstar $pzsz
rasm86 deskosif $pzsz
rasm86 gsx2 $pzsz
rasm86 longasm $pzsz
rasm86 rtlasm $pzsznc
rasm86 proend $pzsz
rasm86 optimopt $pzsz
hc desktop
hc desksupp
hc deskrsrc
hc deskapp
hc deskact
hc deskfun
hc deskdir
hc deskinf
hc deskins
hc deskwin
hc deskobj
hc deskfpd
hc deskpro
hc deskif
hc deskgraf
hc deskgsx
hc gembind
hc gemdos
hc optimize
hc deskglob
hc hcsmall
link86 desktop[i]
