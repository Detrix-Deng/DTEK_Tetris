// Contributed by Dave
#include "performance.h"

void start_performance(){
    // Reset all performance related registers
    asm volatile("csrw mcycle, x0");
    asm volatile("csrw mcycleh, x0");
    asm volatile("csrw minstret, x0");
    asm volatile("csrw minstreth, x0");
    asm volatile("csrw mhpmcounter3, x0");
    asm volatile("csrw mhpmcounter3h, x0");
    asm volatile("csrw mhpmcounter4, x0");
    asm volatile("csrw mhpmcounter4h, x0");
    asm volatile("csrw mhpmcounter5, x0");
    asm volatile("csrw mhpmcounter5h, x0");
    asm volatile("csrw mhpmcounter6, x0");
    asm volatile("csrw mhpmcounter6h, x0");
    asm volatile("csrw mhpmcounter7, x0");
    asm volatile("csrw mhpmcounter7h, x0");
    asm volatile("csrw mhpmcounter8, x0");
    asm volatile("csrw mhpmcounter8h, x0");
    asm volatile("csrw mhpmcounter9, x0");
    asm volatile("csrw mhpmcounter9h, x0");
}

void stop_performance(struct performance_value *performance_value){
    // Read values from all performance related registers
    asm("csrr %0, mcycle" : "=r"(performance_value->mcycle));
    asm("csrr %0, mcycleh" : "=r"(performance_value->mcycleh));
    asm("csrr %0, minstret" : "=r"(performance_value->minstret));
    asm("csrr %0, minstreth" : "=r"(performance_value->minstreth));
    asm("csrr %0, mhpmcounter3" : "=r"(performance_value->mhpmcounter3));
    asm("csrr %0, mhpmcounter3h" : "=r"(performance_value->mhpmcounter3h));
    asm("csrr %0, mhpmcounter4" : "=r"(performance_value->mhpmcounter4));
    asm("csrr %0, mhpmcounter4h" : "=r"(performance_value->mhpmcounter4h));
    asm("csrr %0, mhpmcounter5" : "=r"(performance_value->mhpmcounter5));
    asm("csrr %0, mhpmcounter5h" : "=r"(performance_value->mhpmcounter5h));
    asm("csrr %0, mhpmcounter6" : "=r"(performance_value->mhpmcounter6));
    asm("csrr %0, mhpmcounter6h" : "=r"(performance_value->mhpmcounter6h));
    asm("csrr %0, mhpmcounter7" : "=r"(performance_value->mhpmcounter7));
    asm("csrr %0, mhpmcounter7h" : "=r"(performance_value->mhpmcounter7h));
    asm("csrr %0, mhpmcounter8" : "=r"(performance_value->mhpmcounter8));
    asm("csrr %0, mhpmcounter8h" : "=r"(performance_value->mhpmcounter8h));
    asm("csrr %0, mhpmcounter9" : "=r"(performance_value->mhpmcounter9));
    asm("csrr %0, mhpmcounter9h" : "=r"(performance_value->mhpmcounter9h));
}