CC       = gcc
CFLAGS   = -Wall -g
INCLUDES = -I.
LDFLAGS  = -lpthread
VPATH    = .
OBJS     =

TARGETS  = system_ls_root fork_process fork_and_exec_process \
		   clone_thread pthread_tid_and_pid pthread_create \
		   pthread_create_with_param pthread_join pthread_return_values \
		   pthread_mutex pthread_cond

all : $(TARGETS)

system_ls_root:system_ls_root.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

fork_process:fork_process.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

fork_and_exec_process:fork_and_exec_process.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clone_thread:clone_thread.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

pthread_tid_and_pid:pthread_tid_and_pid.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

pthread_create:pthread_create.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

pthread_create_with_param:pthread_create_with_param.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

pthread_join:pthread_join.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

pthread_return_values:pthread_return_values.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

pthread_mutex:pthread_mutex.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

pthread_cond:pthread_cond.o $(OBJS)
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
