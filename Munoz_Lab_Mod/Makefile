GCC = gcc
FLAGS = -c

PROGRAMAS: labPrincipal


labPrincipal:
	$(GCC) $(FLAGS) libreriaLab.c
	$(GCC) $@.c libreriaLab.o -o $@
	$(RM) *.o


clear:
	$(RM) *.o $(PROGRAMAS)

