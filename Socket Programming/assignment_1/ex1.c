#include <stdio.h>


int main(int argc, char **argv){

struct route{ 
   int routeID;
   char descrp[25];

};

  struct Route{ 
   int routeID;
   char descrp[25];

};


 struct Route route1;
 struct Route longroutes[10];
 struct Route *routePtr;


 printf("Enter the input\n");
 scanf("%d", &route1.routeID);
 scanf("%s", &route1.descrp);

 longroutes[2] = route1;
 routePtr = &longroutes[2];

// printf("%d", sizeof(route1.routeID));
 printf("value of longroutes %d and %s\n", routePtr->routeID, routePtr->descrp);
// printf("value of routeID is %d\n",longroutes[2].routeID);
 //printf("value of descrp is %s\n", longroutes[2].descrp);





} 
