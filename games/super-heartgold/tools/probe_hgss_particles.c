/* Include the privately adapted unit to exercise its original static allocators. */
#include "particle_system-native.c"
#include <stdio.h>
#include <stdlib.h>

#define CHECK(condition) do { if (!(condition)) abort(); } while (0)
void GF_AssertFail(void) { abort(); }

int main(void) {
    unsigned cases = 0;
    for (unsigned slot = 0; slot < 16; ++slot) {
        for (unsigned size = 1; size < 128; ++size) {
            unsigned char *arena = malloc(512);
            CHECK(arena != NULL);
            ParticleSystem system = {0};
            system.heapStart = system.heapCurrent = arena;
            system.heapEnd = arena + 512;
            sParticleSystems[slot] = &system;
            unsigned char *first = sParticleSystemAllocFuncs[slot](size);
            memset(first, 0xA5, size);
            uintptr_t *second = sParticleSystemAllocFuncs[slot](sizeof(uintptr_t));
            CHECK((uintptr_t)second % sizeof(void *) == 0);
            CHECK((unsigned char *)second >= first + size);
            *second = (uintptr_t)arena;
            CHECK(*second == (uintptr_t)arena);
            for (unsigned i = 0; i < size; ++i) CHECK(first[i] == 0xA5);
            CHECK((unsigned char *)system.heapCurrent <= arena + 512);
            sParticleSystems[slot] = NULL;
            free(arena);
            ++cases;
        }
    }
    printf("Particle allocators: %u alignment and preservation cases passed\n", cases);
    return 0;
}
