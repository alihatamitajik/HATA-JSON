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
    JSON *tmp = array->child;
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
    if(item->string)free(item->string);
    item->string=StrDuplicate(string);
    /* Observe the Object as an array and put the next item in it */
    AddItemArrayJSON(object,item);
}

/* Prototypes Used in output */
void OutputObjectJSON(JSON *object,char *output);
void OutputArrayJSON(JSON *array,char *output);
void OutputStringJSON(JSON *string,char *output);

/* Create an output string of our JSON Object */
char *OutputJSON(JSON *rootobject)
{
    char *output;
    JSON *tmp;
    /* opening the accolade */
    strcpy(output,"{");
    if(rootobject->child == NULL){
        /* closing the accolade if the root object doesn't have any child*/
        strcat(output,"}");
        return output;
    }
    tmp = rootobject->child;
    do{
        /* Adding title */
        strcat(output,"\"");
        strcat(output,tmp->string);
        strcat(output,"\":");
        switch(tmp->type)
        {
            /* Choose what kind is our JSON* */
            case 1: OutputStringJSON(tmp,output);break;
            case 2: OutputArrayJSON(tmp,output);break;
            case 3: OutputObjectJSON(tmp,output);break;
            default:break;
        }
        tmp = tmp->next;
        /* Adding a separator if there is another child */
        if(tmp != NULL)strcat(output,",");
    }while(tmp != NULL);/* Looping while there is child left */

    /* closing the accolade*/
    strcat(output,"}");
    return output;

}

void OutputArrayJSON(JSON *array,char *output)
{
    JSON *tmp;
    strcat(output,"[");
    /* opening the [ */
    if(array->child == NULL){
        strcat(output,"]");
        return;
    }
    tmp = array->child;
    do{
        switch(tmp->type)
        {
            /* Choose what kind is our JSON* */
            case 1: OutputStringJSON(tmp,output);break;
            case 2: OutputArrayJSON(tmp,output);break;
            case 3: OutputObjectJSON(tmp,output);break;
            default:break;
        }
        tmp = tmp->next;
        /* Adding a separator if there is another child */
        if(tmp != NULL)strcat(output,",");
    }while(tmp != NULL);/* Looping while there is child left */
    /* closing the ] */
    strcat(output,"]");
    return;
}

void OutputStringJSON(JSON *string,char *output)
{
    char tmp[2047];
    sprintf(tmp,"\"%s\"",string->valuestring);
    strcat(output,tmp);
}

void OutputObjectJSON(JSON *object,char *output)
{
    JSON *tmp;
    /* opening the accolade */
    strcat(output,"{");
    if(object->child == NULL){
        /* closing the accolade if the root object doesn't have any child*/
        strcat(output,"}");
        return;
    }
    tmp = object->child;
    do{
        /* Adding title */
        strcat(output,"\"");
        strcat(output,tmp->string);
        strcat(output,"\":");
        switch(tmp->type)
        {
            /* Choose what kind is our JSON* */
            case 1: OutputStringJSON(tmp,output);break;
            case 2: OutputArrayJSON(tmp,output);break;
            case 3: OutputObjectJSON(tmp,output);break;
            default:break;
        }
        tmp = tmp->next;
        /* Adding a separator if there is another child */
        if(tmp != NULL)strcat(output,",");
    }while(tmp != NULL);/* Looping while there is child left */
    /* closing the accolade*/
    strcat(output,"}");
    return;
}
/* Get information from Objects */

/* Search between all children and find the child that has same string if not found it returns NULL */
JSON *GetObjectItemJSON(JSON *root,const char *string)
{
    JSON *tmp = root->child;
    /* While there exist children*/
    while(tmp != NULL){
        /* if both strings are the-same it returns the item*/
        if(!strcmp(tmp->string,string))return tmp;
        /* if it wasn't go to next child*/
        tmp = tmp->next;
    }
    /* If nothing found it returns NULL*/
    return NULL;
}

/* Go through the array and returns the index's item */
JSON *GetArrayItemJSON(JSON *array,int index)
{
    /* Set a counter that alert us when we reach the wanted index*/
    int counter = 0;
    /* Going trough the array*/
    JSON *tmp = array->child;
    /* While there are other children */
    while( tmp ){
        /* It checks if it reaches the index or not if it was it returns the item */
        if(counter==index)return tmp;
        /* Else it go trough the next child */
        tmp = tmp->next;
        /* And add on counter*/
        counter++;
    }
    /* If there isn't any child with this index it returns NULL*/
    return NULL;
}

/* It Finds the array size */
int   GetArraySizeJSON(JSON *array)
{
    /* Set a counter that alert us when we reach the wanted index*/
    int counter = 0;
    /* Going trough the array*/
    JSON *tmp = array->child;
    /* While there are other children */
    while( tmp ){
        /*  it go trough the next child */
        tmp = tmp->next;
        /* And add on counter */
        counter++;
    }
    /* Returns size of array */
    return counter;
}
