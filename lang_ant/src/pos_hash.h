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

typedef struct MemHashTable MemHashTable; 
struct MemHashTable {
    HashTable *htable;
    PosStateList *head;
    PosStateList *tail;
};

typedef enum {ENTER, SEARCH} Action;

/* Public functions */
MemHashTable *create_mem_htable(int nel);

void free_mem_htable(MemHashTable *mem_htable);

PosState *add_state_pos(MemHashTable *, XYPos, State);

PosState *get_state_pos(MemHashTable *, XYPos);

#endif
