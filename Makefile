# Root Makefile proxying to src/Makefile

all:
	@cd src && mingw32-make all

debug:
	@cd src && mingw32-make debug

clean:
	@cd src && mingw32-make clean

run:
	@cd src && mingw32-make run

.PHONY: all debug clean run
