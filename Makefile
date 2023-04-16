CFLAGS_GFREENECT := -I/usr/local/include/gfreenect-0.1
LDFLAGS_GFREENECT := -lgfreenect-0.1

all:
	gcc main.c `pkg-config --cflags --libs glib-2.0 clutter-1.0` -o main.out $(CFLAGS_GFREENECT) $(LDFLAGS_GFREENECT)

clean:
	rm -f main.out
