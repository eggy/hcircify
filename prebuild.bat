@echo off
git rev-parse --short HEAD >post.tmp
set /p result=<post.tmp

git rev-parse --symbolic-full-name --abbrev-ref HEAD >post.tmp
set /p result2=<post.tmp

set timestamp=%DATE:/=-%@%TIME::=-%
set timestamp=%timestamp: =%

if not exist BUILD.txt >BUILD.txt echo 0
for /f %%x in (BUILD.txt) do (
set /a build=%%x+1
)

echo #pragma once > src/gitversion.h 
echo static const char GitStr[] ="%result%.%result2%";  >> src/gitversion.h
echo static const char GitHash[] ="%result%"; >> src/gitversion.h
echo static const char BuildDate[] = "%timestamp%"; >> src/gitversion.h
echo static const char BuildNumber[] = "%build%"; >> src/gitversion.h

echo #define VER_BUILD %build% >> src/gitversion.h
echo #define VER_HASH "%result%"  >> src/gitversion.h
echo #define VER_GITSTR  "%result%.%result2%"  >> src/gitversion.h
del post.tmp
