YFLAGS = -d		# force creation of y.tab.h
OBJS = hoc.o init.o math.o symbol.o	# abbreviation

hoc3:	$(OBJS)
	cc -o hoc3 $(OBJS) -lm 

hoc.o math.o:	hoc.h

init.o symbol.o:	hoc.h y.tab.h

clean:
	rm -f hoc3 $(OBJS) y.tab.[ch]
