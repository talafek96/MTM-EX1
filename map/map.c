#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> 

/** The initial size of Key's arry in a Map */
#define MAP_INITIAL_SIZE 10

/** The pactor by which to expand the Key's arry when needed */
#define MAP_EXPAND_FACTOR 2

/** Return by 'mapFindKey' function when didn't finde such key */
#define MAP_NO_SUCH_KEY -1

//--------------------KEY-STRUCT--------------------//
typedef struct key_t
{
    char* id;
    char* value;
}*Key;

typedef enum
{
    KEY_SUCCESS = 0, 
    KEY_OUT_OF_MEMORY,
    KEY_NULL_ARGUMENT
} KeyResult;

static void keyDestroy(Key key);
static Key keyCreate(const char* key_id, const char* key_value);
static KeyResult keySetValue(Key key, const char *value);
static KeyResult keyCopy(Key dest, const Key source);
static inline char *keyGetID(Key key);
static inline char *keyGetValue(Key key);

//--------------------MAP-STRUCT--------------------//
struct Map_t {
    Key* keys;
    int size;
    int max_size;
    int iterator;
};

static int mapFindKey(Map map, const char* key);
static MapResult mapExpand(Map map);

//--------------------KEY-FUNCTIONS--------------------//
/**
 * @param key - A key struct to destroy and deallocate all it's components.
 * */
static void keyDestroy(Key key)
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
static Key keyCreate(const char* key_id, const char* key_value)
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
    Key key = malloc(sizeof(Key));
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
static KeyResult keySetValue(Key key, const char *value)
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

/**
 * @param dest - A destination key to copy into.
 * @param source - A constant source key to copy from.
 * @return
 * Assuming dest and source are not null vals.
 * KEY_OUT_OF_MEMORY - in case of memory allocation error.
 * KEY_SUCCESS - in the case which the copy was successful.
 * */
static KeyResult keyCopy(Key dest, const Key source)
{
    assert(dest != NULL && source != NULL);
    char *copy_id = malloc(strlen(source->id) + 1);
    char *copy_value = malloc(strlen(source->value) + 1);
    if(!copy_id || !copy_value)
    {
        free(copy_id);
        free(copy_value);
        return KEY_OUT_OF_MEMORY;
    }
    dest->id = copy_id;
    dest->value = copy_value;
    return KEY_SUCCESS;
}

static inline char *keyGetID(Key key)
{
    assert(key != NULL);
    return key->id;
}

static inline char *keyGetValue(Key key)
{
    assert(key != NULL);
    return key->value;
}

//--------------------STATIC-FUNCTIONS--------------------//
/**
 * @param map - The Key's map
 * @param key - The wanted key
 * @return 
 * -1 if key not found 
 * otherwise the key index 
 */

static int mapFindKey(Map map, const char* key)
{
    assert (map != NULL && key != NULL);
    for (int index =0 ; index < map->size ; index++)
    {
        if (!strcmp(key,keyGetID((map->keys[index]))))
        {
            return index;
        }
    }
    return MAP_NO_SUCH_KEY;
}


static MapResult mapExpand(Map map)
{
    assert(map != NULL);
    int new_size = MAP_EXPAND_FACTOR* map->max_size ;
    Key* new_kays_arry = realloc(map->keys, new_size* sizeof(*new_kays_arry));
    if (new_kays_arry == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    } 
    map->keys = new_kays_arry;
    map->max_size = new_size;
    return MAP_SUCCESS;
}

//--------------------HEADER-FUNCTIONS--------------------//
Map mapCreate()
{
    Map new_map = malloc(sizeof(*new_map));
    if (new_map == NULL)
    {
        return NULL;
    }
    Key* new_array = malloc(MAP_INITIAL_SIZE*sizeof(Key));
    if (new_array == NULL)
    {
        free(new_map);
        return NULL;
    }
    new_map->keys = new_array;
    new_map->size = 0;
    new_map->max_size = MAP_INITIAL_SIZE;
    return new_map;
}

void mapDestroy(Map map)
{
    if(!map)
    {
        return;
    }
    for(int i = 0; i < map->size; i++)
    {
        keyDestroy((map->keys)[i]);
    }
    free(map->keys);
    free(map);
}

Map mapCopy(Map map)
{
    if(!map)
    {
        return NULL;
    }
    Map new_map = mapCreate();
    if(!new_map)
    {
        return NULL;
    }
    for(int i = 0; i < map->size; i++)
    {
        if(keyCopy((new_map->keys)[i], (map->keys)[i]) == KEY_OUT_OF_MEMORY)
        {
            for(int j = 0; j < i; j++)
            {
                keyDestroy((new_map->keys)[j]);
            }
            mapDestroy(new_map);
            return NULL;            
        }
    }
    new_map->size = map->size;
    new_map->max_size = map->max_size;
    new_map->iterator = map->iterator;
    return new_map;
}

int mapGetSize(Map map)
{
    if (map == NULL)
    {
        return -1;
    }
    return map->size;
}

bool mapContains(Map map, const char* key)
{
    if (map == NULL || key == NULL)
    {
        return false;
    }
    if (mapFindKey(map,key) == MAP_NO_SUCH_KEY)
    {
        return false;
    }
    return true;
}

MapResult mapPut(Map map, const char* key, const char* data)
{
    if (map == NULL || key == NULL || data == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    int key_index = mapFindKey(map,key);
    if (key_index == MAP_NO_SUCH_KEY)
    {
        Key new_key = keyCreate(key, data);
        if (new_key == NULL)
        {
            return MAP_OUT_OF_MEMORY;
        }
        if ((map->size >= map->max_size) && mapExpand(map) != MAP_SUCCESS)
        {
            keyDestroy(new_key);
            return MAP_OUT_OF_MEMORY;
        }
        map->size = map->size + 1;
        map->keys[map->size] = new_key;
        return MAP_SUCCESS;
    }
    if (keySetValue(map->keys[key_index] , data) != KEY_SUCCESS)
    {
        return MAP_OUT_OF_MEMORY;
    }
    return MAP_SUCCESS;
}

char* mapGet(Map map, const char* key)
{
    if(!map || !key)
    {
        return NULL;
    }
    for(int i = 0; i < map->size; i++)
    {
        if(!strcmp(key, keyGetID((map->keys)[i])))
        {
            return keyGetValue((map->keys)[i]);
        }
    }
    return NULL;
}

MapResult mapRemove(Map map, const char* key)
{
    if(!map || !key)
    {
        return MAP_NULL_ARGUMENT;
    }
    for(int i = 0; i < map->size; i++)
    {
        if(!strcmp(keyGetID((map->keys)[i]), key))
        {
            keyDestroy((map->keys)[i]);
            if(i != map->size - 1)
            {
                (map->keys)[i] = (map->keys)[map->size - 1];
            }
            map->size--;
            return MAP_SUCCESS;
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

char* mapGetFirst(Map map)
{
    if(!map)
    {
        return NULL;
    }
    map->iterator = 0;
    return keyGetID((map->keys)[map->iterator]);
}

char* mapGetNext(Map map)
{
    if(!map)
    {
        return NULL;
    }
    map->iterator++;
    return keyGetID((map->keys)[map->iterator]);
}

MapResult mapClear(Map map)
{
    if (map == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    for(int i = 0; i < map->size; i++)
    {
        keyDestroy((map->keys)[i]);
    }
    map->size = 0;
    return MAP_SUCCESS;
}