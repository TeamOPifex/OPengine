#pragma once

struct OPnetworkSocketType {
    enum Enum {
        STREAM = 0, // SOCK_STREAM
        DGRAM = 1, // SOCK_DGRAM
        RAW = 2, // SOCK_RAW
        DGRAM_SEQ = 3 // SOCK_SEQPACKET
    };
};