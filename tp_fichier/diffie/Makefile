# CHAPITRE 1 : BUT FINAL

but : all

# CHAPITRE 2 : VARIABLES

OFILES = main.o \
         gene.o 

C_OFILES = clt-dh.o \
			gene.o

S_OFILES = srv-dh.o \
			gene.o

CC = gcc

CFLAGS = -Wall -pedantic -g

# CHAPITRE 3 : DEPENDANCES (REGLES IMPLICITES)

gene.o : gene.h

main.o : gene.h

srv-dh.o : gene.h

clt-dh.o : gene.h 


#CHAPITRE 4 : DEPENDANCES AVEC COMMANDES

all : diffie clt srv

clt : $(C_OFILES)
	$(CC) $(CFLAGS) -o clt $(C_OFILES)

srv : $(S_OFILES)
	$(CC) $(CFLAGS) -o srv $(S_OFILES)

diffie : $(OFILES)
	$(CC) $(CFLAGS) -o diffie $(OFILES)

#CHAPITRE 5 : NETTOYAGE DES FICHIERS GENERES

clean :
	-rm -f $(OFILES) diffie

#CHAPITRE 6 : BUTS FACTICES

.PHONY : but clean