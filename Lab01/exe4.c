#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR 100

typedef struct{
    char* word;
    int count;
} word_info;

typedef struct list{
    word_info *john;
    struct list *next;
}list;

list* find_unique_words(char**word_array, int ntot, char c){
    int j;
    list *a = NULL, *aux;
    word_info *word;

    for(j = 0; j < ntot; j++){
        aux = a;

        if  (tolower(word_array[j][0]) != c) continue;
        
        while(aux != NULL){
            if(strcasecmp(word_array[j], aux->john->word) == 0){
                aux->john->count++;
                break;
            }
            aux = aux->next;
        }

        if(aux != NULL) continue;

        word = (word_info *) malloc(sizeof(word_info));
        if(word == NULL) exit(-1);

        word->word = (char*) malloc(strlen(word_array[j]) +1);
        strcpy(word->word, word_array[j]);
        word->count = 1;

        aux = (list*) malloc(sizeof(list));
        if(aux == NULL) exit(-1);

        aux->john = word;
        aux->next = a;

        a = aux;
    }

    return a;

}

void print_uniq_words(list *a){
    list *aux = a;
    int count = 0;
    while (aux != NULL){
        count++;
        aux = aux->next;
    }

    printf("Palavras unicas : %d\n", count);
    
}

int count_words(char **word_array, int ntot, char c){
    int aux, cnt = 0;
    for(aux=0; aux<ntot; aux++){
            if(c==tolower(word_array[aux][0]))
                cnt++;
    }

    return cnt;
}

word_info *more_freq_word(list *word_list){
    word_info *ret = NULL;
    list *aux = word_list;

    while (aux!= NULL)
    {
        if (ret == NULL){
            ret = aux->john;
        }
        else if(ret->count < aux->john->count){
            ret = aux->john;
        }

        aux = aux->next;
    }
    
    return ret;


}

int main(int argc, char *argv[]){
    char **word_array, trash[MAX_STR];
    int wordC = 0, j;
    FILE *f;
    char c;
    list *a = NULL;
    word_info *freq;

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
        printf("numero de palavras começadas por %c :%d\n", c, count_words(word_array, wordC, c));
        a = find_unique_words(word_array, wordC, c);
        print_uniq_words(a);
        freq = more_freq_word(a);
        if( freq == NULL ) printf("Não há palavras");
        else printf("%s %d\n", freq->word, freq->count);
    }

}