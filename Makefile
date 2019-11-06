objs = main.o move.o path.o pqueue.o hash.o

snake: $(objs)
	cc -o snake $(objs) -lcurses

.PHONY : clean
clean:
	-rm snake $(objs) 
