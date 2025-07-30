# minitrace

lightweight syscall tracer for linux processes, focused on file descriptors and memory allocation.

Note: I'll be writing up a blog documenting my motivation which I'll link here later.

---

## features

* attaches to a live process by PID
* traces syscall entry/exit using `ptrace`
* prints syscall name, arguments, and return value
* highlights:

  * `open`, `openat`, `close`
  * `mmap`, `munmap`, `brk`
  * `read`, `write`, `dup`, `pipe`, etc

---

## usage

```sh
$ ./minitrace <pid>
```

prints:

```text
>>> syscall: open    (2)
             rdi = 0x000055...      (filename ptr)
             rsi = 0x000000...      (flags)
             rdx = 0x000000...
return: 0x3                         ← new fd
```

---

## use case

use `minitrace` to:

* debug resource leaks (open FDs not closed)
* observe `mmap`/`brk`-based allocations
* track file and pipe usage across your program
* visualize system-level behavior
---

## build

```sh
gcc -Wall -o minitrace minitrace.c
```

---

## notes

* x86\_64 only
* run as root or ensure target process is traceable

---

## todo

* track FD table live (show open/close lifecycle)
* show filename strings for `openat`
* optional filters: `--fd`, `--mmap`, `--write`
* log syscall timing (cost)

---

## references

* `man 2 ptrace`
* `man 2 syscalls`
* linux source: `arch/x86/entry/syscalls/syscall_64.tbl`

