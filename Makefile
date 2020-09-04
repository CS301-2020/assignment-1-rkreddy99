all:shell

shell: shell.c
	@echo "Building"
	gcc -o shell shell.c

clean:
	@echo "Cleaning.."
	rm shell