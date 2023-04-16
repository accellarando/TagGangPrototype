CFLAGS_GFREENECT := -I/usr/local/include/gfreenect-0.1
LDFLAGS_GFREENECT := -lgfreenect-0.1

CFLAGS_SKELTRACK := -I/usr/lib
LDFLAGS_SKELTRACK := -lskeltrack-0.1

all:
	gcc -Og -g main2.c `pkg-config --cflags --libs glib-2.0 clutter-1.0` -o main $(CFLAGS_GFREENECT) $(LDFLAGS_GFREENECT) $(CFLAGS_SKELTRACK) $(LDFLAGS_SKELTRACK)

clean:
	rm -f main
