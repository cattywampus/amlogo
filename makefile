all: 
	gcc -Wall -D__PLATFORM_OS_X__ -o amlogo amlogo.c -framework OPENGL -framework GLUT

clean:
	rm amlogo
