/****************************** Module Header ******************************\
* Module Name:  ServiceBase.h
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
*
* Provides a base class for a service that will exist as part of a service
* application. CServiceBase must be derived from when creating a new service
* class.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/


#pragma once
#include <windows.h>


namespace MBox
{
    namespace Vol
    {
        namespace Service
        {
            class CServiceBase
            {
            public:

                // Register the executable for a service with the Service Control Manager 
                // (SCM). After you call Run(ServiceBase), the SCM issues a Start command, 
                // which results in a call to the OnStart method in the service. This 
                // method blocks until the service has stopped.
                static bool Run(CServiceBase &aService);

                // Service object constructor. The optional parameters (fCanStop, 
                // fCanShutdown and fCanPauseContinue) allow you to specify whether the 
                // service can be stopped, paused and continued, or be notified when 
                // system shutdown occurs.
                CServiceBase(
                    const wchar_t* aServiceName,
                    bool aCanStop = true,
                    bool aCanShutdown = true,
                    bool aCanPauseContinue = false);

                // Service object destructor. 
                virtual ~CServiceBase(void);

                // Stop the service.
                UINT32 Stop();

                HANDLE GetServiceStatusHandle();

            protected:

                // When implemented in a derived class, executes when a Start command is 
                // sent to the service by the SCM or when the operating system starts 
                // (for a service that starts automatically). Specifies actions to take 
                // when the service starts.
                virtual UINT32 OnStart(UINT32 aArgc, const wchar_t** aArgv);

                // When implemented in a derived class, executes when a Stop command is 
                // sent to the service by the SCM. Specifies actions to take when a 
                // service stops running.
                virtual UINT32 OnStop();

                // When implemented in a derived class, executes when a Pause command is 
                // sent to the service by the SCM. Specifies actions to take when a 
                // service pauses.
                virtual UINT32 OnPause();

                // When implemented in a derived class, OnContinue runs when a Continue 
                // command is sent to the service by the SCM. Specifies actions to take 
                // when a service resumes normal functioning after being paused.
                virtual UINT32 OnContinue();

                // When implemented in a derived class, executes when the system is 
                // shutting down. Specifies what should occur immediately prior to the 
                // system shutting down.
                virtual UINT32 OnShutdown();

                // When implemented in a derived class, executes when the other command.
                virtual UINT32 OnOther(
                    UINT32 aCtrl,
                    UINT32 aEventType,
                    LPVOID aEventData,
                    LPVOID aContext);

                // Set the service status and report the status to the SCM.
                void SetServiceStatus(
                    UINT32 aCurrentState,
                    UINT32 aWin32ExitCode = NOERROR,
                    UINT32 aWaitHint = 0);

                // Log a message to the Application event log.
                void WriteEventLogEntry(const wchar_t* aMessage, UINT16 aType);

                // Log a message to the Application event log.
                void WriteEventLogEntry(
                    UINT16 aType,
                    const wchar_t* aFormats,
                    ...);

#define WriteEventLog(aType, aFormat, ...)\
do{\
    WriteEventLogEntry(aType, L"[%08u][%s] Message: " aFormat L"\n", \
        __FUNCTIONW__, __LINE__, __VA_ARGS__ ); \
}while(false)

            private:

                // Entry point for the service. It registers the handler function for the 
                // service and starts the service.
                static void WINAPI ServiceMain(DWORD aArgc, wchar_t** aArgv);

                // The function is called by the SCM whenever a control code is sent to 
                // the service.
                static DWORD WINAPI ServiceCtrlHandler(
                    DWORD aCtrl,
                    DWORD aEventType,
                    LPVOID aEventData,
                    LPVOID aContext);

                // Start the service.
                UINT32 Start(UINT32 aArgc, const wchar_t** aArgv);

                // Pause the service.
                UINT32 Pause();

                // Resume the service after being paused.
                UINT32 Continue();

                // Execute when the system is shutting down.
                UINT32 Shutdown();

                // Other
                UINT32 Other(
                    UINT32 aCtrl,
                    UINT32 aEventType,
                    LPVOID aEventData,
                    LPVOID aContext);

                // The singleton service instance.
                static CServiceBase *s_Service;

                // The name of the service
                wchar_t* m_Name = nullptr;

                // The status of the service
                SERVICE_STATUS m_Status{};

                // The service status handle
                SERVICE_STATUS_HANDLE m_StatusHandle = nullptr;
            };

        }
    }
}
