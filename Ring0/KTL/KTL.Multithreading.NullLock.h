#pragma once

namespace ktl
{
    inline namespace multithreading
    {
        class null_lock
        {
            bool try_lock() { return true; }
            bool lock() { return true; }
            bool unlock() { return true; }

            ~null_lock() { unlock(); }
        };
    }
}
