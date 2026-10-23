#include "global.h"
#include "game_settings.h"
#include "localization.h"
#include "string_util.h"
#include "characters.h"
struct LocaleEntry { u32 hash; const u8 *en; const u8 *pt; };
#include "data/localization_ptbr.h"
const u8 *L10n_GetText(const u8 *source)
{
    u32 hash=2166136261u;
    const u8 *cursor=source;
    s32 low=0,high=ARRAY_COUNT(sPtLocale)-1,mid;
    u16 length=0;
    if(source==NULL || GameSettings_GetLanguage()!=GAME_LANGUAGE_PTBR)return source;
    while(*cursor!=EOS && length<1000){hash=(hash^*cursor++)*16777619;length++;}
    if(length==1000)return source;
    while(low<=high)
    {
        mid=(low+high)/2;
        if(sPtLocale[mid].hash<hash)low=mid+1;
        else high=mid-1;
    }
    for(;low<ARRAY_COUNT(sPtLocale) && sPtLocale[low].hash==hash;low++)
        if(StringCompare(source,sPtLocale[low].en)==0)return sPtLocale[low].pt;
    return source;
}
