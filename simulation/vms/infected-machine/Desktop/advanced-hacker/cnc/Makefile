compiler = gcc
cflags = -I/usr/include/python3.9 -g
pythonconfig = $(python3-config --embed --ldflags) 
python_cflags = $(shell python3-config --cflags)
python_embed = $(shell python3-config --embed)
all:
	$(compiler) $(cflags) 'client.c' -w $(python_cflags) $(python_embed) -o client#$(pythonconfig) #

debug:
	$(compiler) debug.c $(cflags) -o deb


# gcc -g -I/usr/include/python3.9 'client.c' $(python3-config --embed --ldflags) -o client
