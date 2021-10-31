# Daemon:
A **daemon** is a process that is **not attached to any terminal** which allows it to **run in the background** and execute a set of pre-defined instructions.
A daemon's parent process is init (which has the pid 1).

# How to create a daemon: 
To have the program's parent process be *init* (pid 1), the program must **fork** and the child process must let the **parent process terminate** so that it becomes orphan and the system makes *init* its parent.

Links:

- https://github.com/pasce/daemon-skeleton-linux-c
- https://www.thegeekstuff.com/2012/02/c-daemon-process/