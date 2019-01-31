objs = main.o move.o path.o set.o max_pqueue.o min_pqueue.o hash.o

snake: $(objs)
	cc -o snake $(objs) -lcurses

.PHONY : clean
clean:
	-rm snake $(objs) 
