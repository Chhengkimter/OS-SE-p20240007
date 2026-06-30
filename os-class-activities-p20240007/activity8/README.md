# Class Activity 8 - Memory Management & Virtual Memory

- **Student Name:** Chheng Kimter
   **Student ID:** p20240007
- **Personalization:** a = 7, b = 0 → N = (10a+b) mod 128 = 38.4
- **Programming Language Used:** [...]

## Part 1A — Address translation (by hand)
[your filled translation table]
1. Offset unchanged because: pages (in logical memory) and frames (in physical memory) are exactly the same size. The offset simply indicates the specific byte's relative position or distance from the start of that block. Because moving a block of data into physical memory doesn't rearrange its internal bytes, its relative position within the block remains unchanged.
2. Largest offset = 15, need 4 bits
3. (60 + a) = 67 bytes → 5 pages, internal fragmentation = 13 bytes (show working)

## Part 1B — TLB & Effective Access Time (by hand)
- My page-reference stream: …    Prediction (expected hits): …
[TLB trace table] → measured hits = …/10, α = …
- EAT at my α: … ns   |   EAT at 80% = … |   99% = … |   no TLB = …  (show substitutions)
- Why 99% beats no-TLB by …%: …
![EAT](screenshots/part1_eat.png)   ![TLB](screenshots/part1_tlb.png)

## Part 1C — Paging simulator verification
![Translation](screenshots/task1_translation.png)
- Did the simulator match my 1A table? …
- (Optional) Did the TLB sim reproduce my 1B hit ratio / EAT? …

## Part 2A — Page replacement (by hand)
- My reference string: …    Prediction (FIFO vs LRU): …
[FIFO trace table] → FIFO faults: …
[LRU trace table]  → LRU faults: …
Which faulted more, and did it match my prediction: …

## Part 2B — Demand-paging simulator verification
![FIFO](screenshots/task2_fifo.png)   ![LRU](screenshots/task2_lru.png)
- Did the simulator's counts for my 2A string match my hand totals? … (if not, what was wrong)

## Part 3 — Applied reasoning
1. …  2. …  3. …  4. …  5. …  6. …