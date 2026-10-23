#include "global.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include <stddef.h>

// Compile-time ABI contract with the GBA save format. Do not remove these to
// make a desktop build pass: adjust the portability layer instead.
_Static_assert(sizeof(void *) == 4, "The native engine requires 32-bit pointers");
_Static_assert(sizeof(struct SaveBlock1) == 0x3D68, "SaveBlock1 ABI mismatch");
_Static_assert(sizeof(struct SaveBlock2) == 0xF24, "SaveBlock2 ABI mismatch");
_Static_assert(sizeof(struct PokemonStorage) == 0x83D0, "PC storage ABI mismatch");
_Static_assert(sizeof(struct Pokemon) == 100, "Pokemon ABI mismatch");
_Static_assert(sizeof(struct BoxPokemon) == 80, "BoxPokemon ABI mismatch");
_Static_assert(offsetof(struct SaveBlock2, challenge) == 0xB20, "Challenge save offset mismatch");
_Static_assert(offsetof(struct SaveBlock2, encryptionKey) == 0xF20, "Save encryption offset mismatch");
_Static_assert(offsetof(struct SaveBlock1, playerParty) == 0x38, "Party offset mismatch");
_Static_assert(offsetof(struct SaveBlock1, flags) == 0xEE0, "Flag offset mismatch");
_Static_assert(offsetof(struct SaveBlock1, vars) == 0x1000, "Var offset mismatch");
