#include <nitro/rtc.h>
#include <stdint.h>
int64_t hg_libntr_seconds(unsigned y,unsigned m,unsigned d,unsigned w,unsigned h,unsigned n,unsigned s) {
    RTCDate date={y,m,d,w};
    RTCTime time={h,n,s};
    return RTC_ConvertDateTimeToSecond(&date,&time);
}
