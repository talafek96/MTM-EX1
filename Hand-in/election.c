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
#define ELECTION_MAX_NUM_LENGTH 12

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


//--------------------STATIC-FUNCTIONS--------------------//
/** 
 * @param str - The string you want to convert to int.
 * @return 
 * num - The converted string to the wanted int.
 */
static int stringToInt(const char *str)
{
    assert(*str >= '0' && *str <= 9);
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
    char *new_str = malloc((len+1)*sizeof(char));
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
        if(*vote_key == '-')
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
 * NULL, if allocation was failed.
 * ----If allocation fails, @param ptr will still be allocated!----
 */
static void *expand(void *ptr, int current_size)
{
    void *new_ptr = realloc(ptr, current_size * ELECTION_RESIZE_FACTOR);
    return new_ptr;
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

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes)
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
    char* vote_key = malloc((strlen(area_char_id)+strlen(tribe_char_id)+2)*sizeof(char)); 
    if (vote_key == NULL)
    {
        free(area_char_id);
        free(tribe_char_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    sprintf(vote_key,"%s-%s",area_char_id, tribe_char_id);
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
    char *vote_key = malloc(strlen(str_area_id) + strlen(str_tribe_id) + 1 + 1);
    if(!vote_key)
    {
        free(str_area_id);
        free(str_tribe_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    sprintf(vote_key, "%s-%s", str_area_id, str_tribe_id);
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

ElectionResult electionRemoveTribe (Election election, int tribe_id)
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
    char** remove_votes = malloc(ELECTION_INITIAL_SIZE*sizeof(remove_votes));
    int counter=0, elements_number = ELECTION_INITIAL_SIZE;
    MAP_FOREACH(area, election->areas)
    {
        char *area_vote = malloc(strlen(area)+strlen(tribe_char_id)+1+1);
        if (area_vote == NULL)
        {
            free(tribe_char_id);
            free(remove_votes);
            return ELECTION_OUT_OF_MEMORY;
        }
        sprintf(area_vote, "%s-%s", area, tribe_char_id);
        if (mapContains(election->votes,area_vote))
        {
            if (counter >= elements_number) //Check to see if the pointer goes out-of-bounds
            {
                char **temp_ptr = remove_votes;
                remove_votes = expand(remove_votes, elements_number);
                elements_number *= ELECTION_RESIZE_FACTOR;
                if(remove_votes == NULL)
                {
                    free(temp_ptr);
                    free(tribe_char_id);
                    for(int i = 0; i < counter; i++)
                    {
                        free(remove_votes[i]);
                    }
                    free(remove_votes);
                    return ELECTION_OUT_OF_MEMORY;
                }                      
            }
            remove_votes[counter++]= area_vote;
        }
    }
    for(int i = 0; i < counter; i++)
    {
        mapRemove(election->votes, remove_votes[i]);
        free(remove_votes[i]);
    }
    free(remove_votes);
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
    char **votes_to_remove = malloc(ELECTION_INITIAL_SIZE * sizeof(**votes_to_remove));
    char **areas_to_remove = malloc(ELECTION_INITIAL_SIZE * sizeof(**areas_to_remove));
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
            MAP_FOREACH(vote, election->votes)
            {
                char *tmp = voteKeyToAreaKey(vote);
                if(tmp == NULL)
                {
                    free(tmp);
                    free(areas_to_remove);
                    free(votes_to_remove);
                    return ELECTION_OUT_OF_MEMORY;
                }
                if(!strcmp(area, tmp))
                {
                    if(votes_counter >= votes_element_number) //Checks to see if it goes out-of-bounds
                    {
                        char **tmp_ptr = votes_to_remove;
                        votes_to_remove = expand(votes_to_remove, votes_element_number);
                        votes_element_number *= ELECTION_RESIZE_FACTOR;
                        if(votes_to_remove == NULL)
                        {
                            free(tmp_ptr);
                            free(tmp);
                            free(areas_to_remove);
                            return ELECTION_OUT_OF_MEMORY;
                        }
                    }
                    votes_to_remove[votes_counter++] = vote;
                }
                free(tmp);
            }
            if(areas_counter >= areas_element_number) //Checks to see if it goes out-of-bounds
            {
                char **tmp_ptr = areas_to_remove;
                areas_to_remove = expand(areas_to_remove, areas_element_number);
                areas_element_number *= ELECTION_RESIZE_FACTOR;
                if(areas_to_remove == NULL)
                {
                    free(tmp_ptr);
                    free(votes_to_remove);
                    return ELECTION_OUT_OF_MEMORY;
                }
            }
            areas_to_remove[areas_counter++] = area;
        }
    }
    for(int i = 0; i < votes_counter; i++)
    {
        mapRemove(election->votes, votes_to_remove[i]);
    }
    for(int i = 0; i < areas_counter; i++)
    {
        mapRemove(election->areas, areas_to_remove[i]);
    }
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
    int tribe_votes = 0;
    int max_votes = 0;
    char *max_ptr = NULL, *lowest_id = mapGetFirst(election->tribes);
    if(lowest_id == NULL)
    {
        return statistics;
    }
    MAP_FOREACH(tribe, election->tribes)
    {
        if(stringToInt(tribe) <= stringToInt(lowest_id))
        {
            lowest_id = tribe;
        }
    }
    max_ptr = lowest_id;
    MAP_FOREACH(area, election->areas)
    {
        MAP_FOREACH(tribe, election->tribes)
        {
            char *vote_key = malloc(strlen(area) + strlen(tribe) + 1 + 1);
            if(vote_key == NULL)
            {
                mapDestroy(statistics);
                return NULL;
            }
            sprintf(vote_key,"%s-%s", area, tribe);
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
        if(mapPut(statistics, area, max_ptr) != MAP_SUCCESS)
        {
            mapDestroy(statistics);
            return NULL;
        }
        max_ptr = lowest_id;
        max_votes = 0;
    }
    return statistics;
}