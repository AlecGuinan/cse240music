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

Song* newsong(char* genre, char*artist, char* title, char* album, char* trackid, short popularity, bool shown);

bool readInSongs(SongArrayList *songList);
void printSongList(SongArrayList * songList, int starting_index);
void generateUniqueGenres(LinkListNode ** ptrUniqueGenresLL, SongArrayList songList);

// *************************************************************************************
// CODE: Copy in functions from prior assignments for Array Lists and Link Lists.  //       Modify those functions to handle the structures used in this assignment
// *************************************************************************************

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


/*typedef struct  ArrayList
{
    int size;
    int capacity;
    int* data;
    /* data 
} ArrayList;*/
void addElement(SongArrayList* list, Song data);
SongArrayList* createArrayList(size_t initialcapacity);
void resizeList(SongArrayList* list, size_t sizetoadd);
void removeLastElement(SongArrayList* list);
void shrinkList(SongArrayList* list);
Song getElement(SongArrayList* list, int i);
void freeArrayList(SongArrayList* list);


SongArrayList* createArrayList(size_t initialcapacity) {
    SongArrayList* result = malloc(sizeof(SongArrayList));
    result->capacity = initialcapacity;
    result->size = 0;
    result->songs = malloc(initialcapacity * sizeof(Song));
    return result;
}
void resizeList(SongArrayList* list, size_t sizetoadd) {
    list->capacity += sizetoadd;
    list->songs = realloc((list->songs), list->capacity * sizeof(int));
}

void addElement(SongArrayList* list, Song data) {
    if (list->size == list->capacity) {
        resizeList(list, 10);
    }
    *(list->songs + list->size) = data;
    list->size++;

}
void removeLastElement(SongArrayList* list)
{
    if (list->size == 0)
        return;
    list->size--;
    if (list->size == ((list->capacity) / 4)) {
        shrinkList(list);
    }

}
void shrinkList(SongArrayList* list) {
    int shrinksize;
    if (list->capacity / 2 < 2) {
        shrinksize = 2;
    }
    else {
        shrinksize = list->capacity / 2;
    }
    Song* p = NULL;
    p = realloc(list->songs, shrinksize * sizeof(Song));
    if (p != NULL) {
        list->songs= p;
    }
    else
        printf("Error");
    list->capacity = shrinksize;
}

Song getElement(SongArrayList* list, int i) {
    if (i > list->size) {
    }
    else
        return *(list->songs + i);
}

void freeArrayList(SongArrayList* list) {
    free(list->songs);
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
/*typedef struct {
    char *genre;
    char *artist;
    char *title;
    char *album;
    char *trackid;
    short popularity;
    bool shown;
} Song;*/
bool readInSongs(SongArrayList *songList){
    FILE *file = fopen("songs.txt","r");
    int ch;
    char* pointer=NULL;
    size_t size=0;
    int number;
    char* word;
    char *genre;
    char *artist;
    char *title;
    char *album;
    char *trackid;
    short popularity;
    bool shown = false;

    printf("Test");
    
    while(-1!=getline(&pointer,&size,file)){
        word = strtok(pointer, "    ");
        number = 0;
        printf("test1\n");
        while(number<7){
            printf("Test2\n");
            word = strtok(NULL," ");
        switch(number){
            case 0:// genre
            printf("Test0\n");
            genre = word;
            number ++;
            break;
            case 1://artist
            printf("Test1\n");
            artist=word;
            number ++;
            break;
            case 2://title/
            printf("Test2\n");
            title =word;
            number ++;
            break;
            case 3://album
            number ++;
            printf("Test3\n");
            album = word;
            break;
            case 4://trackid
            number ++;
            printf("Test4\n");
            trackid = word;
            break;
            case 5://popularity
            printf("Test5\n");
            popularity = 2;//(short)atoi(word);
            printf("Test5\n");
            number ++;
            break;
            case 6: // shown
            printf("Test6\n");
            number++;
            break;
        }
        }
            printf("test3\n");
        addElement(songList,*newsong(genre,artist,title,album,trackid,popularity,shown));
        printf("after\n");
    }

    return true;
}
Song* newsong(char* genre, char*artist, char* title, char* album, char* trackid, short popularity, bool shown){
    Song* pointer; 
    pointer = malloc(sizeof(Song));
    pointer->genre=genre;
    pointer->artist=artist;
    pointer->title =title;
    pointer->album=album;
    pointer->trackid=trackid;
    pointer->popularity=popularity;
    pointer->shown=shown;
    return pointer;
}