
SRCS = $(wildcard ./*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

hello:$(OBJS)
	gcc $(addprefix common/,*) -o $@

%.o : %.c
	gcc -c $< -o common/$@


clean:
	-rm hello
	-rm common/*.o
