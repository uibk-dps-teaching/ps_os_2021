#ifndef SCHEDULING_SIM_H
#define SCHEDULING_SIM_H

#include <stdbool.h>
#include <stdlib.h>

#include "my_array.h"

#define MAX_TIMESTEPS 1000
typedef struct {
	const char name;
	const int arrival_time;
	const int service_time;
	const int priority;
	const int remaining_time;

	int user1;
	int user2;
	int user3;
} process_t;

typedef struct {
	int num_processes;
	process_t* procs;
} simulation_data_t;

typedef struct {
	int num_timesteps;
	char proc_names[MAX_TIMESTEPS];
	const char* name;
} schedule_t;

struct scheduler_context_t;
typedef struct scheduler_context_t scheduler_context_t;

size_t get_num_processes(const scheduler_context_t* ctx);

process_t* get_process(const scheduler_context_t* ctx, size_t idx);

int get_timestep(const scheduler_context_t* ctx);

void schedule_process(scheduler_context_t* ctx, process_t* process);

void move_process_to_end(scheduler_context_t* ctx, process_t* process);

typedef void (*scheduling_function)(scheduler_context_t* ctx);

#define compute_schedule(procs, sched) compute_schedule_impl(procs, sched, #sched)

schedule_t compute_schedule_impl(simulation_data_t* data, scheduling_function scheduler,
                                 const char* scheduler_name);

#endif // !SCHEDULING_SIM_H
