VPATH  = .

CC     = g++

INCLUDES = -I. -I./paho.mqtt.c/include -I../../utils/json-parser/rapidjson/

CFLAGS  = -Wall -g -std=gnu++0x

LIBS    =  -pthread -lrt -L./paho.mqtt.c/lib/ -lpaho-mqtt3c

OBJS    = message_box.o message.o

TARGET  = test_message_box

all:$(TARGET)

test_message_box : test_message_box.o $(OBJS)
	$(CC) -o $@ $^ $(INCLUDES) $(LIBS)

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
	rm -f $(TARGET) *.o *.d *.d.* 

