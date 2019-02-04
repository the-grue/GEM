echo off
if "%OS%"=="CDOS" suspend=on
if "%OS%"=="CDOS386" suspend=on
if "%OS%"=="CPCDOS" suspend=on
cd \gemapps\gemsys
gemvdi %1 %2 %3
echo on
