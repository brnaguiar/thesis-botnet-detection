gcc -g ../stats/maths_and_stats.c -I/usr/include/python3.9 'client.c' -lm $(python3-config --embed --ldflags) -o client

