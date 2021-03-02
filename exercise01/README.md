# Exercise Sheet 1

In this exercise you will be asked to work with a shell. The idea is to get
familiar with some shell commands and bash programming.

## Task 1

Consider the following scripts. Explain what each line means.

### Script 1

Explain what happens when you execute this script with and without input
arguments.

```bash
#!/bin/bash
for FN in "$@"
do
    chmod 0750 "$FN"
done
```

### Script 2

Explain what happens when you execute this script with and without input
arguments.
Update the message printed in `usage` to be more helpful.

```bash
#!/bin/bash
function usage {
    echo "Usage: $0 <TODO: fill me in>"
    exit 1
}

ARG1="$1"; shift || usage
ARG2="$1"; shift || usage
ARG3="$1"; shift || usage

grep -n "$ARG3" "$ARG1" > "$ARG2"
```

Hint: `shift` is a shell _built-in_, use `help shift` to get more information.

### Script 3

Explain what happens when you execute this script with and without input
arguments.
Is it working as intended?

```bash
#!/bin/bash
makedir "$1" 2> /dev/null
```

## Task 2

What does this shell script do? Improve its usability by adding error
handling for the following cases:

- Print a help message when the number of provided arguments is not two.
- Log an error message to `stderr` if the file `$OUTFILE` is not writable.
- Think about what else might go wrong with this script.
  Find at least one other edge case and log an appropriate error message.

```bash
#!/bin/bash
INFILE="$1"
OUTFILE="$2"
if [ -e "$INFILE" ]
then
    cat "$INFILE" >> "$OUTFILE"
fi
```

Hint: Take a look at the man page for the "test" command: `man "["` or `man test`.

## Task 3

Write a script **my_backup.sh** to perform a backup of the current directory
(containing only files) into a given folder. The script receives a single
parameter which is the path of the folder where the backup is to be stored.

The script will be invoked as follows:

```bash
bash my_backup.sh /path/to/backup-folder # creates a backup of . into backup-folder
```

If the backup folder already exists, check for each file whether it has
already been backed up. If so, back up a copy of the file by appending a
timestamp to its name. For example, a file `file1` could become
`file1-1614624210`. You can use `date +%s` to get the current time as a UNIX
timestamp.
