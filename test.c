/* Created by Danila Chenchik and Greer Glenn
 * We agree to the UNC honor pledge */

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>

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
  //void *n9 = malloc(1000);
  //char ch[] = {'a','b','c'};
  //*(char **)(n9) = ch;
  //printf("n9 should be abc:%s\n", *(char **)(n9));
  printf("n7 should be 24:%d\n", *(int *)(n7));
  //free(n9);
  //printf("n9 should be junk:%s\n", *(char **)(n9));
  printf("n7 should be 24:%d\n", *(int *)(n7));
  *(int *)(n4) = num4;
  //printf("n9 should be junk:%s\n", *(char **)(n9));
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

  printf("\nTesting calloc **calloc(1,24)**...\n");
  char *poison2 = calloc(1,24);
  int cntr = 0;
  printf("should print a bunch of 0s\n");
  for(;cntr<32; cntr++){
    printf("%x", *(poison2+cntr));
  }
  printf("\n");

  printf("\nTesting realloc... first making a new pointer with malloc(5)\n");
  void *ra1 = malloc(30);
  printf("printing ra1 %p\n", ra1);

  printf("\nTesting realloc... now trying to realloc w smaller size...should return original\n");
  void *ra2 = realloc(ra1, 28);
  ra1 = realloc(ra1, 28);
  printf("printing ra2 %p\n", ra1);
  
  printf("\nTesting realloc... now trying to realloc w bigger size...should return new address\n");
  ra2 = realloc(ra1, 64);
  printf("printing ra2, should be a different address%p\n", ra2);
  printf("printing ra1 should be the same address as before%p\n", ra1);

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
  //printf("structs yo should be junk: %p %s\n", str->yo, str->yo);
  
  char *n12 = (char *)malloc(sizeof(char) * 2);
  n12 = "whhaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaat";
  //n12 = str2;
  //gets(n12);
  printf("n12 should be a long char*: %s\n", n12);
  
  int *n13 = (int *)malloc(20);
  *n13 = 64;
  printf("n13 should be 64: %d\n", *n13);

  //free(n12);
  //printf("n12 should be junk*: %s\n", n12);
  
  printf("about to try to free null\n");
  free(NULL);
  printf("nothing went wrong when trying to free null\n");
  printf("begin testing ex 4\n");

  void *aa = malloc(2000);
  void *bb = malloc(2000);
  void *cc = malloc(2000);
  void *dd = malloc(2000);
  void *ee = malloc(2000);
  void *ff = malloc(2000);
  void *gg = malloc(2000);
  void *hh = malloc(2000);
  
  int numa = 1;
  int numb = 2;
  int numc = 3;
  int numd = 4;
  int nume = 5;
  int numf = 6;
  int numg = 7;
  int numh = 8;
 
  *(int *)(aa) = numa;
  *(int *)(bb) = numb;
  *(int *)(cc) = numc;
  *(int *)(dd) = numd;
  *(int *)(ee) = nume;
  *(int *)(ff) = numf;
  *(int *)(gg) = numg;
  *(int *)(hh) = numh;
  
  printf("address of aa is:%p\n", aa);
  printf("address of bb is:%p\n", bb);
  /*struct sigaction sa;

  memset(&sa, 0, sizeof(sigaction));
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = segfault_sigaction;
  sa.sa_flags   = SA_SIGINFO;

  sigaction(SIGSEGV, &sa, NULL);
  */
  printf("address of cc is:%p\n", cc);
  printf("address of dd is:%p\n", dd);
  printf("address of ee is:%p\n", ee);
  printf("address of ff is:%p\n", ff);
  printf("address of gg is:%p\n", gg);
  printf("address of hh is:%p\n", hh);


  printf("aa should be 1:%d\n", *(int *)(aa));
  printf("bb should be 2:%d\n", *(int *)(bb));
  printf("cc should be 3:%d\n", *(int *)(cc));
  printf("dd should be 4:%d\n", *(int *)(dd));
  printf("ee should be 5:%d\n", *(int *)(ee));
  printf("ff should be 6:%d\n", *(int *)(ff));
  printf("gg should be 7:%d\n", *(int *)(gg));
  printf("hh should be 8:%d\n", *(int *)(hh));

  free(cc);
  free(ff);
  free(gg);

  printf("aa should be 1:%d\n", *(int *)(aa));
  printf("bb should be 2:%d\n", *(int *)(bb));
  //printf("cc should be a pointer:%p\n", cc);
  printf("dd should be 4:%d\n", *(int *)(dd));
  printf("ee should be 5:%d\n", *(int *)(ee));
  //printf("ff should be junk:%d\n", *(int *)(ff));
  printf("gg should be junk:%d\n", *(int *)(gg));
  printf("hh should be 8:%d\n", *(int *)(hh));

  printf("aa should be same address as before:%p\n", aa);
  printf("bb should be same address as before:%p\n", bb);
  printf("cc should be same address as before:%p\n", cc);
  printf("dd should be same address as before:%p\n", dd);
  printf("ee should be same address as before:%p\n", ee);
  printf("ff should be same address as before:%p\n", ff);
  printf("gg should be same address as before:%p\n", gg);
  printf("hh should be same address as before:%p\n", hh);
  //*(int *)(cc) = 33;
  //*(int *)(ff) = 66;
  //*(int *)(gg) = 77;

  printf("aa should be 1:%d\n", *(int *)(aa));
  printf("bb should be 2:%d\n", *(int *)(bb));
  //printf("cc should be 33:%d\n", *(int *)(cc));
  printf("dd should be 4:%d\n", *(int *)(dd));
  printf("ee should be 5:%d\n", *(int *)(ee));
  //printf("ff should be 66:%d\n", *(int *)(ff));
  //printf("gg should be 77:%d\n", *(int *)(gg));
  printf("hh should be 8:%d\n", *(int *)(hh));
  
  free(aa);
  free(bb);
  free(dd);

  //printf("aa should be junk:%d\n", *(int *)(aa));
  //printf("bb should be junk:%d\n", *(int *)(bb));
  //printf("cc should be 33:%d\n", *(int *)(cc));
  //printf("dd should be junk:%d\n", *(int *)(dd));
  printf("ee should be 5:%d\n", *(int *)(ee));
  //printf("ff should be 66:%d\n", *(int *)(ff));
  //printf("gg should be 77:%d\n", *(int *)(gg));
  printf("hh should be 8:%d\n", *(int *)(hh));
  
  printf("aa should be same address as before:%p\n", aa);
  printf("bb should be same address as before:%p\n", bb);
  printf("cc should be same address as before:%p\n", cc);
  printf("dd should be same address as before:%p\n", dd);
  printf("ee should be same address as before:%p\n", ee);
  printf("ff should be same address as before:%p\n", ff);
  printf("gg should be same address as before:%p\n", gg);
  printf("hh should be same address as before:%p\n", hh);
  //*(int *)(aa) = 11;
  //*(int *)(ee) = 55;
  //*(int *)(gg) = 777;
  //*(int *)(hh) = 88;

  //printf("aa should be 11:%d\n", *(int *)(aa));
  //printf("bb should be junk:%d\n", *(int *)(bb));
  //printf("cc should be 33:%d\n", *(int *)(cc));
  //printf("dd should be junk:%d\n", *(int *)(dd));
  printf("ee should be 55:%d\n", *(int *)(ee));
  //printf("ff should be 66:%d\n", *(int *)(ff));
  //printf("gg should be 777:%d\n", *(int *)(gg));
  printf("hh should be 88:%d\n", *(int *)(hh));

  //free(gg);
  //free(ff);
  free(ee);
  //free(dd);
  //free(bb);
  //free(aa);
  printf("aa should be same address as before:%p\n", aa);
  printf("bb should be same address as before:%p\n", bb);
  printf("cc should be same address as before:%p\n", cc);
  printf("dd should be same address as before:%p\n", dd);
  printf("ee should be same address as before:%p\n", ee);
  printf("ff should be same address as before:%p\n", ff);
  printf("gg should be same address as before:%p\n", gg);
  printf("hh should be same address as before:%p\n", hh);
  //printf("aa should be junk:%d\n", *(int *)(aa));
  //printf("bb should be junk:%d\n", *(int *)(bb));
  //printf("cc should be 33:%d\n", *(int *)(cc));
  //printf("dd should be junk:%d\n", *(int *)(dd));
  //printf("ee should be junk:%d\n", *(int *)(ee));
  //printf("ff should be junk:%d\n", *(int *)(ff));
  //printf("gg should be junk:%d\n", *(int *)(gg));
  printf("hh should be 88:%d\n", *(int *)(hh));

  //*(int *)(aa) = 111;

  //printf("aa should be 111:%d\n", *(int *)(aa));
  //printf("bb should be junk:%d\n", *(int *)(bb));
  //printf("cc should be 33:%d\n", *(int *)(cc));
  //printf("dd should be junk:%d\n", *(int *)(dd));
  //printf("ee should be junk:%d\n", *(int *)(ee));
  //printf("ff should be junk:%d\n", *(int *)(ff));
  //printf("gg should be junk:%d\n", *(int *)(gg));
  printf("hh should be 88:%d\n", *(int *)(hh));

  //free(cc);

  //printf("aa should be 111:%d\n", *(int *)(aa));
  //printf("bb should be junk:%d\n", *(int *)(bb));
  //printf("cc should be junk:%d\n", *(int *)(cc));
  //printf("dd should be junk:%d\n", *(int *)(dd));
  //printf("ee should be junk:%d\n", *(int *)(ee));
  //printf("ff should be junk:%d\n", *(int *)(ff));
  //printf("gg should be junk:%d\n", *(int *)(gg));
  printf("hh should be 88:%d\n", *(int *)(hh));

  //free(aa);

  //printf("aa should be junk:%d\n", *(int *)(aa));
  //printf("bb should be junk:%d\n", *(int *)(bb));
  //printf("cc should be junk:%d\n", *(int *)(cc));
  //printf("dd should be junk:%d\n", *(int *)(dd));
  //printf("ee should be junk:%d\n", *(int *)(ee));
  //printf("ff should be junk:%d\n", *(int *)(ff));
  //printf("gg should be junk:%d\n", *(int *)(gg));
  printf("hh should be 88:%d\n", *(int *)(hh));

  free(hh);

  //printf("aa should be junk:%d\n", *(int *)(aa));
  //printf("bb should be junk:%d\n", *(int *)(bb));
  //printf("cc should be junk:%d\n", *(int *)(cc));
  //printf("dd should be junk:%d\n", *(int *)(dd));
  //printf("ee should be junk:%d\n", *(int *)(ee));
  //printf("ff should be junk:%d\n", *(int *)(ff));
  //printf("gg should be junk:%d\n", *(int *)(gg));
  //printf("hh should be junk:%d\n", *(int *)(hh));
  printf("aa should be same address as before:%p\n", aa);
  printf("bb should be same address as before:%p\n", bb);
  printf("cc should be same address as before:%p\n", cc);
  printf("dd should be same address as before:%p\n", dd);
  printf("ee should be same address as before:%p\n", ee);
  printf("ff should be same address as before:%p\n", ff);
  printf("gg should be same address as before:%p\n", gg);
  printf("hh should be same address as before:%p\n", hh);
  
  printf("testing ex4 with 1024 memory blocks");
  /*first superblock*/
  void *aaa = malloc(1000);
  void *bbb = malloc(1000);
  void *ccc = malloc(1000);
  /*first superblock*/
  void *ddd = malloc(1000);
  void *eee = malloc(1000);
  void *fff = malloc(1000);
  /*first superblock*/
  void *ggg = malloc(1000);
  void *hhh = malloc(1000);
  void *iii = malloc(1000);
  /*fourth superblock*/
  void *jjj = malloc(1000);
  void *kkk = malloc(1000);
  void *lll = malloc(1000);

  *(int *)aaa = 1;
  *(int *)bbb = 2;
  *(int *)ccc = 3;
  *(int *)ddd = 4;
  *(int *)eee = 5;
  *(int *)fff = 6;
  *(int *)ggg = 7;
  *(int *)hhh = 8;
  *(int *)iii = 9;
  *(int *)jjj = 10;
  *(int *)kkk = 11;
  *(int *)lll = 12;

  printf("aaa address:%p\n", aaa);
  printf("bbb address:%p\n", bbb);
  printf("ccc address:%p\n", ccc);
  printf("ddd address:%p\n", ddd);
  printf("eee address:%p\n", eee);
  printf("fff address:%p\n", fff);
  printf("ggg address:%p\n", ggg);
  printf("hhh address:%p\n", hhh);
  printf("iii address:%p\n", iii);
  printf("jjj address:%p\n", jjj);
  printf("kkk address:%p\n", kkk);
  printf("lll address:%p\n", lll);
  
  printf("segment 1--------\n");
  printf("aaa value should be 1:%d\n", *(int *)aaa);
  printf("bbb value should be 2:%d\n", *(int *)bbb);
  printf("ccc value should be 3:%d\n", *(int *)ccc);
  printf("ddd value should be 4:%d\n", *(int *)ddd);
  printf("eee value should be 5:%d\n", *(int *)eee);
  printf("fff value should be 6:%d\n", *(int *)fff);
  printf("ggg value should be 7:%d\n", *(int *)ggg);
  printf("hhh value should be 8:%d\n", *(int *)hhh);
  printf("iii value should be 9:%d\n", *(int *)iii);
  printf("jjj value should be 10:%d\n", *(int *)jjj);
  printf("kkk value should be 11:%d\n", *(int *)kkk);
  printf("lll value should be 12:%d\n", *(int *)lll);

  free(aaa);
  free(bbb);
  free(ccc);

  printf("segment 2--------\n");
  printf("aaa value should be junk:%d\n", *(int *)aaa);
  printf("bbb value should be junk:%d\n", *(int *)bbb);
  printf("ccc value should be junk:%d\n", *(int *)ccc);
  printf("ddd value should be 4:%d\n", *(int *)ddd);
  printf("eee value should be 5:%d\n", *(int *)eee);
  printf("fff value should be 6:%d\n", *(int *)fff);
  printf("ggg value should be 7:%d\n", *(int *)ggg);
  printf("hhh value should be 8:%d\n", *(int *)hhh);
  printf("iii value should be 9:%d\n", *(int *)iii);
  printf("jjj value should be 10:%d\n", *(int *)jjj);
  printf("kkk value should be 11:%d\n", *(int *)kkk);
  printf("lll value should be 12:%d\n", *(int *)lll);

  free(ddd);
  free(eee);
  free(fff);
  
  printf("segment 3--------\n");
  printf("aaa value should be junk:%d\n", *(int *)aaa);
  printf("bbb value should be junk:%d\n", *(int *)bbb);
  printf("ccc value should be junk:%d\n", *(int *)ccc);
  printf("ddd value should be junk:%d\n", *(int *)ddd);
  printf("eee value should be junk:%d\n", *(int *)eee);
  printf("fff value should be junk:%d\n", *(int *)fff);
  printf("ggg value should be 7:%d\n", *(int *)ggg);
  printf("hhh value should be 8:%d\n", *(int *)hhh);
  printf("iii value should be 9:%d\n", *(int *)iii);
  printf("jjj value should be 10:%d\n", *(int *)jjj);
  printf("kkk value should be 11:%d\n", *(int *)kkk);
  printf("lll value should be 12:%d\n", *(int *)lll);
  
  free(ggg);
  //free(hhh);
  free(iii);

  printf("segment 4--------\n");
  printf("aaa value should be junk:%d\n", *(int *)aaa);
  printf("bbb value should be junk:%d\n", *(int *)bbb);
  printf("ccc value should be junk:%d\n", *(int *)ccc);
  printf("ddd value should be junk:%d\n", *(int *)ddd);
  printf("eee value should be junk:%d\n", *(int *)eee);
  printf("fff value should be junk:%d\n", *(int *)fff);
  printf("ggg value should be junk:%d\n", *(int *)ggg);
  printf("hhh value should be 8:%d\n", *(int *)hhh);
  printf("iii value should be junk:%d\n", *(int *)iii);
  printf("jjj value should be 10:%d\n", *(int *)jjj);
  printf("kkk value should be 11:%d\n", *(int *)kkk);
  printf("lll value should be 12:%d\n", *(int *)lll);
  
  free(jjj);
  free(kkk);
  
  printf("segment 5--------\n");
  printf("aaa value should be junk:%d\n", *(int *)aaa);
  printf("bbb value should be junk:%d\n", *(int *)bbb);
  printf("ccc value should be junk:%d\n", *(int *)ccc);
  printf("ddd value should be junk:%d\n", *(int *)ddd);
  printf("eee value should be junk:%d\n", *(int *)eee);
  printf("fff value should be junk:%d\n", *(int *)fff);
  printf("ggg value should be junk:%d\n", *(int *)ggg);
  printf("hhh value should be 8:%d\n", *(int *)hhh);
  printf("iii value should be junk:%d\n", *(int *)iii);
  printf("jjj value should be junk:%d\n", *(int *)jjj);
  printf("kkk value should be junk:%d\n", *(int *)kkk);
  printf("lll value should be 12:%d\n", *(int *)lll);
  
  free(lll);

  printf("segment 6--------\n");
  //printf("aaa value should be junk:%d\n", *(int *)aaa);
  //printf("bbb value should be junk:%d\n", *(int *)bbb);
  //printf("ccc value should be junk:%d\n", *(int *)ccc);
  printf("ddd value should be junk:%d\n", *(int *)ddd);
  printf("eee value should be junk:%d\n", *(int *)eee);
  printf("fff value should be junk:%d\n", *(int *)fff);
  printf("ggg value should be junk:%d\n", *(int *)ggg);
  printf("hhh value should be 8:%d\n", *(int *)hhh);
  printf("iii value should be junk:%d\n", *(int *)iii);
  printf("jjj value should be junk:%d\n", *(int *)jjj);
  printf("kkk value should be junk:%d\n", *(int *)kkk);
  printf("lll value should be junk:%d\n", *(int *)lll);
  printf("testing poison\n"); 

  free(hhh);

  printf("segment 7--------\n");
  printf("ggg value should be junk:%d\n", *(int *)ggg);
  printf("hhh value should be 8:%d\n", *(int *)hhh);
  printf("iii value should be junk:%d\n", *(int *)iii);
  printf("jjj value should be junk:%d\n", *(int *)jjj);
  printf("kkk value should be junk:%d\n", *(int *)kkk);
  printf("lll value should be junk:%d\n", *(int *)lll);
  char *poison1 = malloc(200);
  ii = 0;
  printf("about to print a bunch of cds\n");
  for(;ii<256; ii+=8){
    printf("%x", (uint8_t) poison1[ii+7]);
    printf("%x", (uint8_t) poison1[ii+6]);
    printf("%x", (uint8_t) poison1[ii+5]);
    printf("%x", (uint8_t) poison1[ii+4]);
    printf("%x", (uint8_t) poison1[ii+3]);
    printf("%x", (uint8_t) poison1[ii+2]);
    printf("%x", (uint8_t) poison1[ii+1]);
    printf("%x", (uint8_t) poison1[ii]);
    printf("\n");
  }

  free(poison1);
  printf("goinf to print a bunch of abs, first line should be an address\n");
  ii=0;
  for(;ii<256; ii+=8){
    printf("%x", (uint8_t)poison1[ii+7]);
    printf("%x", (uint8_t)poison1[ii+6]);
    printf("%x", (uint8_t)poison1[ii+5]);
    printf("%x", (uint8_t)poison1[ii+4]);
    printf("%x", (uint8_t)poison1[ii+3]);
    printf("%x", (uint8_t)poison1[ii+2]);
    printf("%x", (uint8_t)poison1[ii+1]);
    printf("%x", (uint8_t)poison1[ii]);
    printf("\n");
  }

  printf("\nTesting calloc **calloc(1,24)**...\n");
  char *poison3 = calloc(1,24);
  ii = 0;
  printf("should print a bunch of 0s\n");
  for(;ii<32; ii++){
    printf("%x", *(poison3+ii));
  }
  printf("\n");

  printf("\nTesting realloc... first making a new pointer with malloc(5)\n");
  void *ra3 = malloc(30);
  printf("printing ra1 %p\n", ra3);

  printf("\nTesting realloc... now trying to realloc w smaller size...should return original\n");
  void *ra4 = realloc(ra3, 28);
  //ra1 = realloc(ra1, 28);
  printf("printing ra2 %p\n", ra3);

  printf("\nTesting realloc... now trying to realloc w bigger size...should return new address\n");
  ra4 = realloc(ra3, 64);
  printf("printing ra2, should be a different address%p\n", ra4);
  printf("printing ra1 should be the same address as before%p\n", ra3);
 
  
  return (errno);
}
