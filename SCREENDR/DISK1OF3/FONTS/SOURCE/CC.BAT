hc %1
if errorlevel 1 goto exit
plink86 @%1
pmapsym <%1.map >x.sym
:exit

