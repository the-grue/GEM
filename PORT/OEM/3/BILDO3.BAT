REM FILENAME BILDO3.BAT FUNCTION: BUILD & FORMAT 3.5" DISKETTE
C:
C:\DSKFMT
CD SCRNDRV
COPY @OEM3A.PRN A:
CD ..
CD MINCE
COPY @OEM3B.PRN A:
CD ..
COPY MNCE.SWP A:
CD ..
CD SYSBLD
COPY @OEM3C.PRN A:
CD ..
CD DUPINFO
COPY @OEM3D.PRN A:
CD ..
COPY C:\RELNOTES.PRN A:
REN A:INSTO3.BAT A:INSTALL.BAT
REM TRANSFER COMPLETED; ENTER THE APPLE KEY AND NUMERIC ENTER KEY TO
REM EJECT DISKETTE;
REM LABEL DISKETTE WITH A ORANGE COLORED DISKETTE PAPER LABEL;
REM LABEL DISKETTE WITH A PRINTED LABEL FOR OEM , DISKETTE # 3
