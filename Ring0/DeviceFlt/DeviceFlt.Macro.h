#pragma once

#define DeviceFlt$Function$TraverseCallbackPacketMemberName$Macro(aFunctionName) \
Traverse ## aFunctionName ## Callback

#define DeviceFlt$Function$DeclareTraverseCallbackPacketMember$Macro(aFunctionName) \
template <typename F> \
void DeviceFlt$Function$TraverseCallbackPacketMemberName$Macro(aFunctionName)(F aFunctor);

#define DeviceFlt$Function$TraverseCallbackPacketMember$Macro(aFunctionName, aMember) \
template <typename F> \
void DeviceFlt$Function$TraverseCallbackPacketMemberName$Macro(aFunctionName)(F aFunctor)	\
{                                                                                           \
    auto vCallback = [&aFunctor](ktl::shared_ptr<CallbackPacket> aCallbackPacket, UINT32 aIndex) -> BOOLEAN \
    {                                                                                       \
        if (aCallbackPacket->aMember)														\
        {                                                                                   \
            return aFunctor(aCallbackPacket->aMember, aIndex);								\
        }                                                                                   \
        return FALSE;                                                                       \
    };                                                                                      \
    GetCallbackPacketManager()->TraverseCallbackPacket(vCallback);                               \
}
