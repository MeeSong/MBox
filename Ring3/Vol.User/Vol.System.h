#pragma once


namespace MBox::Vol::System
{
    BOOLEAN Is64BitSystem();

    inline BOOLEAN Is32BitSystem() { return (!Is64BitSystem()); };
}
