#include <cstring>
#include "br_predictor.h"

br_predictor_t::~br_predictor_t() {
    // Deallocate the BHT and PHT.
    delete [] bht;
    delete [] pht;
}

br_target_buffer_t::~br_target_buffer_t() {
    // Deallocate the target address array.
    delete [] buffer;
}



/**********************************
* Assignment 4: Branch Prediction *
***********************************/

// Branch predictor
br_predictor_t::br_predictor_t(unsigned m_bht_bits, unsigned m_pht_bits, unsigned m_hist_len) :
    bht(0),
    pht(0),
    b(m_bht_bits),
    p(m_pht_bits),
    h(m_hist_len) {
    // Create branch history table (BHT) and pattern history table (PHT).
    bht = new uint8_t[1 << b];
    pht = new uint8_t[(1 << p) * (1 << h)];

    // BHT starts with empty histories.
    for (uint64_t i = 0; i < (1ULL << b); i++) {
        bht[i] = 0;
    }

    // PHT counters start at weakly not-taken.
    for (uint64_t i = 0; i < ((1ULL << p) * (1ULL << h)); i++) {
        pht[i] = 1;
    }
}

// Is a branch predicted to be taken?
bool br_predictor_t::is_taken(inst_t *m_inst) {
    uint64_t pc = m_inst->pc >> 2;

    uint64_t bht_idx = pc & ((1ULL << b) - 1);
    uint64_t pht_row = pc & ((1ULL << p) - 1);
    uint64_t hist = bht[bht_idx] & ((1ULL << h) - 1);

    uint64_t pht_idx = (pht_row << h) | hist;
    m_inst->br_pht_idx = pht_idx;

    return pht[pht_idx] >= 2;
}

// Update a prediction counter.
void br_predictor_t::update(inst_t *m_inst) {
    uint64_t pht_idx = m_inst->br_pht_idx;

    // Update the selected 2-bit saturating counter.
    if (m_inst->branch_taken) {
        if (pht[pht_idx] < 3) {
            pht[pht_idx]++;
        }
    } else {
        if (pht[pht_idx] > 0) {
            pht[pht_idx]--;
        }
    }

    // Shift the actual outcome into the local history.
    uint64_t pc = m_inst->pc >> 2;
    uint64_t bht_idx = pc & ((1ULL << b) - 1);
    uint64_t hist_mask = (1ULL << h) - 1;

    bht[bht_idx] = ((bht[bht_idx] << 1) |
                    (m_inst->branch_taken ? 1 : 0)) & hist_mask;
}



// Branch target buffer
br_target_buffer_t::br_target_buffer_t(uint64_t m_size) :
    num_entries(m_size),
    buffer(0) {
    // Create a direct-mapped branch target buffer (BTB).
    buffer = new uint64_t[num_entries];

    // Empty BTB entries contain zero.
    for (uint64_t i = 0; i < num_entries; i++) {
        buffer[i] = 0;
    }
}

// Get a branch target address.
uint64_t br_target_buffer_t::get_target(uint64_t m_pc) {
    uint64_t idx = (m_pc >> 2) & (num_entries - 1);

    return buffer[idx];
}

// Update the branch target buffer.
void br_target_buffer_t::update(uint64_t m_pc, uint64_t m_target_addr) {
    uint64_t idx = (m_pc >> 2) & (num_entries - 1);

    buffer[idx] = m_target_addr;
}