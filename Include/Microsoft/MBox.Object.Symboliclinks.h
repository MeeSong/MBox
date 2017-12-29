#include "MBox.Object.h"


#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    enum SymboliclinkAccessMask : UINT32
    {
        SymboliclinkQuery   = 0x00000001,
        SymboliclinkSet     = 0x00000002,

        SymboliclinkAllAccess   = StandardRightsRequired | SymboliclinkQuery,
        SymboliclinkAllAccessEx = StandardRightsRequired | 0xFFFF,
    };

    extern"C"
    {
        NTSTATUS NTAPI ZwOpenSymbolicLinkObject(
            PHANDLE aLinkHandle,
            ACCESS_MASK aDesiredAccess,
            ObjectAttributes* aObjectAttributes);

        NTSTATUS NTAPI ZwQuerySymbolicLinkObject(
            HANDLE LinkHandle,
            UnicodeString* LinkTarget,
            UINT32* ReturnedLength);

    }
}
#pragma warning(pop)
