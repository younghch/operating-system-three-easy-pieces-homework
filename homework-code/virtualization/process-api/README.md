# Questions
#### 1. Write a program that calls fork().Before calling fork(), have the main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?

Child and parent process have their own address space. Changing values inside one doesn't affect another.

#### 2. Write a program that opens a file (with the open() system call) and then calls fork() to create a new process. Can both the child and parent access the file descriptor returned by open()? What happens when they are writing to the file concurrently, i.e., at the same time?

Yes, both the child and parent can access the file descriptor. There's no problem when they are writing to the file concurrently, but order in undeterministic without wait().

#### 3. Write another program using fork(). The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling wait() in the parent?

wait() is necessary. We can try to use sleep or run large loop in the parent process hoping the OS to switch process, But it's not ensured.

#### 4. Write a program that calls fork() and then calls some form of exec() to run the program /bin/ls. See if you can try all of the variants of exec(), including (on Linux) execl(), execle(), execlp(), execv(), execvp(), and execvpe(). Why do you think there are so many variants of the same basic call?

l and v flags determines the way to pass the parameter. For l pass parameter indiviualy, for v as a array of char*. p flags pass PATH environment together, e flag pass additional envp parameter which specifies the environment for new process image.

#### 5. Now write a program that uses wait() to wait for the child process to finish in the parent. What does wait() return? What happens if you use wait() in the child?

wait() returns pid of child, -1 for error. 
    
#### 6. Write a slight modification of the previous program, this time using waitpid() instead of wait(). When would waitpid() be useful?

waitpid() would be useful when there are multiple child process.

7. Write a program that creates a child process, and then in the child closes standard output (STDOUT FILENO). What happens if the child calls printf() to print some output after closing the descriptor?

8. Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the pipe() system call.
