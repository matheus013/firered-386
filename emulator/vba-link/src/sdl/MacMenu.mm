// Native macOS menu bar and dialogs for the SDL frontend.
#import <Cocoa/Cocoa.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <stdlib.h>

#include "MacMenu.h"

#define LINK_STATUS_TAG 9001
#define LINK_ADDRESS_TAG 9002

@interface VBAMenuTarget : NSObject <NSMenuItemValidation>
@end

@implementation VBAMenuTarget
- (void)fire:(NSMenuItem *)item
{
  sdlMacCommand((int)[item tag], [[item representedObject] intValue]);
}

- (BOOL)validateMenuItem:(NSMenuItem *)item
{
  if([item tag] == LINK_STATUS_TAG) {
    char status[128];
    sdlMacLinkStatus(status, sizeof(status));
    [item setTitle:[NSString stringWithUTF8String:status]];
    return NO;
  }
  if([item tag] == LINK_ADDRESS_TAG) {
    char address[64];
    macLocalAddress(address, sizeof(address));
    [item setTitle:[NSString stringWithFormat:@"IP deste Mac: %s", address]];
    return NO;
  }
  int state = sdlMacItemState((int)[item tag], [[item representedObject] intValue]);
  [item setState:(state & MAC_ITEM_CHECKED) ? NSControlStateValueOn : NSControlStateValueOff];
  return (state & MAC_ITEM_DISABLED) ? NO : YES;
}

- (void)about:(id)sender
{
  macShowMessage("VisualBoyAdvance Link",
                 "VisualBoyAdvance 1.8.0 com suporte a Link LAN.\n"
                 "Port Linux de VBA Link, compilado para macOS.");
}
@end

static VBAMenuTarget *menuTarget = nil;

static NSMenuItem *addItem(NSMenu *menu, NSString *title, int cmd, int arg,
                           NSString *key = @"",
                           NSEventModifierFlags mods = NSEventModifierFlagCommand)
{
  NSMenuItem *item = [[NSMenuItem alloc] initWithTitle:title
                                                action:@selector(fire:)
                                         keyEquivalent:key];
  [item setKeyEquivalentModifierMask:mods];
  [item setTarget:menuTarget];
  [item setTag:cmd];
  [item setRepresentedObject:[NSNumber numberWithInt:arg]];
  [menu addItem:item];
  return item;
}

static NSMenu *addSubmenu(NSMenu *parent, NSString *title)
{
  NSMenuItem *holder = [[NSMenuItem alloc] initWithTitle:title action:nil keyEquivalent:@""];
  NSMenu *menu = [[NSMenu alloc] initWithTitle:title];
  [holder setSubmenu:menu];
  [parent addItem:holder];
  return menu;
}

static void addSeparator(NSMenu *menu)
{
  [menu addItem:[NSMenuItem separatorItem]];
}

static NSMenu *buildFileMenu()
{
  NSMenu *m = [[NSMenu alloc] initWithTitle:@"Arquivo"];
  addItem(m, @"Abrir ROM…", MAC_CMD_OPEN, 0, @"o");
  addSeparator(m);
  addItem(m, @"Carregar estado…", MAC_CMD_LOAD_STATE_FILE, 0, @"l");
  addItem(m, @"Salvar estado…", MAC_CMD_SAVE_STATE_FILE, 0, @"s");
  NSMenu *load = addSubmenu(m, @"Carregar slot");
  NSMenu *save = addSubmenu(m, @"Salvar slot");
  for(int i = 0; i < 10; i++) {
    addItem(load, [NSString stringWithFormat:@"Slot %d (F%d)", i + 1, i + 1],
            MAC_CMD_LOAD_SLOT, i);
    addItem(save, [NSString stringWithFormat:@"Slot %d (Shift+F%d)", i + 1, i + 1],
            MAC_CMD_SAVE_SLOT, i);
  }
  addSeparator(m);
  addItem(m, @"Pausar", MAC_CMD_PAUSE, 0, @"p");
  addItem(m, @"Reiniciar", MAC_CMD_RESET, 0, @"r");
  addSeparator(m);
  addItem(m, @"Importar save (.sav)…", MAC_CMD_IMPORT_BATTERY, 0);
  addItem(m, @"Exportar save (.sav)…", MAC_CMD_EXPORT_BATTERY, 0);
  addItem(m, @"Capturar tela…", MAC_CMD_SCREEN_CAPTURE, 0);
  addSeparator(m);
  addItem(m, @"Fechar", MAC_CMD_CLOSE, 0);
  return m;
}

static NSMenu *buildOptionsMenu()
{
  NSMenu *m = [[NSMenu alloc] initWithTitle:@"Opções"];

  NSMenu *speed = addSubmenu(m, @"Velocidade");
  addItem(speed, @"Sem limite", MAC_CMD_THROTTLE, 0);
  int throttles[] = { 25, 50, 100, 150, 200 };
  for(int i = 0; i < 5; i++)
    addItem(speed, [NSString stringWithFormat:@"%d%%", throttles[i]],
            MAC_CMD_THROTTLE, throttles[i]);
  addSeparator(speed);
  addItem(speed, @"Turbo", MAC_CMD_SPEEDUP, 0, @"t");

  NSMenu *skip = addSubmenu(m, @"Frame skip");
  for(int i = 0; i <= 5; i++)
    addItem(skip, [NSString stringWithFormat:@"%d", i], MAC_CMD_FRAMESKIP, i);
  addSeparator(skip);
  addItem(skip, @"Automático", MAC_CMD_AUTO_FRAMESKIP, 0);

  NSMenu *video = addSubmenu(m, @"Vídeo");
  for(int i = 0; i < 4; i++)
    addItem(video, [NSString stringWithFormat:@"x%d", i + 1], MAC_CMD_SIZE, i,
            [NSString stringWithFormat:@"%d", i + 1]);
  addItem(video, @"Tela cheia", MAC_CMD_FULLSCREEN, 0, @"f",
          NSEventModifierFlagCommand | NSEventModifierFlagControl);
  addSeparator(video);
  NSMenu *layers = addSubmenu(video, @"Camadas");
  const char *layerNames[] = { "BG 0", "BG 1", "BG 2", "BG 3", "OBJ",
                               "WIN 0", "WIN 1", "OBJ WIN" };
  for(int i = 0; i < 8; i++)
    addItem(layers, [NSString stringWithUTF8String:layerNames[i]], MAC_CMD_LAYER, i);
  NSMenu *showSpeed = addSubmenu(video, @"Mostrar velocidade");
  addItem(showSpeed, @"Não", MAC_CMD_SHOW_SPEED, 0);
  addItem(showSpeed, @"Porcentagem", MAC_CMD_SHOW_SPEED, 1);
  addItem(showSpeed, @"Detalhado", MAC_CMD_SHOW_SPEED, 2);

  NSMenu *filter = addSubmenu(m, @"Filtro");
  const char *filterNames[] = { "Normal", "TV Mode", "2xSaI", "Super 2xSaI",
                                "Super Eagle", "Pixelate", "Motion Blur",
                                "AdvanceMAME Scale2x", "Simple 2x", "Bilinear",
                                "Bilinear Plus", "Scanlines", "hq2x", "lq2x" };
  for(int i = 0; i < 14; i++)
    addItem(filter, [NSString stringWithUTF8String:filterNames[i]], MAC_CMD_FILTER, i);
  addSeparator(filter);
  NSMenu *ifb = addSubmenu(filter, @"Mistura entre quadros");
  addItem(ifb, @"Nenhuma", MAC_CMD_IFB, 0);
  addItem(ifb, @"Motion Blur", MAC_CMD_IFB, 1);
  addItem(ifb, @"Smart", MAC_CMD_IFB, 2);

  NSMenu *emu = addSubmenu(m, @"Emulador");
  addItem(emu, @"Pausar quando a janela estiver inativa", MAC_CMD_PAUSE_INACTIVE, 0);
  addItem(emu, @"Mensagens de status", MAC_CMD_STATUS_MESSAGES, 0);
  addSeparator(emu);
  NSMenu *saveType = addSubmenu(emu, @"Tipo de save");
  const char *saveNames[] = { "Automático", "EEPROM", "SRAM", "Flash",
                              "EEPROM+Sensor", "Nenhum" };
  for(int i = 0; i < 6; i++)
    addItem(saveType, [NSString stringWithUTF8String:saveNames[i]], MAC_CMD_SAVE_TYPE, i);
  addSeparator(saveType);
  addItem(saveType, @"Flash 64K", MAC_CMD_FLASH_SIZE, 0);
  addItem(saveType, @"Flash 128K", MAC_CMD_FLASH_SIZE, 1);

  NSMenu *sound = addSubmenu(m, @"Som");
  addItem(sound, @"Mudo", MAC_CMD_MUTE, 0);
  NSMenu *volume = addSubmenu(sound, @"Volume");
  for(int i = 0; i < 4; i++)
    addItem(volume, [NSString stringWithFormat:@"%dx", i + 1], MAC_CMD_VOLUME, i);

  NSMenu *joypad = addSubmenu(m, @"Controle");
  NSMenu *autofire = addSubmenu(joypad, @"Autofire");
  addItem(autofire, @"A", MAC_CMD_AUTOFIRE, 1);
  addItem(autofire, @"B", MAC_CMD_AUTOFIRE, 2);
  addItem(autofire, @"L", MAC_CMD_AUTOFIRE, 512);
  addItem(autofire, @"R", MAC_CMD_AUTOFIRE, 256);
  addItem(joypad, @"Ver teclas…", MAC_CMD_CONTROLS, 0);
  return m;
}

static NSMenu *buildLinkMenu()
{
  NSMenu *m = [[NSMenu alloc] initWithTitle:@"Link"];
  NSMenuItem *status = [[NSMenuItem alloc] initWithTitle:@"Link" action:@selector(fire:)
                                           keyEquivalent:@""];
  [status setTarget:menuTarget];
  [status setTag:LINK_STATUS_TAG];
  [m addItem:status];
  addSeparator(m);
  NSMenu *host = addSubmenu(m, @"Hospedar partida (servidor)");
  for(int i = 2; i <= 4; i++)
    addItem(host, [NSString stringWithFormat:@"%d jogadores", i], MAC_CMD_LINK_HOST, i);
  addItem(m, @"Conectar a um servidor…", MAC_CMD_LINK_JOIN, 0);
  addItem(m, @"Desconectar", MAC_CMD_LINK_DISCONNECT, 0);
  addSeparator(m);
  NSMenu *timeout = addSubmenu(m, @"Tempo limite");
  int timeouts[] = { 500, 1000, 2000, 5000 };
  for(int i = 0; i < 4; i++)
    addItem(timeout, [NSString stringWithFormat:@"%d ms", timeouts[i]],
            MAC_CMD_LINK_TIMEOUT, timeouts[i]);
  addSeparator(m);
  NSMenuItem *address = [[NSMenuItem alloc] initWithTitle:@"IP" action:@selector(fire:)
                                            keyEquivalent:@""];
  [address setTarget:menuTarget];
  [address setTag:LINK_ADDRESS_TAG];
  [m addItem:address];
  return m;
}

static NSMenu *buildHelpMenu()
{
  NSMenu *m = [[NSMenu alloc] initWithTitle:@"Ajuda"];
  addItem(m, @"Teclas do controle…", MAC_CMD_CONTROLS, 0);
  NSMenuItem *about = [[NSMenuItem alloc] initWithTitle:@"Sobre o VisualBoyAdvance Link"
                                                 action:@selector(about:)
                                          keyEquivalent:@""];
  [about setTarget:menuTarget];
  [m addItem:about];
  return m;
}

static void addTopMenu(NSMenu *bar, NSMenu *menu, NSInteger index)
{
  NSMenuItem *holder = [[NSMenuItem alloc] initWithTitle:[menu title] action:nil
                                           keyEquivalent:@""];
  [holder setSubmenu:menu];
  if(index < 0 || index > [bar numberOfItems])
    [bar addItem:holder];
  else
    [bar insertItem:holder atIndex:index];
}

void macInitApp()
{
  @autoreleasepool {
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
  }
}

void macInstallMenu()
{
  @autoreleasepool {
    if(menuTarget == nil)
      menuTarget = [[VBAMenuTarget alloc] init];

    NSMenu *bar = [NSApp mainMenu];
    if(bar == nil) {
      bar = [[NSMenu alloc] init];
      NSMenu *app = addSubmenu(bar, @"VisualBoyAdvance");
      [app addItemWithTitle:@"Sair do VisualBoyAdvance" action:@selector(terminate:)
              keyEquivalent:@"q"];
      [NSApp setMainMenu:bar];
    }

    // Keep the application menu first; put ours before SDL's Window menu.
    NSInteger index = [bar numberOfItems] > 1 ? 1 : -1;
    NSMenu *menus[] = { buildFileMenu(), buildOptionsMenu(), buildLinkMenu() };
    for(int i = 0; i < 3; i++)
      addTopMenu(bar, menus[i], index < 0 ? -1 : index + i);
    addTopMenu(bar, buildHelpMenu(), -1);
  }
}

char *macChooseFile(const char *title, const char *exts, bool save,
                    const char *defaultName)
{
  @autoreleasepool {
    [NSApp activateIgnoringOtherApps:YES];
    NSSavePanel *panel = save ? [NSSavePanel savePanel] : [NSOpenPanel openPanel];
    [panel setTitle:[NSString stringWithUTF8String:title]];
    [panel setMessage:[NSString stringWithUTF8String:title]];
    if(exts) {
      NSArray *types = [[NSString stringWithUTF8String:exts] componentsSeparatedByString:@","];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
      [panel setAllowedFileTypes:types];
#pragma clang diagnostic pop
      [panel setAllowsOtherFileTypes:YES];
    }
    if(save && defaultName)
      [panel setNameFieldStringValue:[NSString stringWithUTF8String:defaultName]];
    if([panel runModal] != NSModalResponseOK)
      return NULL;
    return strdup([[[panel URL] path] fileSystemRepresentation]);
  }
}

bool macAskText(const char *title, const char *message, const char *defaultValue,
                char *out, int outSize)
{
  @autoreleasepool {
    [NSApp activateIgnoringOtherApps:YES];
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:[NSString stringWithUTF8String:title]];
    [alert setInformativeText:[NSString stringWithUTF8String:message]];
    [alert addButtonWithTitle:@"OK"];
    [alert addButtonWithTitle:@"Cancelar"];
    NSTextField *field = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 240, 24)];
    [field setStringValue:[NSString stringWithUTF8String:defaultValue ? defaultValue : ""]];
    [alert setAccessoryView:field];
    [[alert window] setInitialFirstResponder:field];
    if([alert runModal] != NSAlertFirstButtonReturn)
      return false;
    NSString *value = [[field stringValue]
                        stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    if([value length] == 0)
      return false;
    strncpy(out, [value UTF8String], outSize - 1);
    out[outSize - 1] = 0;
    return true;
  }
}

void macShowMessage(const char *title, const char *text)
{
  @autoreleasepool {
    [NSApp activateIgnoringOtherApps:YES];
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:[NSString stringWithUTF8String:title]];
    [alert setInformativeText:[NSString stringWithUTF8String:text]];
    [alert runModal];
  }
}

bool macConfirm(const char *title, const char *text, const char *okButton)
{
  @autoreleasepool {
    [NSApp activateIgnoringOtherApps:YES];
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:[NSString stringWithUTF8String:title]];
    [alert setInformativeText:[NSString stringWithUTF8String:text]];
    [alert addButtonWithTitle:[NSString stringWithUTF8String:okButton]];
    [alert addButtonWithTitle:@"Cancelar"];
    return [alert runModal] == NSAlertFirstButtonReturn;
  }
}

bool macWaitFor(bool (*isReady)(), const char *title, const char *text)
{
  @autoreleasepool {
    if(isReady())
      return true;
    [NSApp activateIgnoringOtherApps:YES];
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:[NSString stringWithUTF8String:title]];
    [alert setInformativeText:[NSString stringWithUTF8String:text]];
    [alert addButtonWithTitle:@"Cancelar"];
    NSTimer *timer = [NSTimer timerWithTimeInterval:0.25 repeats:YES block:^(NSTimer *t) {
        if(isReady())
          [NSApp abortModal];
      }];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSModalPanelRunLoopMode];
    NSModalResponse response = [alert runModal];
    [timer invalidate];
    return response == NSModalResponseAbort;
  }
}

void macLocalAddress(char *out, int outSize)
{
  strncpy(out, "127.0.0.1", outSize - 1);
  out[outSize - 1] = 0;
  struct ifaddrs *list;
  if(getifaddrs(&list) != 0)
    return;
  bool found = false;
  for(struct ifaddrs *a = list; a; a = a->ifa_next) {
    if(!a->ifa_addr || a->ifa_addr->sa_family != AF_INET)
      continue;
    if((a->ifa_flags & IFF_LOOPBACK) || !(a->ifa_flags & IFF_UP))
      continue;
    // Prefer Wi-Fi/Ethernet (en*) over VPN and bridge interfaces.
    bool preferred = strncmp(a->ifa_name, "en", 2) == 0;
    if(found && !preferred)
      continue;
    inet_ntop(AF_INET, &((struct sockaddr_in *)a->ifa_addr)->sin_addr, out, outSize);
    found = true;
    if(preferred)
      break;
  }
  freeifaddrs(list);
}

static NSString *prefKey(const char *key)
{
  return [NSString stringWithFormat:@"vba.%s", key];
}

int macPrefInt(const char *key, int defaultValue)
{
  @autoreleasepool {
    NSUserDefaults *d = [NSUserDefaults standardUserDefaults];
    if([d objectForKey:prefKey(key)] == nil)
      return defaultValue;
    return (int)[d integerForKey:prefKey(key)];
  }
}

void macSetPrefInt(const char *key, int value)
{
  @autoreleasepool {
    [[NSUserDefaults standardUserDefaults] setInteger:value forKey:prefKey(key)];
  }
}

void macPrefString(const char *key, const char *defaultValue, char *out, int outSize)
{
  @autoreleasepool {
    NSString *value = [[NSUserDefaults standardUserDefaults] stringForKey:prefKey(key)];
    strncpy(out, value ? [value UTF8String] : defaultValue, outSize - 1);
    out[outSize - 1] = 0;
  }
}

void macSetPrefString(const char *key, const char *value)
{
  @autoreleasepool {
    [[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithUTF8String:value]
                                              forKey:prefKey(key)];
  }
}
