#include <iostream>
#include <unordered_map>
#include <list>
#include <utility>

class SmartCache {
private:
    // Tracks temporal ordering: {Key, Value}. Front is newest, back is oldest.
    std::list<std::pair<int, int>> usageOrder;
    
    // Quick reference: Key -> Iterator pointing to its position in usageOrder
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> locatorMap;
    
    int storageLimit;

public:
    // Initialize the storage space with a strict upper limit
    SmartCache(int capacity) {
        storageLimit = capacity;
    }
    
    // Retrieve value linked to a specific key
    int fetch(int lookupKey) {
        // Record not found in lookup system
        if (locatorMap.find(lookupKey) == locatorMap.end()) {
            return -1;
        }
        
        // Element found; capture its internal pointer
        auto targetIterator = locatorMap[lookupKey];
        int dynamicValue = targetIterator->second;
        
        // Refresh operational recency by shifting element to front of the chain
        usageOrder.erase(targetIterator);
        usageOrder.push_front({lookupKey, dynamicValue});
        
        // Remap the pointer index to the new head position
        locatorMap[lookupKey] = usageOrder.begin();
        
        return dynamicValue;
    }
    
    // Inject or refresh data mappings within the storage cluster
    void store(int insertKey, int insertValue) {
        // Scenario A: Existing mapping found; update entry value and shift to front
        if (locatorMap.find(insertKey) != locatorMap.end()) {
            usageOrder.erase(locatorMap[insertKey]);
            usageOrder.push_front({insertKey, insertValue});
            locatorMap[insertKey] = usageOrder.begin();
            return;
        }
        
        // Scenario B: Storage capacity reached; trigger eviction sequence
        if (usageOrder.size() == storageLimit) {
            int staleKey = usageOrder.back().first;
            usageOrder.pop_back();       // Drop from structural chain
            locatorMap.erase(staleKey);  // Drop from index lookup system
            std::cout << " -> [EVICTION] Capacity Full. Safely evicted Key: " << staleKey << "\n";
        }
        
        // Finalize transaction by adding data to chain and index map
        usageOrder.push_front({insertKey, insertValue});
        locatorMap[insertKey] = usageOrder.begin();
    }
};

int main() {
    std::cout << "====================================================\n";
    std::cout << "        LRU SMART CACHE SIMULATOR TEST RUN          \n";
    std::cout << "====================================================\n\n";

    // Initialize cache with a small threshold capacity of 3 nodes
    SmartCache systemCache(3);
    std::cout << "[INIT] Configured system with a capacity limit of 3 items.\n\n";

    // Step 1: Populate cache up to its threshold limit
    std::cout << "[STAGE 1] Performing sequential database insertions...\n";
    std::cout << "Storing Key 10 (Val: 100)"; systemCache.store(10, 100); std::cout << " - Success.\n";
    std::cout << "Storing Key 20 (Val: 200)"; systemCache.store(20, 200); std::cout << " - Success.\n";
    std::cout << "Storing Key 30 (Val: 300)"; systemCache.store(30, 300); std::cout << " - Success.\n\n";

    // Step 2: Validate structural balance and query efficiency
    std::cout << "[STAGE 2] Querying internal key states:\n";
    std::cout << " Query Key 10: " << systemCache.fetch(10) << " (Expected output: 100)\n";
    std::cout << " Query Key 20: " << systemCache.fetch(20) << " (Expected output: 200)\n\n";
    std::cout << "Note: Key 30 is now the Least Recently Used (oldest element) because 10 and 20 were just accessed.\n\n";

    // Step 3: Trigger eviction process by passing limits
    std::cout << "[STAGE 3] Forcing data limit threshold violation:\n";
    std::cout << "Storing Key 40 (Val: 400)"; 
    systemCache.store(40, 400); // This must evict Key 30

    // Step 4: Verify accurate node purging
    std::cout << "\n[STAGE 4] Post-Eviction State Verification:\n";
    std::cout << " Query Key 30: " << systemCache.fetch(30) << " (Expected output: -1 due to eviction)\n";
    std::cout << " Query Key 10: " << systemCache.fetch(10) << " (Expected output: 100)\n";
    std::cout << " Query Key 40: " << systemCache.fetch(40) << " (Expected output: 400)\n\n";

    // Step 5: Update an existing key value and check recency change
    std::cout << "[STAGE 5] Modifying an active key value:\n";
    std::cout << "Updating Key 10 value to 999.\n";
    systemCache.store(10, 999);
    
    std::cout << "Adding Key 50 (Val: 500) to trigger another eviction";
    systemCache.store(50, 500); // Key 20 should be evicted now because 10 was just updated and 40 is newer

    std::cout << " Query Key 20: " << systemCache.fetch(20) << " (Expected output: -1 due to eviction)\n";
    std::cout << " Query Key 10: " << systemCache.fetch(10) << " (Expected output: 999)\n";

    std::cout << "\n====================================================\n";
    std::cout << "          ALL SYSTEM DIAGNOSTICS COMPLETED          \n";
    std::cout << "====================================================\n";

    return 0;
}
