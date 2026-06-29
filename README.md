# LRU-Smart-Cache-System
# High-Performance LRU Smart Cache System

This repository contains a self-contained implementation of a Least Recently Used (LRU) Cache designed from scratch in C++. The system achieves strict O(1) time complexity for both data retrieval and updates by combining associative mapping with sequential temporal tracking.

## Architecture

To eliminate the performance limitations of sequential lookups or rigid static structures, this design couples two complementary data structures from the Standard Template Library (STL):

1. **Doubly Linked List (std::list<std::pair<int, int>>)**: Handles the temporal sequencing of cache records. The head of the list represents the most recently accessed elements, while the tail holds the least recently used elements. This allows for constant-time deletion and node shifting once a specific element's position is known.
2. **Hash Map (std::unordered_map<int, std::list<...>::iterator>)**: Connects integer keys directly to their respective node iterators within the doubly linked list. This provides immediate, constant-time lookups and avoids linear scans through memory.

## Algorithmic Complexity

* **Retrieval (fetch)**: O(1) time complexity. The element position is located instantly via the hash map, its value is extracted, and the corresponding list node is moved to the head of the chain.
* **Insertion (store)**: O(1) time complexity. New data points are added straight to the head of the list. If the cache hits its capacity ceiling, the element at the tail of the list is permanently evicted from both the list and the map tracker.
* **Space Complexity**: O(N), where N represents the strict maximum storage limit specified during initialization.

## Testing and Verification

The file contains an embedded driver program in the main execution sequence to run system diagnostics. It validates sequential insertions, value updates, recency tracking modifications, and automated node eviction under full-capacity workloads.

### Building and Running the System

To compile and verify the simulation on a local system, navigate to the project directory and execute the following commands in the terminal:

```bash
g++ -std=c++11 lru_cache.cpp -o cache_simulator
./cache_simulator
```

### Expected Console Output
```text
====================================================
        LRU SMART CACHE SIMULATOR TEST RUN          
====================================================

[INIT] Configured system with a capacity limit of 3 items.

[STAGE 1] Performing sequential database insertions...
Storing Key 10 (Val: 100) - Success.
Storing Key 20 (Val: 200) - Success.
Storing Key 30 (Val: 300) - Success.

[STAGE 2] Querying internal key states:
 Query Key 10: 100 (Expected output: 100)
 Query Key 20: 200 (Expected output: 200)

Note: Key 30 is now the Least Recently Used (oldest element) because 10 and 20 were just accessed.

[STAGE 3] Forcing data limit threshold violation:
Storing Key 40 (Val: 400) -> [EVICTION] Capacity Full. Safely evicted Key: 30

[STAGE 4] Post-Eviction State Verification:
 Query Key 30: -1 (Expected output: -1 due to eviction)
 Query Key 10: 100 (Expected output: 100)
 Query Key 40: 400 (Expected output: 400)

[STAGE 5] Modifying an active key value:
Updating Key 10 value to 999.
Adding Key 50 (Val: 500) to trigger another eviction -> [EVICTION] Capacity Full. Safely evicted Key: 20
 Query Key 20: -1 (Expected output: -1 due to eviction)
 Query Key 10: 999 (Expected output: 999)

====================================================
          ALL SYSTEM DIAGNOSTICS COMPLETED          
====================================================
```
