#pragma once

namespace MBox
{
    namespace OwlProtocol
    {
        enum class IoCode : UINT32
        {
            BeginCode = 0x800,

            Connecttion         = CTL_CODE(FILE_DEVICE_UNKNOWN, BeginCode + 1, METHOD_BUFFERED,  FILE_ANY_ACCESS),
            Disconnection       = CTL_CODE(FILE_DEVICE_UNKNOWN, BeginCode + 2, METHOD_IN_DIRECT, FILE_ANY_ACCESS),

            UserMessage         = CTL_CODE(FILE_DEVICE_UNKNOWN, BeginCode + 3, METHOD_OUT_DIRECT,FILE_ANY_ACCESS),

            KernelMessage       = CTL_CODE(FILE_DEVICE_UNKNOWN, BeginCode + 4, METHOD_OUT_DIRECT,FILE_ANY_ACCESS),
            ReplyKernelMessage  = CTL_CODE(FILE_DEVICE_UNKNOWN, BeginCode + 5, METHOD_IN_DIRECT, FILE_ANY_ACCESS),
        };

#pragma pack(push, 4)
        struct ConnectContextHeader
        {
            IN  UINT64 m_ThreadHandle    = 0;
            IN  UINT64 m_NotifySemaphore = 0;
        };
        
        struct ReplyHeader
        {
            UINT64      m_MessageId = 0;
            NTSTATUS    m_Status    = 0; // sizeof(NTSTATUS) == sizeof(UINT32)
            UINT32      m_Reserve   = 0;
        };

        struct MessageHeader
        {
            UINT64  m_MessageId     = 0;
            UINT32  m_MessageBytes  = 0;
            UINT32  m_ReplyBytes    = 0;
        };
#pragma pack(pop)

    }
}
