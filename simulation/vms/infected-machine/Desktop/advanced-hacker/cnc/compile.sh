gcc -g -I/usr/include/python3.9 'client.c' $(python3-config --embed --ldflags) -o client
