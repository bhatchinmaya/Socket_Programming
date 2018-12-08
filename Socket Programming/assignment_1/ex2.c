#include <stdio.h>

int main(int argc, char **argv){

 double taxrate = 7.3, discountrate;
 char buyer[100], seller[100];
 double *tmpPtr; 
 tmpPtr = &taxrate; 

 printf("value of taxrate %f\n", *tmpPtr);
 discountrate = *tmpPtr;
 printf("value of discountrate %f\n",discountrate);
 printf("address of taxrate %p\n", &taxrate);
 printf("address in tmpPtr %p\n", tmpPtr);

 
 strcpy(buyer, "omsairam");
 strcpy(seller, buyer);
 printf("buyer %s\n", buyer);
 printf("seller %s\n", seller);

 int ret = memcmp(buyer, seller, 8);
 printf("value of memcmp %d\n", ret);

 char new_str[25] = '';
 strcat(new_str, buyer);
 strcat(new_str, seller);
 printf("new string %s \n", new_str);  
 
 int len = strlen(buyer);
 printf("lenght of the buyer array %d \n", len);


}



 
