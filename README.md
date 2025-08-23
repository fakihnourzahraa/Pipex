# Pipex
# Process

-> Parse input
    -> if (!= 5) exit
    -> 4 strings, split 2 and 3
-> Command not found
-> Check for permissions
-> Create pipe
-> Create child process/command
-> Check path
    -> return error if command not found
-> Wait for all processes to end
-> Write to outfile
-> Makefile

Error Codes to Return
Match shell behavior:

Command not found: exit code 127
File not accessible: exit code 1
Permission denied: exit code 126
# Issues ?
-> pipe is full ?
-> make sure to close fd
-> EOF
-> check if envp is null before using it
# Allowed Functions 

access()
int access(const char *pathname, int mode);
access() checks whether the program can access the file pathname. 

The mode specifies the accessibility check(s) to be performed, and is either the value F_OK, or a mask consisting of the bitwise OR of one or more of R_OK, W_OK, and X_OK. F_OK tests for the existence of the file. R_OK, W_OK, and X_OK test whether the file exists and grants read, write, and execute permissions, respectively.

On success (all requested permissions granted), zero is returned. On error (at least one bit in mode asked for a permission that is denied, or some other error occurred), -1 is returned, and errno is set appropriately.

You can find more information on access() here: https://linux.die.net/man/2/access

dup2()
int dup2(int oldfd, int newfd);
dup2() makes newfd be the copy of oldfd, closing newfd first if necessary, but note the following:

If oldfd is not a valid file descriptor, then the call fails, and newfd is not closed.

If oldfd is a valid file descriptor, and newfd has the same value as oldfd, then dup2() does nothing, and returns newfd.

After a successful return from dup2(), the old and new file descriptor may be used interchangeably. They refer to the same open file description and thus share file offset and file status flags; for example, if the file offset is modified by using lseek() on one of the descriptors, the offset is also changed for the other.

On error, the dup2() function returns -1.

You can find more information on dup2() here: https://linux.die.net/man/2/dup2

pipe()
int pipe(int pipefd[2]);
pipe() creates a pipe, a unidirectional data channel that can be used for interprocess communication. The array pipefd is used to return two file descriptors referring to the ends of the pipe. pipefd[0] refers to the read end of the pipe. pipefd[1] refers to the write end of the pipe. Data written to the write end of the pipe is buffered by the kernel until it is read from the read end of the pipe

On success, 0 is returned. On error, -1 is returned, and errno is set appropriately.

fork()
pid_t fork(void);
fork() creates a new process by duplicating the calling process. The new process, referred to as the child, is an exact duplicate of the calling process, referred to as the parent, except for some points that you can find here (there's too may to write them all down here).

waitpid()
pid_t waitpid(pid_t pid, int *status, int options);
The waitpid() system call suspends execution of the calling process until a child specified by pid argument has changed state. By default, waitpid() waits only for terminated children.

You can find more information about wait() and waitpid() here.

wait()
pid_t wait(int *status);
The wait() system call suspends execution of the calling process until one of its children terminates.

You can find more information about wait() and waitpid() here.

execve()
int execve(const char *filename, char *const argv[], char *const envp[]);
execve() executes the program pointed to by filename.

execve() does not return on succes, the calling process is replaced by the executed filename.

unlink()
int unlink(const char *pathname);
unlink() deletes a name from the file system. If that name was the last link to a file and no processes have the file open the file is deleted and the space it was using is made available for reuse.

If the name was the last link to a file but any processes still have the file open the file will remain in existence until the last file descriptor referring to it is closed.

On success, 0 is returned. On error, -1 is returned, and errno is set appropriately.

You can find more information about unlink() here.