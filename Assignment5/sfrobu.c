#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

int fOption = 0;
int cmpCount = 0;

// Uses XOR 42 and makes uppercase if F option
char decode(char x){
  x = (x ^ 42);
  if(fOption)
    x = toupper((unsigned char)x);
  return x;
}

// Compares different inputs based on their decoded values
int frobcmp(char const * a, char const * b){
  for(; *a != ' ' || *b != ' '; a++, b++){
    if(decode(*a) > decode(*b) || (*b == ' ' && *a != ' '))
      return 1;
    if(decode(*a) < decode(*b) || (*a == ' '  && *b != ' '))
      return -1;
  }
  return 0;
}

// Checks input and output errors (if error message occurs)
void checkError(ssize_t status){
  if(status < 0){
    char* errorMsg = "Error in input\n";
    write(2, errorMsg, strlen(errorMsg));
    exit(1);
  }
}

// The compare function for qsort, uses frobcmp
int compare(const void * a, const void * b){
  cmpCount++;
  return frobcmp(*(const char**)a, *(const char**)b);
}

int main(int argc, char* argv[]){

  if(argc > 2){
    char* errorMsg = "Too many arguments\n";
    write(2, errorMsg, strlen(errorMsg));
    exit(1);
  }
  if(argc == 2){
    if(!strncmp(argv[1], "-f", 2))
      fOption = 1;
  }
  
  struct stat sta;
  ssize_t status = fstat(0, &sta);
  checkError(status);

  char* input = NULL;
  char** wordArray = NULL;
  int numWords = 0;
  int ongoing = 0;
  
  size_t filesizewn = sta.st_size + 1;

  // If file is regular
  if(S_ISREG(sta.st_mode)){
    // Allocates and reads
      input = (char*)malloc(sizeof(char) * filesizewn);
      size_t filesizeb = sta.st_size;
      status = read(0, input, sizeof(char) * filesizeb);
      checkError(status);

      int k = 0;
      int count = 0;
      // Counts number of words in input
      for(; k < filesizeb; k++){
	if(k == filesizeb - 1 && input[k] != ' '){
          input = (char*)realloc(input, (filesizewn)*sizeof(char));
          input[k+1] = ' ';
          count++;
          break;
        }

	if(input[k] == ' ')
	  count++;
      }

      // Distributes words into array
      wordArray = (char**)malloc(sizeof(char*) * count);
      int l = 0;
      for(; l < filesizeb; l++){
	if(!ongoing){
	  wordArray[numWords] = &input[l];
	  numWords++;
	  ongoing = 1;
	}
	if(input[l] == ' ')
	  ongoing = 0;
      }
      ongoing = 0;
      lseek(0, filesizeb, SEEK_SET);
  }
  
  char * stanInput = NULL;
  size_t si_len = 0;
  char current[1];
  // Standard Input section, very similar to regular file, but
  // reads byte-by-byte
  while((read(0, current, 1)) > 0){
    if(stanInput == NULL)
      stanInput = (char*)malloc(sizeof(char));
    else
      stanInput = (char*)realloc(stanInput, (si_len + 1)*sizeof(char));
    stanInput[si_len] = current[0];
    si_len++;
  }

  int h = 0;
  int count2 = 0;
  for(; h < si_len; h++){
    if(h == si_len - 1 && stanInput[h] != ' '){
      stanInput = (char*)realloc(stanInput, (si_len + 1)*sizeof(char));
      stanInput[h+1] = ' ';
      count2++;
      break;
    }
    if(stanInput[h] == ' ')
      count2++;
  }

  if(wordArray == NULL)
    wordArray = (char**)malloc(sizeof(char*) * count2);
  else
    wordArray = (char**)realloc(wordArray, sizeof(char*) * (count2 + numWords));
   
  int q = 0;  
  for(; q < si_len; q++){
	if(!ongoing){
	  wordArray[numWords] = &stanInput[q];
	  numWords++;
	  ongoing = 1;
	}
	if(stanInput[q] == ' ')
	  ongoing = 0;
  }
  
  // Sorts resulting array of words
  qsort(wordArray, numWords, sizeof(char*), compare);

  // free memory
  int i;
  int j;
  for(i = 0; i < numWords; i++){
    for(j = 0; wordArray[i][j] != ' '; j++){
      putchar(wordArray[i][j]);
    }
    putchar(' ');
  }

  if(S_ISREG(sta.st_mode))
    free(input);
  
  free(stanInput);

  return 0;
}
