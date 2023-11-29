all:
	gcc -o wg wg.c wg_ip.c

clean:
	rm -f wg
