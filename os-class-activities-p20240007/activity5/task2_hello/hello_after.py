import threading

# Semaphore ordering: H -> E -> L -> L -> O

start_h  = threading.Semaphore(1)  # Process 1 may start immediately
after_e  = threading.Semaphore(0)  # Process 2 waits for HE
after_l1 = threading.Semaphore(0)  # Process 2 second L waits for first L
after_l2 = threading.Semaphore(0)  # Process 3 waits for LL

def process1():
    start_h.acquire()
    print("H", end="", flush=True)
    print("E", end="", flush=True)
    after_e.release()

def process2():
    after_e.acquire()
    print("L", end="", flush=True)
    after_l1.release()
    after_l1.acquire()
    print("L", end="", flush=True)
    after_l2.release()

def process3():
    after_l2.acquire()
    print("O", end="", flush=True)

print("Running hello_after — should always print HELLO:")
t1 = threading.Thread(target=process1)
t2 = threading.Thread(target=process2)
t3 = threading.Thread(target=process3)
t1.start(); t2.start(); t3.start()
t1.join();  t2.join();  t3.join()
print()  # newline
print("Done.")
