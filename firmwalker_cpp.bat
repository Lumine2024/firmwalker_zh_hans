@echo off

if "%~1"=="" (
    echo ----------firmwalker_cpp----------
    echo 用法:
    echo   firmwalker_cpp.bat {已解压的固件文件路径} {可选: 文件保存的地址, 默认为firmwalker.txt}
    echo 例如:
    echo   firmwalker_cpp.bat linksys\fmk\rootfs\
    exit /b 1
)

g++ -std=c++20 firmwalker.cpp -o firmwalker.out

if "%~2"=="" (
	firmwalker.out "%~1"
) else (
	firmwalker.out "%~1" "%~2"
)