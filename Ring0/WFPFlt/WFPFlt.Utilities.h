#pragma once
#include <MBox.Network.Socket.h>


namespace MBox
{
    namespace WFPFlt
    {
        namespace Utilities
        {
            BOOLEAN IsSupportedWFP();

            AddressFamily GetAddressFamilyForLayer(UINT16 aLayerId);

            FWP_DIRECTION GetPacketDirectionForLayer(UINT16 aLayerId);

        }
    }
}
