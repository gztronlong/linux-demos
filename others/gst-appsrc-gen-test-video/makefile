
CGT_ARM_PREFIX  = /home/jack/dm8148/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-
EZSDK           = /home/jack/dm8148/ti-ezsdk_dm814x-evm_5_05_02_00/

GSTREAMER_DIR   = $(EZSDK)/linux-devkit/arm-none-linux-gnueabi/usr/include/gstreamer-0.10
GLIB_DIR        = $(EZSDK)/linux-devkit/arm-none-linux-gnueabi/usr/include/glib-2.0
GLIB_CONFIG_DIR = $(EZSDK)/linux-devkit/arm-none-linux-gnueabi/usr/lib/glib-2.0/include
LIBXML2_DIR     = $(EZSDK)/linux-devkit/arm-none-linux-gnueabi/usr/include/libxml2

CC 		 = $(CGT_ARM_PREFIX)gcc
CFLAGS   = -Wall -g
INCLUDES = -I$(GSTREAMER_DIR) -I$(GLIB_DIR) -I$(GLIB_CONFIG_DIR) -I$(LIBXML2_DIR)
LDFLAGS	 = -L$(EZSDK)/linux-devkit/arm-none-linux-gnueabi/usr/lib \
		   -lgstrtsp-0.10 -lgstapp-0.10 -lgstsdp-0.10 -lgstbase-0.10 \
		   -lpthread -lgstreamer-0.10 -lglib-2.0 -lz \
		   -lgobject-2.0 -lgthread-2.0 -lgmodule-2.0 -lrt -lxml2
VPATH    =
OBJS	 =

TARGETS  = appsrc

all : $(TARGETS)

appsrc:appsrc.o $(OBJS)
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
