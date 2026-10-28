/* Exercise complete SDK loaders against the private, hash-pinned scene assets. */
#include <nitro.h>
#include <nnsys/g2d/load/g2d_NCG_load.h>
#include <nnsys/g2d/load/g2d_NSC_load.h>
#include <nnsys/g2d/load/g2d_NCE_load.h>
#include <nnsys/g2d/load/g2d_NAN_load.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "check failed: %s, line %d\n", #x, __LINE__); exit(42); } } while (0)
extern void WIN_CheckAndFreeCharData(void *, void *);
extern void WIN_CheckAndFreeCellBank(void *, void *);
extern void WIN_CheckAndFreeAnimBank(void *, void *);

static unsigned le16(const u8 *p) { return p[0] | (p[1] << 8); }
static unsigned le32(const u8 *p) { return le16(p) | (le16(p + 2) << 16); }
static void inside(const u8 *base, size_t size, const void *ptr, size_t bytes) {
    uintptr_t offset = (uintptr_t)ptr - (uintptr_t)base;
    CHECK(offset <= size && bytes <= size - offset);
}

static void verify(u8 *data, size_t size, int member) {
    const u8 *disk = data + 24;
    /* Registration is associated with output-pointer storage in this SDK. */
    struct { uintptr_t guard; NNSG2dCharacterData *chr; NNSG2dCellDataBank *cell;
             NNSG2dAnimBankData *anim; uintptr_t end; } handles = {0};
    if (!memcmp(data, "RGCN", 4)) {
        CHECK(member == 54 ? NNS_G2dGetUnpackedBGCharacterData(data, &handles.chr)
                           : NNS_G2dGetUnpackedCharacterData(data, &handles.chr));
        NNSG2dCharacterData *out = handles.chr;
        CHECK(out && out->H == le16(disk) && out->W == le16(disk + 2));
        CHECK(out->pixelFmt == le32(disk + 4) && out->mappingType == le32(disk + 8));
        CHECK(out->characterFmt == le32(disk + 12) && out->szByte == le32(disk + 16));
        CHECK(out->pRawData == disk + le32(disk + 20));
        inside(data, size, out->pRawData, out->szByte);
        WIN_CheckAndFreeCharData(&handles, &handles + 1);
    } else if (!memcmp(data, "RCSN", 4)) {
        NNSG2dScreenData *out = NULL;
        CHECK(NNS_G2dGetUnpackedScreenData(data, &out));
        CHECK((const u8 *)out == disk);
        CHECK(out->screenWidth == 256 && out->screenHeight == 256);
        CHECK(out->szByte == 2048);
        inside(data, size, out->rawData, out->szByte);
    } else if (!memcmp(data, "RECN", 4)) {
        CHECK(NNS_G2dGetUnpackedCellBank(data, &handles.cell));
        NNSG2dCellDataBank *out = handles.cell;
        CHECK(out && out->numCells == le16(disk));
        CHECK(out->cellBankAttr == le16(disk + 2));
        CHECK(out->mappingMode == le32(disk + 8));
        unsigned stride = (out->cellBankAttr & NNS_G2D_CELLBK_ATTR_CELLWITHBR) ? 16 : 8;
        const u8 *cells = disk + le32(disk + 4);
        inside(data, size, cells, stride * out->numCells);
        const u8 *oam = cells + stride * out->numCells;
        for (unsigned i = 0; i < out->numCells; ++i) {
            const NNSG2dCellData *cell = NNS_G2dGetCellDataByIdx(out, i);
            const u8 *src = cells + stride * i;
            CHECK(cell && cell->numOAMAttrs == le16(src) && cell->cellAttr == le16(src + 2));
            CHECK((const u8 *)cell->pOamAttrArray == oam + le32(src + 4));
            inside(data, size, cell->pOamAttrArray, cell->numOAMAttrs * 6);
            if (stride == 16) {
                const NNSG2dCellDataWithBR *br = (const void *)cell;
                CHECK(memcmp(&br->boundingRect, src + 8, 8) == 0);
            }
        }
        CHECK(NNS_G2dGetCellDataByIdx(out, out->numCells) == NULL);
        WIN_CheckAndFreeCellBank(&handles, &handles + 1);
    } else if (!memcmp(data, "RNAN", 4)) {
        CHECK(NNS_G2dGetUnpackedAnimBank(data, &handles.anim));
        NNSG2dAnimBankData *out = handles.anim;
        CHECK(out && out->numSequences == le16(disk) && out->numTotalFrames == le16(disk + 2));
        /* These scene fixtures have neither optional bank. Absence must be NULL. */
        CHECK(le32(disk + 16) == 0 && le32(disk + 20) == 0);
        CHECK(out->pStringBank == NULL && out->pExtendedData == NULL);
        const u8 *seqs = disk + le32(disk + 4);
        const u8 *frames = disk + le32(disk + 8);
        const u8 *contents = disk + le32(disk + 12);
        inside(data, size, seqs, out->numSequences * 16);
        unsigned total = 0;
        for (unsigned i = 0; i < out->numSequences; ++i) {
            const NNSG2dAnimSequenceData *seq = NNS_G2dGetAnimSequenceByIdx(out, i);
            const u8 *src = seqs + 16 * i;
            CHECK(seq && seq->numFrames == le16(src) && seq->loopStartFrameIdx == le16(src + 2));
            CHECK(seq->animType == le32(src + 4) && seq->playMode == le32(src + 8));
            const u8 *sequenceFrames = frames + le32(src + 12);
            inside(data, size, sequenceFrames, seq->numFrames * 8);
            for (unsigned j = 0; j < seq->numFrames; ++j) {
                const u8 *frame = sequenceFrames + 8 * j;
                CHECK(seq->pAnmFrameArray[j].frames == le16(frame + 4));
                CHECK(seq->pAnmFrameArray[j].pContent == contents + le32(frame));
                unsigned element = seq->animType & 255;
                CHECK(element <= 2);
                inside(data, size, seq->pAnmFrameArray[j].pContent, element == 0 ? 2 : element == 1 ? 16 : 8);
            }
            total += seq->numFrames;
        }
        CHECK(total == out->numTotalFrames);
        CHECK(NNS_G2dGetAnimSequenceByIdx(out, out->numSequences) == NULL);
        WIN_CheckAndFreeAnimBank(&handles, &handles + 1);
    } else CHECK(0);
}

int main(int argc, char **argv) {
    CHECK(argc == 18);
    for (int i = 1; i < argc; ++i) {
        FILE *file = fopen(argv[i], "rb"); CHECK(file);
        CHECK(fseek(file, 0, SEEK_END) == 0);
        long length = ftell(file); CHECK(length >= 48 && length < 1024 * 1024);
        rewind(file);
        u8 *data = malloc(length), *before = malloc(length); CHECK(data && before);
        CHECK(fread(data, 1, length, file) == (size_t)length); fclose(file);
        memcpy(before, data, length);
        const char *name = strrchr(argv[i], '/'); CHECK(name);
        int member = atoi(name + 1);
        for (unsigned round = 0; round < 512; ++round) {
            verify(data, length, member);
            CHECK(memcmp(data, before, length) == 0);
        }
        printf("member %d: 512 decode/free cycles passed\n", member);
        free(before); free(data);
    }
    return 0;
}
