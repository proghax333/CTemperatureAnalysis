/*
   Parameters: input_file (FILE pointer), obs (pointer to Observation object)
   Return value (integer):
     - If the observation was read successfully, return 1
     - Otherwise, return 0
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "temperature_data.h"

//year month day hour minute station_number temperature
int read_observation(FILE* input_file, Observation* obs)
{
	if(obs && input_file) {
		return fscanf(input_file, "%d%d%d%d%d%d%f",
			&obs->obs_date.year,
			&obs->obs_date.month,
			&obs->obs_date.day,
			&obs->hour,
			&obs->minute,
			&obs->station_id,
			&obs->temperature) > 0;
	}
	return 0;
}

/* count_observations(filename)
   Given a filename, count and return the number of temperature observations in
   the file. If the file cannot be opened, return -1.

   Parameters: filename (string)
   Return value (integer):
     - If the file cannot be opened for reading, return -1
     - Otherwise, return the number of temperature observations in the file.
*/
int count_observations(char filename[]){
	FILE* file = fopen(filename, "r");
	if(file) {
		int count = 0;
		Observation dummy;
		while(read_observation(file, &dummy)) {
			++count;
		}
		fclose(file);
		return count;
	}
	return -1;
}

/* load_all_observations(filename, array_size, observation_array)
   Given a filename and an array of Observation objects, read as many 
   observations from the file as possible (up to the size of the array)
   and store each observation (in order) into the array.

   If the file cannot be opened, return -1. Otherwise, return the number of 
   observations read.

   If the file contains fewer observations then there are elements in the array,
   stop reading after the last observation in the file. Otherwise, stop reading
   once array_size observations are read.

   Parameters: filename (string), array_size (integer), observation_array (array of Observation)
   Return value (integer):
     - If the file could not be opened, return -1
     - Otherwise, return the total number of observations read and stored into 
       the array.
*/
int load_all_observations(char filename[], int array_size, Observation observation_array[array_size]){
    FILE* file = fopen(filename, "r");
    
    if(file) {
    	int i;
    	for(i = 0; i < array_size &&
			read_observation(
				file,
				&observation_array[i]
			); ++i);
		
		fclose(file);
		return i;
	}
    return -1;
}

/* print_station_extremes(num_observations, obs_array)
   Given an array of Observation objects, compute and print the
    _extreme observations_ (lowest temperature observed and highest
   temperature observed) for each station that has at least one
   observation in the dataset.

   The output will contain one line for each station with at least one 
   observation in the dataset, using a format equivalent to the following:
      Station 1: Minimum = -1.87 degrees (2020-11-21 06:10), Maximum = 10.6 degrees (2020-01-11 01:16)

   You should use the following printf format string to achieve the correct 
   output format.
      "Station %d: Minimum = %.2f degrees (%04d-%02d-%02d %02d:%02d), Maximum = %.2f degrees (%04d-%02d-%02d %02d:%02d)\n"
 
   The output must be ordered by station number (with lower numbered station 
   numbers appearing first). No more than one line of output should be 
   generated for a particular station number.

   In cases where multiple observations achieve the extreme value (e.g. if the
   minimum temperature at a particular station is -1.87, but there are several 
   observations with this temperature), print the date/time of the 
   chronologically earliest observation with that extreme temperature.

   You may assume that all observations contain a station number between 
   1 and 250 (inclusive).

   This function must _not_ use any file I/O features whatsoever.

   Parameters: num_observations (integer), observation_array (array of Observation)
   Return value: None
   Side Effect: A printed representation of station extremes is output to the user.
*/

#define MAX_STATION_NUMBER 250

typedef struct {
	Observation* min;
	Observation* max;
} ObservationExtreme;

int compareObservations(Observation* a, Observation* b) {
	if(a == NULL || b == NULL) {
		return 0;
	}
	Date p = a->obs_date;
	Date q = b->obs_date;
	
	if(p.year < q.year
		|| p.month < q.month
		|| p.day < q.day
		|| a->hour < b->hour
		|| a->minute < b->minute
		|| a->temperature < b->temperature)
	{
		return 1;
	}
	return 0;
}

void print_station_extreme_info(ObservationExtreme* a) {
	if(a) {
		printf("Station %d: "
			"Minimum = %.2f degrees (%04d-%02d-%02d %02d:%02d), "
			"Maximum = %.2f degrees (%04d-%02d-%02d %02d:%02d)\n",
		a->min->station_id,
		a->min->temperature, a->min->obs_date.year, a->min->obs_date.month,
			a->min->obs_date.day, a->min->hour, a->min->minute,
		a->max->temperature, a->max->obs_date.year, a->max->obs_date.month,
			a->max->obs_date.day, a->max->hour, a->max->minute);
	}
}

void print_station_extremes(int num_observations, Observation obs_array[num_observations]) {
    if(num_observations > 0) {
    	ObservationExtreme dummy = {
			.min = NULL, .max = NULL
		};
    	ObservationExtreme extremes[MAX_STATION_NUMBER + 1];
    	for(int i = 0; i < MAX_STATION_NUMBER + 1; ++i) extremes[i] = dummy;
    	
		Observation *observation = NULL;
		int station_id;
		
		for(int i = 0; i < num_observations; ++i) {
			observation = &obs_array[i];
			station_id = observation->station_id;
			
			if(extremes[station_id].min) {
				if(compareObservations(observation, extremes[station_id].min) > 0) {
					extremes[station_id].min = observation;
				}
				if(compareObservations(observation, extremes[station_id].max) == 0) {
					extremes[station_id].max = observation;
				}
			} else {
				extremes[station_id].min = observation;
				extremes[station_id].max = observation;
			}
		}
		for(int i = 1; i < MAX_STATION_NUMBER; ++i) {
			if(extremes[i].min) {
				print_station_extreme_info(&extremes[i]);
			}
		}
	}
}

/* print_daily_averages(num_observations, obs_array)
   Given an array of observation objects, compute and print the average 
   temperature for each day which has at least one observation in the 
   dataset.

   The output must contain only dates which actually appear in at least 
   one observation object in the array. The dates must be in ascending 
   chronological order (so an earlier date must be printed before a later 
   one) and each date may only appear once.

   Each line of output will have the form "year month day average", for 
   example "2020 11 20 10.6" (which would be interpreted to mean that 
   the average temperature on Nov. 21, 2020 was 10.6 degrees).

   Your code may assume that all dates are from years between 2015 and 2020 
   (inclusive) and that the month/day entries are all valid (that is, month
   will always be between 1 and 12 inclusive and day will always be between
   1 and 31 inclusive).
   
   This function must _not_ use any file I/O features whatsoever.

   Parameters: num_observations (integer), observation_array (array of Observation)
   Return value: None
   Side Effect: A printed representation of the average daily temperature is
                output to the user.
*/
typedef struct Node Node;
struct Node {
	long key;
	float value;
	Date date;
	int no_of_observations;
	Node *prev;
	Node *next;
};

Node* nodeCreate(Node* prev, Node* next) {
	
	Node* newNode = (Node*) calloc(1, sizeof(Node));
	newNode->value = 0.0f;
	newNode->no_of_observations = 0;
	newNode->prev = prev;
	newNode->next = next;
	
	return newNode;
}

typedef struct HashList HashList;
struct HashList {
	Node* head;
	Node* tail;
};

HashList* listCreate() {
	HashList* newList = (HashList*) calloc(1, sizeof(HashList));
	newList->head = NULL;
	newList->tail = NULL;
	return newList;
}

Node* listSortedInsert(HashList* list, long key, float value) {
	if(list) {
		if(list->head) {
			if(key < list->head->key) {
				Node* newNode = nodeCreate(NULL, list->head);
				newNode->key = key;
				newNode->value = value;
				newNode->no_of_observations = 1;
				
				list->head->prev = newNode;
				list->head = newNode;
				
				return newNode;
			} else if(key > list->tail->key) {
				Node* newNode = nodeCreate(list->tail, NULL);
				newNode->key = key;
				newNode->value = value;
				newNode->no_of_observations = 1;
				
				list->tail->next = newNode;
				list->tail = newNode;
				
				return newNode;
			} else {
				Node* iterator = list->head;
				
				while(iterator) {
					if(key == iterator->key) {
						++iterator->no_of_observations;
						iterator->value += value;
						return iterator;
					} else if(key < iterator->key) {
						Node* newNode = nodeCreate(iterator->prev, iterator);
						newNode->key = key;
						newNode->value = value;
						newNode->no_of_observations = 1;
						
						iterator->prev = newNode;
						
						return newNode;
					}
					iterator = iterator->next;
				}
			}
		} else {
			Node* newNode = nodeCreate(NULL, NULL);
			newNode->key = key;
			newNode->value = value;
			newNode->no_of_observations = 1;
			
			list->head = newNode;
			list->tail = newNode;
			return newNode;
		}
	}
	return NULL;
}

void listFree(HashList* list) {
	if(list) {
		Node* node = list->head, *t = node;
		if(node) {
			do {
				node = t;
				t = node->next;
				free(node);
			} while(t);
		}
		free(list);
	}
}

void print_daily_averages(int num_observations, Observation obs_array[num_observations]){
	HashList* averages = listCreate();
	
	char buffer[9] = { 0 };
	Observation* temp = NULL;
	Node* temp_node = NULL;
	long temp_key;
	
	for(int i = 0; i < num_observations; ++i) {
		temp = &obs_array[i];
		
		sprintf(buffer, "%04d%02d%02d",
			temp->obs_date.year,
			temp->obs_date.month,
			temp->obs_date.day);
		
		temp_key = strtol(buffer, NULL, 10);
		temp_node = listSortedInsert(averages, temp_key, temp->temperature);
		temp_node->date = temp->obs_date;
	}
	
	Node* iterator = averages->head;
	while(iterator) {
		printf("%04d %02d %02d %.1f\n",
			iterator->date.year,
			iterator->date.month,
			iterator->date.day,
			iterator->value / (float)iterator->no_of_observations);
		
		iterator = iterator->next;
	}
	
	listFree(averages);
}
