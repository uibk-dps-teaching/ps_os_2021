#include "scheduling_sim.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "my_array.h"

struct scheduler_context_t {
	int timestep;
	my_array* processes;
	process_t* scheduled_proc;
	process_t* move_to_end_proc;
};

static void reset_processes(simulation_data_t* data) {
	assert(data != NULL);
	for(int i = 0; i < data->num_processes; ++i) {
		process_t* cur = data->procs + i;
		*(int*)(&cur->remaining_time) = cur->service_time;
		cur->user1 = 0;
		cur->user2 = 0;
		cur->user3 = 0;
	}
}

size_t get_num_processes(const scheduler_context_t* ctx) {
	return my_array_size(ctx->processes);
}

process_t* get_process(const scheduler_context_t* ctx, size_t idx) {
	return my_array_get(ctx->processes, idx);
}

int get_timestep(const scheduler_context_t* ctx) {
	return ctx->timestep;
}

void schedule_process(scheduler_context_t* ctx, process_t* process) {
	if(ctx->scheduled_proc != NULL) {
		fprintf(stderr, "Only one process can be scheduled per timestep.\n");
		abort();
	}
	ctx->scheduled_proc = process;
}

void move_process_to_end(scheduler_context_t* ctx, process_t* process) {
	if(ctx->move_to_end_proc != NULL) {
		fprintf(stderr, "Only one process can be moved per timestep.\n");
		abort();
	}
	ctx->move_to_end_proc = process;
}

schedule_t compute_schedule_impl(simulation_data_t* data, scheduling_function sched_fn,
                                 const char* scheduler_name) {
	assert(data != NULL);
	assert(sched_fn != NULL);
	assert(scheduler_name != NULL);

	reset_processes(data);
	schedule_t sched;
	int timestep = 0;

	my_array* active_procs = my_array_init();
	process_t* move_to_end_proc = NULL;

	process_t* current = NULL;
	for(; timestep < MAX_TIMESTEPS - 1; ++timestep) {
		for(size_t i = 0; i < my_array_size(active_procs); ++i) {
			process_t* p = my_array_get(active_procs, i);
			if(p->remaining_time == 0) {
				my_array_delete(active_procs, i--);
				if(move_to_end_proc == p) {
					move_to_end_proc = NULL;
				}
			}
		}
		for(int i = 0; i < data->num_processes; ++i) {
			process_t* p = data->procs + i;
			if(timestep == p->arrival_time) {
				my_array_push_back(active_procs, p);
			}
		}
		if(move_to_end_proc != NULL) {
			for(size_t i = 0; i < my_array_size(active_procs); ++i) {
				process_t* p = my_array_get(active_procs, i);
				if(p == move_to_end_proc) {
					my_array_delete(active_procs, i);
					my_array_push_back(active_procs, move_to_end_proc);
					move_to_end_proc = NULL;
					break;
				}
			}
			assert(move_to_end_proc == NULL);
		}

		// If there's nothing left to schedule we're done
		if(my_array_size(active_procs) == 0) {
			break;
		}

		scheduler_context_t ctx = { .timestep = timestep,
			                        .processes = active_procs,
			                        .scheduled_proc = NULL,
			                        .move_to_end_proc = NULL };

		sched_fn(&ctx);

		if(ctx.scheduled_proc == NULL) {
			fprintf(stderr, "Exactly one process has to be scheduled per timestep.\n");
			abort();
		}

		current = ctx.scheduled_proc;
		move_to_end_proc = ctx.move_to_end_proc;

		int* rem_time = (int*)(&current->remaining_time);
		(*rem_time)--;

		// Record the schedule
		sched.proc_names[timestep] = current->name;
	}
	assert(my_array_size(active_procs) == 0);

	sched.num_timesteps = timestep;
	sched.proc_names[timestep] = '\0';
	sched.name = scheduler_name;

	my_array_free(active_procs);
	return sched;
}
