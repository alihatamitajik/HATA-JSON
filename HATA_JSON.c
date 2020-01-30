#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HATA_JSON.h"
/*HATA_JSON tries to parse and create JSON strings*/

/*Duplicating the string so that it will be accessible until it'll be freed*/
static unsigned char* StrDuplicate(const unsigned char* string)
{
    size_t length = 0;
    unsigned char *copy = NULL;

    if (string == NULL)return NULL;

    length = strlen((const char*)string) + sizeof("");
    copy = (unsigned char*)malloc(length);
    if (copy == NULL)return NULL;
    memcpy(copy, string, length);

    return copy;
}

/*Creating New Object and return the start pointer*/
JSON* CreateNewObjectJSON(void)
{
    JSON *item = (JSON *)malloc(sizeof(JSON));
    if(item != NULL){
        memset(item,0,sizeof(JSON));
        item->type = JSON_Object;
    }
    return item;
}

/*Creating New Array and return the start pointer*/
JSON* CreateNewArrayJSON(void)
{
    JSON *item = (JSON *)malloc(sizeof(JSON));
    if(item != NULL){
        memset(item,0,sizeof(JSON));
        item->type = JSON_Array;
    }
    return item;
}

/*Creating New String and return the start pointer*/
JSON* CreateNewStringJSON(const char const * string)
{
    JSON *item = (JSON *)malloc(sizeof(JSON));
    if(item != NULL){
        memset(item,0,sizeof(JSON));
        item->type = JSON_String;
        item->valuestring = StrDuplicate((const unsigned char*)string);
    }
    return item;
}

/*
Add item(String/Object/Array) to an array.

*/
void AddItemArrayJSON(JSON *array, JSON *item)
{
    /*temporarily call the child of the array "tmp"*/
    cJSON *tmp = array->child;
    /*If The Item was empty*/
    if (!item) return;
    /*If Any Chain wasn't created*/
    if (!tmp)array->child=item;
    /*If The Chain Already Was Created : */
    else {
        /*It Reaches us to the tail of the chain*/
        while (tmp && tmp->next) tmp=tmp->next;
        /*Making the chain*/
        tmp->next = item;
        item->prev = tmp;
    }
}

/*
 Add Item(String/Object/Array) to a Object
[Objects are Arrays too, But they are Different in output syntax]
*/
void AddItemObjectJSON(JSON *object,const char *string,JSON *item)
{
    if (!item) return;
    /* Put the title in "char* string" */
    item->string=cJSON_strdup(string);
    /* Observe the Object as an array and put the next item in it */
    AddItemArrayJSON(object,item);
}

/* Create an output string of our JSON Object */
char *OutputJSON(JSON *object)
{

}
