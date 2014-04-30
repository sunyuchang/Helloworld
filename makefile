pidd:pidd.o
	cc -o pidd pid1.o
pidd.o:pidd.c
	cc -c pidd.c
clean:
	rm pidd.o
