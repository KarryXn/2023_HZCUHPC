#include <bits/stdc++.h>
#include<omp.h>
#include<sys/time.h>
using namespace std;

int main ()
{
    int i,j;
struct timeval start;
    gettimeofday(&start, NULL);

#pragma omp parallel for
    for(i=0;i<10000;i++)
    {
for(j=0;j<10000;j++)
       } 


 struct timeval end;
    gettimeofday (&end, NULL);
    printf("Using time : %f ms\n", (end.tv_sec-start.tv_sec)*1000.0+(end.tv_usec-start.tv_usec)/1000.0);

}

