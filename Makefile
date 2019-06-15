# CHAPITRE 1 : BUT FINAL

but : all

# CHAPITRE 2 : VARIABLES

#OFILES = clt.o \
         srv.o \
		 main.o


C_OFILES = clt.o 

S_OFILES = srv.o

CC = gcc

CFLAGS = -Wall -pedantic -g

# CHAPITRE 3 : DEPENDANCES (REGLES IMPLICITES)

clt.o : clt.c

srv.o : srv.c

#CHAPITRE 4 : DEPENDANCES AVEC COMMANDES

all :clt srv

clt : $(C_OFILES)
	$(CC) $(CFLAGS) -o clt $(C_OFILES)

srv : $(S_OFILES)
	$(CC) $(CFLAGS) -o srv $(S_OFILES)

#diffie : $(OFILES)
#	$(CC) $(CFLAGS) -o diffie $(OFILES)

#CHAPITRE 5 : NETTOYAGE DES FICHIERS GENERES

clean :
	-rm -f $(C_OFILES) $(S_OFILES)

#CHAPITRE 6 : BUTS FACTICES

.PHONY : but clean