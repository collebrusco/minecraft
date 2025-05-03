.PHONY: default
default:
	mkdir -p build
	cd build && \
	cmake .. && \
	make
# for windows w/ mingw, replace the cmake line above with: 	cmake -G "MinGW Makefiles" .. && \

.PHONY: c
c: cm
	rm -rf ./build/CMakeFiles/minecraft.dir/src/

.PHONY: cm
cm:
	rm -f ./build/CMakeCache.txt
	rm -f ./build/cmake_install.cmake
	rm -f ./build/cmake_install.local.cmake
	rm -f ./build/packaging.json
	rm -f ./build/compile_commands.json


.PHONY: ca
ca:
	rm -rf ./build/
	
.PHONY: r
r: default
	./build/minecraft
.PHONY: update
update:
	git submodule update --remote
	mkdir -p build
	cd build && \
		cmake .. && \
		make

