server:
	gcc http_server.c -o http_server
run-server:
	./http_server 8080
clean:
	rm -f http_server 