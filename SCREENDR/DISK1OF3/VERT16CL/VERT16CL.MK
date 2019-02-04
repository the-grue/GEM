#-----------------------------------------------------------------------
# 9-10-87 jn
# make file for the GEM 2.3 Verticom 16 color screen driver
# (This is the same driver as the 16 color VGA driver.)
#-----------------------------------------------------------------------

finished_driver = sdpsc8.vga
devdep_a86_file = verticmf
link_file = verticmf
font_file = 8x16
pattern_file = hirespat
home_directory = vert16cl
extension1 = vrf
extension2 = ehc
rasm86_switches = $pzsz

.\obj\zyxg.obj: ..\common\zyxg.a86      \
                .\source\names.$(extension1)
        copy .\source\names.$(extension1) ..\common\names.inc
        cd ..\common
          rasm86 zyxg $(rasm86_switches)
          copy zyxg.obj ..\$(home_directory)\obj
          del names.inc
          del zyxg.obj
        cd ..\$(home_directory)

.\obj\entry.obj:        ..\common\entry.a86     \
                        .\source\externs.$(extension1)
        copy .\source\externs.$(extension1) ..\common\externs.a86
        cd ..\common
          rasm86 entry $(rasm86_switches)
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
                        ..\common\gsxdef.$(extension2)
        copy ..\common\gsxdef.$(extension2) ..\common\gsxdef.h
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
                        ..\common\gsxdef.$(extension2)
        copy ..\common\gsxdef.$(extension2) ..\common\gsxdef.h
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
                        ..\common\gsxdef.$(extension2)
        copy ..\common\gsxdef.$(extension2) ..\common\gsxdef.h
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

.\obj\eopttxt1.obj:     ..\common\eopttxt1.a86  \
                        .\source\externs.$(extension1)
        copy .\source\externs.$(extension1) ..\common\externs.a86
        cd ..\common
          rasm86 eopttxt1 $(rasm86_switches)
          copy eopttxt1.obj .\obj
          copy eopttxt1.obj ..\$(home_directory)\obj
          del eopttxt1.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\eopttxt2.obj:     ..\common\eopttxt2.a86  \
                        .\source\externs.$(extension1)
        copy .\source\externs.$(extension1) ..\common\externs.a86
        cd ..\common
          rasm86 eopttxt2 $(rasm86_switches)
          copy eopttxt2.obj ..\$(home_directory)\obj
          del eopttxt2.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\eoptdraw.obj:     ..\common\eoptdraw.a86  \
                        .\source\externs.$(extension1)
        copy .\source\externs.$(extension1) ..\common\externs.a86
        cd ..\common
          rasm86 eoptdraw $(rasm86_switches)
          copy eoptdraw.obj ..\$(home_directory)\obj
          del eoptdraw.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\egamblit.obj:     ..\common\egamblit.a86  \
                        .\source\externs.$(extension1)
        copy .\source\externs.$(extension1) ..\common\externs.a86
        cd ..\common
          rasm86 egamblit $(rasm86_switches)
          copy egamblit.obj ..\$(home_directory)\obj
          del egamblit.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\egablit.obj:      ..\common\egablit.a86   \
                        .\source\externs.$(extension1)
        copy .\source\externs.$(extension1) ..\common\externs.a86
        cd ..\common
          rasm86 egablit $(rasm86_switches)
          copy egablit.obj ..\$(home_directory)\obj
          del egablit.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\$(pattern_file).obj:      ..\common\$(pattern_file).a86   \
                                .\source\externs.$(extension1)
        copy .\source\externs.$(extension1) ..\common\externs.a86
        cd ..\common
          rasm86 $(pattern_file) $(rasm86_switches)
          copy $(pattern_file).obj ..\$(home_directory)\obj
          del $(pattern_file).obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\enddseg.obj:      ..\common\enddseg.a86
        cd ..\common
          rasm86 enddseg $(rasm86_switches)
          copy enddseg.obj ..\$(home_directory)\obj
          del enddseg.obj
          del externs.a86
        cd ..\$(home_directory)

.\obj\$(devdep_a86_file).obj:   .\source\$(devdep_a86_file).a86 \
                        .\source\externs.$(extension1)  \
                        ..\common\devdata.$(extension2) \
                        ..\common\ibmmdvsp.a86  \
                        ..\common\imouse.a86    \
                        ..\common\egammre1.a86  \
                        ..\common\egammre2.a86
        cd source
          copy ..\..\common\ibmmdvsp.a86 .
          copy ..\..\common\imouse.a86 .
          copy ..\..\common\egammre1.a86 .
          copy ..\..\common\egammre2.a86 .
          copy ..\..\common\devdata.$(extension2) *.a86
          copy externs.$(extension1) externs.a86
          rasm86 $(devdep_a86_file) $(rasm86_switches)
          copy $(devdep_a86_file).obj ..\obj
          del $(devdep_a86_file).obj
          del externs.a86
          del devdata.a86
          del ibmmdvsp.a86
          del imouse.a86
          del egammre1.a86
          del egammre2.a86
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
                .\obj\eopttxt1.obj      \
                .\obj\eopttxt2.obj      \
                .\obj\eoptdraw.obj      \
                .\obj\egamblit.obj      \
                .\obj\egablit.obj       \
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

