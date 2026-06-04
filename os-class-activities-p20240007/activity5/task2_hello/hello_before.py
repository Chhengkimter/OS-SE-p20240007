import threading
import time
import random

# NO semaphore ordering — letters race and may print in wrong order

def process1():
    time.sleep(random.uniform(0, 0.1))
    print("H", end="", flush=True)
    time.sleep(random.uniform(0, 0.1))
    print("E", end="", flush=True)

def process2():
    time.sleep(random.uniform(0, 0.1))
    print("L", end="", flush=True)
    time.sleep(random.uniform(0, 0.1))
    print("L", end="", flush=True)

def process3():
    time.sleep(random.uniform(0, 0.1))
    print("O", end="", flush=True)

print("Running hello_before 5 times to show unpredictable order:")
for i in range(5):
    t1 = threading.Thread(target=process1)
    t2 = threading.Thread(target=process2)
    t3 = threading.Thread(target=process3)
    t1.start(); t2.start(); t3.start()
    t1.join();  t2.join();  t3.join()
    print()  # newline after each attempt
