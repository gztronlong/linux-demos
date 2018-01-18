CC 		 = gcc
CFLAGS   = -Wall -g
INCLUDES = -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include
LDFLAGS	 = -lglib-2.0
VPATH    = .
OBJS	 = 

TARGETS  = glib-thread-demo

all : $(TARGETS)

glib-thread-demo:glib-thread-demo.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%.d:%.c
	@set -e; $(CC) -MM $< $(INCLUDES) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
		
sinclude $(OBJS:.o=.d)

.PHONY:
clean:
	rm -f $(TARGETS) *.o *.d *.d.* 
