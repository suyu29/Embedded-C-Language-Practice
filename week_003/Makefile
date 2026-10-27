# Each exercise is an independent program. Run from week_003.
.DEFAULT_GOAL := all
.DELETE_ON_ERROR:

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O0 -g
ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
.SHELLFLAGS := /c
CFLAGS += -D__USE_MINGW_ANSI_STDIO=1
endif

QUESTIONS := q01 q02 q03 q04 q05 q06 challenge
RUN_TARGETS := $(addprefix run-,$(QUESTIONS))

.PHONY: all help $(QUESTIONS) $(RUN_TARGETS)
.SECONDARY:

all: $(QUESTIONS)

$(QUESTIONS): %: %.exe

%.exe: %.c Makefile
	$(CC) $(CFLAGS) "$<" -o "$@"

$(RUN_TARGETS): run-%: %.exe
ifeq ($(OS),Windows_NT)
	.\$*.exe
else
	./$*.exe
endif

help:
	@echo mingw32-make q02       : compile q02.c into q02.exe
	@echo mingw32-make run-q02   : compile and run q02
	@echo mingw32-make all       : compile all seven exercises
