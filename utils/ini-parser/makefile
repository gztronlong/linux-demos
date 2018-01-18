CC		= gcc

SOURCES = test.c dictionary.c iniparser.c
OBJS	= $(patsubst %.c,%.o,$(SOURCES))

TARGET  = test

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) -c $< -o $@

%.d : %.c
	@set -e; rm -f $@; \
	$(CC) -M $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(SOURCES:.c=.d)

.PHONY	: clean

clean:
	-rm $(TARGET) $(OBJS) *.d*
