"""Add native per-scanline HBlank window snapshots for text backgrounds."""
from pathlib import Path

HEADER = '''#pragma once
#include <nitro.h>
struct HGWindowLine {
    u32 display;
    u16 h0, h1, v0, v1, inside, outside;
};
extern HGWindowLine hg_window_lines[2][192];
'''
CAPTURE = '''
// Snapshot native HBlank-driven window state before rasterizing text BGs.
HGWindowLine hg_window_lines[2][192];
static void HG_CaptureWindowLines(void) {
  u16 savedCount = s_reg_GX_VCOUNT;
  for (unsigned line = 0; line < 192; ++line) {
    // HGSS HBlank handlers prepare the next visible line.
    s_reg_GX_VCOUNT = (line + 191) % 192;
    s_reg_GX_DISPSTAT |= REG_GX_DISPSTAT_HBLK_MASK;
    if ((reg_OS_IME & 1) && (reg_OS_IE & OS_IE_H_BLANK) &&
        (s_reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBI_MASK))
      OS_IRQTable[OS_IE_H_BLANK - 1]();
    s_reg_GX_DISPSTAT &= ~REG_GX_DISPSTAT_HBLK_MASK;
    hg_window_lines[0][line] = {s_reg_GX_DISPCNT, s_reg_G2_WIN0H, s_reg_G2_WIN1H,
      s_reg_G2_WIN0V, s_reg_G2_WIN1V, s_reg_G2_WININ, s_reg_G2_WINOUT};
    hg_window_lines[1][line] = {s_reg_GXS_DB_DISPCNT, s_reg_G2S_DB_WIN0H, s_reg_G2S_DB_WIN1H,
      s_reg_G2S_DB_WIN0V, s_reg_G2S_DB_WIN1V, s_reg_G2S_DB_WININ, s_reg_G2S_DB_WINOUT};
  }
  s_reg_GX_VCOUNT = savedCount;
}
'''


def replace_once(text, before, after):
    if text.count(before) != 1: raise ValueError('Native scanline source contract differs: '+before[:60])
    return text.replace(before,after)


def prepare(source):
    irq=source/'include/nitro/os/common/interrupt.h'
    irq_text=irq.read_text()
    old_irq='  #ifdef SDK_PORT\n  return TRUE;\n  #else\n\tu16 prep = reg_OS_IME;\n\treg_OS_IME = OS_IME_ENABLE;\n\treturn (BOOL)prep;\n  #endif'
    # Native registers must observe the same enable/disable/restore contract.
    new_irq='\tu16 prep = reg_OS_IME;\n\treg_OS_IME = OS_IME_ENABLE;\n\treturn (BOOL)prep;'
    if old_irq in irq_text:
        irq.write_text(replace_once(irq_text,old_irq,new_irq))
    elif new_irq not in irq_text:
        raise ValueError('IRQ enable contract differs')
    main=source/'libraries/sim/src/sim_main.cpp'
    g2=source/'libraries/sim/src/sim_g2.cpp'
    if 'HG_CaptureWindowLines' in main.read_text():
        if 'hg_window_lines[isSub != 0][line]' not in g2.read_text(): raise ValueError('Incomplete scanline adaptation')
        return
    m=main.read_text(); g=g2.read_text()
    m=replace_once(m,'static void DrawEngine(BOOL isSub) {', '#include <simulator/hg_scanline_windows.h>\n'+CAPTURE+'\nstatic void DrawEngine(BOOL isSub) {')
    m=replace_once(m,'    // Draw MAIN engine\n    DrawEngine(FALSE);','    HG_CaptureWindowLines();\n\n    // Draw MAIN engine\n    DrawEngine(FALSE);')
    m=replace_once(m,'    // Run HBlank Interrupt\n    OS_IRQTable[OS_IE_H_BLANK - 1]();','    // HBlank window callbacks were evaluated per scanline before drawing.')
    g=replace_once(g,'#include <nitro.h>','#include <nitro.h>\n#include <simulator/hg_scanline_windows.h>')
    before='''      u8 win0Enabled = (dispcnt >> 13) & 1;
      u8 win1Enabled = (dispcnt >> 14) & 1;

      u16 win0H = isSub ? s_reg_G2S_DB_WIN0H : s_reg_G2_WIN0H;
      u16 win1H = isSub ? s_reg_G2S_DB_WIN1H : s_reg_G2_WIN1H;

      u16 win0V = isSub ? s_reg_G2S_DB_WIN0V : s_reg_G2_WIN0V;
      u16 win1V = isSub ? s_reg_G2S_DB_WIN1V : s_reg_G2_WIN1V;

      u16 winIn = isSub ? s_reg_G2S_DB_WININ : s_reg_G2_WININ;
      u16 winOut = isSub ? s_reg_G2S_DB_WINOUT : s_reg_G2_WINOUT;'''
    after='''      const HGWindowLine &windowState = hg_window_lines[isSub != 0][line];
      u8 win0Enabled = (windowState.display >> 13) & 1;
      u8 win1Enabled = (windowState.display >> 14) & 1;
      u16 win0H = windowState.h0, win1H = windowState.h1;
      u16 win0V = windowState.v0, win1V = windowState.v1;
      u16 winIn = windowState.inside, winOut = windowState.outside;'''
    if g.count(before)!=2: raise ValueError('Expected two text BG window paths')
    g=g.replace(before,after)
    (source/'include/simulator/hg_scanline_windows.h').write_text(HEADER)
    main.write_text(m);g2.write_text(g)

if __name__=='__main__':
    import argparse
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--source',type=Path,required=True)
    prepare(parser.parse_args().source)
