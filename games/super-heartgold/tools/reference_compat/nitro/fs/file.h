/* Authorial host declarations for the private filesystem source probe.
 * This is not a Nitro SDK header or implementation.
 */
#ifndef HG_HOST_FS_FILE_H
#define HG_HOST_FS_FILE_H
#include "global.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct FSFile { uint64_t token; } FSFile;
typedef enum FSSeekFileMode { FS_SEEK_SET, FS_SEEK_CUR, FS_SEEK_END } FSSeekFileMode;
void FS_InitFile(FSFile *file);
BOOL FS_OpenFile(FSFile *file, const char *path);
BOOL FS_CloseFile(FSFile *file);
BOOL FS_SeekFile(FSFile *file, s32 offset, FSSeekFileMode mode);
s32 FS_ReadFile(FSFile *file, void *destination, s32 size);
#ifdef __cplusplus
}
#endif
#endif
