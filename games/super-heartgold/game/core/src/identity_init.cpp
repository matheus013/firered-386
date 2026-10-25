#include "hg/identity_init.hpp"
#include <algorithm>

namespace hg {

PreclearedMailboxBlock initialize_precleared_mailbox(GameIdentity identity) {
    PreclearedMailboxBlock result{};
    for (std::size_t slot = 0; slot < 20; ++slot) {
        const auto start = slot * 56;
        result.bytes[start + 5] = static_cast<std::byte>(identity.language);
        result.bytes[start + 6] = static_cast<std::byte>(identity.version);
        result.bytes[start + 7] = std::byte{0xff}; // MAIL_NONE
        std::fill_n(result.bytes.begin() + start + 8, 22, std::byte{0xff}); // name/icons
        for (std::size_t message = 0; message < 3; ++message) {
            const auto at = start + 32 + message * 8;
            std::fill_n(result.bytes.begin() + at, 2, std::byte{0xff}); // no message bank
            // msg_no at +2 is zero due to pre-clear, not MailMsg_Init.
            std::fill_n(result.bytes.begin() + at + 4, 4, std::byte{0xff}); // empty EC words
        }
    }
    return result;
}

PreclearedSafariBlock initialize_precleared_safari(GameIdentity identity) {
    PreclearedSafariBlock result{};
    result.bytes[0x5e2] = static_cast<std::byte>(identity.language);
    result.bytes[0x5e3] = static_cast<std::byte>(identity.version);
    std::fill_n(result.bytes.begin() + 0x5e8, 16, std::byte{0xff}); // leader name EOS
    // Area sets and leader have original clear operations. The final level,
    // active-set and unlock-time fields are zero only because of pre-clear.
    return result;
}

} // namespace hg
