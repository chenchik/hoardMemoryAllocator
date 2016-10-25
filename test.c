#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int main() {
  void *x = malloc(31);
  printf("Hello printing 32 bytes x %p\n", x);
  x = malloc(30);
  printf("32 bytes apart %p\n", x);
  x = malloc(8);
  printf("32 bytes apart %p\n", x);
  free(x);
  printf("justfreed x %p\n", x);
  void *d = malloc(8);
  printf("32 bytes apart %p\n", d);
  void *e = malloc(33);
  printf("64 byte superblock %p\n", e);
  void *f = malloc(61);
  printf("64 bytes apart superblock %p\n", f);
  void *g = malloc(64);
  printf("64 bytes apart superblock %p\n", g);
  void *h = malloc(65);
  printf("128 bytes apart superblock %p\n", h);
  void *i = malloc(128);
  printf("128 bytes apart superblock %p\n", i);
  void *j = malloc(2000);
  printf("2048 bytes apart superblock %p\n", j);
  void *k = malloc(2020);
  printf("2048 bytes apart superblock %p\n", k);
  void *l = malloc(2021);
  printf("2048 bytes apart superblock %p\n", l);
  void *m = malloc(2048);
  printf("2048 bytes apart superblock %p\n", m);
  void *n = malloc(8);
  void *n1 = malloc(8);
  void *n2 = malloc(8);
  void *n3 = malloc(33);
  void *n4 = malloc(37);
  void *n5 = malloc(129);
  void *n6 = malloc(140);
  int num = 17;
  int num1 = 18;
  int num2 = 19;
  int num3 = 20;
  int num4 = 21;
  int num5 = 22;
  int num6 = 23;
  int num7 = 24;
  *(int *)(n) = num;
  printf("n should be 17:%d\n", *(int *)(n));
  free(n);
  printf("n is:%d\n", *(int *)(n));
  *(int *)(n1) = num1;
  *(int *)(n2) = num2;
  printf("n1 should be 18:%d\n", *(int *)(n1));
  void *n7 = malloc(140);
  *(int *)(n7) = num7;
  printf("n2 should be 19:%d\n", *(int *)(n2));
  void *n8 = malloc(140);
  printf("n7 should be 23:%d\n", *(int *)(n7));
  printf("n2 should be 19:%d\n", *(int *)(n2));
  free(n1);
  printf("n7 should be 24:%d\n", *(int *)(n7));
  printf("n2 should be 19:%d\n", *(int *)(n2));
  free(n2);
  free(n8);
  printf("n7 should be 24:%d\n", *(int *)(n7));
  printf("n2 should be junk:%d\n", *(int *)(n2));
  printf("n8 should be junk:%d\n", *(int *)(n8));
  *(int *)(n3) = num3;
  void *n9 = malloc(1000);
  char ch[] = {'a','b','c'};
  *(char **)(n9) = ch;
  printf("n9 should be abc:%s\n", *(char **)(n9));
  printf("n7 should be 24:%d\n", *(int *)(n7));
  free(n9);
  printf("n9 should be junk:%s\n", *(char **)(n9));
  printf("n7 should be 24:%d\n", *(int *)(n7));
  *(int *)(n4) = num4;
  printf("n9 should be junk:%s\n", *(char **)(n9));
  printf("n4 should be 21:%d\n", *(int *)(n4));
  printf("n7 should be 24:%d\n", *(int *)(n7));
  free(n7);
  printf("n4 should be 21:%d\n", *(int *)(n4));
  printf("n7 should be junk:%d\n", *(int *)(n7));
  void *n10 = malloc(66666);
  //int num10 = 666;
  //*(int *)(n10) = num10;
  printf("n4 should be 21:%d\n", *(int *)(n4));
  //printf("n10 should be junk:%d\n", *(int *)(n10));
  //free(n10);
  printf("n4 should be 21:%d\n", *(int *)(n4));
  //printf("n10 should be junk:%d\n", *(int *)(n10));
  free(n4);
  printf("n4 should be junk:%d\n", *(int *)(n4));
  //printf("n10 should be junk:%d\n", *(int *)(n10));



  void *zero = malloc(0);
  printf("zero should be null:%p\n", zero);
  void *neg = malloc(-1);
  printf("negative should be null:%p\n", neg);
  

  int *firstarr = (int *) malloc(sizeof(int) * 3);
  firstarr[0]=6;
  firstarr[1]=565;
  firstarr[2]=72;
  int ii = 0;
  for(; ii < 3; ii++){
    printf("first arr:%d\n", firstarr[ii]);
  }

  typedef struct A{
    int a;
    int b;
    volatile char *yo;
  } A;
  
  volatile A *str = (A *) malloc(sizeof(A));
  str->a = 12;
  str->b = 34;
  str->yo = (char *) malloc(sizeof(char) * 10);
  //gets(str->yo);
  memcpy(str->yo, "yoooooooo", 10);

  printf("structs a should be 12: %d\n", str->a);
  printf("structs b should be 34: %d\n", str->b);
  printf("structs yo should be yoyoyoyo: %s\n", str->yo);

  free(str);

  printf("structs a should be junk: %d\n", str->a);
  printf("structs b should be junk: %d\n", str->b);
  printf("structs yo should be junk: %p %s\n", str->yo, str->yo);
  
  char *n12 = (char *)malloc(sizeof(char) * 2);
  //n12 = "whhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaat";
  //n12 = str2;
  gets(n12);
  printf("n12 should be a long char*: %s\n", n12);
  
  int *n13 = (int *)malloc(20);
  *n13 = 64;
  printf("n13 should be 64: %d\n", *n13);

  free(n12);
  printf("n12 should be junk*: %s\n", n12);
   


  return (errno);
}
