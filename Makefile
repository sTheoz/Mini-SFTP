# CHAPITRE 1 : BUT FINAL

but : all

# CHAPITRE 2 : VARIABLES

#OFILES = clt.o \
         srv.o \
		 main.o


C_OFILES = clt.o request.o response.o fileF.o

S_OFILES = srv.o request.o response.o fileF.o

CC = gcc

CFLAGS = -Wall -pedantic -g

# CHAPITRE 3 : DEPENDANCES (REGLES IMPLICITES)

response.o : fileF.c request.c

clt.o : clt.c request.c	response.c fileF.c

srv.o : srv.c request.c response.c fileF.c

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