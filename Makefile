test:
	gcc -o test text.c server.c thread_pool.c -lpthread

test1:
	gcc -o test1 test1.c message.c  -ljson -I/usr/local/include/json -L/usr/local/lib -std=c99
	
test3:
	gcc -o test3 test3.c message.c  server.c thread_pool.c -lpthread -ljson -I/usr/local/include/json -L/usr/local/lib -std=c99