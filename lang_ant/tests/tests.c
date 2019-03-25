#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lang_ant_types.h"
#include "pos_hash.h"

// todo: write a test for the rehash function

static void print_stack_and_heap(){
    char c;
    char *ptr;
    ptr = malloc(sizeof(char));
    printf("stack at: %p\theap at: %p\n", &c, ptr);
}

static int test_create_xypos_hash_small(){
    HashTable *my_hash_table;
    XYPos my_pos;
    char *my_data, *entered_data, *found_data;
    my_data = "My Data";
    my_pos.x = 1;
    my_pos.y = 2;

    my_hash_table = create_xypos_hash(10);
    if (my_hash_table == NULL)
        return 1;

    entered_data = (char *)search_table(my_hash_table, my_pos, my_data, ENTER);
    if (strcmp(my_data, entered_data))
        return 2;

    found_data = (char *)search_table(my_hash_table, my_pos, NULL, SEARCH);
    if (strcmp(my_data, found_data))
        return 3;

    free_xypos_hash(my_hash_table);

    return 0;
}

static int test_create_xypos_hash_not_found(){
    HashTable *my_hash_table;
    XYPos my_pos = {1, 2};
    XYPos other_pos = {2, 2};
    char *my_data, *entered_data, *found_data;
    my_data = "My Data";
    my_pos.x = 1;
    my_pos.y = 2;

    my_hash_table = create_xypos_hash(10);
    if (my_hash_table == NULL)
        return 1;
    entered_data = (char *)search_table(my_hash_table, my_pos, my_data, ENTER);

    found_data = (char *)search_table(my_hash_table, other_pos, NULL, SEARCH);
    if (found_data != NULL)
        return 2;

    free_xypos_hash(my_hash_table);

    return 0;
}

static int test_create_xypos_hash_pos_match_overwrites_data(){
    HashTable *my_hash_table;
    XYPos my_pos;
    char *my_data1, *my_data2, *entered_data, *found_data;
    my_data1 = "My Data 1";
    my_data2 = "My Data 2";
    my_pos.x = 1;
    my_pos.y = 2;

    my_hash_table = create_xypos_hash(10);
    if (my_hash_table == NULL)
        return 1;

    entered_data = (char *)search_table(my_hash_table, my_pos, my_data1, ENTER);
    if (strcmp(my_data1, entered_data))
        return 2;

    found_data = (char *)search_table(my_hash_table, my_pos, NULL, SEARCH);
    if (strcmp(my_data1, found_data))
        return 3;

    entered_data = (char *)search_table(my_hash_table, my_pos, my_data2, ENTER);
    if (strcmp(my_data1, entered_data))
        return 4;

    found_data = (char *)search_table(my_hash_table, my_pos, NULL, SEARCH);
    if (strcmp(my_data1, found_data))
        return 5;

    free_xypos_hash(my_hash_table);

    return 0;
}

static int test_can_add_more_elements_than_size(){
    HashTable *my_hash_table;
    XYPos my_pos;
    int my_data_cnt = 10;
    char *my_data[10] = {"a0", "b1", "c2", "d3", "e4", "f5", "g6", "h7", "i8", "j9"};
    char *entered_data, *found_data;
    int n = 100000;

    my_hash_table = create_xypos_hash(100);
    if (my_hash_table == NULL)
        return 1;

    for (int i = 0; i < n; i++){
        my_pos.x = i;
        my_pos.y = i * 2;
        entered_data = (char *)search_table(
            my_hash_table, my_pos, my_data[i % my_data_cnt], ENTER
        );
        if (!entered_data)
            return 1;
    }

    for (int i = 0; i < n; i++){
        my_pos.x = i;
        my_pos.y = i * 2;
        found_data = search_table(my_hash_table, my_pos, NULL, SEARCH);
        if (!found_data)
            return 2;
        if (strcmp(found_data, my_data[i % my_data_cnt]))
            return 3;
    }

    free_xypos_hash(my_hash_table);

    return 0;
}

static int test_that_we_are_reclaiming_memory(){
    HashTable *my_hash_table;
    XYPos my_pos;
    char *my_data[10] = {"a0", "b1", "c2", "d3", "e4", "f5", "g6", "h7", "i8", "j9"};
    char *found_data;
    int size = 100000;  // 5294419
    int iter = 100;
    int elements = 10000;  // 166

    for (int i = 0; i < iter; i++){
        //print_stack_and_heap();
        my_hash_table = create_xypos_hash(size);
        if (!my_hash_table)
            return 1;
        for (int j = 0; j < elements; j++){
            my_pos.x = j;
            my_pos.y = j;
            if (!search_table(my_hash_table, my_pos, my_data[j % 10], ENTER))
                return 2;
        }
        for (int j = 0; j < elements; j++){
            my_pos.x = j;
            my_pos.y = j;
            found_data = search_table(my_hash_table, my_pos, my_data[j % 10], SEARCH);
            if (!found_data)
                return 3;
            if (strcmp(found_data, my_data[j % 10]))
                return 4;
        }
        free_xypos_hash(my_hash_table);
        my_hash_table = NULL;
    }
    return 0;
}

static char *test_result(int result_code){
    if (result_code == 0)
        return "Success";
    else
        return "Failed"; 
    
}

int main(int *argc, char **argv){
    printf(
        "Create xy pos hash table: %s\n", 
        test_result(test_create_xypos_hash_small())
    ); 

    printf(
        "XYPos not found: %s\n",
        test_result(test_create_xypos_hash_not_found())
    ); 
    printf(
        "Create xy pos overwrites pos: %s\n", 
        test_result(test_create_xypos_hash_pos_match_overwrites_data())
    ); 

    printf(
        "Can add more elements than size: %s\n", 
        test_result(test_can_add_more_elements_than_size())
    ); 

    printf(
        "Make sure we are freeing memeory: %s\n", 
        test_result(test_that_we_are_reclaiming_memory())
    ); 
}
