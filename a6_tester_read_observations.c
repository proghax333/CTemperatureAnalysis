/* a6_tester_read_observations.c

   A tester which reads Observation objects from a file
   and prints each Observation as it is read. This might 
   be useful for debugging your file I/O functions.

   B. Bird - 11/21/2020
*/

#include <stdio.h>
#include "temperature_data.h"

int main(){

    char input_filename[] = "observations.txt";

    FILE* input_file = fopen(input_filename,"r");

    if(!input_file){
        printf("Error: Unable to open input file\n");
        return 1;
    }

    Observation obs;
    int i = 0;
    while(read_observation(input_file,&obs) == 1){
        Date d = obs.obs_date;
        printf("Observation %d: ", i);
        printf("Date/Time: %04d-%02d-%02d %02d:%02d, ", d.year,d.month,d.day,obs.hour,obs.minute);
        printf("Station: %d, Temperature: %.1f\n", obs.station_id, obs.temperature);
        i++;
    }


    fclose(input_file);
    
    return 0;
}