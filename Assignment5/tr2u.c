#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int contains(int len, char tar, const char* patt);

int main(int argc, char* argv[]){

  // Check num of arguments
  if(argc != 3){
    char* errorMsg = "Wrong number of arguments, need two.\n";
    write(2, errorMsg, strlen(errorMsg)); 
    exit(1);
  }

  const char* from = argv[1];
  const char* to = argv[2];
  int len_f = strlen(from);
  int len_t = strlen(to);

  // Check if lengths aren't the same
  if(len_f != len_t){
    char* errorMsg = "Arguments need to be the same length.\n";
    write(2, errorMsg, strlen(errorMsg));
    exit(1);
  }

  // Check if duplicates in from
  size_t i = 0;
  size_t j;
  for(; i != len_f-1; i++){
    for(j = i+1; j != len_f; j++){
      if(from[i] == from[j]){
	char* errorMsg = "Duplicates in 'from' error.\n"; 
	write(2, errorMsg, strlen(errorMsg));
	exit(1);
      }
    }
  }

  // Transliterate
  char current[2];
  while(read(0, current, 1) > 0){     
    int num = contains(len_f, current[0], from);
    if(num >= 0){
      current[0] = to[num];
    }
    write(1, current, 1);
  }
  return 0;
}

int contains(int len, char tar, const char* patt){
  int i = 0;
  for(; i < len; i++){
    if(patt[i] == tar)
      return i;
  }
  return -1;
}
