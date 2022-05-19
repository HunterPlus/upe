YFLAGS = -d

OBJS = hoc.o code.o init.o math.o symbol.o

hoc5:	$(OBJS)
	cc -o hoc5 $(OBJS) -lm

hoc.o code.o init.o symbol.o math.o:	hoc.h

code.o init.o symbol.o:	x.tab.h

x.tab.h:	y.tab.h
	-cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h

pr:	hoc.y hoc.h code.c init.c math.c symbol.c
	@pr $?
	@touch pr

clean:
	rm -f $(OBJS) [xy].tab.[ch] hoc5	

