/* a6_tester_basic.c

   A tester which uses a fixed array of Observation objects
   (useful for testing the functions to compute station extremes
    and daily averages without worrying about file I/O).

   B. Bird - 11/21/2020
*/

#include <stdio.h>
#include "temperature_data.h"

int main(){
    Observation obs_array[] = {
        //Two observations for 2020-06-10 at station 7
        { {2020, 6, 10}, 1, 11, 7, 1 },
        { {2020, 6, 10}, 1, 16, 7, 2 },
        //Two observations for 2020-06-10 at station 9
        { {2020, 6, 10}, 13, 21, 9, 10 },
        { {2020, 6, 10}, 13, 26, 9, 20 },

        //Two observations for 2018-03-04 at station 1
        { {2018, 3, 4}, 15, 12, 1, 15 },
        { {2018, 3, 4}, 19, 13, 1, 25 },

        //Two observations for 2020-09-07 at station 1
        { {2020, 9, 7}, 18, 7, 1, 9 },
        { {2020, 9, 7}, 11, 1, 1, 12 },

        //Two observations for 2020-11-21 at stations 10 and 12
        { {2020, 11, 21}, 16,  1, 10, 8 },
        { {2020, 11, 21}, 18, 39, 12, 14 },
    };
    int num_observations = 10;

    printf("Daily Averages:\n");
    print_daily_averages(num_observations, obs_array);

    printf("Station Extremes:\n");
    print_station_extremes(num_observations, obs_array);
    return 0;
}