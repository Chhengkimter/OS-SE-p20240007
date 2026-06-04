import threading
import time
import random

BUFFER_CAPACITY = 100
buffer = []
produced_count = 0
packaged_count = 0
lock = threading.Lock()
running = True

def producer(machine_id):
    global produced_count, running
    pair_id = 0
    while running:
        pair_id += 1
        p1 = f"M{machine_id}-{pair_id}-P1"
        p2 = f"M{machine_id}-{pair_id}-P2"
        time.sleep(random.uniform(0.01, 0.05))
        # NO semaphore: may overflow buffer
        if len(buffer) + 2 > BUFFER_CAPACITY:
            print("The producing machine is broken")
            running = False
            return
        buffer.append(p1)
        buffer.append(p2)
        produced_count += 1

def consumer():
    global packaged_count, running
    while running:
        time.sleep(random.uniform(0.01, 0.03))
        # NO semaphore: may underflow or grab mismatched particles
        if len(buffer) < 2:
            if len(buffer) == 0:
                print("The packaging machine is broken")
                running = False
                return
            continue
        p1 = buffer.pop(0)
        p2 = buffer.pop(0)
        # Check pair validity
        parts1 = p1.rsplit("-", 1)
        parts2 = p2.rsplit("-", 1)
        if parts1[0] != parts2[0] or parts1[0].replace("P1","") != parts2[0].replace("P2",""):
            base1 = "-".join(p1.split("-")[:2])
            base2 = "-".join(p2.split("-")[:2])
            if base1 != base2:
                print(f"Pairs are incorrect: {p1} + {p2}")
                running = False
                return
        packaged_count += 1
        print(f"Produced pairs: {produced_count} | Packaged pairs: {packaged_count} | Buffer particles: {len(buffer)}")

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
    print("\nStopped by user.")
