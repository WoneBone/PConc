#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR 100
int main(int argc, char *argv[]){
    char **word_array, trash[MAX_STR];
    int wordC = 0, j;
    FILE *f;
    char c;
    int aux;

    if (argc != 2){
        puts("Número de argumentos errado");
        exit(-1);
    }

    f = fopen(argv[1], "r");
    if (f == NULL){
        exit(-1);
    }

    while(fgets(trash, MAX_STR, f)){
        wordC++;
    }
    rewind(f);

    word_array = (char **) malloc(sizeof(char *) * wordC);
    if (word_array == NULL) exit(-1);

    for (j = 0; j < wordC; j++){
        if(fgets(trash, MAX_STR, f) == NULL) break;

        word_array[j] = (char *) malloc(sizeof(char) *(strlen(trash) + 1));
        strcpy(word_array[j], trash);
    }

    for(c='a'; c<'z'; c++){
        for(aux=0; aux<wordC; aux++){
            if(c==tolower(word_array[aux][0]))
                printf("%s", word_array[aux]);
        }
    }

}