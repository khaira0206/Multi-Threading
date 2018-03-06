#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>



struct node {
    int data;
    struct node *next;
};
struct node *freelist,*list_1, *list_2;


sem_t f_b, l1_b, l2_b;
sem_t  e ,count, l1_count, l2_count;

//creates an empty list
createEmptyList(struct node **head)
{
    
    *head = NULL;
}

//appends new node to the list
append(struct node **head, int num)
{
    struct node *temp, *r;
    temp = *head;
    
    if(temp == NULL)
    {
        temp = malloc(sizeof(struct node));
        temp->data = num;
        temp->next = NULL;
        *head = temp;
    }
    else{
        while(temp->next!= NULL)
        temp = temp->next;
        
        r = malloc(sizeof(struct node));
        temp->data = num;
        r->next = NULL;
        temp->next = r;
    }
}

//appends the unlinked node(item) to the list(head)
appendTo_list(struct node **head, struct node **item)
{
    
    struct node *temp1,*temp2;
    temp1 = *head;
    
    if(*head == NULL)
    {
        *head = *item;
    }
    else{
        temp1 = *head;
        while(temp1->next!= NULL)
        temp1 = temp1->next;
        temp1->next = *item;
               
    }
    
    
}

//deletes the node from the end of the list
struct node *deleteFromEnd(struct node **head)
{
    struct node *ptr, *loc;
    if(*head==NULL)
    {
        
        return ;
    }
    else if((*head)->next == (struct node*) NULL)
    {
        ptr = *head;
        *head = NULL;
        ptr->next= NULL;
        return ptr;
    }
    else{
        loc = *head;
        ptr = (*head)->next;
        
        while(ptr->next != NULL)
        {
            loc = ptr;
            ptr = ptr->next;
        }
        loc->next = NULL;
        ptr->next = NULL;
        return ptr;
    }
    
}

//display the items of the list
display(struct node *head)
{
    
    while(head != NULL)
    {
        printf("\n");
        printf("%d \n", head->data);
        head = head->next;
    }
    
}

void *process_1(void *arg)
{
    int number=0;
    while(1)
    {
        struct node *ptr1;
        
        number++;
        
        //make sure that there is atleast one element left in the free list for process 2 to consume
        sem_wait(&count);
        sem_wait(&e);
        
        printf("Unlinking from the free list\n");
        
        //mutual exclusion
        sem_wait(&f_b);
        ptr1 = deleteFromEnd(&freelist);
        sem_post(&f_b);
        
        //display(freelist);
        
        //produce something in block ptr1
        
        ptr1->data = rand() % 100;
        //ptr1->next = NULL;
        
        printf("linking to the list_1\n");
        
        //mutual exclusion
        sem_wait(&l1_b);
        appendTo_list(&list_1,&ptr1);
        sem_post(&l1_b);
        
        sem_post(&l1_count);
        
        printf("Now I M displaying list_1 %d contents\n", number);
        
        //display(list_1);
        
    }
    return NULL;
}

void *process_2(void *arg)
{
    int number=0;
    struct node *ptr1,*ptr2;
    
    while(1){
        number++;
        
        // first checks whether there is any element in the list_1
        sem_wait(&l1_count);
        
        printf("Unlinking from the  list_1\n");
        sem_wait(&l1_b);
        ptr1 = deleteFromEnd(&list_1);
        sem_post(&l1_b);
        
        
        //ptr1->next = NULL;
        
        
        //checks that is there any element in the free list
        sem_wait(&e);
        
        
        printf("Unlinking from the free list\n");
        sem_wait(&f_b);
        ptr2 = deleteFromEnd(&freelist);
        sem_post(&f_b);
        
        //ptr2->next = NULL;
        
        ptr2->data = ptr1->data;
        
        printf("linking to the list_2\n");
        
        //appends the node to the list_2
        sem_wait(&l2_b);
        appendTo_list(&list_2,&ptr2);
        sem_post(&l2_b);
        
        //V operation to increase the count that somthing is added to the list_2
        sem_post(&l2_count);
        
        printf("linking to the free list\n");
        sem_wait(&f_b);
        appendTo_list(&freelist,&ptr1);
        sem_post(&f_b);
        sem_post(&count);
        
        
        //V operation for the free list that some element is added to the free list
        sem_post(&e);
        
        
        printf("Now I M displaying list2 %d \n", number);
        //display(list_2);
        
    }
    
    return NULL;
}

void *process_3(void *arg)
{
    int number =0;
    struct node *ptr1;
    
    while(1)
    {
        number++;
        
        // checks whether there is any element in the list_2 before deleting the node
        sem_wait(&l2_count);
        
        printf("Unlinking from the list_2\n");
        sem_wait(&l2_b);
        ptr1 = deleteFromEnd(&list_2);
        sem_post(&l2_b);
        
        
        
        ptr1->data = 0;
        
        printf("linking to the free list_2\n");
        
        //appends the node back to the free list
        sem_wait(&f_b);
        appendTo_list(&freelist,&ptr1);
        sem_post(&f_b);
        
        sem_post(&e);
        
        
        printf("Now I M displaying process_3 %d contents \n", number);
        //display(list_2);
    }
    
    return NULL;
}


int main () {
    
    int n;
    int m;
    printf("Enter the count of buffer storage:");
    scanf("%d", &n);
    
    m = n-1;
    
    sem_init(&f_b, 1, 1);
    sem_init(&l1_b, 1, 1);
    sem_init(&l2_b, 1, 1);
    
    
    sem_init(&e, 1, n);
    sem_init(&count,1,m);
    sem_init(&l1_count, 1, 0);
    sem_init(&l2_count, 1, 0);
    
    
    
    createEmptyList(&freelist);
    createEmptyList(&list_1);
    createEmptyList(&list_2);
    
    
    
    int i;
    for(i=0; i<n; i++)
    {
        append(&freelist,i);
    }
    
    //display(freelist);
    
    pthread_t th1,th2,th3;
    
    pthread_create(&th1,NULL,process_1,(void *)NULL);
    pthread_create(&th2,NULL,process_2,(void *)NULL);
    pthread_create(&th3,NULL,process_3,(void *)NULL);
    
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    pthread_join(th3,NULL);
    
    return 0;
}
