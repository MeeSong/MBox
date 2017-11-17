#pragma once


namespace MBox::Vol::System
{
    bool Is64BitSystem();

    inline bool Is32BitSystem() { return (!Is64BitSystem()); };
}
