gcc -g -I/usr/include/python3.9 'server.c' $(python3-config --embed --ldflags) -o server
