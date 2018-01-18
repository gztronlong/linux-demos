CC       = gcc
CFLAGS   = -Wall -g
INCLUDES = -I.
LDFLAGS  = -lpthread
VPATH    = .
OBJS     =

TARGETS  = file_copy_system file_copy_stdio create_1m_file

all : $(TARGETS)

file_copy_system:file_copy_system.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

file_copy_stdio:file_copy_stdio.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

create_1m_file:create_1m_file.o $(OBJS)
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
