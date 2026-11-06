@echo off
echo Running encoder on: %~1
py "C:\Users\Brondum\Documents\GitHub\Python\studieplusimageinfo\encoder_fe.py" "%~1"
echo.
echo Exit code: %errorlevel%
pause