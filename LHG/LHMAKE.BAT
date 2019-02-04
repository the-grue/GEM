echo off
cls
masm LHgem,,NUL,NUL
link LHgem,,NUL,,
del lhgem.obj
del lhgem.bak
rem
masm HIgem,,NUL,NUL
link HIgem,,NUL,,
del higem.obj
del higem.bak
