#include "minicrt.h"

int main(int argc, char const *argv[])
{
  int i;
  FILE* file;
  char** v = (char**) malloc(argc * sizeof(char*));
  for(int i = 0; i < argc; i ++){
    v[i] = malloc(strlen(argv[i]) + 1);
    strcpy(v[i], argv[i]);
  }
  file = fopen("text.txt", "w");
  for(int i = 0; i < argc; i ++){
    int len = v[i];
    fwrite(&len, 1, sizeof(int), file);
    fwrite(v[i], 1, len, file);
  }
  fclose(file);

  file = fopen("text.txt", "r");
  for(int i = 0; i < argc; i ++){
    int len;
    char* buffer;
    fread(&len, 1, sizeof(int), file);
    buffer[len] = '\0';
    printf("%d %s\n", len, buffer);
    free(buffer);
    free(v[i]);
  }
  fclose(file);

  return 0;
}
