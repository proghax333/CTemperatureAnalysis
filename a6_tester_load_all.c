/* a6_tester_load_all.c

   A tester which reads Observation objects from a file
   using load_all_observations and then prints each 
   Observation back out. This might be useful for
   debugging your file I/O functions.

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

    for(int i = 0; i < num_observations; i++){
        Observation obs = obs_array[i];
        Date d = obs.obs_date;
        printf("Observation %d: ", i);
        printf("Date/Time: %04d-%02d-%02d %02d:%02d, ", d.year,d.month,d.day,obs.hour,obs.minute);
        printf("Station: %d, Temperature: %.1f\n", obs.station_id, obs.temperature);
    }
    
    return 0;
}