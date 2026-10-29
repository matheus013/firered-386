#include <nitro.h>
#include <nnsys/g2d/g2d_Animation.h>
#include <nnsys/g2d/load/g2d_NAN_load.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "failed line %d: %s\n", __LINE__, #x); exit(42); } } while (0)
extern void WIN_CheckAndFreeAnimBank(void *, void *);

int main(int argc, char **argv) {
    CHECK(argc == 5);
    unsigned sequences = 0, ticks = 0;
    for (int i = 1; i < argc; ++i) {
        FILE *file = fopen(argv[i], "rb"); CHECK(file);
        CHECK(fseek(file, 0, SEEK_END) == 0);
        long length = ftell(file); CHECK(length > 40 && length < 1048576);
        rewind(file);
        void *data = malloc(length); CHECK(data);
        CHECK(fread(data, 1, length, file) == (size_t)length); fclose(file);
        struct { u64 guard; NNSG2dAnimBankData *bank; u64 end; } owner = {0};
        CHECK(NNS_G2dGetUnpackedAnimBank(data, &owner.bank));
        for (unsigned s = 0; s < owner.bank->numSequences; ++s) {
            const NNSG2dAnimSequenceData *seq = NNS_G2dGetAnimSequenceByIdx(owner.bank, s);
            CHECK(seq && seq->loopStartFrameIdx == 0);
            CHECK(seq->playMode == NNS_G2D_ANIMATIONPLAYMODE_FORWARD ||
                  seq->playMode == NNS_G2D_ANIMATIONPLAYMODE_FORWARD_LOOP);
            unsigned duration = 0;
            for (unsigned j = 0; j < seq->numFrames; ++j) {
                CHECK(seq->pAnmFrameArray[j].frames > 0);
                duration += seq->pAnmFrameArray[j].frames;
            }
            NNSG2dAnimController ctrl;
            NNS_G2dInitAnimCtrl(&ctrl);
            NNS_G2dBindAnimCtrl(&ctrl, seq);
            NNS_G2dStartAnimCtrl(&ctrl);
            for (unsigned t = 0; t < duration * 3 + 2; ++t) {
                unsigned time = t;
                BOOL active = TRUE;
                if (seq->playMode == NNS_G2D_ANIMATIONPLAYMODE_FORWARD_LOOP) time %= duration;
                else if (time >= duration) { time = duration - 1; active = FALSE; }
                unsigned expected = 0;
                while (time >= seq->pAnmFrameArray[expected].frames)
                    time -= seq->pAnmFrameArray[expected++].frames;
                CHECK(NNS_G2dGetAnimCtrlCurrentFrame(&ctrl) == expected);
                CHECK(NNS_G2dIsAnimCtrlActive(&ctrl) == active);
                CHECK(NNS_G2dGetAnimCtrlCurrentElement(&ctrl) == seq->pAnmFrameArray[expected].pContent);
                NNS_G2dTickAnimCtrl(&ctrl, FX32_ONE);
                ++ticks;
            }
            printf("sequence=%u duration=%u mode=%u passed\n", s, duration, seq->playMode);
            ++sequences;
        }
        WIN_CheckAndFreeAnimBank(&owner, &owner + 1);
        free(data);
    }
    printf("PLAYBACK sequences=%u ticks=%u\n", sequences, ticks);
}
