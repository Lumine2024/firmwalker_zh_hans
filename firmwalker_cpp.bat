@echo off

if "%~1"=="" (
    echo ----------firmwalker_cpp----------
    echo �÷�:
    echo   firmwalker_cpp.bat {�ѽ�ѹ�Ĺ̼��ļ�·��} {��ѡ: �ļ�����ĵ�ַ, Ĭ��Ϊfirmwalker.txt}
    echo ����:
    echo   firmwalker_cpp.bat linksys\fmk\rootfs\
    exit /b 1
)

g++ -std=c++20 firmwalker.cpp -o firmwalker.out

if "%~2"=="" (
	firmwalker.out "%~1"
) else (
	firmwalker.out "%~1" "%~2"
)