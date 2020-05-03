#include "map.h"
#include "key.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> 

/** The initial size of Key's arry in a Map */
#define MAP_INITIAL_SIZE 100

/** The pactor by which to expand the Key's arry when needed */
#define MAP_EXPAND_FACTOR 2

/** Return by 'mapFindKey' function when didn't finde such key */
#define MAP_NO_SUCH_KEY -1




//--------------------MAP-STRUCT--------------------//
struct Map_t {
    Key* keys;
    int size;
    int max_size;
    int iterator;
};

static int mapFindKey(Map map, const char* key);
static MapResult mapExpand(Map map);



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
    for (int index = 0; index < map->size; index++)
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
    int new_size = MAP_EXPAND_FACTOR * map->max_size ;
    Key* new_keys_array = realloc(map->keys, new_size* sizeof(*new_keys_array));
    if (new_keys_array == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    } 
    map->keys = new_keys_array;
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
    new_map->iterator = 0;
    return new_map;
}

void mapDestroy(Map map)
{
    if(!map)
    {
        return;
    }
    mapClear(map);
    free(map->keys);
    free(map);
}

Map mapCopy(Map map)
{
    Map new_map = mapCreate();
    if(!new_map || !map)
    {
        mapDestroy(new_map);
        return NULL;
    }
    for(int i = 0; i < map->size; i++)
    {
        (new_map->keys)[i] = keyCreate(keyGetID((map->keys)[i]), keyGetValue((map->keys)[i]));
        if((new_map->keys)[i] == NULL)
        {
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
        map->keys[map->size++] = new_key;
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
    return map->size > 0? keyGetID((map->keys)[map->iterator]) : NULL;
}

char* mapGetNext(Map map)
{
    if(!map)
    {
        return NULL;
    }
    map->iterator++;
    return map->iterator >= map->size? NULL : keyGetID((map->keys)[map->iterator]);
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