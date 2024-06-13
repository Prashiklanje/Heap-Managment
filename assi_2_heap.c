#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 999

typedef enum
{
    false,
    true
} bool;
typedef struct List_node_tag
{
    int start;
    int end;
    char name[5];
    struct List_node_tag *next;
    struct List_node_tag *prev;
    int is_Free;
} List_node;


typedef struct heap_tag
{
    List_node *lptr;
} heap;

List_node *CreateAllocatedNode(int Start, int end, char name[])
{
    List_node *ptr = (List_node *)malloc(sizeof(List_node));
    if (ptr != NULL)
    {
        ptr->start = Start;         //create a node of allocate node
        ptr->end = end;
        strcpy(ptr->name, name);
        ptr->next = NULL;
        ptr->prev = NULL;
        ptr->is_Free = 0;       //false
    }
    return ptr;
}

List_node *CreateFreeNode(int start, int end)
{
    List_node *ptr = (List_node *)malloc(sizeof(List_node));
    if (ptr != NULL)
    {
        ptr->start = start;
        ptr->end = end;             // create a free node of specific length
        ptr->next = NULL;
        ptr->prev = NULL;
        ptr->is_Free = 1;               //true
    }
    return ptr;
}

bool isListFull(heap *h)
{
    bool res = true;
    List_node *ptr = h->lptr;
    while (ptr != NULL && !(ptr->is_Free))  //not free
    {
        ptr = ptr->next;
    }
    if (ptr != NULL)
        res = false;
    return res;
}

bool isListEmpty(heap *h)
{
    int res = true;
    List_node *ptr = h->lptr;
    while (ptr != NULL && ptr->is_Free)   // is free
    {
        ptr = ptr->next;
    }
    if (ptr != NULL)
        res = false;
    return res;
}

bool isAlreadyThere(heap *h, char name[])
{
    List_node *lptr = h->lptr;  // heap or top pointer 
    bool res = false;           // checking  for same variable name if present then return ture
    int val;
    while (lptr != NULL)
    {
        val = strcmp(name, lptr->name); // if name are equal then val = 0 
        if (!(lptr->is_Free) && val == 0)
        {
            res = true;
        }
        lptr = lptr->next;
    }
    return res;
}

void allocateBestFit(heap *h, int size, char name[])
{
    List_node *lptr = h->lptr;  // top pointer
    if (lptr == NULL)
    {
        printf("\nNo free list for heap is created yet!!!!\n");
    }
    else
    {
        List_node *nptr = lptr;
        List_node *ptr = NULL;
        int Size1 = 1001;
        while (nptr != NULL)
        {                       //size 
            if (nptr->is_Free && (nptr->end - nptr->start + 1) >= size)
            {
                if ((nptr->end - nptr->start + 1) < Size1)
                {
                    Size1 = (nptr->end - nptr->start + 1);  // you find smallest sufficient space or equal space
                    ptr = nptr;
                }
            }
            nptr = nptr->next;
        }
        int end;
        if (ptr != NULL)
            end = ptr->start + size - 1;  // requaried space
        if (ptr == NULL || (ptr->next == NULL && (end > MAX_SIZE)))
        {
            printf("\nThis much free space is not available!!!!\n");
        }
        else
        {
            List_node *aptr = CreateAllocatedNode(ptr->start, end, name);
            if (aptr == NULL)
            {
                printf("\nAllocation failure!!!\n");
            }
            else
            {
                if (end == ptr->end)    // ptr is smallest free space ka top pointer
                {
                    lptr = aptr;
                    if (ptr->prev != NULL)
                    {
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    if (ptr->next != NULL)
                    {
                        ptr->next->prev = lptr;
                        lptr->next = ptr->next;
                    }
                    free(ptr);
                }
                else
                {
                    ptr->start = end + 1;
                    lptr = aptr;
                    if (ptr->prev != NULL)
                    {
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    lptr->next = ptr;
                    ptr->prev = lptr;
                }
                printf("\nSpace allocation successful!!!\n");
                while (lptr->prev != NULL)
                {
                    lptr = lptr->prev;
                }
                h->lptr = lptr;  // return set the head or top on first point 
            }
        }
    }
}

void FreeSpace(heap *h, char name[])  // deallocation  space
{
    List_node *lptr = h->lptr;
    List_node *ptr = NULL;
    if (isListEmpty(h))
    {
        printf("\nNo space is allocated yet!!!\n");
    }
    else
    {
        int found = 0, val;
        while (lptr != NULL && !found)
        {
            val = strcmp(name, lptr->name);
            if (!(lptr->is_Free) && val == 0)
            {
                ptr = lptr;
                found = 1;              // here you find variable
            }
            lptr = lptr->next;
        }
        if (ptr == NULL)
        {
            printf("\nThere is no such variable name so freeing up space not done!!!!!\n");
        }
        else
        {
            List_node *aptr;
            ptr->is_Free = 1;
            if (ptr->prev != NULL && ptr->prev->is_Free)      // if previous space is free space
            {
                aptr = ptr->prev;  // aptr store delete name
                ptr->start = aptr->start;
                if (aptr->prev != NULL) 
                    aptr->prev->next = ptr;    // here marge the adjacent free space
                ptr->prev = aptr->prev;
                free(aptr);
            }
            if (ptr->next != NULL && ptr->next->is_Free)        // if next space is free
            {
                aptr = ptr->next;
                ptr->end = aptr->end;
                if (aptr->next != NULL)
                    aptr->next->prev = ptr;
                ptr->next = aptr->next;
                free(aptr);
            }
            lptr = ptr;
            while (lptr->prev != NULL)
            {
                lptr = lptr->prev;
            }
            printf("\nFreeing up space successful!!!\n");
            h->lptr = lptr;
        }
    }
}


void displayAllocatedList(heap *h)
{
    if (isListEmpty(h))
    {
        printf("\nNo space in heap is allocated to show!!!\n");
    }
    else
    {
        List_node *ptr = h->lptr;
        printf("\nVariable\tStarting\tEnding\t\tSize\n");
        while (ptr != NULL)
        {
            if (!(ptr->is_Free))
                printf("%s\t\t%d\t\t%d\t\t%d\n", ptr->name, ptr->start, ptr->end, ptr->end - ptr->start + 1);
            ptr = ptr->next;
        }
    }
}

void displayFreeList(heap *h)
{
    if (isListFull(h))
    {
        printf("\nNo space in heap is free to show!!!\n");
    }
    else
    {
        List_node *ptr = h->lptr;
        printf("\nStarting\tEnding\t\tSize\n");
        while (ptr != NULL)
        {
            if (ptr->is_Free)
                printf("%d\t\t%d\t\t%d\n", ptr->start, ptr->end, (ptr->end) - (ptr->start) + 1);
            ptr = ptr->next;
        }
    }
}

void displayHeap(heap *h)   // display all space allocated and deallocated
{
    printf("\nAllocated blocks are :\n");
    displayAllocatedList(h);
    printf("\nFree blocks are :\n");
    displayFreeList(h);
    printf("\n");
}

void deleteList(heap *h)   // delete or free all space or data
{
    List_node *ptr;
    ptr = h->lptr;
    while (h->lptr != NULL)
    {
        ptr = h->lptr;
        h->lptr = h->lptr->next;
        free(ptr);
    }
}

int main()
{
    heap myheap;
    heap *ptr = &myheap;
    List_node *nptr = CreateFreeNode(0, MAX_SIZE);
    if (nptr == NULL)
    {
        printf("\nThis much space is too large for free list creation\n");
    }
    else
    {
        ptr->lptr = nptr;
        int exit = 0, select, size = 0;
        char name[50];
        printf("Total heap space available is %d\n", 1 + (nptr->end) - (nptr->start));
        while (exit == 0)
        {
            printf("\nEnter the value:\n1 to Allocate space using Bestfit\n2 to Free space\n3 to display whole heap\n0 to exit\n");
            printf("\nEnter value:");
            scanf("%d", &select);
            switch (select)
            {
            case 1:
            {
                printf("\nEnter variable name:");
                scanf("%s", name);
                printf("Enter the size:");
                scanf("%d", &size);
                if (isListFull(ptr))
                    printf("\nHeap is completely allocated!!!\n");
                else if (isAlreadyThere(ptr, name))
                    printf("\nVariable name already existing. Please choose another!!!\n");
                else
                    allocateBestFit(ptr, size, name);
                break;
            }
            case 2:
            {
                printf("\nEnter variable name:");
                scanf("%s", name);
                FreeSpace(ptr, name);
                break;
            }
            case 3:
            {
                displayHeap(ptr);
                break;
            }
            case 0:
            {
                printf("\nExited successfully!!!\n");
                exit = 1;
                break;
            }
            
            default:
                printf("\nInvalid input!\n");
                break;
            }
        }
        deleteList(ptr);
    }
    return 0;
}