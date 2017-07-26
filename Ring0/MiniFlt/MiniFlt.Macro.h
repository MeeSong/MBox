#pragma once

#define MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(aFunctionName) \
Traverse ## aFunctionName ## Callback

#define MiniFlt$Function$DeclareTraverseCallbackPacketMember$Macro(aFunctionName) \
template <typename F> \
void MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(aFunctionName)(F aFunctor);

#define MiniFlt$Function$TraverseCallbackPacketMember$Macro(aFunctionName, aMember) \
template <typename F> \
void MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(aFunctionName)(F aFunctor)     \
{                                                                                           \
    auto vCallback = [&aFunctor](ktl::shared_ptr<CallbackPacket> aCallbackPacket, UINT32 aIndex) -> BOOLEAN \
    {                                                                                       \
        if (aCallbackPacket->aMember)                                                    \
        {                                                                                   \
            return aFunctor(aCallbackPacket->aMember, aIndex);                           \
        }                                                                                   \
        return FALSE;                                                                       \
    };                                                                                      \
    TraverseCallbackPacket(vCallback);                                                      \
}
