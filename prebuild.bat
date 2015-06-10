@echo off
git rev-parse --short HEAD >post.tmp
set /p result=<post.tmp

git rev-parse --symbolic-full-name --abbrev-ref HEAD >post.tmp
set /p result2=<post.tmp
set timestamp=%DATE:/=-%@%TIME::=-%
set timestamp=%timestamp: =%

echo char GitStr[] ="%result%-%timestamp%"; > include/gitversion.h
del post.tmp
