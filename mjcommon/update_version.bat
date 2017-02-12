cd %1
for /f "delims=" %%i in ('git describe') do set ver=%%i
del /q version.h
echo #ifndef MJ_VERSION >> version.h
echo #define MJ_VERSION "%ver%" >> version.h
echo #endif >> version.h
