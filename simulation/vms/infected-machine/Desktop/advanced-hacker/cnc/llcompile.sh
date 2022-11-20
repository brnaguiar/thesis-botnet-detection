gcc -g -I/usr/include/python3.9 'client_lowlatency.c' $(python3-config --embed --ldflags) -o ll
