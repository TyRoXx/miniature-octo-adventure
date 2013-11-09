#include <assert.h>
#include <nds.h>

int main(void)
{
	videoSetModeSub(MODE_0_2D);
	consoleDemoInit();

	iprintf("Hello, world!\n");
	for (;;)
	{
		swiWaitForVBlank();
	}
	return 0;
}
