#include <stdio.h>
#include <stdlib.h>
#include "lang_ant_types.h"
#include "pos_hash.h"

#define DEFAULT_STATE 0

static HashTable *create_xypos_hash(int nel){
    HashTable *hash_table;
    hash_table = malloc(sizeof(HashTable));
    hash_table->size = 0;
    hash_table->capacity = nel;
    hash_table->salt = 2027;
    hash_table->elements = calloc(sizeof(HashElem *), nel);
    for (int i = 0; i < nel; i++)
        hash_table->elements[i] = NULL;
    return hash_table;
}

static int _get_index_for_pos(HashTable *htable, XYPos pos){
    int capacity, salt, index, mod_posx, mod_posy, xpart, ypart;
    capacity = htable->capacity;
    salt = htable->salt;

    mod_posx = (pos.x + capacity) % capacity;
    mod_posy = (pos.y + capacity) % capacity;
    xpart = ((long)mod_posx * salt) % capacity;
    ypart = ((long)mod_posy + salt) % capacity;

    index = (xpart + ypart) % capacity;
    return index;
}

static HashElem *_make_new_hash_elem(XYPos pos, void *data){
    HashElem *hep;
    hep = malloc(sizeof(HashElem));
    hep->pos = pos;
    hep->data = data;
    hep->next = NULL;
    return hep;
}

static int _same_pos(XYPos pos1, XYPos pos2){
    return (pos1.x == pos2.x) && (pos1.y == pos2.y);
}

static void *search_table(HashTable *htable, XYPos pos, void *data, Action action){
    int index;
    HashElem *hep;
    index = _get_index_for_pos(htable, pos);
 
    hep = htable->elements[index];
    if(!hep){
        if(action == ENTER){
            hep = _make_new_hash_elem(pos, data); 
            htable->elements[index] = hep;
            htable->size++;
            return hep->data;
        }
    }else{
        while(hep->next != NULL){
            if(_same_pos(hep->pos, pos)){
                return hep->data;
            }else{
                hep = hep->next;
            }
        }
        if(_same_pos(hep->pos, pos)){
            return hep->data;
        }else if(action == ENTER){
            hep->next = _make_new_hash_elem(pos, data); 
            hep = hep->next;
            htable->size++;
            return hep->data;
        }
    }
    return NULL;
} 

/*
 * Rehashes the given table and returns a new table,
 * the old table will not be freed, so the caller is
 * responsible for their own references.
 */
static HashTable *rehash(HashTable *htable, int nel){
    HashElem *cur_elem;
    HashTable *new_table = create_xypos_hash(nel);

    for (int i = 0; i < htable->capacity; i++){
        cur_elem = htable->elements[i];
        while (cur_elem){
            search_table(new_table, cur_elem->pos, cur_elem->data, ENTER);
            cur_elem = cur_elem->next;
        }
    }
    return new_table;
}

static void free_hash_elem(HashElem *hash_elem){
    if(hash_elem->next == NULL){
        free(hash_elem);
        hash_elem = NULL;
    }else{
        free_hash_elem(hash_elem->next);
        hash_elem->next = NULL;
        free(hash_elem);
        hash_elem = NULL;
    }
}

static void free_xypos_hash(HashTable *htable){
    for(int i = 0; i < htable->capacity; i++){
        if(htable->elements[i] != NULL){
            free_hash_elem(htable->elements[i]);
            htable->elements[i] = NULL;
        }
    }
    free(htable->elements);
    htable->elements = NULL;
    free(htable);
    htable = NULL;
}


/* Manage stored PosState items */
static PosStateList *new_pos_state_list_item(PosState *pos_state){
    PosStateList *pos_state_list;
    pos_state_list = malloc(sizeof(PosStateList));
    if(pos_state_list == NULL){
        fprintf(stderr, "Could not allocate memory for PosStateList");
        exit(EXIT_FAILURE);
    }else{
        pos_state_list->pos_state = pos_state;
        pos_state_list->next = NULL;
        return pos_state_list;
    }
}

static void free_pos_state_list(PosStateList *pos_state_list){
    PosStateList *cur, *temp;
    cur = pos_state_list;
    while (cur){
        temp = cur->next;
        free(cur->pos_state);
        free(cur); 
        cur = temp;
    }
}

static HashTable *check_htable(HashTable *htable){
    HashTable *new_htable;
    if ((htable->size * 100) / htable->capacity > 90){
        new_htable = rehash(htable, (htable->capacity) * 2); 
        free_xypos_hash(htable);
        return new_htable;
    }else{
        return htable;
    }
}

static PosState *new_pos_state(XYPos pos, State state){
    PosState *pos_state;
    pos_state = malloc(sizeof(PosState));
    if(pos_state == NULL){
        fprintf(stderr, "Could not allocate memory for PosState");
        exit(EXIT_FAILURE);
    }else{
        pos_state->state = state;
        return pos_state;
    }
}


/* Public functions */
MemHashTable *create_mem_htable(int nel){
    MemHashTable *mem_htable;
    mem_htable = malloc(sizeof(MemHashTable));
    PosState *pos_state;
    XYPos origin = {0, 0};
    mem_htable->htable = create_xypos_hash(nel);
    pos_state = new_pos_state(origin, DEFAULT_STATE);  // this is stupid
    mem_htable->head = new_pos_state_list_item(pos_state);
    mem_htable->tail = mem_htable->head;
    return mem_htable;
}

void free_mem_htable(MemHashTable *mem_htable){
    free_xypos_hash(mem_htable->htable);
    free_pos_state_list(mem_htable->head);
    mem_htable->htable = NULL;
    mem_htable->head = NULL;
    mem_htable->tail = NULL;
    free(mem_htable);
}

PosState *add_state_pos(MemHashTable *mem_htable, XYPos pos, State state){
    PosState *stored_pos_state, *pos_state;
    PosStateList *tail;

    pos_state = new_pos_state(pos, state);
    stored_pos_state = (PosState *)search_table(mem_htable->htable, pos, (void *)pos_state, ENTER);
    if(stored_pos_state == NULL){
        fprintf(stderr, "Failed to enter new state into hash table.\n");
        exit(EXIT_FAILURE);
    } else if (stored_pos_state != pos_state) {
        free(pos_state);
    } else {
        tail = mem_htable->tail;
        tail->next = new_pos_state_list_item(pos_state);
        mem_htable->tail = tail->next;
    }
    mem_htable->htable = check_htable(mem_htable->htable);
    return stored_pos_state; 
}

PosState *get_state_pos(MemHashTable *mem_htable, XYPos pos){
    return add_state_pos(mem_htable, pos, DEFAULT_STATE);
}
