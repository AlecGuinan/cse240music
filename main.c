#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char *genre;
    char *artist;
    char *title;
    char *album;
    char *trackid;
    short popularity;
    bool shown;
} Song;

typedef struct SongArrayList {
    Song *songs;         
    size_t size;         
    size_t capacity;     
} SongArrayList;

typedef struct {
    char *genre;         
    int count;
} GenreInfo;
typedef struct LinkListNode{
    GenreInfo *data;
    struct LinkListNode* next;
}LinkListNode;

bool readInSongs(SongArrayList *songList);
void printSongList(SongArrayList * songList, int starting_index);
void generateUniqueGenres(LinkListNode ** ptrUniqueGenresLL, SongArrayList songList);

// *************************************************************************************
// CODE: Copy in functions from prior assignments for Array Lists and Link Lists.  //       Modify those functions to handle the structures used in this assignment
// *************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
typedef struct LinkedList
{
   int data;
   struct LinkedList *next;
} LinkedList;
void addElementLinkedList(int data);
void addElementInTheNthPosition(int data,int position);
void removeLastElementLinkedList();
void removeNthElement(int position);
int searchElementByValue(int value);

LinkedList *head;

void addElementLinkedList(int data){
    LinkedList *pointer;
    LinkedList *list = malloc(sizeof(LinkedList));
    list->data= data;
    list->next = NULL;
    if(head == NULL){
        head = list;
        return;
    }
    pointer = head;
    
    while(pointer->next!= NULL){

        pointer = pointer->next;
    }

    pointer->next = list;

}
void addElementInTheNthPosition(int data,int position){
    
    LinkedList *pointer;
    LinkedList *temp;
    pointer = head;
    if(position ==1){
        temp = head;
        pointer = malloc(sizeof(LinkedList));
        pointer->data = data;
        pointer->next = temp;
        head = pointer;
        return;
    }
    if(pointer == NULL){
        addElementLinkedList(data);
        return;
    }
    for(int i=0; i < position-2; i++){
        if(pointer->next == NULL){// I need to check this further out, or I will have a second for loop to get me to the right position
            if(i+3<position){
                printf("Invalid Operation");
                return;
            }
        }
        pointer = pointer->next;
    }
        temp = pointer->next;
        pointer->next = malloc(sizeof(LinkedList));
        pointer->next->data = data;
        pointer->next->next = temp;
}
void removeLastElementLinkedList(){
    LinkedList *pointer;
    LinkedList *temp;
    pointer = head;
    if(pointer->next == NULL){
        head = NULL;
        free(pointer);
        return;
    }
    while(pointer->next->next != NULL){
        pointer= pointer->next;
    }
    temp = pointer->next;
    pointer->next = NULL;
    free(temp);
}
void removeNthElement(int position){
    LinkedList *temp;
    LinkedList *pointer;
    if(position == 1){
        temp = head;
        if(head->next == NULL){
            head = NULL;
            free(temp);
            return;
        }
        head = head->next;
        free(temp);
        return;
    }
        pointer = head;
    for(int i=0; i < position-2; i++){
        if(pointer->next->next == NULL){// I need to check this further out, or I will have a second for loop to get me to the right position
            if(i+3<position){
                printf("Invalid Operation");
                return;
            }
        }
        pointer = pointer->next;
    }
        temp = pointer->next;
        pointer->next = pointer->next->next;
        free(temp);

}
int searchElementByValue(int value){
    LinkedList *pointer;
    pointer = head;

    while(pointer != NULL){
        if(pointer->data == value){
            return 1;
        }
        pointer= pointer->next;
    }
    return 0;
}

//

#include <stdio.h>
#include <stdlib.h>

typedef struct  ArrayList
{
    int size;
    int capacity;
    int* data;
    /* data */
} ArrayList;
void addElement(ArrayList* list, int data);
struct ArrayList* createArrayList(int initialcapacity);
void resizeList(ArrayList* list, int sizetoadd);
void removeLastElement(ArrayList* list);
void shrinkList(ArrayList* list);
int getElement(ArrayList* list, int i);
void freeArrayList(ArrayList* list);

/*int main()
{
    ArrayList* list = createArrayList(10);
    printf("%d \n", list->capacity);
    for (int i = 0; i < 31; i++)
    {
        addElement(list, i);
    }
    printf("%d \n", list->capacity);
    for (int i = 0; i < 30; i++) {
        removeLastElement(list);
    }
    printf("%d \n", list->capacity);
    freeArrayList(list);
    return 0;
}*/

ArrayList* createArrayList(int initialcapacity) {
    ArrayList* result = malloc(sizeof(ArrayList));
    result->capacity = initialcapacity;
    result->size = 0;
    result->data = malloc(initialcapacity * sizeof(int));
    return result;
}
void resizeList(ArrayList* list, int sizetoadd) {
    list->capacity += sizetoadd;
    list->data = realloc((list->data), list->capacity * sizeof(int));
}

void addElement(ArrayList* list, int data) {
    if (list->size == list->capacity) {
        resizeList(list, 10);
    }
    *(list->data + list->size) = data;
    list->size++;

}
void removeLastElement(ArrayList* list)
{
    if (list->size == 0)
        return;
    list->size--;
    if (list->size == ((list->capacity) / 4)) {
        shrinkList(list);
    }

}
void shrinkList(ArrayList* list) {
    int shrinksize;
    if (list->capacity / 2 < 2) {
        shrinksize = 2;
    }
    else {
        shrinksize = list->capacity / 2;
    }
    int* p = NULL;
    p = realloc(list->data, shrinksize * sizeof(int));
    if (p != NULL) {
        list->data = p;
    }
    else
        printf("Error");
    list->capacity = shrinksize;
}

int getElement(ArrayList* list, int i) {
    if (i > list->size) {
        return -1;
    }
    else
        return *(list->data + i);
}

void freeArrayList(ArrayList* list) {
    free(list->data);
    free(list);
}

int main(){
    SongArrayList songList;
    
    // CODE: probably need to initialize songList

    readInSongs(&songList);

//    generateUniqueGenres(&headUniqueGenreLL, songList);

    // CODE: Need browsing and navigation loop
    
    return 0;
}