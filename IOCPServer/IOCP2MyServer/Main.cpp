#include <time.h>
#include "MyServer.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE prevInstance, PSTR cmdLine, int showCmd) 
{
	//初始化随机数
	srand((long)time(0));
	//生成一个引擎实例并执行
	CMyServer app(hInstance);
	app.Init();
	app.Run();
	app.ShutDown();
	return 0;
}