#ifndef POS_HASH
#define POS_HASH
#include "lang_ant_types.h"

typedef struct HashElem HashElem;
struct HashElem {
    XYPos pos;
    void *data;
    HashElem *next;
};

typedef struct HashTable HashTable;
struct HashTable {
    int size;
    int capacity;
    int salt;
    HashElem **elements;
};

typedef enum {ENTER, SEARCH} Action;

HashTable *create_xypos_hash(int);

HashElem *_make_new_hash_elem(XYPos, void *data);

void *search_table(HashTable *, XYPos, void *data, Action);

void free_xypos_hash(HashTable *);

HashTable *rehash(HashTable *, int);

#endif
