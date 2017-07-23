#include "stdafx.h"
#include "WFPFlt.Manager.Provider.h"
#include "WFPFlt.ApiWrapper.Provider.h"
#include "WFPFlt.ApiWrapper.Sublayer.h"
#include "WFPFlt.Manager.Engine.h"
#include "WFPFlt.Manager.Transaction.h"

namespace MBox
{
    namespace WFPFlt
    {
        // ProviderGuid {E8FB5F88-2C77-49D5-96AF-FEE7FF9276A4}
        static const GUID s_ProviderGuid = 
        {
            0xe8fb5f88,
            0x2c77,
            0x49d5,
            { 0x96, 0xaf, 0xfe, 0xe7, 0xff, 0x92, 0x76, 0xa4 }
        };

        // {6C23F3F4-6861-478D-982A-8B240E40B58A}
        static const GUID s_ProviderSublayerGuid =
        { 
            0x6c23f3f4,
            0x6861,
            0x478d,
            { 0x98, 0x2a, 0x8b, 0x24, 0xe, 0x40, 0xb5, 0x8a }
        };


        static const wchar_t s_ProviderDefaultName[] = { L"WFPFlt.DefaultName.Provider" };
        static const wchar_t s_ProviderDefaultDescription[] = { L"WFPFlt.DefaultDescription.Provider" };


        NTSTATUS ProviderManager::Initialize()
        {
            return STATUS_SUCCESS;
        }

        void ProviderManager::Uninitialize()
        {
            DeleteProvider();
        }

        NTSTATUS ProviderManager::AddProvider()
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            auto vEngineHandle = GetEngineManager()->GetEngineHandle();

            // Transaction Auto Commit
            TransactionManager vTransactionManager{};
            vStatus = vTransactionManager.BeginTransaction(vEngineHandle);
            if (!NT_SUCCESS(vStatus))
            {
                return vStatus;
            }

            for (;;)
            {
                WFPApiWrapper::WFPFltProvider vProvider{};
                WFPApiWrapper::ProviderAddParameter vParameter{};

                vParameter.m_EngineHandle = vEngineHandle;
                vParameter.m_SecurityDescriptor = nullptr;
                vParameter.m_Provider = &vProvider;

                vProvider.m_ProviderGuid = s_ProviderGuid;
                vProvider.m_DisplayData.m_Name = const_cast<wchar_t*>(s_ProviderDefaultName);
                vProvider.m_DisplayData.m_Description = const_cast<wchar_t*>(s_ProviderDefaultDescription);
                vProvider.m_Flags = FWPM_PROVIDER_FLAG_PERSISTENT;

                vStatus = WFPApiWrapper::ProviderAdd(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    if (STATUS_FWP_ALREADY_EXISTS != vStatus)
                    {
                        break;
                    }
                    vStatus = STATUS_SUCCESS;
                }

                WFPApiWrapper::WFPFltSublayer vSublayer{};
                WFPApiWrapper::SublayerAddParameter vSublayerAddParameter{};
                vSublayerAddParameter.m_EngineHandle = vEngineHandle;
                vSublayerAddParameter.m_Sublayer = &vSublayer;

                vSublayer.m_SublayerGuid = s_ProviderSublayerGuid;
                vSublayer.m_ProviderGuid = const_cast<GUID*>(&s_ProviderGuid);
                vSublayer.m_DisplayData.m_Name = const_cast<wchar_t*>(s_ProviderDefaultName);
                vSublayer.m_DisplayData.m_Description = const_cast<wchar_t*>(s_ProviderDefaultDescription);

                vStatus = WFPApiWrapper::SublayerAdd(&vSublayerAddParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    if (STATUS_FWP_ALREADY_EXISTS != vStatus)
                    {
                        break;
                    }
                    vStatus = STATUS_SUCCESS;
                }

                break;
            }

            if (!NT_SUCCESS(vStatus))
            {
                vTransactionManager.AbortTransaction();
            }
                
            return vStatus;
        }

        NTSTATUS ProviderManager::DeleteProvider()
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            auto vEngineHandle = GetEngineManager()->GetEngineHandle();

            // Transaction Auto Commit
            TransactionManager vTransactionManager{};
            vStatus = vTransactionManager.BeginTransaction(vEngineHandle);
            if (!NT_SUCCESS(vStatus))
            {
                return vStatus;
            }

            for (;;)
            {
                WFPApiWrapper::SublayerDeleteByKeyParameter vSublayerDeleteParameter{};
                vSublayerDeleteParameter.m_EngineHandle = vEngineHandle;
                vSublayerDeleteParameter.m_SublayerGuid = &s_ProviderSublayerGuid;

                vStatus = WFPApiWrapper::SublayerDeleteByKey(&vSublayerDeleteParameter); 
                if (!NT_SUCCESS(vStatus))
                {
                    if (STATUS_FWP_SUBLAYER_NOT_FOUND != vStatus)
                    {
                        break;
                    }
                    vStatus = STATUS_SUCCESS;
                }

                WFPApiWrapper::ProviderDeleteByKeyParameter vParameter{};
                vParameter.m_EngineHandle = vEngineHandle;
                vParameter.m_ProviderGuid = &s_ProviderGuid;

                vStatus = WFPApiWrapper::ProviderDeleteByKey(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    if (STATUS_FWP_PROVIDER_NOT_FOUND != vStatus)
                    {
                        break;
                    }
                    vStatus = STATUS_SUCCESS;
                }

                break;
            }

            if (!NT_SUCCESS(vStatus))
            {
                vTransactionManager.AbortTransaction();
            }

            return vStatus;
        }

        const GUID * GetProviderGuid()
        {
            return &s_ProviderGuid;
        }


        ProviderManager * GetProviderManager()
        {
            static ProviderManager sProviderManager;
            return &sProviderManager;
        }

}
}

