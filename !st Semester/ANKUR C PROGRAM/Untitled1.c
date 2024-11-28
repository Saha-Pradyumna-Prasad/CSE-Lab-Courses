#include<stdio.h>
int main()
{
    int id;
  printf("Please enter last 3 digit of your id:");
  scanf("%d", &id);
  printf("You are in:");
    if(id%2)
    {
      if(id<160)
        printf("CSE\n");

    else

        printf("EEE\n";
    }
    else
{
    if(id<161)
        printf("CE\n");
    else
        printf("LAW\n");

    }
return 0;


}
