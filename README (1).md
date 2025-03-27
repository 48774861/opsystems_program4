# Operating Systems: Programming 3
Name: Marc Pham  
CS5343 student must perform Program 1 and Program 2 only.
## Program 1
Program 1 asks us to pass two numbers to a thread T1 and have the thread return the sum to the parent process for printing. The program `program1.c` begins by defining a ThreadParams struct with two integers `m` and `n` that will be passed into the thread. Then, the program creates a thread T1 (called `a_thread` in the program) that adds the two integers `m` and `n`, returning the result to the `result`. The program then prints `m + n = result`, substituting the numbers for the variable names.  

Looking at the snapshot (program1_programming3.png), we see three different examples where `m` and `n` are given and the sum of the two is printed.
![plot](program1_programming3.png)
## Program 2
Program 2 asks us to create two threads T1 and T2, where T1 creates a file named Thread.txt and T2 writes "Hello its T2" into the file. The program `program2.c` begins by creating thread T1 (called `thread1` in the program), which opens Thread.txt with write-only and create access and stores the file's descriptor in `file` in the main process. Then, the program creates thread T2 (called `thread2` in the program), passing in the file descriptor stored in the `file` to the thread. Using the file descriptor, thread T2 writes "Hello its T2" to the file and outputs something depending on it the write was successful or not.  

Looking at the snapshot (program2_programming3.png), we see that running the program created the Thread.txt file. Using `cat` command to output the file's contents, we see that the program successfully wrote "Hello its T2" to the Thread.txt.
![plot](program2_programming3.png)
