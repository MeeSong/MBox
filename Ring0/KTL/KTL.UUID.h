#pragma once
#include "KTL.Random.h"
#include <guiddef.h>

namespace ktl
{
    namespace UUID
    {
        enum UUIDVersion : ktl::u8
        {
            TimeBased       = 0x1,
            DecUid,
            NameBased,
            Random,
            NameBasedSha1
        };

        struct UUID
        {
            ktl::u32 m_Data1 = 0;
            ktl::u16 m_Data2 = 0;
            ktl::u16 m_Data3 = 0;
            ktl::u8  m_Data4[8] = { 0 };


            UUID() = default;

            explicit UUID(const UUID& aOther);
            explicit UUID(const ::GUID& aOther);

            explicit UUID(UUID&& aOther);
            explicit UUID(::GUID&& aOther);

            UUID& operator=(const UUID& aOther);
            UUID& operator=(const ::GUID& aOther);

            UUID& operator=(UUID&& aOther);
            UUID& operator=(::GUID&& aOther);

            bool operator==(const UUID& aOther) const;
            bool operator==(const ::GUID& aOther) const;

            bool is_nil() const;
            UUIDVersion get_version() const;
        };

        static_assert(sizeof(UUID) == sizeof(::GUID)
            && sizeof(UUID) == sizeof(ktl::u64) * 2,
            "ktl::UUID != ::GUID !!!!");

        void CreateUUID(UUID* aUUID, random::mt19937_64& aRandomEngine);
        void CreateGUID(::GUID* aGUID, random::mt19937_64& aRandomEngine);
    }
}
