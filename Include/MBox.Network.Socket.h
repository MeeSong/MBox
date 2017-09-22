#pragma once

enum AddressFamily : ktl::u16
{
    Unspecified = 0,
    Unix        = 1,
    IPv4        = 2,
    IPX         = 6,
    AppleTalk   = 16,
    NetBios     = 17,
    ATM         = 22,
    IPv6        = 23,
    InfraredData = 26,
    Bluetooth   = 32,

    Max         = 35,
};
