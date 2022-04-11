#include <iostream>
#include <stdlib.h>
#include<windows.h>
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
#pragma comment(lib, "Kernel32.lib")
using namespace std;
int main()
{
    putenv("QT_PLUGIN_PATH=");
    putenv("QML2_IMPORT_PATH=");
	STARTUPINFO si; //一些必备参数设置
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi; //必备参数设置结束
	if(!CreateProcess(NULL, "Assistant.exe", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) //8888为命令行参数，ExcuteApp.exe为当前目录下的一个exe文件。
	{
			cout<<"Create Fail!"<<endl;
			exit(1);
	}
	else
	{cout<<"Sucess!"<<endl;}
	return 0;
}