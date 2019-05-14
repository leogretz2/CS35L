#include <stdio.h>
#include <stdlib.h>

int frobcmp(char const * a, char const * b){
  for(; *(a-1) != ' ' && *(b-1) != ' '; a++, b++){
    if((*a ^ 42) > (*b ^ 42) || (*b == ' ' && *a != ' '))
      return 1;
    if((*a ^ 42) < (*b ^ 42) || (*a == ' '  && *b != ' '))
      return -1;
  }
  return 0;
}

void checkInputError(){
  if(ferror(stdin)){
    fprintf(stderr, "Error in input\n");
    exit(1);
  }
}

void checkOutputError(){
  if(ferror(stdin)){
    fprintf(stderr, "Error in input\n");
    exit(1);
  }
}

int compare(const void * a, const void * b){
  return frobcmp(*(const char**)a, *(const char**)b);
}

int main(){

  char * word = NULL;
  char ** wordArray = NULL;
  int word_len = 0;
  int numWords = 0;

  char prev;
  char current = getchar();
  checkInputError();
  if(current != EOF){
    ungetc(current, stdin);
    current = ' ';
  }
    
  while(!feof(stdin)){
    prev = current;
    current = getchar();
    checkInputError();
   
    if(!feof(stdin)){  
      if(word == NULL){
	word = (char*)malloc(sizeof(char));
      }else{
	word = (char*)realloc(word, (word_len+1)*sizeof(char));
      }
      word[word_len] = current;
      word_len++;
    } else {
      if(prev != ' '){
	word = (char*)realloc(word, (word_len+1)*sizeof(char));
	word[word_len] = ' ';
	word_len++;
      }
      if(word == NULL){
	break;
      }
    }

    if(word[word_len-1] == ' '){
      if(wordArray == NULL){
	wordArray = (char**)malloc(sizeof(char*));
      }else{
	wordArray = (char**)realloc(wordArray, (numWords+1)*sizeof(char*));
      }
      wordArray[numWords] = word;
      numWords++;
      word = NULL;
      word_len = 0;
    }    
  }
 
  qsort(wordArray, numWords, sizeof(char*), compare);
  
  int i;
  int j;
  for(i = 0; i < numWords; i++){
    for(j = 0; wordArray[i][j] != ' '; j++){
      putchar(wordArray[i][j]);
      checkOutputError();
    }
    putchar(' ');

    free(wordArray[i]);
  }

  free(wordArray);
  
  return 0;
}
