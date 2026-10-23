// Native macOS menu bar for the SDL frontend.
// The menu only queues commands; SDL.cpp runs them from its main loop.
#ifndef VBA_MACMENU_H
#define VBA_MACMENU_H

enum {
  MAC_CMD_OPEN = 1,
  MAC_CMD_LOAD_STATE_FILE,
  MAC_CMD_SAVE_STATE_FILE,
  MAC_CMD_LOAD_SLOT,          // arg: slot 0-9
  MAC_CMD_SAVE_SLOT,          // arg: slot 0-9
  MAC_CMD_IMPORT_BATTERY,
  MAC_CMD_EXPORT_BATTERY,
  MAC_CMD_SCREEN_CAPTURE,
  MAC_CMD_PAUSE,
  MAC_CMD_RESET,
  MAC_CMD_CLOSE,
  MAC_CMD_THROTTLE,           // arg: 0 (none) or percent
  MAC_CMD_FRAMESKIP,          // arg: 0-5
  MAC_CMD_AUTO_FRAMESKIP,
  MAC_CMD_SIZE,               // arg: 0-3 (x1-x4)
  MAC_CMD_FULLSCREEN,
  MAC_CMD_FILTER,             // arg: filter index
  MAC_CMD_IFB,                // arg: 0-2
  MAC_CMD_LAYER,              // arg: 0-7
  MAC_CMD_SHOW_SPEED,         // arg: 0-2
  MAC_CMD_STATUS_MESSAGES,
  MAC_CMD_PAUSE_INACTIVE,
  MAC_CMD_SPEEDUP,
  MAC_CMD_SAVE_TYPE,          // arg: 0-5 (cpuSaveType)
  MAC_CMD_FLASH_SIZE,         // arg: 0 = 64K, 1 = 128K
  MAC_CMD_MUTE,
  MAC_CMD_VOLUME,             // arg: 0-3
  MAC_CMD_AUTOFIRE,           // arg: autoFire mask
  MAC_CMD_LINK_HOST,          // arg: number of players
  MAC_CMD_LINK_JOIN,
  MAC_CMD_LINK_DISCONNECT,
  MAC_CMD_LINK_TIMEOUT,       // arg: ms
  MAC_CMD_CONTROLS
};

#define MAC_ITEM_CHECKED  1
#define MAC_ITEM_DISABLED 2

// Implemented in SDL.cpp.
void sdlMacCommand(int cmd, int arg);
int sdlMacItemState(int cmd, int arg);
void sdlMacLinkStatus(char *out, int outSize);

// Implemented in MacMenu.mm.
void macInitApp();
void macInstallMenu();
// Returns a malloc'd path, or NULL if cancelled. exts: comma separated, or NULL.
char *macChooseFile(const char *title, const char *exts, bool save,
                    const char *defaultName);
bool macAskText(const char *title, const char *message, const char *defaultValue,
                char *out, int outSize);
void macShowMessage(const char *title, const char *text);
bool macConfirm(const char *title, const char *text, const char *okButton);
// Shows a cancellable panel until isReady() returns true. False if cancelled.
bool macWaitFor(bool (*isReady)(), const char *title, const char *text);
void macLocalAddress(char *out, int outSize);
int macPrefInt(const char *key, int defaultValue);
void macSetPrefInt(const char *key, int value);
void macPrefString(const char *key, const char *defaultValue, char *out, int outSize);
void macSetPrefString(const char *key, const char *value);

#endif
