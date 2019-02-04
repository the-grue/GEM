erase output.app
erase output.lst
set OBJ=\OBJ\COMPACT\ASM;\OBJ\COMPACT\LIB;\OBJ\COMPACT\C
set comspec=c:\command.com
plink86 @output > olink.err
type olink.err
pause copy to \GEMSYS?
msym < output.lst > output.sym
pause run GEM??
gem
