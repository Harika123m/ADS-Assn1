#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int record=256;
// since program said maximum size would not cross 10000, max as 10000 instead of dynamic expansion
// the maximum size of record is 200.

typedef struct index_S {
	int key;
	long offset;
	}index_S;

typedef struct avail_S{ 
	int size; 
	long off;  
	} avail_S;


// functions used in this program :
// tmparator function (built in) is used for comparisons and sorting the given list in either ascending or descending
int comparator_1(const void *a, const void *b);
int comparator_2(const void *a, const void *b);
int comparator_3(const void *a, const void *b);

// the three types of fits:
int first(struct avail_S *avail_list,int length_record,int addition,int* avail_size);
int best(struct avail_S *avail_list,int length_record,int addition,int* avail_size);
int worst(struct avail_S *avail_list,int length_record,int addition,int* avail_size);

// functions for adding, finding and deleting a record
int add(int key,struct index_S *key_index,int *index_size);
int find(int key,struct index_S *key_index,int *index_size);
int del(int key,struct index_S *key_index,int *index_size);

// main function 
int main(int argc, char *argv[])
{
index_S index_S[10000];
avail_S avail_S[10000];
char user[500],copy_user[500],*userinput[3], *inputargs, buffer[500]; 
int i,l,k,calling_best_on_add=0, calling_worst_on_add=0,old,holes_data,avail_size=-1,index_size=-1,num_holes=0,empty_holes=0,length_index,length_av,addition=0,start=0,length_record,size_nul;
FILE *ptr_datafile,*ptr_indexfile,*ptr_afile;
long off_pos;



if((fopen(argv[2], "r+b"))!=NULL)
{
	ptr_indexfile=fopen("index.db","rb");
        if(ptr_indexfile!=0) {	
	fseek(ptr_indexfile,start,SEEK_END);
	length_index=ftell(ptr_indexfile);
	fseek(ptr_indexfile,start,SEEK_SET);
        k=0;
    	while(ftell(ptr_indexfile)<length_index)
    	{
    	fread(&index_S[k],sizeof(struct index_S),1,ptr_indexfile);
    	k++;
    	index_size++;
    	}
        

    	
   }
	ptr_afile=fopen("availability.db","rb");
		
	if(ptr_afile!=0) {			
	fseek(ptr_afile,start,SEEK_END);
	length_av=ftell(ptr_afile);
	fseek(ptr_afile,start,SEEK_SET);		    	
    	k=0;
    	while(ftell(ptr_afile)<length_av)
    	{
    	fread(&avail_S[k],sizeof(struct avail_S),1,ptr_afile);
    	k++;
    	avail_size++;
    	}
        

    	
     }
fclose(ptr_indexfile);  
fclose(ptr_afile);
						
}


while(1) {

fgets(user,record,stdin);
strcpy(copy_user,user);
// apply tokenizer to the commands
inputargs=strtok(copy_user," \n");
l=0;
while(inputargs != NULL)
{
userinput[l++]=inputargs;
inputargs=strtok(NULL," ");

}
	
// for the different parts of command, take the input from file and save it in the following variables
char *func;
int key;
char *record_data;

func=userinput[0];
key=atoi(userinput[1]);
record_data=userinput[2];

// if command says end, apply the following function
if(strcmp(func,"end")==0)
{
// after the file management of data is done, the Index and avalability list is printed

ptr_indexfile=fopen("index.db","wb");
fseek(ptr_indexfile,start,SEEK_SET);
for(k=0;k<=index_size;k++) fwrite(&index_S[k],sizeof(struct index_S),1,ptr_indexfile);

fclose(ptr_indexfile);
ptr_afile=fopen("availability.db","wb");
fseek(ptr_afile,start,SEEK_SET);
for(k=0;k<=avail_size;k++) fwrite(&avail_S[k],sizeof(struct avail_S),1,ptr_afile);

fclose(ptr_afile);


printf("Index:\n");
for(k=0;k<=index_size;k++) printf("key=%d: offset=%lu\n",index_S[k].key,index_S[k].offset);
				
printf("Availability:\n");
if(strcmp(argv[1],"--worst-fit")==0)
{
qsort(avail_S,(avail_size+1),sizeof(struct avail_S),comparator_2); // after kalpesh added
for(k=avail_size;k>=0;k--)
{
printf("size=%d: offset=%lu\n",avail_S[k].size,avail_S[k].off);
empty_holes++;
num_holes +=avail_S[k].size;
}
printf("Number of holes: %d\n",empty_holes);
printf("Hole space: %d\n",num_holes);

}



else {
for(k=0;k<=avail_size;k++) 
{
printf("size=%d: offset=%lu\n",avail_S[k].size,avail_S[k].off);
empty_holes++;
num_holes +=avail_S[k].size;
}
printf("Number of holes: %d\n",empty_holes);
printf("Hole space: %d\n",num_holes);

}

return 0;
}



else if(strcmp(func,"add")==0)
{ 

length_record=sizeof(int)+strlen(record_data)-1; // changed the length (-1)

addition=1;
// depending on the command line argument, the following functions are applied
if(avail_size>=0)
{ 
if (strcmp(argv[1],"--first-fit")==0) holes_data=first(avail_S,length_record,addition, &avail_size);


else if(strcmp(argv[1],"--best-fit")==0)
{
holes_data=best(avail_S,length_record,addition, &avail_size);
calling_best_on_add++;
}
else if(strcmp(argv[1],"--worst-fit")==0)
{
holes_data=worst(avail_S,length_record,addition, &avail_size);
calling_worst_on_add++; 
}
} 



if(add(key,index_S,&index_size)==-1) printf("Record with SID=%d exists\n",key); // changed to -1
else
{ 
index_size++;

index_S[index_size].key= key;


if((ptr_datafile=fopen(argv[2],"r+b"))==NULL) ptr_datafile=fopen(argv[2],"w+b");
		
if(avail_size>=0)	
	{  	
if(holes_data==-1) off_pos=index_S[index_size-1].offset+length_record;

else
{  
off_pos=avail_S[holes_data].off;
for(k=holes_data;k<(avail_size);k++)
{
avail_S[k].size=avail_S[k+1].size;
avail_S[k].off=avail_S[k+1].off;
}
avail_size--;
if(calling_best_on_add>0)   qsort(avail_S,(avail_size+1),sizeof(struct avail_S),comparator_2); 


if(calling_worst_on_add>0)  qsort(avail_S,(avail_size+1),sizeof(struct avail_S),comparator_3); 


}  

} 
else if(index_size==0)  off_pos=0;

else
{	
fseek(ptr_datafile,start, SEEK_END);
off_pos = ftell(ptr_datafile);
}
old=length_record; // changed length of record
index_S[index_size].offset=off_pos;

fseek(ptr_datafile,off_pos,SEEK_SET);
int size=strlen(record_data)-1;
fwrite(&size,sizeof(int),1,ptr_datafile);
int toseek=off_pos+2;

fseek(ptr_datafile,(off_pos+sizeof(int)),SEEK_SET);
fwrite(userinput[2],size,1,ptr_datafile);
////////////// added fwrite
fclose(ptr_datafile);
int len_qsort=index_size+1;
qsort(index_S,len_qsort,sizeof(struct index_S),comparator_1);

} 

}

// if the command is find, the following is executed
else if(strcmp(func,"find")==0)
{
long a=find(key,index_S,&index_size);


if(a==-1)  printf("No record with SID=%d exists\n",key);
else

{
ptr_datafile=fopen(argv[2],"rb");

fseek(ptr_datafile,a,SEEK_SET);
fread(&size_nul,sizeof(int), 1, ptr_datafile); // changed &length_record to size_nul

fseek(ptr_datafile,(sizeof(int)+a),SEEK_SET);
 

fread(&buffer,size_nul,1,ptr_datafile); // change &length_record to size_nul
fclose(ptr_datafile);
buffer[size_nul] = '\0'; // change &length_record to size_nul
printf("%s\n",buffer);



}

}

// if the command line says delete, the following code is executed
else if(strcmp(func,"del")==0)
{
long b= del(key,index_S,&index_size);

if(b==-1)   printf("No record with SID=%d exists\n",key);
else
{
ptr_datafile=fopen(argv[2],"r+b");

fseek(ptr_datafile,b,SEEK_SET);
fread(&size_nul,sizeof(int), 1, ptr_datafile);// changed &length_record to size_nul

avail_size++;			
avail_S[avail_size].off=b;
avail_S[avail_size].size=size_nul+sizeof(int);// changed &length_record to size_nul
fclose(ptr_datafile);



}


}


} 


return 0;
}


int comparator_1(const void *a, const void *b)
{
index_S *index_a=(index_S *) a;
index_S *index_b=(index_S *) b;
return index_a->key-index_b->key;

}

int comparator_2(const void *a, const void *b)
{
avail_S *avail_a=(avail_S *) a;
avail_S *avail_b=(avail_S *) b;
return avail_a->size-avail_b->size;

}
int comparator_3(const void *a, const void *b)
{
avail_S *avail_a=(avail_S *) a;
avail_S *avail_b=(avail_S *) b;
return avail_b->size-avail_a->size;

}

int first(struct avail_S *avail_list,int length_record,int addition,int* avail_size)
{	
int i,size_diff,holes_data,is_avail=0;

if(addition==1)
{
for(i=0;i<=*avail_size;i++)
{
if(avail_list[i].size>=(length_record))
{
holes_data=i;
is_avail=1;
size_diff=avail_list[holes_data].size - (length_record); 

if(size_diff>0)
{
++(*avail_size);							 
avail_list[*avail_size].size=size_diff;  
avail_list[*avail_size].off=avail_list[holes_data].off + length_record; 


}

break;
}
}

if (is_avail==1) return holes_data;

else return -1;


}

exit(1);
}

int best(struct avail_S *avail_list,int length_record,int is_add,int* avail_size)
{	
int i,size_diff,holes_data,is_avail=0;


for(i=0;i<=*avail_size;i++)
{
if(avail_list[i].size>=(length_record))
{
holes_data=i;
is_avail=1;
size_diff=avail_list[holes_data].size - (length_record); 

if(size_diff>0)
{
++(*avail_size);							 
avail_list[*avail_size].size=size_diff;  
avail_list[*avail_size].off=avail_list[holes_data].off + length_record; 


}

break;
}
}

if (is_avail==1)
return holes_data;

else 
return -1;


}

int worst(struct avail_S *avail_list,int length_record,int is_add,int* avail_size)
{	
int i,size_diff,holes_data,is_avail=0;
		

for(i=*avail_size;i>=0;i--)
{
if(avail_list[i].size>=(length_record))
{
holes_data=i;
is_avail=1;
size_diff=avail_list[holes_data].size - (length_record); 

if(size_diff>0)
{
++(*avail_size);							 
avail_list[*avail_size].size=size_diff;  
avail_list[*avail_size].off=avail_list[holes_data].off + length_record; 


}

break;
}
}

if (is_avail==1)
return holes_data;

else 
return -1;


}



int add(int key,struct index_S *key_index,int *index_size)
{
int i;


for(i=0;i<=*index_size;i++)
{
if(key_index[i].key==key) return -1;		

}
return 1;
}


int find(int key,struct index_S *key_index,int *index_size)
{
int i,current;
long offset=-1;
int find=0;


for(i=0;i<=*index_size;i++)
{
if(key_index[i].key==key)			
{
current=i;

offset=key_index[i].offset;
if(find)
{
for(i=current;i<((*index_size));i++)
{  
key_index[i].offset=key_index[i+1].offset;
key_index[i].key=key_index[i+1].key;
}

(*index_size)--;

}
break;

}

}

 return offset;	

}
int del(int key,struct index_S *key_index,int *index_size)
{
int i,current;
long offset=-1;
int del=1;

for(i=0;i<=*index_size;i++)
{
if(key_index[i].key==key)			
{
current=i;
offset=key_index[i].offset;

if(del)
{
for(i=current;i<((*index_size));i++)
{  
key_index[i].offset=key_index[i+1].offset;
key_index[i].key=key_index[i+1].key;
}

(*index_size)--;

}
break;

}

}

 return offset;	

}


















