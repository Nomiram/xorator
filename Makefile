all:
	gcc -o xorator.exe main.c
sanitize:
	gcc -o xorator.exe main.c -fsanitize=address