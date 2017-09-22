#pragma once
#include <fwpmk.h>
#include <fwptypes.h>
#include <MBox.Network.Socket.h>
#include <Vol.Kernel\Vol.System.h>


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
