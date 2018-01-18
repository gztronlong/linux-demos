GCC=gcc

SOURCES=test.c cJSON.c
OBJS=$(patsubst %.c,%.o,$(SOURCES))

TARGET=test

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
