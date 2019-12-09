# Solve the traffic Deadlock problem
## Introduction
This is the project 1-1 of my OS course at ZJU.

### Features
- Deaclock free
- Hungry free

## How to use this program
This program is tested on Ubuntu 18.04 LTS.

### Compile & Link
```console
# clear all the compiled files
$ make clear

# start new compilation
$ make

# compile with multiple cores
$ make -j4
```

### Run the program
This program takes one string argument, each character in the string represents a direction.
- 'n': north
- 'e': east
- 's': south
- 'w': west


```console
$ ./p1-1 nsewwewn
```

## Debug with GDB
After the compilation,
```console
$ gdb p1-1
(gdb) set pagination off
(gdb) set target-async on
(gdb) set non-stop on
(gdb) breakpoint main         # set breakpoints as you like
(gdb) run <argument>
```

#### Single Threaded
To go to the next instruction
```console
(gdb) continue
```

To step into an instruction
```console
(gdb) step
```

To debug with an interactive Windows
```console
(gdb) layout next
```
To refresh the buggy debug window,

```console
(gdb) refresh
```

To set a watch on a variable,

```console
(gdb) watch x
```

#### Multi-threaded
To see the info of all the threads,
```console
(gdb) info threads
```

To make the program continue asynchronously,
```console
(gdb) continue &
```

To apply commnds to several threads simultaneously,
```console
(gdb) thread apply <thread number> <command>
```
As an example
```console
(gdb) thread apply 5 7 9 backtrace
(gdb) thread apply 1 2 3 4 continue&
```

