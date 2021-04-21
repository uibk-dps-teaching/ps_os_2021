#ifndef SCHEDULING_UTILITY_H
#define SCHEDULING_UTILITY_H

#include "scheduling_sim.h"
#include <stdio.h>

simulation_data_t* read_simulation_data(const char* input_file);

void free_simulation_data(simulation_data_t* data);

void print_simulation_data(FILE* out, const simulation_data_t*);

void print_schedule(FILE* out, schedule_t schedule, const simulation_data_t* data);

#endif // !SCHEDULING_UTILITY_H
