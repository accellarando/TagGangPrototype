all:
	gcc main.c `pkg-config --cflags --libs glib-2.0 clutter-1.0` -o main.out

clean:
	rm -f main.out
