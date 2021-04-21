#include "scheduling_utility.h"
#include "scheduling_sim.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN_SCHEDULING_FUN 10

static int match_lines(FILE* fp, simulation_data_t* data) {
	assert(fp);
	assert(!data || (data && data->procs));

	int line = 0;
	process_t tmp = { 0 };

	while(fscanf(fp, "%c;%d;%d;%d\n", (char*)&tmp.name, (int*)&tmp.arrival_time,
	             (int*)&tmp.service_time, (int*)&tmp.priority) == 4) {
		if(data) {
			memcpy(data->procs + line, &tmp, sizeof(tmp));
		}
		++line;
	}
	rewind(fp);
	return line;
}

static simulation_data_t* allocate_simulation_data(size_t num_procs) {
	simulation_data_t* data = malloc(sizeof(*data));
	if(data == NULL) {
		return NULL;
	}
	data->num_processes = num_procs;
	data->procs = malloc(sizeof(*data->procs) * data->num_processes);
	if(data->procs == NULL) {
		free(data);
		return NULL;
	}
	return data;
}

simulation_data_t* read_simulation_data(const char* input_file) {
	assert(input_file);
	FILE* fp = fopen(input_file, "r");
	if(!fp) {
		return NULL;
	}

	simulation_data_t* data = allocate_simulation_data(match_lines(fp, NULL));
	if(data == NULL) {
		fclose(fp);
		return NULL;
	}

	match_lines(fp, data);
	fclose(fp);
	return data;
}

void print_simulation_data(FILE* out, const simulation_data_t* data) {
	fprintf(out, "Simulation Data:\n");
	fprintf(out, "name\tarrival_time\tservice_time\tpriority\n");
	for(int i = 0; i < data->num_processes; ++i) {
		process_t proc = data->procs[i];
		printf("%c    \t%12u\t%12u\t%8u\n", proc.name, proc.arrival_time, proc.service_time,
		       proc.priority);
	}
	fprintf(out, "\n");
}

void print_schedule(FILE* out, schedule_t sched, const simulation_data_t* data) {
	int offset_len = strlen(sched.name) / 2;
	fprintf(out, "===================================");
	fprintf(out, "%*s%*s", MAX_LEN_SCHEDULING_FUN + offset_len, sched.name,
	        MAX_LEN_SCHEDULING_FUN - offset_len, "");
	fprintf(out, "===================================\n\n");

	// header
	fprintf(out, "  ");
	for(int i = 0; i < sched.num_timesteps; i++) {
		fprintf(out, " %2d ", i);
	}
	fprintf(out, "\n");

	int start_time[256];
	int end_time[256] = { 0 };

	for(int i = 0; i < 256; ++i) {
		start_time[i] = sched.num_timesteps;
	}

	for(int i = 0; i < sched.num_timesteps; i++) {
		int cur = (int)sched.proc_names[i];
		if(i < start_time[cur]) {
			start_time[cur] = i;
		}
		if(i > end_time[cur]) {
			end_time[cur] = i;
		}
	}

	const int is_terminal = isatty(fileno(out));
	const char* ansi_green = is_terminal ? "\033[92m" : "";
	const char* ansi_red = is_terminal ? "\033[91m" : "";
	const char* ansi_reset = is_terminal ? "\033[0m" : "";

	// schedule
	for(int i = 0; i < data->num_processes; ++i) {
		const process_t* proc = &data->procs[i];
		fprintf(out, "%c:", proc->name);
		for(int t = 0; t < sched.num_timesteps; t++) {
			const bool is_arriving = t == proc->arrival_time;
			const bool is_last_t = t == end_time[(int)proc->name];
			const char* color = is_arriving ? ansi_green : (is_last_t ? ansi_red : "");
			const char symbol = sched.proc_names[t] == proc->name ? 'x' : (is_arriving ? '*' : ' ');
			fprintf(out, "  %s%c%s ", color, symbol, ansi_reset);
		}
		putc('\n', out);
	}
	fprintf(out, "\n\n");

	int response_time_sum = 0;
	int turnaround_time_sum = 0;
	fprintf(out, "name\tresponse_time\tturnaround_time\n");
	for(int i = 0; i < data->num_processes; ++i) {
		const int arrival_time = data->procs[i].arrival_time;
		const int name = data->procs[i].name;
		const int rt = start_time[name] - arrival_time;
		const int tt = end_time[name] - arrival_time + 1;
		response_time_sum += rt;
		turnaround_time_sum += tt;
		fprintf(out, "%c   \t%13d\t%16d\n", name, rt, tt);
	}

	fprintf(out, "AVG \t%13.2f\t%16.2f\n", response_time_sum / (float)data->num_processes,
	        turnaround_time_sum / (float)data->num_processes);

	fprintf(out, "\n\n\n");
}

void free_simulation_data(simulation_data_t* data) {
	assert(data);
	assert(data->procs);
	free(data->procs);
	free(data);
}
