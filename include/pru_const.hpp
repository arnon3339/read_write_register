#ifndef PRU_CONST_HPP
#define PRU_CONST_HPP

namespace cmdtype
{
    const uint8_t fmwrw       =   0xAA;   //  RegisterModule Module R/W Request
    const uint8_t alpide_rw   =   0xFF;   //  ALPIDE chip R/W Request
    const uint8_t spcmd       =   0xBB;   //  Special command request
    const uint8_t pru_rply    =   0x03;   //  pRU Reply

    const uint8_t readop      =   0xAA;   //  READ_OPCODE
    const uint8_t writeop     =   0xFF;   //  WRITE_OPCODE
    const uint8_t spwoffld    =   0x10;   //  Deprecated (only used for PTB), Spawn offload thread
    const uint8_t deloffld    =   0x20;   //  Deprecated (only used for PTB), Delete offload thread
} // namespace cmdtype


#endif