#ifndef PERFORMANCE_H
#define PERFORMANCE_H

// Contains performance related functions and struct

// Descrptions from "DTEK-V Hardware Counter Introduction"
struct performance_value{
    // Counts the number of clock cycles that elapsed.
    int mcycle;
    int mcycleh;
    // Counts the number of instructions that have been retired.
    int minstret;
    int minstreth;
    // Counts the number of memory instructions that have been retired.
    int mhpmcounter3;
    int mhpmcounter3h;
    // Counts the number of times an instruction-fetch resulted in a I-cache miss.
    int mhpmcounter4;
    int mhpmcounter4h;
    // Counts the number of times an memory operation resulted in a D-cache
    int mhpmcounter5;
    int mhpmcounter5h;
    // Counts the number of stalls the CPU experienced due to I-cache misses.
    int mhpmcounter6;
    int mhpmcounter6h;
    // Counts the number of stalls the CPU experienced due to D-cache misses.
    int mhpmcounter7;
    int mhpmcounter7h;
    // Counts the number of stalls that the CPU experienced due to data hazards
    // that could not be solved by forwarding.
    int mhpmcounter8;
    int mhpmcounter8h;
    // Counts the number of stalls that the CPU experienced due to expensive ALU
    // operations.
    int mhpmcounter9;
    int mhpmcounter9h;
};

extern void start_performance();
extern void stop_performance(struct performance_value performance_value);

#endif