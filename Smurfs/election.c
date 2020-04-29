#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "election.h"

//--------------------DEFINES--------------------//
#define ELECTION_STR_TO_INT '0'
#define ELECTION_INITIAL_SIZE 100
#define ELECTION_RESIZE_FACTOR 2

#define REPEATED_MAP_REMOVE(map, item_bank, num_of_items) \
    do { \
        for(int i = 0; i < num_of_items; i++) \
        { \
            mapRemove(map, item_bank[i]); \
        } \
    } while(0)

#define FREE_ARRAY(array, counter) \
    do \
    { \
        for(int i = 0; i < counter; i++) \
        { \
            free(array[i]); \
        } \
        free(array); \
    } while(0)

//----------STRUCT&FUNCTION-DECLARATIONS----------//
struct election_t
{
    Map areas;
    Map tribes;
    Map votes; //Key syntax: "area_key-tribe_key"
};

static int stringToInt(const char *str);
static char *intToString(int num);
static bool isValidName(const char *name);
static char *voteKeyToAreaKey(const char *vote_key);
static void *expand(void *ptr, int current_size);
static const char *electionFindLowestID(Map map);
static char *electionGenerateVoteKey(const char *area, const char *tribe);
static const char *electionGetChosenTribeByArea(Election election, const char *area);
static ElectionResult electionGetVoteListByArea(Election election, const char *area, char **votes_bank,
                                                int *votes_size, int *votes_counter);
static ElectionResult electionGetVoteListByTribe(Election election, const char *tribe, char **votes_bank,
                                                int *votes_size, int *votes_counter);

//--------------------STATIC-FUNCTIONS--------------------//
/** 
 * @param str - The string you want to convert to int.
 * @return 
 * num - The converted string to the wanted int.
 */
static int stringToInt(const char *str)
{
    assert(*str >= '0' && *str <= '9');
    int num = 0;
    while (*str) 
    {
        num *= 10;
        num += *str - ELECTION_STR_TO_INT;
        str++;
    }
    return num;
}

/**
 * @param num - The int you want to convert to string.
 * @return 
 * new_str - A pointer to the result string.
 */
static char *intToString(int num)
{
    assert(num >= 0);
    int len = 0;
    int temp = num;
    if(temp == 0)
    {
        len = 1;
    }
    while (temp > 0)
    {
        temp /= 10;
        len++;
    }
    char *new_str = malloc(len+1);
    if(new_str == NULL)
    {
        return NULL;
    }
    new_str[len]= '\0';
    for (int i =(len-1); i >=0 ; i--)
    {
        new_str[i]= ((num%10)+ELECTION_STR_TO_INT);
        num /= 10;
    }
    return new_str;
}

/**
 * @param vote_key - a vote key.
 * @return
 * If reaches the EOS but a dash was not encountered - NULL
 * Allocation failed - NULL
 * ELSE - a pointer to an area key string 
 */
static char *voteKeyToAreaKey(const char *vote_key)
{
    assert(vote_key != NULL);
    int counter = 0;
    bool dash = false;
    while(*(vote_key+counter) && !dash)
    {
        if(*(vote_key + counter) == '-')
        {
            dash = true;
            continue;
        }
        counter++;
    }
    if(!dash)
    {
        return NULL;
    }
    char *area_key = malloc(counter + 1); //+1 for the '\0' char
    if(area_key == NULL)
    {
        return NULL;
    }
    for(int i = 0; i < counter; i++)
    {
        area_key[i] = vote_key[i];
    }
    area_key[counter] = '\0';
    return area_key;
}

/**
 * @param name - A name of a tribe/area to test.
 * @return
 * True if the name is valid.
 * False if the name is not valid.
*/
static bool isValidName(const char *name)
{
    while (*name)
    {
        if ((*name < 'a' || *name > 'z') && *name != ' ')
        {
            return false;
        }
        name++;
    }
    return true;
}

/**
 * @param ptr - Pointer to the array you want to expand
 * @param current_size - The current size of the array
 * @return 
 * An address to the new pointer, if allocation was successful
 * NULL, if the allocation failed.
 * ----If allocation fails, @param ptr will still be allocated!----
 */
static void *expand(void *ptr, int current_size)
{
    void *new_ptr = realloc(ptr, current_size * ELECTION_RESIZE_FACTOR);
    return new_ptr;
}

/**
 * @param map - A map with keys of strings that represent int numbers.
 * @return 
 * An pointer to the key with the lowest ID in the map - if allocation was successful (NOT A COPY!)
 * NULL, if the allocation failed or if the map is empty.
 */
static const char *electionFindLowestID(Map map)
{
    const char *lowest_id = mapGetFirst(map);
    if(lowest_id == NULL)
    {
        return NULL;
    }
    MAP_FOREACH(element, map)
    {
        if(stringToInt(element) <= stringToInt(lowest_id))
        {
            lowest_id = element;
        }
    }
    return lowest_id;
}

/**
 * @param area - The area key as a const string.
 * @param tribe - The tribe key as a const string.
 * @return 
 * A new string with the format of the corresponding vote key.
 * NULL, if the allocation failed.
 */
static char *electionGenerateVoteKey(const char *area, const char *tribe)
{
    assert(area != NULL && tribe != NULL);
    char *vote_key = malloc(strlen(area) + strlen(tribe) + 1 + 1);
    if(vote_key == NULL)
    {
        return NULL;
    }
    sprintf(vote_key,"%s-%s", area, tribe);
    return vote_key;
}

/**
 * @param election - A pointer to the election struct.
 * @param area - An area key as a const string.
 * @return 
 * A pointer to the key of the chosen tribe by the @param area.
 * NULL, if the allocation failed, or if the map is empty.
 */
static const char *electionGetChosenTribeByArea(Election election, const char *area)
{
    int tribe_votes = 0, max_votes = 0;
    const char *max_ptr = electionFindLowestID(election->tribes);
    if(max_ptr == NULL)
    {
        return NULL;
    }
    MAP_FOREACH(tribe, election->tribes)
    {
        char *vote_key = electionGenerateVoteKey(area, tribe);
        if(vote_key == NULL)
        {
            return NULL;
        }
        if(mapContains(election->votes, vote_key))
        {
            tribe_votes = stringToInt(mapGet(election->votes, vote_key));
            if(tribe_votes > max_votes)
            {
                max_votes = tribe_votes;
                max_ptr = tribe;
            }
            else if(tribe_votes == max_votes)
            {
                if(stringToInt(tribe) < stringToInt(max_ptr))
                {
                    max_ptr = tribe;
                }
            }
        }
        free(vote_key);
        tribe_votes = 0;
    }
    return max_ptr;
}

/**
 * @param election - A pointer to the election struct.
 * @param area - An area key as a const string.
 * @param votes_bank - An allocated array of strings to store all the votes by the area.
 * @param votes_size - A pointer to the initial size of the array.
 * @param votes_counter - A pointer to a variable to store the number of votes by the area.
 * @return 
 * ELECTION_SUCCESS - if the function succeeds.
 * ELECTION_OUT_OF_MEMORY - in case of allocation or re-allocation fail.
 * --IN CASE OF ALLOCATION FAIL, @param votes_bank WILL STILL BE ALLOCATED!--
 */
static ElectionResult electionGetVoteListByArea(Election election, const char *area, char **votes_bank,
                                                int *votes_size, int *votes_counter)
{
    MAP_FOREACH(vote, election->votes)
    {
        char *tmp = voteKeyToAreaKey(vote);
        if(tmp == NULL)
        {
            return ELECTION_OUT_OF_MEMORY;
        }
        if(!strcmp(area, tmp))
        {
            if(*votes_counter >= *votes_size) //Checks to see if it goes out-of-bounds
            {
                char **tmp_ptr = expand(votes_bank, *votes_size * sizeof(*votes_bank));
                *votes_size *= ELECTION_RESIZE_FACTOR;
                if(tmp_ptr == NULL)
                {
                    free(tmp);
                    return ELECTION_OUT_OF_MEMORY;
                }
            }
            votes_bank[(*votes_counter)++] = vote;
        }
        free(tmp);
    }
    return ELECTION_SUCCESS;
}

/**
 * @param election - A pointer to the election struct.
 * @param tribe - An tribe key as a const string.
 * @param votes_bank - An allocated array of strings to store all the votes by the tribe.
 * @param votes_size - A pointer to the initial size of the array.
 * @param votes_counter - A pointer to a variable to store the number of votes by the tribe.
 * @return 
 * ELECTION_SUCCESS - if the function succeeds.
 * ELECTION_OUT_OF_MEMORY - in case of allocation or re-allocation fail.
 * --IN CASE OF ALLOCATION FAIL, @param votes_bank WILL STILL BE ALLOCATED!-- 
 */
static ElectionResult electionGetVoteListByTribe(Election election, const char *tribe, char **votes_bank,
                                                int *votes_size, int *votes_counter)
{
    MAP_FOREACH(area, election->areas)
    {
        char *area_vote = electionGenerateVoteKey(area, tribe);
        if (area_vote == NULL)
        {
            return ELECTION_OUT_OF_MEMORY;
        }
        if (mapContains(election->votes,area_vote))
        {
            if (*votes_counter >= *votes_size) //Check to see if the pointer goes out-of-bounds
            {
                char **temp_ptr = expand(votes_bank, (*votes_size) * sizeof(char *));
                *votes_size *= ELECTION_RESIZE_FACTOR;
                if(temp_ptr == NULL)
                {
                    free(temp_ptr);
                    return ELECTION_OUT_OF_MEMORY;
                }
            }
            votes_bank[(*votes_counter)++]= area_vote;
        }
        else
        {
            free(area_vote);
        }
    }
    return ELECTION_SUCCESS;
}

//--------------------HEADER-FUNCTIONS--------------------//
Election electionCreate()
{
    Map new_areas_map = mapCreate();
    Map new_tribes_map = mapCreate();
    Map new_votes_map = mapCreate();
    Election new_election = malloc(sizeof(*new_election));
    if (!new_areas_map || !new_tribes_map || !new_votes_map || !new_election)
    {
        mapDestroy(new_votes_map);
        mapDestroy(new_areas_map);
        mapDestroy(new_tribes_map);
        electionDestroy(new_election);
        return NULL;
    }
    new_election->areas = new_areas_map;
    new_election->tribes = new_tribes_map;
    new_election->votes = new_votes_map;
    return new_election;
}

void electionDestroy(Election election)
{
    if(election == NULL)
    {
        return;
    }
    mapDestroy(election->votes);
    mapDestroy(election->areas);
    mapDestroy(election->tribes);
    free(election);
}

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name)
{
    if (election == NULL || tribe_name == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0)
    {
        return ELECTION_INVALID_ID;
    }
    char* tribe_char_id = intToString(tribe_id);
    if (tribe_char_id == NULL)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    if (mapContains(election->tribes, tribe_char_id))
    {
        free(tribe_char_id);
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (!isValidName(tribe_name))
    {
        free(tribe_char_id);
        return ELECTION_INVALID_NAME;
    }
    if (mapPut(election->tribes, tribe_char_id, tribe_name) != MAP_SUCCESS)
    {
        free(tribe_char_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(tribe_char_id);
    return ELECTION_SUCCESS;
    
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name)
{
    if (election == NULL || area_name == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (area_id < 0)
    {
        return ELECTION_INVALID_ID;
    }
    char* area_char_id = intToString(area_id);
    if (area_char_id == NULL)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    if (mapContains(election->areas, area_char_id))
    {
        free(area_char_id);
        return ELECTION_AREA_ALREADY_EXIST;
    }
    if (!isValidName(area_name))
    {
        free(area_char_id);
        return ELECTION_INVALID_NAME;
    }
    if (mapPut(election->areas, area_char_id, area_name) != MAP_SUCCESS)
    {
        free(area_char_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(area_char_id);
    return ELECTION_SUCCESS;
}

char* electionGetTribeName (Election election, int tribe_id) ////////TO CHANGE
{
    if(tribe_id < 0)
    {
        return NULL; 
    }
    char* tribe_char_id = intToString(tribe_id);
    if (tribe_char_id == NULL)
    {
        return NULL;
    }
    char* tmp = mapGet(election->tribes, tribe_char_id);
    if(!tmp)
    {
        free(tribe_char_id);
        return NULL;
    }
    char* tribe_name = malloc(strlen(tmp) + 1);
    if(!tribe_name)
    {
        free(tribe_char_id);
        return NULL;
    }
    strcpy(tribe_name, tmp);
    free(tribe_char_id);
    return tribe_name;
}

ElectionResult electionAddVote(Election election, int area_id, int tribe_id, int num_of_votes)  
{
    if (election == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (area_id < 0 || tribe_id < 0)
    {
        return ELECTION_INVALID_ID;
    }
    if (num_of_votes <= 0)
    {
        return ELECTION_INVALID_VOTES;
    }
    char *area_char_id = intToString(area_id);
    char *tribe_char_id = intToString(tribe_id);
    if (!area_char_id || !tribe_char_id)
    {
        free(area_char_id);
        free(tribe_char_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    if (!mapContains(election->areas, area_char_id))
    {
        free(area_char_id);
        free(tribe_char_id);
        return ELECTION_AREA_NOT_EXIST;
    }
    if (!mapContains(election->tribes, tribe_char_id))
    {
        free(area_char_id);
        free(tribe_char_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    char* vote_key = electionGenerateVoteKey(area_char_id,tribe_char_id);
    if (vote_key == NULL)
    {
        free(area_char_id);
        free(tribe_char_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    char *new_num_of_votes_str = mapContains(election->votes, tribe_char_id)?
    intToString(stringToInt(mapGet(election->votes, vote_key)) + num_of_votes) : intToString(num_of_votes);
    if (new_num_of_votes_str == NULL)
    {
        free(vote_key);
        free(area_char_id);
        free(tribe_char_id);
        return ELECTION_OUT_OF_MEMORY; 
    }
    if (mapPut(election->votes, vote_key, new_num_of_votes_str) != MAP_SUCCESS)
    {
        free(new_num_of_votes_str);
        free(vote_key);
        free(area_char_id);
        free(tribe_char_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(new_num_of_votes_str);
    free(vote_key);
    free(area_char_id);
    free(tribe_char_id);
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes)
{
    if(!election)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if(area_id < 0 || tribe_id < 0)
    {
        return ELECTION_INVALID_ID;
    }
    if(num_of_votes < 1)
    {
        return ELECTION_INVALID_VOTES;
    }
    char *str_area_id = intToString(area_id);
    char *str_tribe_id = intToString(tribe_id);
    if(!str_area_id || !str_tribe_id)
    {
        free(str_area_id);
        free(str_tribe_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    if(!mapContains(election->areas, str_area_id))
    {
        free(str_area_id);
        free(str_tribe_id);
        return ELECTION_AREA_NOT_EXIST;
    }
    if(!mapContains(election->tribes, str_tribe_id))
    {
        free(str_area_id);
        free(str_tribe_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    char *vote_key = electionGenerateVoteKey(str_area_id,str_tribe_id);
    if(!vote_key)
    {
        free(str_area_id);
        free(str_tribe_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    if(!mapContains(election->votes, vote_key))
    {
        free(vote_key);
        free(str_area_id);
        free(str_tribe_id);
        return ELECTION_SUCCESS;
    }
    int new_votes = (stringToInt(mapGet(election->votes, vote_key)) - num_of_votes) <= 0?
                     0 : (stringToInt(mapGet(election->votes, vote_key)) - num_of_votes);
    char *str_new_votes = intToString(new_votes);
    if(!str_new_votes)
    {
        free(vote_key);
        free(str_area_id);
        free(str_tribe_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    if(mapPut(election->votes, vote_key, str_new_votes) == MAP_OUT_OF_MEMORY)
    {
        free(vote_key);
        free(str_area_id);
        free(str_tribe_id);
        free(str_new_votes);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(vote_key);
    free(str_area_id);
    free(str_tribe_id);
    free(str_new_votes);
    return ELECTION_SUCCESS;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name)
{
    if(!election || !tribe_name)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if(tribe_id < 0)
    {
        return ELECTION_INVALID_ID;
    }
    char *str_tribe_id = intToString(tribe_id);
    if(!str_tribe_id)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    if(!mapContains(election->tribes, str_tribe_id))
    {
        free(str_tribe_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if(!isValidName(tribe_name))
    {
        free(str_tribe_id);
        return ELECTION_INVALID_NAME;
    }
    if(mapPut(election->tribes, str_tribe_id, tribe_name) == MAP_OUT_OF_MEMORY)
    {
        free(str_tribe_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(str_tribe_id);
    return ELECTION_SUCCESS;
}


ElectionResult electionRemoveTribe(Election election, int tribe_id)
{
    if (election == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0)
    {
        return ELECTION_INVALID_ID;
    }
    char *tribe_char_id = intToString(tribe_id);
    if(tribe_char_id == NULL)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    if (!mapContains(election->tribes, tribe_char_id))
    {
        free(tribe_char_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    char** remove_votes = malloc(ELECTION_INITIAL_SIZE*sizeof(*remove_votes));
    if (remove_votes == NULL)
    {
        free(tribe_char_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    int counter = 0, elements_number = ELECTION_INITIAL_SIZE;
    if(electionGetVoteListByTribe(election,tribe_char_id,remove_votes, &elements_number, &counter) != ELECTION_SUCCESS)
    {
        FREE_ARRAY(remove_votes, counter);
        mapRemove(election->tribes, tribe_char_id);
        free(tribe_char_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    REPEATED_MAP_REMOVE(election->votes, remove_votes, counter);
    FREE_ARRAY(remove_votes, counter);
    mapRemove(election->tribes, tribe_char_id);
    free(tribe_char_id);
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area)
{
    if(election == NULL || should_delete_area == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    char **votes_to_remove = malloc(ELECTION_INITIAL_SIZE * sizeof(*votes_to_remove));
    char **areas_to_remove = malloc(ELECTION_INITIAL_SIZE * sizeof(*areas_to_remove));
    if(areas_to_remove == NULL || votes_to_remove == NULL)
    {
        free(areas_to_remove);
        free(votes_to_remove);
        return ELECTION_OUT_OF_MEMORY;
    }
    int votes_counter = 0, areas_counter = 0, votes_element_number = ELECTION_INITIAL_SIZE,
        areas_element_number = ELECTION_INITIAL_SIZE;
    MAP_FOREACH(area, election->areas)
    {
        if(should_delete_area(stringToInt(area)))
        {
            if(electionGetVoteListByArea(election, area, votes_to_remove, &votes_element_number, &votes_counter)
            != ELECTION_SUCCESS)
            {
                free(areas_to_remove);
                free(votes_to_remove);
                return ELECTION_OUT_OF_MEMORY;
            }
            if(areas_counter >= areas_element_number) //Checks to see if it goes out-of-bounds
            {
                // char **tmp_ptr = realloc(areas_to_remove, (areas_element_number * sizeof(*areas_to_remove)) * ELECTION_RESIZE_FACTOR);
                char **tmp_ptr = expand(areas_to_remove, areas_element_number * sizeof(*areas_to_remove));
                areas_element_number *= ELECTION_RESIZE_FACTOR;
                if(tmp_ptr == NULL)
                {
                    free(areas_to_remove);
                    free(votes_to_remove);
                    return ELECTION_OUT_OF_MEMORY;
                }
            }
            areas_to_remove[areas_counter++] = area;
        }
    }
    REPEATED_MAP_REMOVE(election->votes, votes_to_remove, votes_counter);
    REPEATED_MAP_REMOVE(election->areas, areas_to_remove, areas_counter);
    free(areas_to_remove);
    free(votes_to_remove);
    return ELECTION_SUCCESS;
}

Map electionComputeAreasToTribesMapping (Election election)
{
    Map statistics = mapCreate();
    if(statistics == NULL)
    {
        return NULL;
    }
    const char *max_ptr = NULL;
    MAP_FOREACH(area, election->areas)
    {
        max_ptr = electionGetChosenTribeByArea(election, area);
        if(max_ptr == NULL)
        {
            return NULL;
        }
        if(mapPut(statistics, area, max_ptr) != MAP_SUCCESS)
        {
            mapDestroy(statistics);
            return NULL;
        }
    }
    return statistics;
}