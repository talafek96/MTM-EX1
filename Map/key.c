#include "key.h"
#include <stdlib.h>
#include <string.h>
//--------------------KEY-STRUCT--------------------//
struct key_t
{
    char* id;
    char* value;
};

//--------------------KEY-FUNCTIONS--------------------//
/**
 * @param key - A key struct to destroy and deallocate all it's components.
 * */
void keyDestroy(Key key)
{
    if (key == NULL)
    {
        return;
    }
    free(key->id);
    free(key->value);
    free(key);
}

/**
 * @param key_id - Constant string for the ID of the key.
 * @param key_value - constant string for the value of the key.
 * @return
 * NULL - in case of null arguments, memory allocation fail for the string copies/key struct.
 * In case of SUCCESS - a pointer for the new allocated key.
 * */
Key keyCreate(const char* key_id, const char* key_value)
{
    if(!key_id || !key_value)
    {
        return NULL;
    }
    char *id = malloc(strlen(key_id) + 1);
    char *value = malloc(strlen(key_value) + 1);
    if(!id || !key_value)
    {
        return NULL;
    }
    strcpy(id, key_id);
    strcpy(value, key_value);
    Key key = malloc(sizeof(*key));
    if(!key)
    {
        return NULL;
    }
    key->id = id;
    key->value = value;
    return key;
}

/**
 * @param key - The key you want to chang it's value
 * @param value - The new value of the key
 * @return 
 * KEY_NULL_ARGUMENT - if one of the args are NULL
 * KEY_OUT_OF_MEMORY - if the memory allocation fails
 * KEY_SUCCESS - if the value successfully set
 */
KeyResult keySetValue(Key key, const char *value)
{
    if (key == NULL || value == NULL)
    {
        return KEY_NULL_ARGUMENT;
    }
    char* new_value = malloc((strlen(value) +1)*sizeof(char));
    if (new_value == NULL)
    {
        return KEY_OUT_OF_MEMORY;
    }
    strcpy(new_value, value);
    free(key->value);
    key->value = new_value;
    return KEY_SUCCESS;
}

char *keyGetID(Key key)
{
    return key != NULL? key->id : NULL;
}

char *keyGetValue(Key key)
{
    return key != NULL? key->value : NULL;
}