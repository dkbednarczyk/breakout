.PHONY: build run clean fmt build-windows

build:
	cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build -j
	cp -r resources ./build/resources

build-windows:
	cmake -S . -B ./build-win \
	  -DCMAKE_BUILD_TYPE=Release \
	  -DCMAKE_SYSTEM_NAME=Windows \
	  -DCMAKE_C_COMPILER=zig -DCMAKE_C_COMPILER_ARG1=cc \
	  -DCMAKE_CXX_COMPILER=zig -DCMAKE_CXX_COMPILER_ARG1=c++ \
	  -DCMAKE_RC_COMPILER=zig -DCMAKE_RC_COMPILER_ARG1=rc \
	  -DCMAKE_C_FLAGS="--target=x86_64-windows-gnu" \
	  -DCMAKE_CXX_FLAGS="--target=x86_64-windows-gnu"

	cmake --build ./build-win -j
	cp -r resources ./build-win/resources

run: build
	./build/Breakout

clean:
	rm -rf build
	rm -rf build-win

fmt:
	clang-format -i src/*.cpp include/*.h
