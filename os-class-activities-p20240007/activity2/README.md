# Class Activity 2 — Processes & Inter-Process Communication

- **Student Name:** Chheng Kimter
- **Student ID:** p20240007
- **Date:** 5, April, 2026

---

## Task 1: Process Creation on Linux (fork + exec)

### Compilation & Execution

Screenshot of compiling and running `forkchild.c`:

![Task 1 - Compile & Run](screenshots/task1_compile_run.png)

### Process Tree

Screenshot of the parent-child process tree (using `ps --forest`, `pstree`, or `htop` tree view):

![Task 1 - Process Tree](screenshots/task1_process_tree.png)

### Output

```
total 32
drwxrwxr-x 2 rize rize  4096 Apr  2 18:50 .
drwxrwxr-x 7 rize rize  4096 Apr  2 18:47 ..
-rwxrwxr-x 1 rize rize 16400 Apr  2 18:50 forkchild
-rw-rw-r-- 1 rize rize  1431 Apr  2 18:48 forkchild.c
-rw-rw-r-- 1 rize rize     0 Apr  2 18:50 result_forkchild.txt
Parent process (PID: 8850) — creating child...
Parent: waiting for child (PID: 8851) to finish...
Parent: child exited with status 0
Parent: done.

```

### Questions

1. **What does `fork()` return to the parent? What does it return to the child?**

   > fork() returns the child's PID to the parent so the parent knows which process it just created. For the child it returns 0, which is how the child knows it's the child. If something goes wrong it returns -1 but that usually means you're out of memory or hit some system limit.

2. **What happens if you remove the `waitpid()` call? Why might the output look different?**

   > If you remove waitpid() the parent just keeps going and might finish before the child does. The child becomes a zombie process for a bit since the parent isn't around to collect its exit status. The output might look jumbled because both processes are printing at the same time with no coordination. The shell prompt might also appear before the child finishes printing.

3. **What does `execlp()` do? Why don't we see "execlp failed" when it succeeds?**

   > execlp() replaces the current process with a new program — in this case ls. Once it succeeds, the rest of the code in the child just doesn't exist anymore because the entire process image gets replaced. So the perror("execlp") line never runs because by the time execlp works, there's no code left to run after it. We only see that error line if execlp actually fails.

4. **Draw the process tree for your program (parent → child). Include PIDs from your output.**

   > forkchild (PID: 8850)
        └── forkchild → ls -la (PID: 8851)

5. **Which command did you use to view the process tree (`ps --forest`, `pstree`, or `htop`)? What information does each column show?**

   > I used ps -ef --forest. The columns show: UID (who owns the process), PID (the process ID), PPID (parent's PID), C (CPU usage), STIME (when it started), TTY (which terminal), TIME (CPU time used), and CMD (the command). The --forest part is what draws the tree with indentation so you can see which process spawned which.

---

## Task 2: Process Creation on Windows

### Compilation & Execution

Screenshot of compiling and running `winprocess.c`:

![Task 2 - Compile & Run](screenshots/task2_compile_run.png)

### Task Manager Screenshots

Screenshot showing process tree in the **Processes** tab (mspaint nested under your program):

![Task 2 - Task Manager Tree](screenshots/task2_taskmanager_tree.png)

Screenshot showing PID and Parent PID in the **Details** tab:

![Task 2 - Task Manager Details](screenshots/task2_taskmanager_details.png)

### Questions

1. **What is the key difference between how Linux creates a process (`fork` + `exec`) and how Windows does it (`CreateProcess`)?**

   > On Linux you do it in two steps — fork makes a copy of the current process first, then exec swaps in the new program. Windows combines it into one call with CreateProcess. You just tell it which program to run and it handles everything. There's no intermediate "copy of the parent" phase on Windows.

2. **What does `WaitForSingleObject()` do? What is its Linux equivalent?**

   > It blocks the parent process until the child process finishes. The equivalent on Linux is waitpid(). Both basically do the same thing — make the parent wait instead of just running ahead and exiting.

3. **Why do we need to call `CloseHandle()` at the end? What happens if we don't?**

   > Windows uses handles to track resources like processes and threads. If you don't close them the OS still thinks something is using that resource and won't free it until the whole program exits. It's basically a resource leak. In a small program like this it doesn't really matter but in a long-running program it would eventually cause problems.

4. **In Task Manager, what was the PID of your parent program and the PID of mspaint? Do they match your program's output?**

   > My parent process (winprocess.exe) had PID 5628 and the child process (Notepad) had PID 27616 with Thread ID 9768. These matched exactly what showed up in Task Manager when I checked. The program printed both PIDs in the terminal so it was easy to verify. I used Notepad instead of mspaint since it's easier to find on my system but the behavior is the same — CreateProcess() launches it as a child either way.

5. **Compare the Processes tab (tree view) and the Details tab (PID/PPID columns). Which view makes it easier to understand the parent-child relationship? Why?**

   > The Processes tab tree view is easier to understand at a glance because you can literally see mspaint indented under the parent program. But the Details tab with the PID/PPID columns is more precise — you can actually verify the numbers match what the program printed. For learning I'd say the tree view is better, but for debugging the Details tab is more useful.

---

## Task 3: Shared Memory IPC

### Compilation & Execution

Screenshot of compiling and running `shm-producer` and `shm-consumer`:

![Task 3 - Compile & Run](screenshots/task3_compile_run.png)

### Output

```
Consumer: reading from shared memory 'OS-chhengkimter'
Consumer: message = "Hello, this is shared memory IPC!"
Consumer: shared memory unlinked.

```

### Questions

1. **What does `shm_open()` do? How is it different from `open()`?**

   > shm_open() creates a shared memory object that lives in memory (usually under /dev/shm). Regular open() creates or opens a file on disk. The main difference is that shared memory is in RAM so it's much faster to access, and it's specifically designed so multiple processes can map the same region into their own address space. It still returns a file descriptor though, which is kind of interesting.

2. **What does `mmap()` do? Why is shared memory faster than other IPC methods?**

   > mmap() maps the shared memory object into the process's virtual address space so you can read and write it like a regular pointer. It's fast because there's no copying — both processes literally access the same physical memory. With pipes or message queues the kernel has to copy data from one process to another. With shared memory the data just sits there and both processes can reach it directly.
3. **Why must the shared memory name match between producer and consumer?**

   > Because that's how they find each other. The name is like the address of the shared memory region in the OS. If the names don't match the consumer just opens a different (probably nonexistent) region. It's the same idea as using the same filename when two programs need to open the same file.

4. **What does `shm_unlink()` do? What would happen if the consumer didn't call it?**

   > shm_unlink() removes the shared memory object from the system. If nobody calls it the shared memory stays around even after both programs exit. You can check with ls /dev/shm and see it still there. Eventually it wastes memory, and if you run the producer again with the same name it might conflict with the leftover object.

5. **If the consumer runs before the producer, what happens? Try it and describe the error.**

   > You get an error like shm_open: No such file or directory. The consumer tries to open a shared memory object that doesn't exist yet. I tried it and got that exact error plus the hint message we added. The consumer just exits immediately with a failure code.

---

## Task 4: Message Queue IPC

### Compilation & Execution

Screenshot of compiling and running `sender` and `receiver`:

![Task 4 - Compile & Run](screenshots/task4_compile_run.png)

### Output

```
Receiver: message received from queue '/queue-chhengkimter'
Receiver: message = "Hello from sender! This is message queue IPC."
Receiver: queue unlinked.

```

### Questions

1. **How is a message queue different from shared memory? When would you use one over the other?**

   > Shared memory is like a shared whiteboard — both processes can read and write anywhere on it, and you have to manage synchronization yourself. A message queue is more like passing notes — you send a message and the other process receives it as a complete unit in order. Message queues are better when you want structured communication with clear boundaries between messages, or when you don't want to deal with synchronization. Shared memory is better when you need raw speed and are moving a lot of data.

2. **Why does the queue name in `common.h` need to start with `/`?**

   > It's a POSIX naming convention. The kernel uses the name to create an entry in a virtual filesystem (usually /dev/mqueue). Without the slash the name isn't a valid POSIX message queue name and mq_open will just fail. It's similar to how shared memory names also start with /.

3. **What does `mq_unlink()` do? What happens if neither the sender nor receiver calls it?**

   > mq_unlink() removes the queue from the system. If nobody unlinks it the queue stays alive even after both programs exit. You can actually see it by looking in /dev/mqueue. If you run the sender again it'll just reuse the same queue which might have leftover messages in it, which could confuse the receiver. It's good practice to always clean up after yourself.

4. **What happens if you run the receiver before the sender?**

   > The receiver tries to open a queue that doesn't exist yet and fails with No such file or directory, plus the hint message. It exits immediately without receiving anything. The sender needs to run first to create the queue.

5. **Can multiple senders send to the same queue? Can multiple receivers read from the same queue?**

   > Yes to both, actually. Multiple senders can all push messages into the same queue and they'll stack up in order. Multiple receivers can also read from the same queue but each message only gets delivered to one receiver — whoever calls mq_receive first gets it. So if you have two receivers they'd split the messages between them, not both get every message. This makes it useful for work-queue style setups where you want to spread tasks across multiple workers.

---

## Reflection

What did you learn from this activity? What was the most interesting difference between Linux and Windows process creation? Which IPC method do you prefer and why?

> Honestly the most surprising thing was how different Linux and Windows handle creating processes. On Linux you basically clone yourself first with fork() then swap in the new program with exec(), which felt weird at first. Windows just does it in one step with CreateProcess() which makes more sense to me logically.
For the IPC stuff, I think message queues are easier to work with because you just send and receive messages without worrying about anything else. Shared memory feels more complicated since you have to be careful about how both processes access it. But I get why shared memory is faster, there's no copying involved, both processes just point to the same memory.
Overall this activity made the lecture content a lot clearer. Reading about fork() is one thing but actually seeing the parent and child processes show up in ps --forest with their PIDs made it click.