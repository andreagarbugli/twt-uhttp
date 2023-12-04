@echo off

set EXAMPLE=%1

REM find the file in the `python` directory, starting with the number %EXAMPLE%
for /r "python" %%F in ("%EXAMPLE%*.py") do (
    set "FILE=%%F"
    goto :run
)

echo File not found.
exit /b

:run
REM run the file
python "%FILE%"
