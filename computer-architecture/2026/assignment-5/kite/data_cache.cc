#include <cstdlib>
#include <iostream>
#include "data_cache.h"

using namespace std;

data_cache_t::data_cache_t(uint64_t *m_ticks, uint64_t m_cache_size,
                           uint64_t m_block_size, uint64_t m_ways,
                           uint64_t m_victim_size) :
    memory(0),
    ticks(m_ticks),
    blocks(0),
    victim_cache(0),
    cache_size(m_cache_size),
    block_size(m_block_size),
    num_sets(0),
    num_ways(m_ways),
    block_offset(0),
    set_offset(0),
    block_mask(0),
    set_mask(0),
    num_accesses(0),
    num_misses(0),
    num_loads(0),
    num_stores(0),
    missed_inst(0) {
    // Calculate the block offset.
    uint64_t val = block_size;
    while(!(val & 0b1)) {
        val = val >> 1; block_offset++;
        block_mask = (block_mask << 1) | 0b1;
    }
    // Check if the block size is a multiple of doubleword.
    if((block_size & 0b111) || (val != 1)) {
        cerr << "Error: cache block size must be a multiple of doubleword" << endl;
        exit(1);
    }

    // Check if the number of ways is a power of two.
    val = num_ways;
    while(!(val & 0b1)) { val = val >> 1; }
    if(val != 1) {
        cerr << "Error: number of ways must be a power of two" << endl;
        exit(1);
    }

    // Calculate the number of sets.
    num_sets = cache_size / block_size / num_ways;
    // Calculate the set offset and mask.
    val = num_sets;
    while(!(val & 0b1)) {
        val = val >> 1; set_offset++;
        set_mask = (set_mask << 1) | 0b1;
    }
    set_offset += block_offset;
    set_mask = set_mask << block_offset;
    // Check if the number of sets is a power of two.
    if(val != 1) {
        cerr << "Error: number of sets must be a power of two" << endl;
        exit(1);
    }
    
    // Allocate cache blocks.
    blocks = new block_t*[num_sets]();
    for(uint64_t i = 0; i < num_sets; i++) { blocks[i] = new block_t[num_ways](); }

    // Create a victim cache.
    victim_cache = new victim_cache_t(m_victim_size);
}

data_cache_t::~data_cache_t() {
    // Deallocate the cache blocks.
    for(uint64_t i = 0; i < num_sets; i++) { delete [] blocks[i]; }
    delete [] blocks;

    // Destruct the victim cache.
    delete victim_cache;
}

// Connect to the lower-level memory.
void data_cache_t::connect(data_memory_t *m_memory) { memory = m_memory; }

// Is cache free?
bool data_cache_t::is_free() const { return !missed_inst; }

// Read data from cache.
void data_cache_t::read(inst_t *m_inst) {
    // Check the memory address alignment.
    uint64_t addr = m_inst->memory_addr;
    if(addr & 0b111) {
        cerr << "Error: invalid alignment of memory address " << addr << endl;
        exit(1);
    }

    // Calculate the set index and tag.
    uint64_t set_index = (addr & set_mask) >> block_offset;
    uint64_t tag = addr >> set_offset;

    // Check direct-mapped cache entry.
    block_t *block = &blocks[set_index][0];
    if(!block->valid || (block->tag != tag)) { block = 0; }

    if(block) { // Cache hit
        // Update the last access time.
        block->last_access = *ticks;
        // Read a doubleword in the block.
        m_inst->rd_val = *(block->data + ((addr & block_mask) >> 3));
#ifdef DATA_FWD
        m_inst->rd_ready = true;
#endif
        num_accesses++;
        num_loads++;
    }
    else { // Cache miss
        /* Assignment 5: Cache 
           - The baseline cache simply loads the missed block from memory.
           - In this assignment, the victim cache must be first looked up.
           - On a victim cache hit, the cache line is replaced, and the
             evicted cache block from the data cache is inserted into the
             victim cache.
           - On a victim cache miss, it follow the same baseline procedure. */
        // Check the victim cache before requesting the block from memory.
        block_t v_block = victim_cache->remove(addr & ~block_mask);

        if(v_block.valid) {
            block_t *main_block = &blocks[set_index][0];

            if(main_block->valid) {
                // Move the current data-cache block to the victim cache.
                block_t old_block = *main_block;

                // Rebuild the old block address from its data-cache tag and set index.
                uint64_t old_addr = (old_block.tag << set_offset) |
                                    (set_index << block_offset);

                // Store the old block with a victim-cache tag.
                old_block.tag = old_addr >> block_offset;
                victim_cache->insert(old_block);
            }

            // Place the victim block back into the direct-mapped cache.
            *main_block = v_block;
            main_block->tag = tag;
            main_block->last_access = *ticks;

            // Read the requested doubleword from the cache block.
            m_inst->rd_val = *(main_block->data + ((addr & block_mask) >> 3));
#ifdef DATA_FWD
            m_inst->rd_ready = true;
#endif
            num_accesses++;
            num_loads++;
        }
        else {
            // If the victim cache also misses, load the block from memory.
            missed_inst = m_inst;
            memory->load_block(addr & ~block_mask, block_size);
            num_misses++;
#ifdef DEBUG
            cout << *ticks << " : cache miss : addr = " << addr
                 << " (tag = " << tag << ", set = " << set_index << ")" << endl;
#endif
        }
    }
}

// Write data in memory.
void data_cache_t::write(inst_t *m_inst) {
    // Check the memory address alignment.
    uint64_t addr = m_inst->memory_addr;
    if(addr & 0b111) {
        cerr << "Error: invalid alignment of memory address " << addr << endl;
        exit(1);
    }

    // Calculate the set index and tag.
    uint64_t set_index = (addr & set_mask) >> block_offset;
    uint64_t tag = addr >> set_offset;

    // Check the direct-mapped cache entry.
    block_t *block = &blocks[set_index][0];
    if(!block->valid || (block->tag != tag)) { block = 0; }

    if(block) { // Cache hit
        // Update the last access time and dirty flag.
        block->last_access = *ticks;
        block->dirty = true;
        // Write a doubleword in the block.
        *(block->data + ((addr & block_mask) >> 3)) = m_inst->rs2_val;
        num_accesses++;
        num_stores++;
    }
    else { // Cache miss
        /* Assignment 5: Cache 
           - The procedure is the same as handling load instructions. */
        // Check the victim cache before requesting the block from memory.
        block_t v_block = victim_cache->remove(addr & ~block_mask);

        if(v_block.valid) {
            block_t *main_block = &blocks[set_index][0];

            if(main_block->valid) {
                // Move the current data-cache block to the victim cache.
                block_t old_block = *main_block;

                // Rebuild the old block address from its data-cache tag and set index.
                uint64_t old_addr = (old_block.tag << set_offset) |
                                    (set_index << block_offset);

                // Store the old block with a victim-cache tag.
                old_block.tag = old_addr >> block_offset;
                victim_cache->insert(old_block);
            }

            // Place the victim block back into the direct-mapped cache.
            *main_block = v_block;
            main_block->tag = tag;
            main_block->last_access = *ticks;

            // Mark the block dirty because this access writes data.
            main_block->dirty = true;

            // Write the requested doubleword into the cache block.
            *(main_block->data + ((addr & block_mask) >> 3)) = m_inst->rs2_val;

            num_accesses++;
            num_stores++;
        }
        else {
            // If the victim cache also misses, load the block from memory.
            missed_inst = m_inst;
            memory->load_block(addr & ~block_mask, block_size);
            num_misses++;
#ifdef DEBUG
            cout << *ticks << " : cache miss : addr = " << addr
                 << " (tag = " << tag << ", set = " << set_index << ")" << endl;
#endif
        }
    }
}

// Handle a memory response.
void data_cache_t::handle_response(int64_t *m_data) {
    // Calculate the set index and tag.
    uint64_t addr = missed_inst->memory_addr;
    uint64_t set_index = (addr & set_mask) >> block_offset;
    uint64_t tag = addr >> set_offset;

    // Block replacement
    block_t *victim = &blocks[set_index][0];
    if(victim->valid) {
#ifdef DEBUG
        cout << *ticks << " : cache eviction : addr = " << addr
             << " (tag = " << tag << ", set = " << set_index << ")" << endl;
#endif
        /* Assignment 5: Cache 
           - The victim block needs to be inserted into the victim cache
             before the corresponding cache line is replaced by the loaded
             memory block from main memory. */
        // Save the evicted data-cache block in the victim cache.
        block_t old_block = *victim;

        // Rebuild the evicted block address before changing its tag.
        uint64_t old_addr = (old_block.tag << set_offset) |
                            (set_index << block_offset);

        // Convert the data-cache tag to a victim-cache tag.
        old_block.tag = old_addr >> block_offset;

        // Insert the evicted block into the victim cache.
        victim_cache->insert(old_block);
    }
    // Place the missed block.
    *victim = block_t(tag, m_data, /* valid */ true);

    // Replay the cache access.
    if(missed_inst->op == op_ld) { read(missed_inst); }
    else { write(missed_inst); }
    // Clear the missed instruction so that the cache becomes free.
    missed_inst = 0;
}

// Run data cache.
bool data_cache_t::run() {
    memory->run();          // Run the data memory.
    return missed_inst;     // Return true if the cache is busy.
}

// Print cache stats.
void data_cache_t::print_stats() {
    cout << endl << "Data cache stats:" << endl;
    cout.precision(3);
    cout << "    Number of loads = " << num_loads << endl;
    cout << "    Number of stores = " << num_stores << endl;
    cout << "    Miss rate = " << fixed
         << (num_accesses ? double(num_misses) / double(num_accesses) : 0)
         << " (" << num_misses << "/" << num_accesses << ")" << endl;
    cout.precision(-1);

    // Print victim cache stats.
    victim_cache->print_stats();
}

// Victim cache
victim_cache_t::victim_cache_t(uint64_t m_size) :
    num_entries(0),
    size(m_size),
    num_accesses(0),
    num_hits(0),
    num_writebacks(0),
    blocks(0) {
    blocks = new block_t[size]();
}

victim_cache_t::~victim_cache_t() {
    delete [] blocks;
}

block_t victim_cache_t::remove(uint64_t m_addr) {
    block_t block;  // Invalid block

    /* Assignment 5: Cache 
       - Victim cache entries (i.e., blocks[]) need to be searched to find
         if there is a matching entry.
       - The matching entry becomes the returning block, such as:
         return blocks[i];
       - Otherwise, the invalid block is returned to the data cache's query
         to indicate that there is no matching entry in the victim cache. */
    // Count this victim-cache lookup.
    num_accesses++;

    // Compare entries using the block address as the victim-cache tag.
    uint64_t find_tag = m_addr >> 5;

    // Search all valid victim-cache entries.
    for(uint64_t i = 0; i < num_entries; i++) {
        if(blocks[i].valid && (blocks[i].tag == find_tag)) {
            // Save the matched block before removing it.
            block = blocks[i];
            num_hits++;

            // Shift later entries forward to fill the removed slot.
            for(uint64_t j = i; j + 1 < num_entries; j++) {
                blocks[j] = blocks[j + 1];
            }

            num_entries--;
            return block;
        }
    }

    return block;
}

void victim_cache_t::insert(block_t m_block) {
    /* Assignment 5: Cache 
       - If the victim cache is full, the oldest entry is evicted and written
         back to the memory if dirty, such as: */
    // Do not insert an invalid block.
    if(!m_block.valid) { return; }

    // Evict the oldest victim entry when the FIFO queue is full.
    if(num_entries == size) {
        // Count a writeback only when the evicted victim block is dirty.
        if(blocks[0].dirty) { num_writebacks++; }

        /* Assignment 5: Cache 
           - Then, the remaining entries are shifted forward.
           - Lastly, the new victim block is placed at the end of FIFO queue,
             such as: */
        // Shift entries forward to keep FIFO order.
        for(uint64_t i = 0; i + 1 < size; i++) {
            blocks[i] = blocks[i + 1];
        }

        // Put the new block at the end of the FIFO queue.
        blocks[size - 1] = m_block;
    }
    else {
        // Append the new block when the victim cache is not full.
        blocks[num_entries] = m_block;
        num_entries++;
    }

}

void victim_cache_t::print_stats() {
    /* Do not modify this function */
    cout << endl << "Victim cache stats:" << endl;
    cout.precision(3);
    cout << "    Number of writebacks = " << num_writebacks << endl;
    cout << "    Hit rate = " << fixed
         << (num_accesses ? double(num_hits) / double(num_accesses) : 0)
         << " (" << num_hits << "/" << num_accesses << ")" << endl;
    cout.precision(-1);
}

