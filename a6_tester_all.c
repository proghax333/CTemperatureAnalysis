/* a6_tester_all.c
   
   A tester which reads Observation objects from a file
   and then prints the daily averages and station
   extremes based on the data read.

   B. Bird - 11/21/2020
*/

#include <stdio.h>
#include "temperature_data.h"

int main(){

    char input_filename[] = "observations.txt";

    int num_observations = count_observations(input_filename);
    printf("Counted %d observations\n", num_observations);

    Observation obs_array[num_observations];
    int num_read = load_all_observations(input_filename,num_observations,obs_array);
    printf("Read %d observations\n", num_read);

    printf("Daily Averages:\n");
    print_daily_averages(num_observations, obs_array);

    printf("Station Extremes:\n");
    print_station_extremes(num_observations, obs_array);
    
    return 0;
}