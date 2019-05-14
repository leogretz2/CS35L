#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkInOutError();
int contains(int len, char tar, const char* patt);

int main(int argc, char* argv[]){

  // Check num of arguments
  if(argc != 3){
    fprintf(stderr, "Wrong number of arguments, need two.\n");
    exit(1);
  }

  const char* from = argv[1];
  const char* to = argv[2];
  int len_f = strlen(from);
  int len_t = strlen(to);

  // Check if lengths aren't the same
  if(len_f != len_t){
    fprintf(stderr, "Arguments need to be the same length.\n");
    exit(1);
  }

  // Check if duplicates in from
  size_t i = 0;
  size_t j;
  for(; i != len_f-1; i++){
    for(j = i+1; j != len_f; j++){
      if(from[i] == from[j]){
	fprintf(stderr, "Duplicates in 'from' error.\n");
	exit(1);
      }
    }
  }

  // Transliterate
  while(!feof(stdin)){
    char current = getchar();
    checkInOutError();
    int num = contains(len_f, current, from);
    if(num >= 0){
      current = to[num];
    }
    if(current != EOF){
      putchar(current);
    }
    checkInOutError();
  }
  return 0;
}

void checkInOutError() {
  if (ferror(stdin) != 0) {
    fprintf(stderr, "Error with I/O.");
    exit(1);
  }
}

int contains(int len, char tar, const char* patt){
  int i = 0;
  for(; i < len; i++){
    if(patt[i] == tar)
      return i;
  }
  return -1;
}
