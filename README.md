# UIBK Operating Systems Lab 2021

This repository contains material required to complete exercises for the OS
lab in the 2021 summer semester, including exercise sheets and their
associated materials. Note that some of this material is based on previous
years.

**Make sure to read this document carefully, as it contains vital information
regarding the grading of exercises.**

## Schedule

This schedule gives you an overview of the topics we will discuss on each
date. It will be updated continuously throughout the semester. Note that
exercises associated with a given date are to be discussed on that day, i.e.,
solutions for that exercise have to be handed in via OLAT by 17:00 on the
previous day (Tuesdays).

| Date       | Exc              | Topic                         | Notes                                 |
| ---------- | ---------------- | ----------------------------- | ------------------------------------- |
| 2021-03-03 | -                |                               | Introduction to UNIX, shell scripting |
| 2021-03-10 | [1](exercise01)  | Shell scripting               | C Refresher                           |
| 2021-03-17 | [2](exercise02)  | Binary Search Tree            |                                       |
| 2021-03-24 | [3](exercise03)  | Processes, signal handling    |                                       |
| 2021-04-14 | [4](exercise04)  | FIFOs, pipes & sockets        |                                       |
| 2021-04-21 | [5](exercise05)  | Shared memory & semaphores    |                                       |
| 2021-04-28 | [6](exercise06)  | Scheduling simulator          |                                       |
| 2021-05-05 | [7](exercise07)  | Threads, mutexes & deadlocks  |                                       |
| 2021-05-12 | [8](exercise08)  | Atomics & condition variables |                                       |
| 2021-05-19 | [9](exercise09)  | Threads advanced - Chat       |                                       |
| 2021-05-26 | [10](exercise10) | Memory management             |                                       |
| 2021-06-02 | [11](exercise11) | Linking                       |                                       |

## General Information

All programming exercise solutions you hand in **must** compile properly on
the [ZID-GPL server](https://www.uibk.ac.at/zid/systeme/linux/lpccs_4/benutzeranleitung_zid-gpl.html).
The easiest way of ensuring this is by developing on ZID-GPL directly, either
through a terminal or using an editor/IDE with support for remote development
via SSH (such as [Visual Studio
Code](https://code.visualstudio.com/docs/remote/ssh)).

Note that the default version of the GCC compiler present on ZID-GPL is
_extremely_ outdated. You may load a newer version by calling `module load gcc/9.2.0`.

### Connecting to ZID-GPL via SSH

**NOTE**: You have to be inside the University network to be able to connect
via SSH. See [here](https://www.uibk.ac.at/zid/netz-komm/vpn/) for more
information.

To log into ZID-GPL via SSH, use the following command on Linux/Mac OSX:

`ssh -l <username> zid-gpl.uibk.ac.at`

On Windows, you can use Powershell (Windows 10) or a third-party SSH client
such as [PuTTY](https://www.putty.org/).

### Coding Guidelines

All programming exercises are conducted in C.

Your code should always compile without warnings, when passing the flags `-Wall -Wextra -Werror -std=gnu11`.

Make sure your code is properly
formatted using either your IDE/Text editor of choice, or by using a tool
such as `clang-format`. You can find an example
[.clang-format](.clang-format) file in this repository. **Failure to
consistently format code may result in lower scores.**

Try to write _self-documenting code_ by choosing descriptive variable and
function names. While you may want to add comments to certain sections of
your code, try to avoid trivial comments such as `fopen(...); // open file`.
**All names and comments should be written in English**.

Finally, all submitted code must be accompanied by a GNU Makefile. That
is, your solution must be able to be compiled with a single call to `make`.
You can find an example Makefile and more information
[here](example_makefile).

For some more specific topics such as proper error handling, check out our
[Frequently Asked Questions (FAQ)](FAQ.md).
