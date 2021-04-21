#include <math.h>
#include <stdio.h>

#include "scheduling_sim.h"
#include "scheduling_utility.h"

// ---------------------------------------------------------------------------
// Example schedulers
// ---------------------------------------------------------------------------

void fcfs(scheduler_context_t* ctx) {
	process_t* selected = get_process(ctx, 0);
	schedule_process(ctx, selected);
}

void round_robin(scheduler_context_t* ctx) {
	process_t* selected = get_process(ctx, 0);
	schedule_process(ctx, selected);
	move_process_to_end(ctx, selected);
}

// ---------------------------------------------------------------------------
// Implement your schedulers here
// ---------------------------------------------------------------------------


void srtf(scheduler_context_t *ctx) {
    int arr_size = (int) get_num_processes(ctx);
    process_t *selected = get_process(ctx, 0);
    for (int i = 1; i < arr_size; i++) {
        process_t *current = get_process(ctx, i);
        if (current->remaining_time < selected->remaining_time) {
            selected = current;
        }
    }
	schedule_process(ctx, selected);
}

void round_robin_q4(scheduler_context_t *ctx) {
    process_t *selected = get_process(ctx, 0);
    //using user variable 1 as "quantum counter"
    //since the user values are initialized with 0 -> max value is 3 for q4
    if(selected->user1 < 3) {
        selected->user1 += 1;
    } else {
        selected->user1 = 0;
		move_process_to_end(ctx, selected);
    }
	schedule_process(ctx, selected);
}

void hpf_p(scheduler_context_t *ctx) {
    int size = (int) get_num_processes(ctx);
    process_t *selected = get_process(ctx, 0);
    for (int i = 1; i < size; i++) {
        process_t *current = get_process(ctx, i);
        if (current->priority > selected->priority) {
            selected = current;
        }
    }
	schedule_process(ctx, selected);
}

void hpf_np(scheduler_context_t *ctx) {
    int arr_size = (int) get_num_processes(ctx);
    process_t *selected = get_process(ctx, 0);
    if (selected->user1) {
		schedule_process(ctx, selected);
    } else {
		for (int i = 1; i < arr_size; i++) {
        	process_t *current = get_process(ctx, i);
        	if (current->user1) {
				schedule_process(ctx, selected);
			} else if (current->priority > selected->priority) {
				selected = current;
			} 
		}
    }
    if(!selected->user1) {
		selected->user1 += 1;
	}
	schedule_process(ctx, selected);
}

// ---------------------------------------------------------------------------

int main(int argc, char** argv) {
	if(argc != 2) {
		fprintf(stderr, "Error: Usage: %s <filename>\nExample: %s input.csv\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	simulation_data_t* data = read_simulation_data(argv[1]);
	if(data == NULL) {
		fprintf(stderr, "Error: Unable to open the file '%s' for reading!\n", argv[1]);
		return EXIT_FAILURE;
	}
	print_simulation_data(stdout, data);

	// -----------------------------------------------------------------------
	// Uncomment these calls to your schedulers
	// -----------------------------------------------------------------------

	// Examples
	print_schedule(stdout, compute_schedule(data, fcfs), data);
	print_schedule(stdout, compute_schedule(data, round_robin), data);

	// Task 1
	printf("Task 1: \n\n");
	print_schedule(stdout, compute_schedule(data, srtf), data);
	//print_schedule(stdout, compute_schedule(data, round_robin_q4), data);

	// Task 2
	printf("Task 2: \n\n");
	print_schedule(stdout, compute_schedule(data, hpf_p), data);
	print_schedule(stdout, compute_schedule(data, hpf_np), data);

	// Task 3
	// print_schedule(stdout, compute_schedule(data, my_scheduler), data);

	// -----------------------------------------------------------------------

	free_simulation_data(data);

	return EXIT_SUCCESS;
}
