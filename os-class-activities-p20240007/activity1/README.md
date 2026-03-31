# Class Activity 1 — System Calls in Practice

- **Student Name:** Chheng Kimter
- **Student ID:** p20240007
- **Date:** 31/3/2025

---

## Warm-Up: Hello System Call

Screenshot of running `hello_syscall.c` on Linux:

![Hello syscall](os-class-activities-p20240007/activity1/screenshots/hello_syscall.png)

Screenshot of running `hello_winapi.c` on Windows (CMD/PowerShell/VS Code):

![Hello WinAPI](os-class-activities-p20240007/activity1/screenshots/hello_winapi.png)

Screenshot of running `copyfilesyscall.c` on Linux:

![Copy file syscall](os-class-activities-p20240007/activity1/screenshots/copy_file_syscall.png)

---

## Task 1: File Creator & Reader

### Part A — File Creator

**Describe your implementation:** [What differences did you notice between the library version and the system call version?]

**Version A — Library Functions (`file_creator_lib.c`):**

<!-- Screenshot: gcc -o file_creator_lib file_creator_lib.c && ./file_creator_lib && cat output.txt -->
![Task 1A - Library](os-class-activities-p20240007/activity1/screenshots/Task1_A_A.png)

**Version B — POSIX System Calls (`file_creator_sys.c`):**

<!-- Screenshot: gcc -o file_creator_sys file_creator_sys.c && ./file_creator_sys && cat output.txt -->
![Task 1A - Syscall](os-class-activities-p20240007/activity1/screenshots/Task1_A_B.png)

**Questions:**

1. **What flags did you pass to `open()`? What does each flag mean?**

   > O_WRONLY | O_CREAT | O_TRUNC. O_WRONLY opens the file for writing only; O_CREAT creates the file if it doesn't exist; O_TRUNC erases the existing content of the file if it already exists.

2. **What is `0644`? What does each digit represent?**

   > It is an octal representation of file permissions. 6 (4+2) gives the Owner read/write access. The first 4 gives the Group read-only access. The second 4 gives Others read-only access.

3. **What does `fopen("output.txt", "w")` do internally that you had to do manually?**

   > It translates the "w" mode into the specific flags (O_WRONLY|O_CREAT|O_TRUNC), handles default permissions, and sets up a user-space buffer to make future writes more efficient.

### Part B — File Reader & Display

**Describe your implementation:** [Your notes]

**Version A — Library Functions (`file_reader_lib.c`):**

![Task 1B - Library](os-class-activities-p20240007/activity1/screenshots/Task1_B_A.png)

**Version B — POSIX System Calls (`file_reader_sys.c`):**

![Task 1B - Syscall](os-class-activities-p20240007/activity1/screenshots/Task1_B_B.png)

**Questions:**

1. **What does `read()` return? How is this different from `fgets()`?**

   > read() returns the number of bytes actually read (or 0 at EOF). fgets() returns a pointer to the string buffer and automatically adds a null terminator (\0), which read() does not do.

2. **Why do you need a loop when using `read()`? When does it stop?**

   > read() only fetches up to the size of the buffer provided. If a file is larger than the buffer, you must loop to get the rest. It stops when read() returns 0.

---

## Task 2: Directory Listing & File Info

**Describe your implementation:** [Your notes]

### Version A — Library Functions (`dir_list_lib.c`)

![Task 2 - Version A](os-class-activities-p20240007/activity1/screenshots/Task2_A.png)

### Version B — System Calls (`dir_list_sys.c`)

![Task 2 - Version B](os-class-activities-p20240007/activity1/screenshots/Task2_B.png)

### Questions

1. **What struct does `readdir()` return? What fields does it contain?**

   > It returns struct dirent. Key fields include d_name (the filename) and d_type (the type of file, like directory or regular file).

2. **What information does `stat()` provide beyond file size?**

   > It provides file permissions (mode), owner/group IDs, timestamps (access, modification, change), and the number of hard links.

3. **Why can't you `write()` a number directly — why do you need `snprintf()` first?**

   > write() sends raw bytes to the terminal. If you send an integer like 500, the terminal tries to interpret that binary value as an ASCII character (which usually looks like garbage). snprintf() converts the number into a string of characters (like '5', '0', '0') that humans can read.

---

## Optional Bonus: Windows API (`file_creator_win.c`)

Screenshot of running on Windows:

![Task 1 - Windows](screenshots/task1_win.png)

### Bonus Questions

1. **Why does Windows use `HANDLE` instead of integer file descriptors?**

   > [Your answer]

2. **What is the Windows equivalent of POSIX `fork()`? Why is it different?**

   > [Your answer]

3. **Can you use POSIX calls on Windows?**

   > [Your answer]

---

## Task 3: strace Analysis

**Describe what you observed:** [What surprised you about the strace output? How many more system calls did the library version make?]

### strace Output — Library Version (File Creator)

<!-- Screenshot: strace -e trace=openat,read,write,close ./file_creator_lib -->
<!-- IMPORTANT: Highlight/annotate the key system calls in your screenshot -->
![strace - Library version File Creator](os-class-activities-p20240007/activity1/screenshots/strace_lib_reader.png)

### strace Output — System Call Version (File Creator)

<!-- Screenshot: strace -e trace=openat,read,write,close ./file_creator_sys -->
<!-- IMPORTANT: Highlight/annotate the key system calls in your screenshot -->
![strace - System call version File Creator](os-class-activities-p20240007/activity1/screenshots/strace_sys_task1.png)

### strace Output — Library Version (File Reader or Dir Listing)

![strace - Library version](os-class-activities-p20240007/activity1/screenshots/strace_lib_reader.png)

### strace Output — System Call Version (File Reader or Dir Listing)

![strace - System call version](os-class-activities-p20240007/activity1/screenshots/strace_sys_task1.png)

### strace -c Summary Comparison

<!-- Screenshot of `strace -c` output for both versions -->
![strace summary - Library](os-class-activities-p20240007/activity1/screenshots/strace_sum_lib.png)
![strace summary - Syscall](os-class-activities-p20240007/activity1/screenshots/strace_sum_sys.png)

### Questions

1. **How many system calls does the library version make compared to the system call version?**

   > According to strace -c, the library version made 152 calls while the system call version made 147.

2. **What extra system calls appear in the library version? What do they do?**

   > I observed extra brk calls (3 vs 1), which are used for memory heap allocation. There was also a getrandom call for library initialization.

3. **How many `write()` calls does `fprintf()` actually produce?**

   > It produced 1 write() call. This confirms that the C library buffers the data and sends it to the kernel in one single batch.

4. **In your own words, what is the real difference between a library function and a system call?**

   > A library function is a "user-space" helper that makes coding easier and more efficient through buffering. A system call is the "kernel-space" gateway; it is the only way a program can actually touch the hardware.

---

## Task 4: Exploring OS Structure

### System Information

> 📸 Screenshot of `uname -a`, `/proc/cpuinfo`, `/proc/meminfo`, `/proc/version`, `/proc/uptime`:

![System Info](os-class-activities-p20240007/activity1/screenshots/Task4_A.png)

### Process Information

> 📸 Screenshot of `/proc/self/status`, `/proc/self/maps`, `ps aux`:

![Process Info](os-class-activities-p20240007/activity1/screenshots/Task4_B.png)

### Kernel Modules

> 📸 Screenshot of `lsmod` and `modinfo`:

![Kernel Modules](os-class-activities-p20240007/activity1/screenshots/Task4_C.png)

### OS Layers Diagram

> 📸 Your diagram of the OS layers, labeled with real data from your system:

![OS Layers Diagram](os-class-activities-p20240007/activity1/screenshots/Task4_os_layer_diagram.png)

### Questions

1. **What is `/proc`? Is it a real filesystem on disk?**

   > It is a virtual filesystem created by the kernel in RAM. It does not exist on your hard drive; it is a window into the kernel's internal data structures.

2. **Monolithic kernel vs. microkernel — which type does Linux use?**

   > Linux is a monolithic kernel because the entire OS sits in kernel space, though it uses "modules" to load drivers dynamically.

3. **What memory regions do you see in `/proc/self/maps`?**

   > I see the executable code (text), the heap (for dynamic memory), the stack (for local variables), and segments for shared libraries like libc.so.

4. **Break down the kernel version string from `uname -a`.**

   > 6.8.0-106-generic: 6 is the major version, 8 is the minor version, 0 is the patch level, and 106-generic is the specific Ubuntu build/distribution info.
5. **How does `/proc` show that the OS is an intermediary between programs and hardware?**

   > It allows a user program to "read" hardware status (like CPU speed or RAM usage) as if they were simple text files, without the program needing to know how to talk to the actual CPU or memory chips directly.

---

## Reflection

What did you learn from this activity? What was the most surprising difference between library functions and system calls?

> Sorry teacher, I ma be honest, the only interesting part are the coding parts and the structure of my OS layers. Also this is a nightmare please give us less tasks, everything is so time consuming and I almost lose my mind doing the screenshots and naming them. I know I submitted late it's because I planned to do it this weekend but got hit with a fever of 38.5 , anyway thank you please take it into consideration. 