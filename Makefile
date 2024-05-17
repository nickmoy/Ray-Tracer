EXE = play.out

IMGUI_DIR = 
# SOURCES = src/*.cpp
# SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
# SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
# OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
# OS_NAME := $(shell uname -s)

# Need to add IMGUI headers to -I

CC = g++
C++STD = -std=c++17
LIBS = -IInclude -LLib -lSDL2-2.0.0 -framework OpenGL
CC-FLAGS = -IInclude

##--------------------------------------------------------------------
## BUILD RULES
##--------------------------------------------------------------------

all: $(EXE)
	@echo ">> Build successful!"

${EXE}: build/main.o build/window.o build/renderer.o build/physics.o build/utils/renderer_utils.o
	${CC} ${C++STD} build/main.o build/window.o build/renderer.o build/physics.o build/utils/renderer_utils.o -o play.out ${LIBS}
	@chmod u+x play.out

build/main.o: src/main.cpp
	${CC} ${C++STD} -c src/main.cpp -o main.o ${CC-FLAGS}
	@mv main.o build
	@echo ">> main.o object file created\n"

build/window.o: src/window.cpp
	${CC} ${C++STD} -c src/window.cpp -o window.o ${CC-FLAGS}
	@mv window.o build
	@echo ">> window.o object file created\n"

build/renderer.o: src/renderer.cpp
	${CC} ${C++STD} -c src/renderer.cpp -o renderer.o ${CC-FLAGS}
	@mv renderer.o build
	@echo ">> renderer.o object file created\n"

build/physics.o: src/physics.cpp
	${CC} ${C++STD} -c src/physics.cpp -o physics.o ${CC-FLAGS}
	@mv physics.o build
	@echo ">> physics.o object file created\n"

build/utils/renderer_utils.o: src/utils/renderer_utils.cpp
	${CC} ${C++STD} -c src/utils/renderer_utils.cpp -o renderer_utils.o ${CC-FLAGS}
	@mv renderer_utils.o build/utils
	@echo ">> physics.o object file created\n"

clean:
	@rm -rf build/*
	@mkdir build/utils
	@test -f ./${EXE} && rm ${EXE} || true
	@echo ">> Project cleaned!"

