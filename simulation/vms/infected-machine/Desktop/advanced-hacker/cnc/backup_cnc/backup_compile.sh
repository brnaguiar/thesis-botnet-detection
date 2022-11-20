gcc -g -I/usr/include/python3.9 'client_backup.c' $(python3-config --embed --ldflags) -o backup
