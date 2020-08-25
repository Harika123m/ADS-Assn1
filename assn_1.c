//Date   : jan 23 2018
//Author : M. Harika

#include<stdio.h>
#include<string.h>
#include <sys/time.h>

int binary_search(int,int,int);
int binary_disk_search(int,int,int,FILE *);


int main(int argc, char *argv[])
{ 
// common part for 4 operations

 int count,i;
 int binary_count=0;
 FILE *key;
 FILE *seek;
 struct record
{ 
  int a;
};

 
 struct record myRecord_seek;
 struct record myRecord_key;
 struct record disk_lin;

//-------------------------
struct timeval op1,op2,exec_tm_open;
gettimeofday( &op1, NULL ); 
key=fopen(argv[2],"rb");
gettimeofday( &op2, NULL ); 


exec_tm_open.tv_sec=op2.tv_sec-op1.tv_sec;
exec_tm_open.tv_usec=op2.tv_usec-op1.tv_usec;

//printf( "Time: %ld.%06ld", exec_tm_open.tv_sec, exec_tm_open.tv_usec );

//---------------------------

 seek=fopen(argv[3],"rb");


 if (!seek)
 { 
    printf("Unable to open seek data file :(");     return 1;
 }
 if(!key)
 {
      printf("Unable to open keys data file :(");  return 1;
 }

 //open seek file for knowing length
 fseek(seek,0,SEEK_END);
 int sz_seek;
 sz_seek=ftell(seek);
 int s_seek=sz_seek/4;


// open key file for knowing length
 fseek(key,0,SEEK_END);
 int sz_key;
 sz_key=ftell(key);
 int s_key=sz_key/4;
 

// go back to start of key file


//--------------------------------------------------------
fseek(key,0,SEEK_SET);
//--------------------------------------------------------
int arr_key[s_key];





// go back to start of seek file
 fseek(seek,0,SEEK_SET);
 int arr_seek[s_seek];


for ( count=0; count <s_seek ; count++)
 { 
  fread(&myRecord_seek,sizeof(int),1,seek);  
 // printf("%d\n",myRecord_seek.a);
  arr_seek[count]=myRecord_seek.a;
 }  fclose(seek); 







//-------------------------------------------------------------
int binary_search(int start, int end, int item)
{
 if(end>=start)
 { 
  int middle=(((start+end)%2)==0)? ((start+end)/2) : ((end-1+start)/2);
  
  if(item==arr_key[middle]) return 1;
  if(item<arr_key[middle]){ return binary_search(start,middle-1, item);}
  return binary_search(middle+1,end,item);
 }

  return 0;

}


int binary_disk_search(int start,int end,int item,FILE *ptr3)
{

     if(end>=start)
    {
     int k;
     int middle=(((start+end)%2)==0)? ((start+end)/2) : ((end-1+start)/2);
     fseek(ptr3,middle*sizeof(int),SEEK_SET);
     fread(&disk_lin,sizeof(int),1,ptr3); 

     k=disk_lin.a;
    if(k==item) return 1 ;
     if(item<k) return binary_disk_search(start,middle-1,item,ptr3);
     return binary_disk_search(middle+1,end,item,ptr3);
    }
     

  return 0 ;
   }
//----------------------------------------------------------------------

if(strcmp(argv[1],"--mem-lin")==0)
{

struct timeval time1,time2,exec_tm;

int linear_count=0;
int output_linear[s_seek];

gettimeofday( &time1, NULL ); 
for ( count=0; count <s_key ; count++)
 { 
  fread(&myRecord_key,sizeof(int),1,key); 

  arr_key[count]=myRecord_key.a;

 }  fclose(key); 

//printf( "\n Seconds: %ld\n", tm1.tv_sec );
//printf( "\n Microseconds: %ld\n", tm1.tv_usec );
for(count=0;count<s_seek;count++) 
 {

  output_linear[count]=0;
  for(i=0;i<s_key;i++)
 {
  if(arr_seek[count]==arr_key[i])
   {
     output_linear[count]=1;
     printf("%12d: Yes\n",arr_seek[count]);
     linear_count++;
     break;
   }
             
  }
   if(output_linear[count]==0) printf("%12d: No\n", arr_seek[count]);

}

gettimeofday( &time2, NULL ); 

exec_tm.tv_sec=time2.tv_sec-time1.tv_sec;
exec_tm.tv_usec=time2.tv_usec-time1.tv_usec;

printf( "Time: %ld.%06ld", exec_tm.tv_sec+exec_tm_open.tv_sec, exec_tm.tv_usec+exec_tm_open.tv_usec );


}






if(strcmp(argv[1],"--mem-bin")==0)
{
 struct timeval time1,time2,exec_tm;
 int output_binary[s_seek];
 int binary_count=0;

 gettimeofday( &time1, NULL ); 

for ( count=0; count <s_key ; count++)
 { 
  fread(&myRecord_key,sizeof(int),1,key); 

  arr_key[count]=myRecord_key.a;

 }  fclose(key); 

for(count=0;count<s_seek;count++)
{


  output_binary[count]=  binary_search(0,s_key,arr_seek[count]);
  if(output_binary[count]==1)
 {
    binary_count++;
    printf("%12d: Yes\n",arr_seek[count]);
    
 }
 if(output_binary[count]==0)
 {
   printf("%12d: No\n", arr_seek[count]);
 }
     
}  

 gettimeofday( &time2, NULL ); 
exec_tm.tv_sec=time2.tv_sec-time1.tv_sec;
exec_tm.tv_usec=time2.tv_usec-time1.tv_usec;

printf( "Time: %ld.%06ld", exec_tm.tv_sec + exec_tm_open.tv_sec, exec_tm.tv_usec + exec_tm_open.tv_usec );


}

 

if(strcmp(argv[1],"--disk-lin")==0)
{
  struct timeval time1,time2,exec_tm;
  gettimeofday( &time1, NULL ); 
  FILE *ptr3;
  int k1;
  ptr3=fopen(argv[2],"rb");

  int haha=0;
  for(count=0;count<s_seek;count++)
    {
         fseek(ptr3,0,SEEK_SET);
        int output_disk_lin=0;
      for(i=0;i<s_key;i++)
        {
            fread(&disk_lin,sizeof(int),1,ptr3); 
            k1=disk_lin.a;
            if(k1==arr_seek[count])
                  {
                    printf("%12d: Yes\n",arr_seek[count]);
                    output_disk_lin=1;
                    haha++;
                 } 

                 
         }
     if(output_disk_lin==0)   printf("%12d: No\n",arr_seek[count]);
   }

  gettimeofday( &time2, NULL ); 
 exec_tm.tv_sec=time2.tv_sec-time1.tv_sec;
exec_tm.tv_usec=time2.tv_usec-time1.tv_usec;

printf( "Time: %ld.%06ld", exec_tm.tv_sec + exec_tm_open.tv_sec, exec_tm.tv_usec+exec_tm_open.tv_usec );

}


if(strcmp(argv[1],"--disk-bin")==0)

{
   struct timeval time1,time2,exec_tm;
  gettimeofday( &time1, NULL ); 
  FILE *ptr3;
  int s1;
  ptr3=fopen(argv[2],"rb");

  int outcome_disk_binary=0;
  int haha=0;
  for(count=0;count<s_seek;count++)
    {
        fseek(ptr3,0,SEEK_SET);
        s1=arr_seek[count];
        haha=binary_disk_search(0,s_key,s1,ptr3);
        if(haha==1) 
        {
         outcome_disk_binary++;
         printf("%12d: Yes\n",arr_seek[count]);
        }
        else    printf("%12d: No\n",arr_seek[count]);
         

   }
     


 gettimeofday( &time2, NULL ); 
fclose(ptr3);
 exec_tm.tv_sec=time2.tv_sec-time1.tv_sec;
exec_tm.tv_usec=time2.tv_usec-time1.tv_usec;

printf( "Time: %ld.%06ld", exec_tm.tv_sec + exec_tm_open.tv_sec, exec_tm.tv_usec+exec_tm_open.tv_usec );

}


 return 0;
}


















