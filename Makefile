target=test test2

all:$(target)

test:
	gcc -o test test.c server.c thread_pool.c -lpthread

test2:
	gcc -o test2 test2.c server.c thread_pool.c message.c -lpthread -ljson -I/usr/local/include/json -L/usr/local/lib -std=c99

clean:
	rm $(target)

