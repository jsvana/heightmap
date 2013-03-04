CC=clang
PROG=opengl
OBJS=main.o bmp.o
CFLAGS=-ggdb

all: $(OBJS)
	$(CC) -ggdb -o $(PROG) $(OBJS) -framework OpenGL -framework GLUT -L/usr/local/Cellar/devil/1.7.8/lib -lIL

clean:
	rm $(OBJS)
