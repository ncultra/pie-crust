.PHONY: all
all: pie_crust
pie_crust: pie-crust.o pmparser.o
	gcc  -o $@ $^

pie-crust.o: pie-crust.c pie-crust.h 
	gcc -Wall -g -c $<

pmparser.o: pmparser.c pmparser.h
	gcc -Wall -g -c $<

.PHONY: clean
clean:
	rm -v *.o pie_crust

.PHONY: lint
lint:
	find . -name "*.c"  -exec cppcheck --force {} \;
	find . -name "*.h"  -exec cppcheck --force {} \;

.PHONY: pretty
pretty:
	find . -name "*.c"  -exec indent -gnu {} \;
	find . -name "*.h"  -exec indent -gnu {} \;

.PHONY: scan-build
scan-build:
	scan-build make all
