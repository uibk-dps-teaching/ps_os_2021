# Exercise Sheet 11

In this exercise sheet you will explore different ways of linking and loading
program libraries.

# Task 1

Alongside this exercise sheet, you will find two binary files: `some_program`
and `some_library.so`, the former being an executable that should run on most
`x86_64` Linux machines (and in particular is guaranteed to run on ZID-GPL), while
the latter is a _shared library_ used by the executable.
You can try running the executable like so:

```bash
LD_LIBRARY_PATH=. ./some_program
```

Unfortunately, the library contains a bug and thus does not behave as expected.
While the library implementation is available in
[some_library.c](some_library.c), the name of the function got corrupted.

- Use the `nm` utility to find the name of the buggy library function (its name
  will also tell you its intended behavior).
- Read up on how GCC can be used to compile shared libraries (also called
  _shared objects_).
- Fix the bug in the library function.
- Write a Makefile to compile the revised version of `some_library.so`.
- Verify that `some_program` now behaves as expected.
- Is `some_library.so` the only shared library used by `some_program`? Use the
  `ldd` utility to check.

Then:

- Re-implement the functionality of `some_program` and `some_library.so` in a
  single program `my_program`.
- Extend your Makefile to compile this program as a fully _static_ executable
  (read up on how this can be done with GCC).
- Compare the sizes of `some_program` and `my_program` - what do you find?

Equipped with this knowledge, answer the following questions (provide your
answers as a `*.txt` or `*.md` file):

- What is the difference between dynamic and static linking?
- When would you use dynamic linking?
- When would you use static linking?
- What is _position independent code_, and why is it useful?
- What is the purpose of the `LD_LIBRARY_PATH` environment variable?

**NOTE**: If you are hesitant to run an unknown binary (which you should be!),
you may first want inspect its contents to make sure no funny business is
going on, for example like so:

```bash
objdump --disassemble some_program --section=.text
```

# Task 2

While dynamic linking is usually handled automatically by the operating system
upon program execution, in some situations it can be useful to do it manually.
In particular, _plugin systems_ are often implemented in this way.

Write a program `number_fun` that receives as an argument a single number as
well as an arbitrary list of shared libraries that act as _plugins_ for the
program. The program then applies each plugin to the number and prints the
resulting number for each plugin.

Read the man pages for `dlopen` and `dlsym` to find out how to obtain a function
pointer from a shared object.

Then implement the following plugins:

- `double.so` which multiplies the number by 2.
- `square.so` which computes the square of the number.
- `negate.so` which negates the number.
- One additional plugin with a functionality of your own choosing.

Usage example:

```
./number_fun 3 ./double.so ./square.so ./negate.so
./double.so: 6
./square.so: 9
./negate.so: -3
```

# Task 3

The dynamic linker allows to _inject_ arbitrary shared objects into any
dynamically linked program's address space using the `LD_PRELOAD` environment
variable.

Use this mechanism to _wrap_ the standard library's `malloc` function in your
own function, inside a shared object called `malloc_spy.so`, which then prints
`"allocating <size> byte"` before each allocation. Make sure that your wrapper
function still calls `malloc` internally and returns the expected result. Read
up on how you can obtain the original function pointer for a wrapped function
using `dlsym`.

Try your wrapper function on an existing executable such as `ls`.

**Important**: Don't use any of the `printf`-family of functions for printing
your message, as these use `malloc` internally. Instead, only use `write`. You
can use the following function for printing the byte size:

```c
void print_number(size_t number) {
	if(number > 9) {
		print_number(number / 10);
	}
	const char digit = '0' + number % 10;
	write(STDOUT_FILENO, &digit, 1);
}
```
