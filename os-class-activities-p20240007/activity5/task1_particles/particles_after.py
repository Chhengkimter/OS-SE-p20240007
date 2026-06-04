import threading
import time
import random

BUFFER_CAPACITY = 100
MAX_PAIRS = BUFFER_CAPACITY // 2

buffer = []
produced_count = 0
packaged_count = 0
running = True

empty_pairs = threading.Semaphore(MAX_PAIRS)   # 50 free pair slots
full_pairs  = threading.Semaphore(0)            # 0 ready pairs
mutex       = threading.Semaphore(1)            # buffer critical section

def producer(machine_id):
    global produced_count, running
    pair_id = 0
    while running:
        pair_id += 1
        p1 = f"M{machine_id}-{pair_id}-P1"
        p2 = f"M{machine_id}-{pair_id}-P2"
        time.sleep(random.uniform(0.02, 0.08))

        empty_pairs.acquire()   # wait for a free pair slot
        mutex.acquire()         # enter critical section

        if len(buffer) + 2 > BUFFER_CAPACITY:
            print("The producing machine is broken")
            running = False
            mutex.release()
            full_pairs.release()
            return

        buffer.append(p1)
        buffer.append(p2)
        produced_count += 1

        mutex.release()
        full_pairs.release()    # signal a pair is ready

def consumer():
    global packaged_count, running
    while running:
        time.sleep(random.uniform(0.03, 0.1))

        full_pairs.acquire()    # wait for a ready pair
        mutex.acquire()         # enter critical section

        if len(buffer) < 2:
            print("The packaging machine is broken")
            running = False
            mutex.release()
            empty_pairs.release()
            return

        p1 = buffer.pop(0)
        p2 = buffer.pop(0)

        base1 = "-".join(p1.split("-")[:2])
        base2 = "-".join(p2.split("-")[:2])
        if base1 != base2:
            print(f"Pairs are incorrect: {p1} + {p2}")
            running = False
            mutex.release()
            empty_pairs.release()
            return

        packaged_count += 1
        buf_size = len(buffer)

        mutex.release()
        empty_pairs.release()   # free the slot

        print(f"Produced pairs: {produced_count} | Packaged pairs: {packaged_count} | Buffer particles: {buf_size}")

threads = []
for i in range(1, 4):
    t = threading.Thread(target=producer, args=(i,), daemon=True)
    threads.append(t)
    t.start()

c = threading.Thread(target=consumer, daemon=True)
c.start()

try:
    while running:
        time.sleep(0.1)
except KeyboardInterrupt:
    print("\nStopped by user (Ctrl+C).")
