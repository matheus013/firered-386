#ifndef HGSS_NATIVE_OVERLAY_BRIDGE_H
#define HGSS_NATIVE_OVERLAY_BRIDGE_H
#include "hg/native_overlays.h"
/* Bind only while no previous registry is bound. Unbind requires inactivity.
 * Registry storage must outlive the binding; do not close or reinitialize it
 * before unbinding. Modules are already linked, so all load modes complete
 * synchronously. No ROM data or ARM code is loaded by this bridge.
 */
int HGSS_BindNativeOverlays(HgNativeOverlayRegistry *registry);
int HGSS_UnbindNativeOverlays(void);
#endif
