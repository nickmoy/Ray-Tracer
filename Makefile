EXE = play.out
SOURCES = src/*.cpp
OBJS = build/main.o build/window.o build/renderer.o build/physics.o build/utils/renderer_utils.o

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

${EXE}: ${OBJS}
	${CC} ${C++STD} $^ -o play.out ${LIBS}
	@chmod u+x play.out

build/%.o: src/%.cpp
	${CC} ${C++STD} -c $^ -o $@ ${CC-FLAGS}
	@echo ">> $@ object file created\n"

clean:
	@rm -rf build/*
	@mkdir build/utils
	@test -f ./${EXE} && rm ${EXE} || true
	@echo ">> Project cleaned!"

