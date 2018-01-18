CC       = gcc
CFLAGS   = -Wall -g
INCLUDES =
LDFLAGS  = -lpthread
VPATH    =
OBJS     =

TARGETS  = server client

all : $(TARGETS)

server:server.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

client:client.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%.d:%.c
	@set -e; $(CC) -MM $< $(INCLUDES) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o:%.cpp
	$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%.d:%.cpp
	@set -e; $(CC) -MM $< $(INCLUDES) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

sinclude $(OBJS:.o=.d)

.PHONY:
clean:
	rm -f $(TARGETS) *.o *.d *.d.*
