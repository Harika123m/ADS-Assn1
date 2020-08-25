#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 100

typedef struct { /* B-tree node */ 
int n; /* Number of keys in node */ 
int *keyNode; /* Node's keys */ 
long *child; /* Node's child subtree offsets */ 
} btree_node;
void write_to_file(int order, FILE *fp, long *root, btree_node *tree);
void printer(long root, FILE *fp, int order, btree_node *node);
struct queue
{
long 	offset;
int 	level,position;
struct 	queue *ptr;
};


int record=256;
int comparator(const void *a, const void *b)
{
int l = *(const int *)a;
int r = *(const int *)b;
return (l-r);
}

void read_from_file(int order, FILE *fp, long root, btree_node *tree)
{
tree->n = 0;
tree->keyNode = (int *) calloc( order - 1, sizeof( int ) ); 
tree->child = (long *) calloc( order, sizeof( long ) );
fseek(fp, root, SEEK_SET);
fread( &tree->n, sizeof( int ), 1, fp );
fread( tree->keyNode, sizeof( int ), order - 1, fp );
fread( tree->child, sizeof( long ), order, fp );
}

void case1(int order, int key, FILE *fp, long *root)
{
btree_node tree;
tree.n = 0;
tree.keyNode = (int *) calloc( order - 1, sizeof( int ) ); 
tree.child = (long *) calloc( order, sizeof( long ) );
tree.keyNode[0]=key;
//printf("\n came to case 1 to insert : %d", tree.keyNode[0]);
long size=ftell(fp);
*root = size;
fwrite(root, sizeof(long), 1, fp);
tree.n=1;
write_to_file(order,fp,root,&tree);
}
void case2(int order, int key, FILE *fp, long *root, btree_node *tree, int root_or_not, long *left_offset, long *right_offset,int *height, long *offset)  // root is filled up - split
{
int mid = (order/2) + 1;
btree_node RootNode, RightNode;
//printf("\n came to case 2 to insert %d", key);
RootNode.n = 0; // one element goes to new root
RootNode.keyNode = (int*)calloc((order-1), sizeof(int));
RootNode.child = (long*)calloc(order, sizeof(long));
RightNode.n = 0; // the second half (excluding root ) go to right
RightNode.keyNode = (int*)calloc((order-1), sizeof(int));
RightNode.child = (long*)calloc(order, sizeof(long));
int i;
//printf("\n elements in existing tree are : ");
//for(i=0;i<tree->n;i++) printf(" %d", tree->keyNode[i]);
RootNode.child[0]=*root;
int sorting[order];
for(i=0;i<order-1;i++)
{
sorting[i]=tree->keyNode[i]; // copy all values of the node into an array
}
sorting[order-1]=key; // add even the new key to be inserted into that array

//printf("\n before sorting ");
//for(i=0;i<order;i++) printf(" %d", sorting[i]);
qsort(&sorting,order, sizeof(int), comparator);  // sort everything
//printf("\n after sorting : ");
//for(i=0;i<order;i++) printf(" %d", sorting[i]);
//printf("\n after sorting : the root node is %d", sorting[mid-1]);
RootNode.keyNode[0]=sorting[mid-1];

// copy values in sorting less than mid in left child and more than mid in right node
int j=0;
//printf("\n copied these in right node : ");
for(i=mid;i<order;i++)  // copy right node values
{
RightNode.keyNode[j]=sorting[i];
//printf(" %d", RightNode.keyNode[j]);
j++;
}
//printf("\n copied theswe in left node : ");
for(i=0;i<mid-1;i++)
{
tree->keyNode[i]=sorting[i];
//printf(" %d", tree->keyNode[i]);
}
tree->n=order/2;
RootNode.n=1;
RightNode.n=((order%2)==0)?((order/2)-1):(order/2);
// write to file
write_to_file(order,fp,root, tree);
fseek(fp,0,SEEK_END);
*offset=ftell(fp);
RootNode.child[1]=*offset;
write_to_file(order,fp,offset,&RightNode);
*left_offset=RootNode.child[0];
*right_offset=RootNode.child[1];
*height = sorting[mid-1];
if(root_or_not==1)
{
*offset=ftell(fp);
//*root=*offset;
write_to_file(order,fp,offset,&RootNode);
//printf("\n updated root");
*root=*offset;
}
} // case 2 end
void case3(int order,int key,FILE *fp,long *root, btree_node *tree)
{
//printf("\n came to case 3 to insert %d", key);
//printf("\n elements already in tree :");
int i;
//for(i=0;i<tree->n;i++) printf(" %d", tree->keyNode[i]);
int length=tree->n+1;
int sorting[length];

for(i=0;i<length-1;i++)
{
sorting[i]=tree->keyNode[i];
}
sorting[length-1]=key;
//printf("\n before sorting :");
//for(i=0;i<length;i++) printf(" %d", sorting[i]);
qsort(&sorting,length,sizeof(int), comparator);
//printf("\n after sorting ");
//for(i=0;i<length;i++) printf(" %d", sorting[i]);
//printf("\n copythem these in tree ");
for(i=0;i<length;i++)
{
tree->keyNode[i]=sorting[i];
//printf("\n %d", tree->keyNode[i]);
}
tree->n=length;
write_to_file(order,fp,root,tree);
//printf("\n inserted %d", key);
}

void case4(int order,int key,FILE *fp,long *root, btree_node *tree,long *left_offset,long *right_offset, int *height, int root_or_not, long *offset, int split)
{
int i;

btree_node RootNode, RightNode;
RootNode.n = 0; // one element goes to new root
RootNode.keyNode = (int*)calloc((order-1), sizeof(int));
RootNode.child = (long*)calloc(order, sizeof(long));
RightNode.n = 0; // the second half (excluding root ) go to right
RightNode.keyNode = (int*)calloc((order-1), sizeof(int));
RightNode.child = (long*)calloc(order, sizeof(long));
RootNode.child[0]=*root;
long children[order+1];   
int no_of_children=0; 
for(i=0;i<split;i++)    
{
//printf(" %d", no_of_children);
children[no_of_children]=tree->child[i];
no_of_children++;
}
//printf("\n %d", no_of_children);
children[no_of_children]=*left_offset;   
no_of_children++;
children[no_of_children]=*right_offset;
for(i=split+1;i<=tree->n;i++)
{
no_of_children++;
//printf("\n %d", no_of_children);
children[no_of_children]=tree->child[i];
}     
//printf("\n the no of children are : %d", no_of_children);
int mid =(order/2) + 1;
int length=tree->n+1;
int sorting[length];
for(i=0;i<length-1;i++)
{
sorting[i]=tree->keyNode[i];
}
sorting[length-1]=*height;
qsort(&sorting,length,sizeof(int), comparator);
RootNode.keyNode[0]=sorting[mid-1];
int j=0;
for(i=mid;i<order;i++)  
{
RightNode.keyNode[j]=sorting[i];
j++;
}
for(i=0;i<mid-1;i++)
{
tree->keyNode[i]=sorting[i];
}
for(i=0;i<=mid;i++)               // i<mid to i<=mid
{
tree->child[i]=children[i];
}
int to_insert=0;
for(i=mid;i<order+1;i++)
{
RightNode.child[to_insert]=children[i];
to_insert++;
}
tree->n=order/2;
RightNode.n=((order%2)==0)?((order/2) -1):(order/2);
RootNode.n=1;
write_to_file(order,fp,root, tree);
//printf("\n root Node is %d", RootNode.keyNode[0]);
//printf("\n tree from root-left node : ");

fseek(fp, 0, SEEK_END);
*offset = ftell(fp);
RootNode.child[1] = *offset;
write_to_file(order,fp,offset,&RightNode);
long size=ftell(fp);
*root = size;
fwrite(root, sizeof(long), 1, fp);
write_to_file(order,fp,root,&RootNode);
//printf("\n tree from root - right node : ");

*height = sorting[mid-1];       
*left_offset=RootNode.child[0];
*right_offset=RootNode.child[1];   
if(root_or_not==1)
{

*offset=ftell(fp);
//*root=*offset;
write_to_file(order,fp,offset,&RootNode);
*root=*offset;
}
//printf("\n full tree is :");
}
void case5(int order,int key,FILE *fp,long *root, btree_node *tree, long *left_offset, long *right_offset, int *height)
{
//printf("\n came into case 5");
//printf("\n last time it got split at %d", *height);
//printf("\n elements already in tree :");
int i;
//for(i=0;i<tree->n;i++) printf(" %d", tree->keyNode[i]);
int length=tree->n+1;
int sorting[length];
for(i=0;i<length-1;i++)
{
sorting[i]=tree->keyNode[i];
}
sorting[length-1]=*height;
//printf("\n before sorting :");
//for(i=0;i<length;i++) printf(" %d", sorting[i]);
qsort(&sorting,length,sizeof(int), comparator);
//printf("\n after sorting ");
//for(i=0;i<length;i++) printf(" %d", sorting[i]);
//printf("\n copythem these in tree ");
for(i=0;i<length;i++)
{
tree->keyNode[i]=sorting[i];
//printf("\n %d", tree->keyNode[i]);
}
int where_to_insert,o;
for(where_to_insert=0;where_to_insert<length;where_to_insert++)
{
if(*height==tree->keyNode[where_to_insert]) break;            
}
//printf("\n for add %d child node to insert is : %d",key, where_to_insert);

//change offsets  after 'where to insert'

/*	for(o=where_to_insert;o<length-1;o++)
{
tree->child[o+2]=tree->child[o+1];
}  */
int p;
for(p=tree->n;p>where_to_insert;p--)
{
tree->child[p+1]=tree->child[p];
}
tree->child[p+1]=*right_offset;
tree->child[p]=*left_offset;
tree->n=length;
//tree->child[2]=tree->child[1];
//tree->child[0]=*left_offset;
//tree->child[1]=*right_offset;

write_to_file(order,fp,root,tree);
}
void write_to_file(int order, FILE *fp, long *root, btree_node *tree)
{
//printf("\n came to write to file function");
fseek(fp, *root, SEEK_SET);
fwrite( &tree->n, sizeof( int ), 1, fp );
fwrite( tree->keyNode, sizeof( int ), order - 1, fp );
fwrite( tree->child, sizeof( long ), order, fp );
int i;
//printf("\n wrote the following to file : ");
//for(i=0;i<tree->n;i++) printf(" %d", tree->keyNode[i]);
}

//    add(key,order,fp,&root,&left,&right,&height,type);
int  add(int key,int order,FILE *fp, long *root, long *left_offset, long *right_offset, int *height, int root_or_not)
{
btree_node tree;
long offset=0;
if(*root==-1)  // no tree till now - inserting first element 
{
//printf("\n going to case 1");
case1(order, key,fp,root);
return 0;
}
read_from_file(order,fp,*root,&tree);
//printf("\n in main add function, tree at this stage : ");

if(tree.child[0]==0)        // tree is existing with only root and no children
{
if(tree.n==order-1) 
{   // root is filled up - split 
//("\n going to case 2");
case2(order,key,fp,root,&tree,root_or_not,left_offset,right_offset,height,&offset);
return 1;  
}
else
{
// printf("\n going to case 3");
case3(order,key,fp,root,&tree);           // root is not filled - insert element
return 0;
}
}
else    // Node has children
{
//printf("\n Node has children  ");
int i;
// find position where to insert
for(i=0;i<tree.n;i++)
{

//if(key > tree.keyNode[i]) i++;
if(key< tree.keyNode[i]) break;
}
int root_not=0;
int split_required=add(key,order,fp,&tree.child[i],left_offset,right_offset,height,root_not);          
if(split_required==0)  // after adding to child, child did not split
{
//printf("\n child didn't get split");
return 0;
}  
else       // child got split
{  
//printf("\n child got split, so may go to cas4 or cas 5"); 
if(tree.n==order-1) 
{
//printf("\n going to case 4");
case4(order,key,fp,root,&tree,left_offset,right_offset,height,root_not, &offset, i);  // parent has no space to accept the splitted node
return 1;   // split again
}
else
{
//printf("\n going to case 5");
case5(order,key,fp,root,&tree,left_offset,right_offset,height);              // parent has space to accept the splitted node
return 0;            // no need to split again
}
}
}
}

char common_find(int key, int order, FILE *fp, long root)
{
if(root==-1) // no elements in Btree
{
return 'n';
}
btree_node tree;

read_from_file(order,fp,root, &tree);

int i;
// printf("\n trying to find %d from these elements : ",key);
// for(i=0;i<tree.n;i++) printf(" %d", tree.keyNode[i]);

for(i=0;i<tree.n;i++)
{
if(key==tree.keyNode[i])
{
return 'y';
}
if(key< tree.keyNode[i]) break;
}
if(tree.child[i]!=0)
{ 
//printf("\n came to find in children of %d", tree.keyNode[i-1]);
return common_find(key, order, fp, tree.child[i]);
}
else return 'n';

}

int insert(int key, int order, FILE *fp, long *root, long *left_offset, long *right_offset, int *height)
{
//printf("\n in insert: %d %d",key, order);
char existing = common_find(key,order,fp,*root); 
//printf("\n existing : %c", existing);   
if(existing=='y') 
{
printf("Entry with key=%d already exists\n",key);
return 0;
}
if(existing=='n')
{
// add it
int root_yes=1;
add(key,order,fp,root,left_offset,right_offset,height,root_yes);
}
return 0;
}
void printer(long root, FILE *fp, int order, btree_node *node)
{
struct queue *front=NULL,*iter=NULL;
struct queue *first = (struct queue*)malloc(sizeof(struct queue));
// initialize the root to first queue member 
first->offset = root;
int first_count=1,i,j;
first->level = first_count;
first->position = first_count;
// no further elements as of now
first->ptr = NULL;
front=first;
// if there are elements in queue
while(front)
{
read_from_file(order, fp, front->offset,node);
if(first_count == 1)
{
printf("%2d: ", first_count);
first_count++;  
}
else if(front->level==1)
{
if(front->position==1)
{
printf("\n%2d: ",first_count);
first_count++;   
}
}
int k=node->n-1;
for(i= 0;i<k;i++)
{
printf("%d,", node->keyNode[i]);
}
if(node->n>0)
printf( "%d ", node->keyNode[k]);
for(i=0;i<=k+1;i++)
{
if(node->child[i])   // not null(0)
{
// if child exists, cretae a structure for it
struct queue *node2 = (struct queue*)malloc(sizeof(struct queue));
node2->offset = node->child[i];
if(front->level==1) 
{
if(i==0) node2->level=1;
}
else
{
node2->level = 0;
}
iter=front;
node2->ptr=NULL;
node2->position=1;
while(iter->ptr)
{
iter=iter->ptr;
}
iter->ptr = node2;	
}	
}
iter=front;
front=front->ptr;
// if iter has some data, clear it
if(iter)
free(iter);
}
printf("\n");

}

int find(int key, int order, FILE *fp, long root)
{

char finding = common_find(key,order,fp,root);
if(finding =='y') 
{
printf("Entry with key=%d exists\n", key);
return 0;
}

else
{
printf("Entry with key=%d does not exist\n", key);
return 0;
}
}
void end(long root, FILE *fp)
{

//printf("\n file ended");
fseek(fp, 0, SEEK_SET);
fwrite(&root, sizeof(long), 1, fp);
}

int main(int argc, char *argv[])
{

char user[500],copy_user[500],*userinput[3], *inputargs;
int l;

if(argc!=3)
{
printf("\n incorrect number of arguments ");
return 0;
}

int order=atoi(argv[2]);
int height=0;
FILE *fp; /* Input file stream */ 
long root, left_offset=0, right_offset=0; /* Offset of B-tree root node */ 
fp = fopen( argv[1], "r+b" ); /* If file doesn't exist, set root offset to unknown and create * file, otherwise read the root offset at the front of the file */ 
if ( fp == NULL )
{
root = -1; 
fp = fopen(argv[1], "w+b" ); 
//printf("\n file was not existing, so just opened");
fwrite( &root, sizeof( long ), 1, fp );
}
else
{
fread( &root, sizeof( long ), 1, fp ); 
// printf("\n file already existing, read root");
}

char buffer[BUFFERSIZE];
char *func[2];
while(fgets(buffer, BUFFERSIZE , stdin) != NULL)
{
 char *p;
int flag=0;
p = strtok(buffer," \n");
int i=0;


if(strcmp(p,"add")==0 || strcmp(p,"find")==0)
{ 
flag=1;
while (p!= NULL)
{
func[i]=p;
p = strtok (NULL, " ");
i++;

}
} 

else func[i]=p;
int key; 
if(flag==1)
{
//printf("\n found key");
key=atoi(func[1]); 
} 

if(strcmp(func[0],"add")==0)
{ 
//printf("\n option 1"); 
//printf("\n adding %d\n", key);
insert(key,order,fp,&root, &left_offset, &right_offset, &height);
//printf("\n ----------------------");

}
if(strcmp(func[0],"find")==0)
{
//printf("\n option 2");

find(key,order,fp,root);
//printf("\n ----------------------");
}
if(strcmp(func[0],"print")==0)
{
btree_node node;
//printf("\n option 3"); 
printer(root,fp,order, &node);
//printf("\n ----------------------");
}
if(strcmp(func[0],"end")==0) 
{
//printf("\n option 4");
end(root,fp);
//printf("\n ----------------------");
} 
 } // while 
return 0;
} // main






















