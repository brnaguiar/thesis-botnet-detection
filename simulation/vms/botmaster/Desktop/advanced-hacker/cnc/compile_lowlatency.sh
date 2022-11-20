gcc ../stats/maths_and_stats.c   -g -I/usr/include/python3.9 'server_lowlatency.c' $(python3-config --embed --ldflags) -o ll
