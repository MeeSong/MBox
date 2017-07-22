#include "KTL.UUID.h"
#include "KTL.Type.Uitility.h"

namespace ktl
{
    namespace UUID
    {
        UUID::UUID(const UUID & aOther)
        {
            operator=(aOther);
        }

        UUID::UUID(const::GUID & aOther)
            : UUID(reinterpret_cast<const UUID&>(aOther))
        { }

        UUID::UUID(UUID && aOther)
        {
            operator=(forward<UUID>(aOther));
        }

        UUID::UUID(::GUID && aOther)
            : UUID(reinterpret_cast<UUID&&>(aOther))
        { }

        UUID & UUID::operator=(const UUID & aOther)
        {
            reinterpret_cast<ktl::u64*>(this)[0] = reinterpret_cast<const ktl::u64*>(&aOther)[0];
            reinterpret_cast<ktl::u64*>(this)[1] = reinterpret_cast<const ktl::u64*>(&aOther)[1];

            return (*this);
        }

        UUID & UUID::operator=(const::GUID & aOther)
        {
            return (operator=(reinterpret_cast<const UUID&>(aOther)));
        }

        UUID & UUID::operator=(UUID && aOther)
        {
            reinterpret_cast<ktl::u64*>(this)[0] = reinterpret_cast<const ktl::u64*>(&aOther)[0];
            reinterpret_cast<ktl::u64*>(this)[1] = reinterpret_cast<const ktl::u64*>(&aOther)[1];

            aOther = { 0 };
            return (*this);
        }

        UUID & UUID::operator=(::GUID && aOther)
        {
            return (operator=(reinterpret_cast<UUID&&>(aOther)));
        }

        bool UUID::operator==(const UUID & aOther) const
        {
            return (reinterpret_cast<const ktl::u64*>(this)[0] == reinterpret_cast<const ktl::u64*>(&aOther)[0]
                && reinterpret_cast<const ktl::u64*>(this)[1] == reinterpret_cast<const ktl::u64*>(&aOther)[1]);
        }

        bool UUID::operator==(const ::GUID & aOther) const
        {
            return (operator==(reinterpret_cast<const UUID&>(aOther)));
        }

        bool UUID::is_nil() const
        {
            return (operator==(UUID{}));
        }

        UUIDVersion UUID::get_version() const
        {
            return UUIDVersion((m_Data3 >> 12) & 0xF);
        }

        void CreateUUID(UUID * aUUID, random::mt19937_64 & aRandomEngine)
        {
            if (nullptr == aUUID) return;

            ((ktl::u64*)aUUID)[0] = aRandomEngine();
            ((ktl::u64*)aUUID)[1] = aRandomEngine();

            aUUID->m_Data3 = (aUUID->m_Data3 & 0x0FFF) | (UUIDVersion::Random << 12);
            aUUID->m_Data4[0] = (aUUID->m_Data4[0] & 0x3F) | 0x80;
        }

        void CreateGUID(::GUID * aGUID, random::mt19937_64 & aRandomEngine)
        {
            return CreateUUID(reinterpret_cast<UUID*>(aGUID), aRandomEngine);
        }
        
    }
}
