exe = 		\
	test_hello

all     :       $(exe)

$(exe)  :       %:%.o
	gcc -O2 -g -o $@ $^ -L. -lzlog -lpthread -Wl,-rpath .

.c.o	:
	gcc -O2 -g -Wall -D_GNU_SOURCE -o $@ -c $< -I. -I.

clean	:
	rm -f press.log* *.o $(exe)

.PHONY : clean all
