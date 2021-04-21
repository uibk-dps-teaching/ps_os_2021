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
	// print_schedule(stdout, compute_schedule(data, srtf), data);
	// print_schedule(stdout, compute_schedule(data, round_robin_q4), data);

	// Task 2
	// print_schedule(stdout, compute_schedule(data, hpf_p), data);
	// print_schedule(stdout, compute_schedule(data, hpf_np), data);

	// Task 3
	// print_schedule(stdout, compute_schedule(data, my_scheduler), data);

	// -----------------------------------------------------------------------

	free_simulation_data(data);

	return EXIT_SUCCESS;
}
