GCC     = gcc

SOURCES = nv12_border.c
OBJS    = $(patsubst %.c,%.o,$(SOURCES))

TARGET  = nv12_boder.exe

$(TARGET) : $(OBJS)
	$(GCC) -o $@ $^ -lm
%.o : %.c
	$(GCC) -c $< -o $@

%.d : %.c
	@set -e; rm -f $@; \
	$(GCC) 			-M $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

-include $(SOURCES:.c=.d)

.PHONY:rebuild clean

rebuild:clean $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJS) *.d* *.o*
