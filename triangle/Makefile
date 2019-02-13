BIN = hellot
CC = clang
FLAGS = 	-std=c99 -DAPPLE -Wall -pedantic -Wextra -Werror
INC =  	-I/usr/local/include
LIB_PATH = 
LOC_LIB = 
DYN_LIB = -lGLEW -lglfw
FRAMEWORKS = -framework Cocoa -framework OpenGL -framework IOKit
SRC = main.c

all:
	${CC} ${FLAGS} ${FRAMEWORKS} -o ${BIN} ${SRC} ${INC} ${LOC_LIB} ${DYN_LIB}