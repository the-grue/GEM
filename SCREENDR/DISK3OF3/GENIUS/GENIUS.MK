#-----------------------------------------------
# 9-3-87 jn
# make file for the GEM 2.3 genius screen driver
#-----------------------------------------------

finished_driver = sdgen8.vga
devdep_a86_file = genius
link_file = genius4
font_file = 8x16
pattern_file = hirespat
home_directory = genius
extension = gen
rasm_switches = $pzsz

.\obj\zyxg.obj: ..\common\zyxg.a86      \
                .\source\names.$(extension)
        copy .\source\names.$(extension) ..\common\names.inc
        cd ..\common
          rasm86 zyxg $(rasm_switches)
          copy zyxg.obj ..\$(home_directory)\obj
          del names.inc
          del zyxg.obj
        cd ..\$(home_directory)

.\obj\entry.obj:        ..\common\entry.a86     \
                        .\source\externs.$(extension)
        copy .\source\externs.$(extension) ..\common\externs.a86
        cd ..\common
          rasm86 entry $(rasm_switches)
          copy entry.obj ..\$(home_directory)\obj
          del entry.obj
        cd ..\$(home_directory)


.\obj\jmptbl.obj:       ..\common\jmptbl.c      \
                        ..\common\portab.h      \
                        ..\common\jmptbl.h
        cd ..\common
          lc1 jmptbl
          lc2 jmptbl -sCODE -v 
          copy jmptbl.obj ..\$(home_directory)\obj
          del jmptbl.obj
        cd ..\$(home_directory)


.\obj\monobj.obj:       ..\common\monobj.c      \
                        ..\common\portab.h      \
                        .\source\gsxdef.$(extension)
        copy source\gsxdef.$(extension) ..\common\gsxdef.h
        cd ..\common
          lc1 monobj
          lc2 monobj -sCODE -v 
          copy monobj.obj ..\$(home_directory)\obj
          del monobj.obj
          del gsxdef.h
        cd ..\$(home_directory)

.\obj\monout.obj:       ..\common\monout.c      \
                        ..\common\portab.h      \
                        ..\common\gsxextrn.h    \
                        .\source\gsxdef.$(extension)
        copy source\gsxdef.$(extension) ..\common\gsxdef.h
        cd ..\common
          lc1 monout
          lc2 monout -sCODE -v 
          copy monout.obj ..\$(home_directory)\obj
          del monout.obj
          del gsxdef.h
        cd ..\$(home_directory)

.\obj\opttext.obj:      ..\common\opttext.c     \
                        ..\common\portab.h      \
                        ..\common\fontdef.h     \
                        ..\common\gsxextrn.h    \
                        .\source\gsxdef.$(extension)
        copy source\gsxdef.$(extension) ..\common\gsxdef.h
        cd ..\common
          lc1 opttext -mD -s
          lc2 opttext -sCODE -v 
          copy opttext.obj ..\$(home_directory)\obj
          del opttext.obj
          del gsxdef.h
        cd ..\$(home_directory)

.\obj\isin.obj:         ..\common\isin.c        \
                        ..\common\portab.h
        cd ..\common
          lc1 isin
          lc2 isin -sCODE -v 
          copy isin.obj ..\$(home_directory)\obj
          del isin.obj
        cd ..\$(home_directory)

.\obj\dummy.obj:        ..\common\dummy.c       \
                        ..\common\portab.h
        cd ..\common
          lc1 dummy
          lc2 dummy -sCODE -v 
          copy dummy.obj ..\$(home_directory)\obj
          del dummy.obj
        cd ..\$(home_directory)

.\obj\opttxt1.obj:      ..\common\opttxt1.a86   \
                        .\source\externs.$(extension)
        copy .\source\externs.$(extension) ..\common\externs.a86
        cd ..\common
          rasm86 opttxt1 $(rasm_switches)
          copy opttxt1.obj .\obj
          copy opttxt1.obj ..\$(home_directory)\obj
          del opttxt1.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\opttxt2.obj:      ..\common\opttxt2.a86   \
                        .\source\externs.$(extension)
        copy .\source\externs.$(extension) ..\common\externs.a86
        cd ..\common
          rasm86 opttxt2 $(rasm_switches)
          copy opttxt2.obj ..\$(home_directory)\obj
          del opttxt2.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\opttdraw.obj:     ..\common\opttdraw.a86  \
                        .\source\externs.$(extension)
        copy .\source\externs.$(extension) ..\common\externs.a86
        cd ..\common
          rasm86 opttdraw $(rasm_switches)
          copy opttdraw.obj ..\$(home_directory)\obj
          del opttdraw.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\monspblt.obj:     ..\common\monspblt.a86  \
                        .\source\externs.$(extension)
        copy .\source\externs.$(extension) ..\common\externs.a86
        cd ..\common
          rasm86 monspblt $(rasm_switches)
          copy monspblt.obj ..\$(home_directory)\obj
          del monspblt.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\rastop.obj:       ..\common\rastop.a86    \
                        .\source\externs.$(extension)
        copy .\source\externs.$(extension) ..\common\externs.a86
        cd ..\common
          rasm86 rastop $(rasm_switches)
          copy rastop.obj ..\$(home_directory)\obj
          del rastop.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\$(pattern_file).obj:      ..\common\$(pattern_file).a86   \
                                .\source\externs.$(extension)
        copy .\source\externs.$(extension) ..\common\externs.a86
        cd ..\common
          rasm86 $(pattern_file) $(rasm_switches)
          copy $(pattern_file).obj ..\$(home_directory)\obj
          del $(pattern_file).obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\enddseg.obj:      ..\common\enddseg.a86
        cd ..\common
          rasm86 enddseg $(rasm_switches)
          copy enddseg.obj ..\$(home_directory)\obj
          del enddseg.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\$(devdep_a86_file).obj:   .\source\$(devdep_a86_file).a86 \
                        .\source\externs.$(extension)   \
                        .\source\devdata.$(extension)   \
                        ..\common\ibmmdvsp.a86  \
                        ..\common\imouse.a86    \
                        ..\common\monmmre1.a86  \
                        ..\common\monmmre2.a86
        cd source
          copy ..\..\common\ibmmdvsp.a86 .
          copy ..\..\common\imouse.a86 .
          copy ..\..\common\monmmre1.a86 .
          copy ..\..\common\monmmre2.a86 .
          copy devdata.$(extension) *.a86
          copy externs.$(extension) externs.a86
          rasm86 $(devdep_a86_file) $(rasm_switches)
          copy $(devdep_a86_file).obj ..\obj
          del $(devdep_a86_file).obj
          del externs.a86
          del devdata.a86
          del ibmmdvsp.a86
          del imouse.a86
          del monmmre1.a86
          del monmmre2.a86
        cd ..


.\obj\$(font_file).obj: ..\fonts\obj\$(font_file).obj
        copy ..\fonts\obj\$(font_file).obj .\obj

$(finished_driver): .\obj\zyxg.obj      \
                .\obj\entry.obj         \
                .\obj\jmptbl.obj        \
                .\obj\monobj.obj        \
                .\obj\monout.obj        \
                .\obj\opttext.obj       \
                .\obj\isin.obj          \
                .\obj\dummy.obj         \
                .\obj\opttxt1.obj       \
                .\obj\opttxt2.obj       \
                .\obj\opttdraw.obj      \
                .\obj\monspblt.obj      \
                .\obj\rastop.obj        \
                .\obj\$(pattern_file).obj       \
                .\obj\enddseg.obj       \
                .\obj\$(font_file).obj          \
                .\obj\$(devdep_a86_file).obj    \
                .\source\$(link_file).inp
        copy .\source\$(link_file).inp .\obj\.
        cd obj
          link86 $(link_file)[i
          copy $@ ..\.
          del $@
          del $(link_file).inp
        cd ..

