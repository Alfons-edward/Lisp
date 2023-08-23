
HEADER_DIR := include
SRC_DIR := src

all:
	gcc -std=c99 -Wall -I ${HEADER_DIR} ${SRC_DIR}/prompt.c ${SRC_DIR}/mpc.c  -ledit -o prompt