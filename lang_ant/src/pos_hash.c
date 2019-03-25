#include <stdio.h>
#include <stdlib.h>
#include "lang_ant_types.h"
#include "pos_hash.h"

HashTable *create_xypos_hash(int nel){
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

HashElem *_make_new_hash_elem(XYPos pos, void *data){
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

void *search_table(HashTable *htable, XYPos pos, void *data, Action action){
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
HashTable *rehash(HashTable *htable, int nel){
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

void free_xypos_hash(HashTable *htable){
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

