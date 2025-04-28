#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/logger.h>
#include "Application.h"
LOG_MODULE(main);

int main() {
	Driver* mc = new Application();
	mc->start();
	delete mc;
	return 0;
}
