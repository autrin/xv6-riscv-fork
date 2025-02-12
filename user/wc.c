#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
/*
@Author of the changes: Autrin Hakimi
*/
char buf[512];

void
wc(int fd, char *name)
{
  int i, n;
  int l, w, c, inword, vowels; // Adding the variable vowels to count the number of vowels in the file

  l = w = c = vowels = 0;
  inword = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      c++;
      if(buf[i] == '\n')
        l++;
      if(strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if(!inword){
        w++;
        inword = 1;
      }
      if (strchr("aAeEiIoOuU", buf[i])) // Checks if the character at position i is a vowel
                                        //strchr() returns a pointer to the first occurrence of 
                                        // the character buf[i] in the string "aAeEiIoOuU"
        vowels++;

    }
  }
  if(n < 0){
    printf("wc: read error\n");
    exit(1);
  }

  if(name[0] != '\0')
    // Only print filename if it's provided
    printf("%d %d %d %d %s\n", l, w, c, vowels, name); // Modified the output to include the number of vowels
  else
    // No filename if reading from stdin
    printf("%d %d %d %d\n", l, w, c, vowels); 
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    wc(0, "");
    exit(0);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      printf("wc: cannot open %s\n", argv[i]);
      exit(1);
    }
    wc(fd, argv[i]);
    close(fd);
  }
  exit(0);
}
