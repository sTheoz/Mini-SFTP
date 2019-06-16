#include <stdio.h>


unsigned int sizeFile(char path[]){
    FILE *file;
    unsigned int size;
 
    file=fopen(path,"r");
 
    if(file)
    {
        fseek (file, 0, SEEK_END);
        size= (unsigned int) ftell (file);
        fclose (file);          
    }else{
        perror("Erreur avec la taile du fichier");
        return -1;
    }
    return size;
}

int sendDir(){
 return 0;
}

int sendFile(){
    return 0;
}

int recvFile(){
    return 0;
}