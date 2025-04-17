#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/logger.h>
#include "Minecraft.h"
LOG_MODULE(main);

int main() {
	Driver* mc = new Minecraft();
	mc->start();
	delete mc;
	return 0;
}
