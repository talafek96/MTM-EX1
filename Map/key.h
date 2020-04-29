#ifndef KEY_H
#define KEY_H

/**
* Key Container
*
* Implements a key container type.
* The type of the key and the value is string (char *)
*
* The following functions are available:
*   keyCreate		- Creates a new key with an ID and a value as const strings.
*   keyDestroy		- Deletes an existing key and frees all resources
*   keySetValue		- Sets a new value to a given key.
*   keyGetID  	    - Returns the ID of a key as a char* (not a copy).
*   keyGetValue		- Returns the value of a key as a char* (not a copy).
*/

typedef struct key_t *Key;

typedef enum
{
    KEY_SUCCESS = 0, 
    KEY_OUT_OF_MEMORY,
    KEY_NULL_ARGUMENT
} KeyResult;

/**
 * @param key - A key struct to destroy and deallocate all it's components.
 * */
void keyDestroy(Key key);

/**
 * @param key_id - Constant string for the ID of the key.
 * @param key_value - constant string for the value of the key.
 * @return
 * NULL - in case of null arguments, memory allocation fail for the string copies/key struct.
 * In case of SUCCESS - a pointer for the new allocated key.
 * */
Key keyCreate(const char* key_id, const char* key_value);

/**
 * @param key - The key you want to chang it's value
 * @param value - The new value of the key
 * @return 
 * KEY_NULL_ARGUMENT - if one of the args are NULL
 * KEY_OUT_OF_MEMORY - if the memory allocation fails
 * KEY_SUCCESS - if the value successfully set
 */
 KeyResult keySetValue(Key key, const char *value);

/**
 * @param key - The key you want it's ID
 * @return 
 * NULL if @param key is NULL, otherwise a pointer to the wanted key's ID (not a copy).
 */
 char *keyGetID(Key key);

 /**
 * @param key - The key you want it's value
 * @return 
 * NULL if @param key is NULL, otherwise a pointer to the wanted key's value (not a copy). 
 */
 char *keyGetValue(Key key);

#endif