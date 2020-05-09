#include <GaiaCore.h>

#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	GaiaCore* g = new GaiaCore();
	g->init("4 Pong Arena");
	g->run();
	g->close();
	
	delete g;

	return 0;
}