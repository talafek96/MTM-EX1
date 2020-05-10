#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
typedef struct node_t {
    int x;
    struct node_t *next;
} *Node;

typedef enum {
    SUCCESS=0,
    MEMORY_ERROR,
    EMPTY_LIST,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
Node nodeCreate(int value);
void nodeDestroy(Node to_destroy);
ErrorCode nodeSetValue(Node list, int val);
ErrorCode nodeAdvance(Node* list);
ErrorCode nodeAddNode(Node list, int value);
ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out);
ErrorCode nodePrintValues(Node list);
ErrorCode nodeMassAddNode(Node* list, int starter, unsigned int repeat);


int main()
{
    Node list1 = nodeCreate(0);
    Node list2 = nodeCreate(0);
    Node first1 = list1;
    Node first2 = list2;
    Node merged_out = nodeCreate(0);
    int x = 0;
    char tmp = '\0';
    printf("Enter values for List 1 (write Q to go next):\n");
    while(scanf(" %d%c", &x, &tmp) && (tmp != 'q' && tmp != 'Q'))
    {
        nodeAddNode(list1, x);
        nodeAdvance(&list1);
    }
    list1 = first1;
    tmp = '\0';
    getc(stdin);
    printf("Enter values for List 2 (write Q to go next):\n");
    while(scanf(" %d%c", &x, &tmp) && (tmp != 'q' && tmp != 'Q'))
    {
        nodeAddNode(list2, x);
        nodeAdvance(&list2);
    }
    list1 = first1;
    list2 = first2;
    printf("List 1 values entered:\n");
    nodePrintValues(list1->next);
    printf("\nList 2 values entered:\n");
    nodePrintValues(list2->next);
    ErrorCode error = mergeSortedLists(list1->next, list2->next, &merged_out);
    if(error != SUCCESS)
    {
        printf("\nERROR! MERGE FAILED WITH ERROR ID %d. EXITING PROGRAM.", error);
        return 0;
    }
    printf("\nMerged list:\n");
    nodePrintValues(merged_out);
    nodeDestroy(merged_out);
    nodeDestroy(list1);
    nodeDestroy(list2);
    return 0;
}

ErrorCode nodeMassAddNode(Node* list, int starter, unsigned int repeat)
{
    if(*list == NULL)
    {
        return NULL_ARGUMENT;
    }
    ErrorCode error = SUCCESS;
    for(int i = 0; i < repeat; i++)
    {
        error = nodeAddNode(*list, starter+i);
        if(error != SUCCESS) return error;
    }
    return SUCCESS;
}

int getListLength(Node list)
{
    if(list == NULL)
    {
        return 0;
    }
    int len = 0;
    while(list != NULL) 
    {
        assert(list != NULL);
        len++;
        list = list->next;
    }
    return len;
}

bool isListSorted(Node list)
{
    assert(list != NULL);
    while(list->next != NULL)
    {
        if((*(list->next)).x < list->x) 
        {
            return false;
        }
        list = list->next; 
    }
    return true;
}

Node nodeCreate(int value)
{
    Node created_node = malloc(sizeof(created_node));
    created_node->x = value;
    created_node->next = NULL;
    return created_node;
}

ErrorCode nodePrintValues(Node list)
{
    if(list == NULL)
    {
        return NULL_ARGUMENT;
    }
    int len = getListLength(list);
    while(len > 0)
    {
        assert(list != NULL);
        printf("%d ", list->x);
        nodeAdvance(&list);
        len--;
    }
    return SUCCESS;
}

ErrorCode nodeSetValue(Node list, int val)
{
    if(list == NULL)
    {
        return NULL_ARGUMENT;
    }
    list->x = val;
    return SUCCESS;
}

ErrorCode nodeAdvance(Node* list)
{
    if(*list == NULL)
    {
        return NULL_ARGUMENT;
    }
    if((*list)->next == NULL)
    {
        return NULL_ARGUMENT;
    }
    *list = (*list)->next;
    return SUCCESS;
}

void nodeDestroy(Node to_destroy)
{
    if(to_destroy == NULL)
    {
        return;
    }
    Node tmp;
    while(to_destroy != NULL)
    {
        tmp = to_destroy;
        to_destroy = to_destroy->next;
        free(tmp);
    }
}

ErrorCode nodeAddNode(Node list, int value)
{
    if(list == NULL)
    {
        return NULL_ARGUMENT;
    }
    while(list->next != NULL)
    {
        list = list->next;
    }
    list->next = malloc(sizeof(list));
    if(list->next == NULL)
    {
        return MEMORY_ERROR;
    }
    (list->next)->next = NULL;
    (list->next)->x = value;
    return SUCCESS;
}

ErrorCode nodeChain(Node dest, Node source)
{ 
    if(!dest || !source)
    {
        return NULL_ARGUMENT;
    }
    while(source != NULL)
    {
        if(nodeAddNode(dest, source->x) != SUCCESS)
        {
            return MEMORY_ERROR;
        }
        source = source->next;
    }
    return SUCCESS;
}

ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out)
{
    if(list1 == NULL || list2 == NULL) 
    {
        return EMPTY_LIST;
    }
    if(!isListSorted(list1) || !(isListSorted(list2)))
    {
        return UNSORTED_LIST;
    }
    if(merged_out == NULL)
    {
        return NULL_ARGUMENT;
    }
    Node merged_head = *merged_out;
    int len1 = getListLength(list1);
    int len2 = getListLength(list2);
    bool is_first = true;
    while(len1 > 0 && len2 > 0)
    {
        assert(list1 != NULL && list2 != NULL);
        if(list1->x >= list2->x)
        {
            if(is_first)
            {
                merged_head->x = list2->x;
                is_first = false;
            }
            else if(nodeAddNode(merged_head, list2->x) != SUCCESS)
            {
                nodeDestroy(merged_head);
                return MEMORY_ERROR;
            }
            len2--;
            list2 = list2->next;
        }
        else if(list1->x < list2->x)
        {
            if(is_first)
            {
                merged_head->x = list1->x;
                is_first = false;
            }
            else if(nodeAddNode(merged_head, list1->x) != SUCCESS)
            {
                nodeDestroy(merged_head);
                return MEMORY_ERROR;
            }
            len1--;
            list1 = list1->next;
        }
    }
    if(len1 > 0)
    {
        assert(list1 != NULL);
        if(nodeChain(merged_head, list1) != SUCCESS)
        {
            nodeDestroy(merged_head);
            return MEMORY_ERROR;
        }
    }
    if(len2 > 0)
    {
        assert(list2 != NULL);
        if(nodeChain(merged_head, list2) != SUCCESS)
        {
            nodeDestroy(merged_head);
            return MEMORY_ERROR;
        }
    }
    return SUCCESS;
}