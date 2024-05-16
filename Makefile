CC = g++
C++STD = -std=c++17
BUILD_FLAGS = -I Include -L Lib -l SDL2-2.0.0 -framework OpenGL
COMPILE_FLAGS = -I Include


play.out: build/main.o build/window.o build/renderer.o build/physics.o
	${CC} ${C++STD} build/main.o build/window.o build/renderer.o build/physics.o -o play.out ${BUILD_FLAGS}
	@chmod u+x play.out
	@echo ">> Build successful!"

build/main.o: src/main.cpp
	${CC} ${C++STD} -c src/main.cpp -o main.o ${COMPILE_FLAGS}
	@mv main.o build
	@echo ">> main.o object file created"

build/window.o: src/window.cpp
	${CC} ${C++STD} -c src/window.cpp -o window.o ${COMPILE_FLAGS}
	@mv window.o build
	@echo ">> window.o object file created"

build/renderer.o: src/renderer.cpp
	${CC} ${C++STD} -c src/renderer.cpp -o renderer.o ${COMPILE_FLAGS}
	@mv renderer.o build
	@echo ">> renderer.o object file created"

build/physics.o: src/physics.cpp
	${CC} ${C++STD} -c src/physics.cpp -o physics.o ${COMPILE_FLAGS}
	@mv physics.o build
	@echo ">> physics.o object file created"

clean:
	@rm -rf build/*
	@test -f ./play.out && rm play.out || true
	@echo ">> Project cleaned!"

