#include<stdio.h>
#include <stdlib.h>


static int p_length =1;
int pro, res;
static int avl_ln = 0;
static int deep=0;
int returned_row;
int returned_col;

int check_if_returned(int *returned, int row)
{ 
int m,k,flag2=0;
for(k= 0; k<res; k++)
{
if(returned[k]!=-1)
{
flag2 =1;
}
}
if(flag2==0)
{
returned[row]=0;
return row;
}

for(m=0; m<res; m++)
{
if(returned[m]!=row)
{
	if(returned[m]!=-1)
	{
return row;
	}
}
}
return -1;
}
int has_row_zero(int req[][res], int *returned)
{
int i,j,k,m;
int flag =0;
int has_zero;
int zero[res];
int row;

for(i=0; i<pro; i++)
{
has_zero =1;
for(j=0; j<res; j++)
{
zero[j]=req[i][j];
}
for(k=0; k<res;k++)
{
if(zero[k]!=0)
{
has_zero = 0;
}
}
if(has_zero ==1)
{
row = check_if_returned(returned, i);

if(row !=-1)
break;
}
}
printf("%d",row);
return row;
}



int *update_alloc(int req[][res],int row)
{
	int *ptr1;
	
int i, j;
int k=0;

for(i=0; i<res; i++)
{
if(req[row][i] == 1)
{

avl_ln++;
}
}
ptr1 = (int*) malloc((avl_ln)* sizeof(int));

for(i=0; i<res; i++)
{
if(req[row][i] == 1)
{
req[row][i]-=1;
*(ptr1+k) = i;
k++;

}
}
return ptr1;
}
void update_req(int req[][res],int row, int returned_col)
{
	

if(req[row][returned_col]==1)
{
req[row][returned_col] =req[row][returned_col] - 1;
}

}

int *check_next_sink(int req[][res],int col,int *returned)
{
int length =0;
int i;
int j;
int l, duplicate=0;
int *ptr1;
int flag3=0;

ptr1 = (int*) malloc(sizeof(int));

for(i =0; i<pro; i++)
{
if(req[i][col]!=0)
{
flag3=1;

}
}

if(flag3==0)
{	
*ptr1 = 500;

return ptr1;
}

for(i=0; i<pro; i++)
{
if(req[i][col]!=0)
{	
p_length++;
} 
}
int *ptr;
int k=0;

ptr = (int*) malloc((p_length)* sizeof(int));
for(j=0; j<pro; j++)
{
if(req[j][col]!=0)
{
	
*(ptr+k) = j;
k++;
} 
	
}
	
return ptr;
}
int ch_if_dlf(int req[][res])
{
	int yeah=1;
	int i,j;
	for(i=0; i<pro; i++)
	{
	for(j=0; j<res; j++)
	{
	if(req[i][j]!=0)
	{
	yeah = 0;
	}
	}
	}
return yeah;
}

int rec_func(int req[][res],int alloc[][res],int *returned, int pro, int res)
{
	int i;
 int r_int;


deep++;
returned_row = has_row_zero(req,returned);


if(returned_row == -1)
{
return -1;
}
printf("\nreturned_row value %d", returned_row);


for(i=0; i<res; i++)
{
if(returned[i]==-1)
returned[i]= returned_row;
break;
}

int *p2;
int j;
int k;

 
p2 = update_alloc(alloc,returned_row);
int size2 = avl_ln;

avl_ln =0;
int arr2[size2];



for(i=0; i<size2; i++)
 {
 arr2[i] = *(p2 + i);
 }
 
 
 
printf("\n After updation Allocation M \n");
r_int = display(alloc);



int *p1;


for(j=0; j<size2; j++)
{


p1 = check_next_sink(req,arr2[j],returned);
int size1=p_length;

p_length= 0;



int arr1[size1];


for(i=0; i<size1; i++)
 {
 	
 arr1[i] = *(p1 + i);
 }


 free(p1);
 
 if(arr1[j]!=500)
 {

  
 for(i =0; i<size1; i++)
 {
 	for(k =0; k<size2; k++)
 	{
 	update_req(req,arr1[j],arr2[k]);

 printf("\nAfter updation Request M depth of rec %d",deep);
r_int = display(req);
 rec_func(req,alloc,returned,pro,res);
 	}
 
 }
 
}
}
}

int display(int arr[][res])
{
	int i,j;
for(i=0;i<pro;i++)
{
printf("\nP%d\t   ",i+1);
 for(j=0;j<res;j++)
 {
    printf("%d ",arr[i][j]);
 }
 }
return 1;
}

int main()
{
	
printf("Enter the number of processes:\n");
	scanf("%d",&pro);
	printf("Enter the number of resources:\n");
	scanf("%d",&res);
	 int returned[pro];	
int i,j;
int req[pro][res];
int alloc[pro][res];
int ans = 0;

for(i=0; i<res; i++)
{
returned[i]=-1;
}

printf("Enter the request Matrix\n");
for(i =0; i<pro; i++)
{
for(j =0; j<res; j++)
{
scanf("%d",&req[i][j]);
}
}

printf("Enter the Allocation Matrix\n");
for(i =0; i<pro; i++)
{
for(j =0; j<res; j++)
{
scanf("%d",&alloc[i][j]);
}
}
int zero1[res],k;
int has_z=0;
for(i =0; i<pro; i++)
{
	if(has_z==res)
{
break;
}
	has_z=0;
for(j= 0; j<res; j++)
{
zero1[j]=req[i][j];
}
for(k=0; k<res;k++)
{
if(zero1[k]==0)
{
has_z = has_z+1 ;
if(has_z==res)
{
break;
}


}
}
}

if(has_z==res){
	
ans = rec_func(req,alloc,returned,pro,res);
int ans1=0;
for(i =0; i<pro; i++)
{
for(j =0; j<res; j++)
{
if(req[i][j]!=0)
{
ans1=1;
}
}
}
if(ans1 == 1|| ans == -1)
{
printf("\n Deadlock \n");
}
else
printf("\n Deadlock Free \n");
}
else
{
printf("\n Deadlock \n");
}
}
