#pragma once

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
