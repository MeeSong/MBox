#pragma once
#include <MBox.Network.Socket.h>
#include <fwptypes.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace Utilities
        {
            AddressFamily GetAddressFamilyForLayer(UINT16 aLayerId);

            FWP_DIRECTION GetPacketDirectionForLayer(UINT16 aLayerId);

        }
    }
}
