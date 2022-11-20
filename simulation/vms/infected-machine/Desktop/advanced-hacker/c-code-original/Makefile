compiler = gcc
cflags = -lm
stats = src/stats/maths_and_stats

pure:
	${compiler} ${stats}.c src/tcpc.c ${cflags} -o ctcp

py:
	${compiler} ${stats}.c src/tcppy.c ${cflags} -o ptcp
