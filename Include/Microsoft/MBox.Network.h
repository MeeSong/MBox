#pragma once


#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    enum class NetworkAdapterCharacteristics : UINT32
    {
        NCF_VIRTUAL                     = 0x00000001,
        NCF_SOFTWARE_ENUMERATED         = 0x00000002,
        NCF_PHYSICAL                    = 0x00000004,
        NCF_HIDDEN                      = 0x00000008,
        NCF_NO_SERVICE                  = 0x00000010,
        NCF_NOT_USER_REMOVABLE          = 0x00000020,
        NCF_MULTIPORT_INSTANCED_ADAPTER = 0x00000040,
        NCF_HAS_UI                      = 0x00000080,
        NCF_SINGLE_INSTANCE             = 0x00000100,
        NCF_FILTER                      = 0x00000400,
        NCF_DONTEXPOSELOWER             = 0x00001000,
        NCF_HIDE_BINDING                = 0x00002000,
        NCF_NDIS_PROTOCOL               = 0x00004000,
        NCF_FIXED_BINDING               = 0x00020000,
        NCF_LW_FILTER                   = 0x00040000
    };

}
#pragma warning(pop)
