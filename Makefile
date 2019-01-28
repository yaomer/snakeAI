objs = main.o move.o search.o set.o stack.o queue.o hash.o

snake: $(objs)
	cc -o snake $(objs) -lcurses

.PHONY : clean
clean:
	-rm snake $(objs) 
