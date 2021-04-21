#!/bin/bash

# This is a small convenience script to check the output of tasks 1 & 2, and to
# run a memory debugger (Valgrind or `leaks` on OSX) on the test executable.

function usage {
    echo "Usage: $0 <task executable>" 1>&2
    exit 1
}

EXE=$1; shift || usage

# First check whether the tests pass at all
"./$EXE" 1> /dev/null
TESTS_RET=$?

# If tests passed, compare outputs and run memory debugger
if [ "$TESTS_RET" -eq 0 ]; then
    "./$EXE" | diff "bstree_${EXE}_output.txt" -
    DIFF_RET=$?

    if [ $(uname -s) == "Darwin" ]; then
        MallocStackLoggingNoCompact=1 leaks --atExit -- "./$EXE"
    else
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 -- "./$EXE"
    fi
fi
MEM_RET=$?

echo -e "\nResults:";

if [ "$TESTS_RET" -ne 0 ]; then
    echo "❌ $EXE did not pass all tests."
else
    echo "✔ $EXE passed all tests."
fi

if [ "$TESTS_RET" -eq 0 ]; then
    if [ "$DIFF_RET" -ne 0 ]; then
        echo "❌ Output of $EXE did not match expected output."
    else
        echo "✔ Output of $EXE matches expected output."
    fi

    if [ "$MEM_RET" -ne 0 ]; then
        echo "❌ $EXE appears to have a memory-related bug."
    else
        echo "✔ $EXE does not have any memory-related bugs."
    fi
else
    echo "⚠ Fix tests to compare outputs and check for memory-related bugs."
fi
