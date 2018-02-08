GCC= g++

SOURCES = tutor.cpp
OBJS= $(patsubst %.cpp,%.o,$(SOURCES))

TARGET  = tutor

$(TARGET) : $(OBJS)
	$(GCC) -o $@ $^
%.o : %.c
	$(GCC) -c $< -o $@
	
%.d : %.cpp
	@set -e; rm -f $@; \
	$(GCC) 			-M $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$
	
-include $(SOURCES:.cpp=.d)
	
.PHONY: clean
	
clean:
	-rm $(TARGET) $(OBJS) *.d*
