#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<sys/time.h>

// helper functions :

int comparator(const void *a, const void *b)
{
    int l = *(const int *)a;
    int r = *(const int *)b;
    return (l-r);
}

int findmin(int *pointers, int block_size)
{

    int i;
    int result;
    for( i = 0; i < block_size; i++)
    {
        if( *(pointers + i) != -1)
        {
            result = *(pointers + i);
            return result;
        }
    }
    return -1;
}

void getmin(int *pointers, int block_size)
{
    int i;

    for(i = 0; i < block_size; i++)
    {
        if( *(pointers + i) != -1)
        {
            *(pointers + i) = -1;
            break;
        }
    }
}


void sift(int *heap, int i, int n)
{
    int j, k, smallest, temp;
    while((i*2)+1<n)
    {
        j=(i*2)+1;
        k=j+1;
        if(k<n && heap[k]<heap[j])
        {
            smallest=k;
        }
        else
        {
            smallest=j;
        }
        if(heap[i]<heap[smallest])
        {
            return;
        }
        temp=heap[i];
        heap[i]=heap[smallest];
        heap[smallest]=temp;
        i=smallest;
    }
}

void heapify(int *heap, int n)
{
    int i=n/2;
    for(i;i>=0;i--)
    {
        sift(heap,i,n);
    }

}

void heap_sort(int *heap, int n)
{
    int i=n/2,j=1;
    for(i;i>=0;i--)
    {
        sift(heap,i,n);
    }
    while(n-1>0)
    {

        heapify(&heap[j],n-1);
        j++;
        n--;
    }
}

// important  functions :

void basic(char *input_file_name, int sz_inp, char *output_file_name);
void multistep(char *input_file_name, int sz_inp, char *output_file_name);
void replacement(char *input_file_name, int sz_inp, char *output_file_name);
void merge(FILE *out, int runs, int type, int runs_to_merge,char *input_file_name);


// function main :
int main(int argc, char *argv[])
{


if(argc!=4)
   {
     printf("\n incorrect number of arguments ");
     return 0;
   }
 // argv[0] is a.out
 // argv[1] is type of merge
 // argv[2] is input file name 
 // argv[3] is output file 
  
  
  struct timeval op1,op2,exec_tm_open;
  FILE *inp;
  inp=fopen(argv[2],"r");
  fseek(inp,0,SEEK_END);
  int sz_inp;
  sz_inp=ftell(inp);
  sz_inp/=4;
  fclose(inp);
  //printf("\n found input file length : %d", sz_inp);


  if((strcmp(argv[1], "--basic"))==0)
  {

	   gettimeofday( &op1, NULL ); 
	   basic(argv[2], sz_inp, argv[3]);
	   //printf("\n called basic");
	   gettimeofday( &op2, NULL );
           exec_tm_open.tv_sec=op2.tv_sec-op1.tv_sec;
           exec_tm_open.tv_usec=op2.tv_usec-op1.tv_usec;
          /* if(exec_tm_open.tv_usec<1) {
           exec_tm_open.tv_usec + 1000000 ;
           exec_tm_open.tv_sec-1;
            } */
           //printf( "\nTime: %ld.%06ld", exec_tm_open.tv_sec, exec_tm_open.tv_usec );
printf("Time: %lf\n", (op2.tv_sec - op1.tv_sec)+(op2.tv_usec-op1.tv_usec)/1000000.0);

  }
 
  if((strcmp(argv[1], "--multistep"))==0)
     {

           gettimeofday( &op1, NULL ); 
	   multistep(argv[2], sz_inp, argv[3]);
           //printf("\n called multistep");
	   gettimeofday( &op2, NULL );
           exec_tm_open.tv_sec=op2.tv_sec-op1.tv_sec;
           exec_tm_open.tv_usec=op2.tv_usec-op1.tv_usec;
          /* if(exec_tm_open.tv_usec<1) {
           exec_tm_open.tv_usec + 1000000 ;
           exec_tm_open.tv_sec-1;
            }
           printf( "\nTime: %ld.%06ld", exec_tm_open.tv_sec, exec_tm_open.tv_usec ); */
printf("Time: %lf\n", (op2.tv_sec - op1.tv_sec)+(op2.tv_usec-op1.tv_usec)/1000000.0);
     }

  if((strcmp(argv[1], "--replacement"))==0) 
    {

          gettimeofday( &op1, NULL ); 
	  replacement(argv[2], sz_inp, argv[3]);
         // printf("\n called replacemnt");
	  gettimeofday( &op2, NULL );
          exec_tm_open.tv_sec=op2.tv_sec-op1.tv_sec;
          exec_tm_open.tv_usec=op2.tv_usec-op1.tv_usec;
          /*if(exec_tm_open.tv_usec<1) {
           exec_tm_open.tv_usec + 1000000 ;
           exec_tm_open.tv_sec-1;
            }
          printf( "\nTime: %ld.%06ld", exec_tm_open.tv_sec, exec_tm_open.tv_usec );  */
printf("Time: %lf\n", (op2.tv_sec - op1.tv_sec)+(op2.tv_usec-op1.tv_usec)/1000000.0);
     }
         
 
}

void basic(char *input_file_name, int sz_inp, char *output_file_name)
{
          FILE *inp;
          inp=fopen(input_file_name,"r");
	  int i;
	  int runs=sz_inp/1000;
	  FILE *out;
          out=fopen(output_file_name,"w");
	  if( (runs*1000) != sz_inp) runs++;
	  if(sz_inp<1000) // say it has just 1 run of length less than 1000
	   {
             //printf("\n called basic when just 1 run");
             int *less_1000_input=malloc(sz_inp*sizeof(int));
             char *filename=malloc(sizeof(char) * 10);
             strcpy(filename,input_file_name);
	     strcat(filename, "000");
             FILE *file_less_than_one;
             file_less_than_one=fopen(filename,"w");
             fread(less_1000_input, sizeof(int), sz_inp, inp);
             qsort(less_1000_input,sizeof(int),sz_inp,comparator);
             fwrite(less_1000_input,sizeof(int), sz_inp,file_less_than_one);
             //printf("\n wrote in o/p file for case of 1 run");
             fclose(file_less_than_one);
             //printf("\n closed file for only 1 run");
             free(less_1000_input);
             free(filename);
	   
	   }
	  else  // say there are more than 1 run
	 {
	        //  printf("\n found more than 1 run");
		  for(i=1; i<= runs-1; i++)
		     {
                       int *inpbuffer=malloc(sizeof(int)*1000);
		       fread(inpbuffer, sizeof(int), 1000, inp);
		       qsort(inpbuffer, 1000, sizeof(int), comparator);
		       char *filename2=malloc(sizeof(char)*15);
		       char *index=malloc(sizeof(char)*4);
		       strcpy(filename2,input_file_name);
		       sprintf(index, ".%03d", i - 1);
		       strcat(filename2, index);
		       FILE *file;
		       file=fopen(filename2,"w");
		       fwrite(inpbuffer, sizeof(int), 1000, file);
                      // printf("\n write to file %s", filename2);
		       fclose(file);



		     }
                      int *inpbuffer=malloc(sizeof(int)*1000);

		    fread(inpbuffer, sizeof(int), (size_t)(sz_inp-((1000*(runs-1)))), inp); // The last run may have less than 1000 int

		    qsort(inpbuffer,(size_t)(sz_inp-((1000*(runs-1)))), sizeof(int), comparator);
		    char *filename1=malloc(sizeof(char)*12);
		    char *index = malloc(4 * sizeof(char));
		    sprintf(index, ".%03d", runs - 1);
		    strcpy(filename1, input_file_name);
		    strcat(filename1, index);
		    FILE *file;
		    file = fopen(filename1, "w");
		    fwrite(inpbuffer, sizeof(int),(size_t)(sz_inp-((1000*(runs-1)))), file);
                    //printf("\n wrote to file %s", filename1);
		    fclose(file);
		    fclose(inp);
		    free(inpbuffer);
		    free(index);
		    free(filename1);

		

	 }



   int runs_to_merge=0;
  // printf("\n sending to merge from basic");
   merge(out, runs,0,runs_to_merge,input_file_name);
   fclose(out);
   }

void merge(FILE *out, int runs, int type, int runs_to_merge,char *input_file_name)
{
  char *pattern=malloc(sizeof(char) * (strlen(input_file_name)+10));
  strcpy(pattern,input_file_name);
  if(type==0) strcat(pattern,".%03d");
  if(type==1) strcat(pattern,".super.%03d");
      
 // printf("\n came to merge and no of runs are : %d", runs);
  if(runs==1)
    { 
     // printf("\n entered merge : case where runs are 1");
      char *file=malloc(12*sizeof(char));
      sprintf(file,pattern,0);
      FILE *f_one;
      f_one=fopen(pattern,"r");
      fseek(f_one,0,SEEK_END);
      int sz_f_one;
      sz_f_one=ftell(f_one);
      sz_f_one/=4;
      int *only_one_run=malloc(sizeof(int)*(sz_f_one));
      fread(only_one_run, sizeof(int), sz_f_one, f_one);
      fwrite(only_one_run, sizeof(int), sz_f_one, out);

    }
  else
    {
      int i;
      //printf("\n entered merge : case of  runs %d", runs);
      int *inpbuffer=malloc(sizeof(int)*1000);
      int *outbuffer=malloc(sizeof(int)*1000);
      int block_size=1000/runs;
      char *file_names=malloc(sizeof(char) * strlen(pattern)); // changed ..
      FILE *run_files[runs];
      for(i=0;i<runs;i++)
       {
         sprintf(file_names,pattern,i+runs_to_merge);
         run_files[i]=fopen(file_names,"r");
        // printf("\n created run file for %s", file_names);
       }
       int *pointers[runs];
      for(i=0;i<runs;i++)
        {

          fread(inpbuffer+i*block_size,sizeof(int), (size_t) block_size, run_files[i]);
          pointers[i]=inpbuffer+i*block_size;
          //printf("\n read files and fixed pointer for run_files : %d",i); 
        }
      int key;
    int element_min;
    int index; 
    int out_index = 0;
    int empty_file = 0;
    while(empty_file < runs)
    {
        element_min = INT_MAX;
        index = -1;
        for( i = 0; i < runs; i++)
        {
            key = findmin(pointers[i], block_size);
            if( key != -1)
            {
                if( key < element_min)
                {
                    element_min = key;
                    index = i;
                }
            }
            else
            {
                if(run_files[i] != NULL && !feof(run_files[i]))
                {
                    fread(pointers[i], sizeof(int), (size_t)block_size, run_files[i]);
                    key = findmin(pointers[i], block_size);
                    if(key == -1)
                    {
                        empty_file++;
                        run_files[i] = NULL;
                    }
                    else if ( key < element_min) {
                        element_min=key;
                        index = i;
                    }
                }
                else if(run_files[i]!=NULL)
                {
                    empty_file++;
                    run_files[i]=NULL;
                } // else

            }
        }//for
        if( index != -1)
        {
            getmin(pointers[index], block_size); 
            outbuffer[out_index]=element_min;
            out_index++;
            if( out_index>=1000) 
            {
                out_index = 0;
                fwrite(outbuffer, sizeof(int), 1000, out);
            }
        }
    } // while

    if(out_index != 0) fwrite(outbuffer, sizeof(int), (size_t)out_index, out);
    
    free(inpbuffer);
    free(outbuffer); 
} // top else

} // merge
//----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void multistep(char *input_file_name,int sz_inp, char *output_file_name)
{



	  FILE *inp;
          inp=fopen(input_file_name,"r");
	  int i;
	  int runs=sz_inp/1000;
	  FILE *out;
          out=fopen(output_file_name,"w");
	  if( (runs*1000) != sz_inp) runs++;
	  if(sz_inp<1000) // say it has just 1 run of length less than 1000
	   {
            // printf("\n called basic when just 1 run");
             int *less_1000_input=malloc(sz_inp*sizeof(int));
             char *filename=malloc(sizeof(char) * 10);
             strcpy(filename,input_file_name);
	     strcat(filename, "000");
             FILE *file_less_than_one;
             file_less_than_one=fopen(filename,"w");
             fread(less_1000_input, sizeof(int), sz_inp, inp);
             qsort(less_1000_input,sizeof(int),sz_inp,comparator);
             fwrite(less_1000_input,sizeof(int), sz_inp,file_less_than_one);
            // printf("\n wrote in o/p file for case of 1 run");
             fclose(file_less_than_one);
             //printf("\n closed file for only 1 run");
             free(less_1000_input);
             free(filename);
	   
	   }
	  else  // say there are more than 1 run
	 {
	         // printf("\n found more than 1 run");
		  for(i=1; i<= runs-1; i++)
		     {
                       int *inpbuffer=malloc(sizeof(int)*1000);
		       fread(inpbuffer, sizeof(int), 1000, inp);
		       qsort(inpbuffer, 1000, sizeof(int), comparator);
		       char *filename2=malloc(sizeof(char)*15);
		       char *index=malloc(sizeof(char)*4);
		       strcpy(filename2,input_file_name);
		       sprintf(index, ".%03d", i - 1);
		       strcat(filename2, index);
		       FILE *file;
		       file=fopen(filename2,"w");
		       fwrite(inpbuffer, sizeof(int), 1000, file);
                       //printf("\n write to file %s", filename2);
		       fclose(file);
		     }
                      int *inpbuffer=malloc(sizeof(int)*1000);

		    fread(inpbuffer, sizeof(int), (size_t)(sz_inp-((1000*(runs-1)))), inp); // The last run may have less than 1000 int

		    qsort(inpbuffer,(size_t)(sz_inp-((1000*(runs-1)))), sizeof(int), comparator);
		    char *filename1=malloc(sizeof(char)*12);
		    char *index = malloc(4 * sizeof(char));
		    sprintf(index, ".%03d", runs - 1);
		    strcpy(filename1, input_file_name);
		    strcat(filename1, index);
		    FILE *file;
		    file = fopen(filename1, "w");
		    fwrite(inpbuffer, sizeof(int),(size_t)(sz_inp-((1000*(runs-1)))), file);
                    //printf("\n wrote to file %s", filename1);
		    fclose(file);
		    fclose(inp);
		    free(inpbuffer);
		    free(index);
		    free(filename1);

		

	 }


    int super_runs = runs / 15;
    if( super_runs*15<runs) super_runs++;
    
    char *filename = malloc((strlen(input_file_name) + 10) * sizeof(char));

    for( i = 0; i < super_runs - 1; i++ )
    {
        char *index = malloc(10 * sizeof(char));
        sprintf(index, ".super.%03d", i);
        strcpy(filename, input_file_name);
        strcat(filename, index);


        FILE *super_run_file = fopen(filename, "w");
        

        merge(super_run_file,15, 0,i*15,input_file_name); //mmmmmmm

        free(index);
        fclose(super_run_file);
    }
    char *index = malloc(10 * sizeof(char));
    sprintf(index, ".super.%03d", super_runs - 1);
    strcpy(filename, input_file_name);
    strcat(filename, index);

    FILE *super_run_file = fopen(filename, "w");
    

    merge(super_run_file, runs - 15 * (super_runs - 1) ,0,0 + (super_runs-1) * 15,input_file_name); // mmmmm
    fclose(super_run_file);

    free(index);
    free(filename);


    
    merge(out,super_runs ,1,0,input_file_name); // mmmmm

}


//-------------------------
//----------------------------------------------------------------------------------------------


//         merge(0, 1, outfile, name_pattern);
// void merge(FILE *out, int runs, int type, int runs_to_merge,char *input_file_nmae);
void replacement(char *input_file_name,int sz_inp, char *output_file_name)
{
    FILE *inp = fopen(input_file_name, "r");

    if(sz_inp<1000)
    {
        int *inpbuffer = malloc(sz_inp * sizeof(int));
        fread(inpbuffer, sizeof(int), sz_inp, inp);
        qsort(inpbuffer, sz_inp, sizeof(int), comparator);
        char *filename4 = malloc((strlen(input_file_name) + 4) * sizeof(char));
        strcpy(filename4, input_file_name);
        strcat(filename4, ".000");
        FILE *less_than_one_run = fopen(filename4, "w");
        fwrite(inpbuffer, sizeof(int), sz_inp, less_than_one_run);
        fclose(less_than_one_run);
        free(filename4);
        free(inpbuffer);
        FILE *out = fopen(output_file_name, "w");
        merge(out,1,0,0,input_file_name);          
    }
   else
  {
    int number=0,out_index=0,buff_index=0,done=0,remaining=0;
    int *primary_heap = malloc(750 * sizeof(int));
    int *buff=malloc(250 * sizeof(int));
    int *output = malloc(1000 * sizeof(int));
    int num_in_heap=750, secondary_heap_p=750;
    char *filename = malloc((strlen(input_file_name) + 4) * sizeof(char));
    int file_index=1,buff_available=1;
    char *index = malloc(4 * sizeof(char));
    sprintf(index, ".%03d", file_index - 1);
    strcpy(filename, input_file_name);
    strcat(filename, index);
    FILE *current_file = fopen(filename, "w");
    fread(primary_heap, sizeof(int), 750, inp);
    fread(buff, sizeof(int), 250, inp);
    heapify(primary_heap,750);
   while(1)
    {
        if(buff_available && buff_index == remaining && done==sz_inp-1000)
        {
            buff_available = 0;
            secondary_heap_p = num_in_heap;
        }
        if(buff_available)
        {
            if(num_in_heap>0)
            {
                output[out_index] = primary_heap[0];
                out_index++;
                if(out_index == 1000)
                {
                    fwrite(output, sizeof(int), 1000, current_file);
                    number+=1000;
                    out_index = 0;
                }
                if(primary_heap[0] <= buff[buff_index])
                {
                    primary_heap[0] = buff[buff_index];
                    buff_index++;
                    if(buff_index == 250 && sz_inp-1000-done>250)
                    {
                        fread(buff,sizeof(int),250,inp);
                        done+=250;
                        buff_index = 0;
                    }
                    else if(buff_index == 250 && sz_inp-1000-done<= 250)
                    {
                        fread(buff, sizeof(int), (size_t) (sz_inp-1000-done),inp);
                        remaining=sz_inp-1000-done;
                        done= sz_inp-1000;
                        buff_index=0;
                    }
                    heapify(primary_heap, num_in_heap);
                }
                else
                {
                    primary_heap[0] = primary_heap[num_in_heap-1];
                    primary_heap[num_in_heap-1] = buff[buff_index];
                    buff_index++;
                    if(buff_index == 250 && sz_inp-1000-done>250)
                    {
                        fread(buff, sizeof(int), 250, inp);
                        done+= 250;
                        buff_index = 0;
                    }
                    else if(buff_index == 250 && sz_inp - 1000-done<=250)
                    {
                        fread(buff, sizeof(int), (size_t) (sz_inp-1000-done),inp);
                        remaining=sz_inp-1000-done;
                        done= sz_inp - 1000;
                        buff_index = 0;
                    }
                    num_in_heap--;
                    heapify(primary_heap, num_in_heap);
                }

            }
            else
            {

                if(out_index>0)
                {
                    fwrite(output, sizeof(int), (size_t) out_index, current_file);
                    number+=out_index;
                    out_index = 0;
                }
                fclose(current_file);
                file_index++;
                sprintf(index, ".%03d", file_index - 1);
                strcpy(filename, input_file_name);
                strcat(filename, index);
                current_file = fopen(filename, "w");
                num_in_heap = 750;
                heapify(primary_heap, num_in_heap);
            }
        }
        else
        {

            if(num_in_heap > 0)
            {
                output[out_index] = primary_heap[0];
                out_index++;
                if(out_index == 1000)
                {
                    fwrite(output, sizeof(int), 1000, current_file);
                    number+=1000;
                    out_index = 0;
                }
                primary_heap[0] = primary_heap[num_in_heap-1];
                num_in_heap--;
                heapify(primary_heap, num_in_heap);
            }
            else
            {
                if(out_index > 0)
                {
                    fwrite(output, sizeof(int), (size_t) out_index, current_file);
                    number+=out_index;
                    out_index = 0;
                }
                fclose(current_file);
                file_index++;
                sprintf(index, ".%03d", file_index - 1);
                strcpy(filename, input_file_name);
                strcat(filename, index);
                current_file = fopen(filename, "w");
                qsort(&primary_heap[secondary_heap_p], (size_t) (750-secondary_heap_p), sizeof(int), comparator);
                fwrite(&primary_heap[secondary_heap_p], sizeof(int), (size_t) (750-secondary_heap_p), current_file);
                fclose(current_file);
                break;
            }
        }

    }
if(out_index > 0)
{
fwrite(output, sizeof(int), (size_t) out_index, current_file);
fclose(current_file);

}
FILE *out = fopen(output_file_name, "w");
merge(out,file_index,0,0,input_file_name);
//void merge(FILE *out, int runs, int type, int runs_to_merge,char *input_file_nmae);

} // else top
}



  
  


  
 
  
  
  
  
