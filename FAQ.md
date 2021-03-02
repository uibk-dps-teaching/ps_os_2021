# Frequently Asked Questions

## Question 1 - Error handling

**Is it good practice to check all return values? Doesn't that make the code a little illegible? Is there a rule of thumb which return values should be ignored and which should be checked?**

In general, you are asked to deliver robust code. Consequently, errors should
be checked and handled appropriately. However, there are downsides to
excessive error handling. It might clutter the code and reduce its
readability. Especially in the context of the lab in which you are supposed
to learn new concepts, error handling may be obstructive if done
inappropriately. Therefore, in the following paragraphs, we distinguish
between real-world code and the lab situation.

### Real-world

As already briefly discussed, there are trade-offs to be considered. There is
no universal rule for appropriate error handling. The following aspects
should be considered when selecting a strategy for error handling.

- Application context: For example, the requirements regarding safety and
  reliability for avionics software and a simple video game you share with your
  friends are entirely different. In seldom cases, error handling might
  introduce too much overhead.
- Maintenance: Excessive error handling might clutter the code, decrease
  readability, introduce new errors, and increase maintenance cost and effort.
- Implementation costs/time: Focusing on every unlikely error case, the
  implementation of the system may exceed cost or time requirements.

### Lab

In the lab, you may ignore potential errors of certain non-essential
functions, such as `printf`. In general, check for errors at your own
discretion -- just be prepared to justify your decisions during lab
presentations! Macros could help to design very robust and yet readable code
(see the example below).

The following program reads lines from the file given in the first argument
and prints every N-th line to the file given in the second argument. The
third argument defines N. The code uses a macro to handle errors in a quick
and concise way, while ensuring proper cleanup of all resources, without
cluttering the code too much.

```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 64
#define PERROR fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, strerror(errno))
#define PERROR_GOTO(label) \
	do { \
		PERROR; \
		goto label; \
	} while(0)

int main(int argc, char** argv) {
	int status = EXIT_FAILURE;
	if(argc != 4) {
		fprintf(stderr, "usage: %s <in-file> <out-file> <N>\n", argv[0]);
		return EXIT_FAILURE;
	}
	long N = strtol(argv[3], NULL, 0);
	if(errno == EINVAL || errno == ERANGE) PERROR_GOTO(end);
	if(N < 1 || N > 100) {
		fprintf(stderr, "error N < 1 or N > 100\n");
		return EXIT_FAILURE;
	}
	FILE* in_fp = fopen(argv[1], "r");
	if(in_fp == NULL) PERROR_GOTO(end);
	FILE* out_fp = fopen(argv[2], "w");
	if(out_fp == NULL) PERROR_GOTO(close_in);
	int line_cnt = 0;
	char buff[BUFF_SIZE];
	while(fgets(buff, BUFF_SIZE, in_fp) != NULL) {
		if((++line_cnt % N) == 0) {
			fprintf(out_fp, "%s", buff);
		}
	}
	status = EXIT_SUCCESS;
	if(fclose(out_fp) == EOF) {
		PERROR;
		status = EXIT_FAILURE;
	}
close_in:
	if(fclose(in_fp) == EOF) {
		PERROR;
		status = EXIT_FAILURE;
	}
end:
	return status;
}
```
