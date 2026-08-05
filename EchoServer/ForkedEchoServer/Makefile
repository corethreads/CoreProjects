all:forked_echoserver
	./forked_echoserver


forked_echoserver: main.c server_services.c
	gcc -o forked_echoserver main.c server_services.c

clean:
	rm -f forked_echoserver


