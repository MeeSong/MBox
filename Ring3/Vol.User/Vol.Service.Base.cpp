/****************************** Module Header ******************************\
* Module Name:  ServiceBase.cpp
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


#pragma region Includes
#include "stdafx.h"
#include "Vol.Service.Base.h"

#include <assert.h>
#include <strsafe.h>
#pragma endregion


namespace MBox
{
    namespace Vol
    {
        namespace Service
        {
#pragma region Static Members

            // Initialize the singleton service instance.
            CServiceBase *CServiceBase::s_Service = nullptr;

            //
            //   FUNCTION: CServiceBase::Run(CServiceBase &)
            //
            //   PURPOSE: Register the executable for a service with the Service Control 
            //   Manager (SCM). After you call Run(ServiceBase), the SCM issues a Start 
            //   command, which results in a call to the OnStart method in the service. 
            //   This method blocks until the service has stopped.
            //
            //   PARAMETERS:
            //   * service - the reference to a CServiceBase object. It will become the 
            //     singleton service instance of this service application.
            //
            //   RETURN VALUE: If the function succeeds, the return value is TRUE. If the 
            //   function fails, the return value is FALSE. To get extended error 
            //   information, call GetLastError.
            //
            bool CServiceBase::Run(CServiceBase &aService)
            {
                s_Service = &aService;

                SERVICE_TABLE_ENTRY vServiceTable[] =
                {
                    { aService.m_Name, ServiceMain },
                    { nullptr, nullptr }
                };

                // Connects the main thread of a service process to the service control 
                // manager, which causes the thread to be the service control dispatcher 
                // thread for the calling process. This call returns when the service has 
                // stopped. The process should simply terminate when the call returns.
                return !!StartServiceCtrlDispatcher(vServiceTable);
            }


            //
            //   FUNCTION: CServiceBase::ServiceMain(DWORD, PWSTR *)
            //
            //   PURPOSE: Entry point for the service. It registers the handler function 
            //   for the service and starts the service.
            //
            //   PARAMETERS:
            //   * dwArgc   - number of command line arguments
            //   * lpszArgv - array of command line arguments
            //
            void WINAPI CServiceBase::ServiceMain(DWORD aArgc, wchar_t** aArgv)
            {
                assert(s_Service != nullptr);

                // Register the handler function for the service
                s_Service->m_StatusHandle = RegisterServiceCtrlHandlerEx(
                    s_Service->m_Name, ServiceCtrlHandler, nullptr);
                if (s_Service->m_StatusHandle == nullptr)
                {
                    throw GetLastError();
                }

                // Start the service.
                s_Service->Start(aArgc, (const wchar_t**)aArgv);
            }


            //
            //   FUNCTION: CServiceBase::ServiceCtrlHandler(DWORD)
            //
            //   PURPOSE: The function is called by the SCM whenever a control code is 
            //   sent to the service. 
            //
            //   PARAMETERS:
            //   * dwCtrlCode - the control code. This parameter can be one of the 
            //   following values: 
            //
            //     SERVICE_CONTROL_CONTINUE
            //     SERVICE_CONTROL_INTERROGATE
            //     SERVICE_CONTROL_NETBINDADD
            //     SERVICE_CONTROL_NETBINDDISABLE
            //     SERVICE_CONTROL_NETBINDREMOVE
            //     SERVICE_CONTROL_PARAMCHANGE
            //     SERVICE_CONTROL_PAUSE
            //     SERVICE_CONTROL_SHUTDOWN
            //     SERVICE_CONTROL_STOP
            //
            //   * This parameter can also be one of the following extended control codes. 
            //   Note that these control codes are not supported by the Handler function.
            //
            //     SERVICE_CONTROL_DEVICEEVENT            0x0000000B
            //     SERVICE_CONTROL_HARDWAREPROFILECHANGE  0x0000000C
            //     SERVICE_CONTROL_POWEREVENT             0x0000000D
            //     SERVICE_CONTROL_SESSIONCHANGE          0x0000000E
            //     SERVICE_CONTROL_TIMECHANGE             0x00000010
            //     SERVICE_CONTROL_TRIGGEREVENT           0x00000020
            //     SERVICE_CONTROL_USERMODEREBOOT         0x00000040
            //
            //   This parameter can also be a user-defined control code ranges from 128 
            //   to 255.
            //
            DWORD WINAPI CServiceBase::ServiceCtrlHandler(
                DWORD dwCtrl,
                DWORD dwEventType,
                LPVOID lpEventData,
                LPVOID lpContext)
            {
                UINT32 vDosError = ERROR_CALL_NOT_IMPLEMENTED;

                switch (dwCtrl)
                {
                case SERVICE_CONTROL_STOP:          vDosError = s_Service->Stop(); break;
                case SERVICE_CONTROL_PAUSE:         vDosError = s_Service->Pause(); break;
                case SERVICE_CONTROL_CONTINUE:      vDosError = s_Service->Continue(); break;
                case SERVICE_CONTROL_SHUTDOWN:      vDosError = s_Service->Shutdown(); break;
                case SERVICE_CONTROL_INTERROGATE:   vDosError = NOERROR; break;
                default: vDosError = s_Service->Other(dwCtrl, dwEventType, lpEventData, lpContext); break;
                }

                return vDosError;
            }

#pragma endregion


#pragma region Service Constructor and Destructor

            //
            //   FUNCTION: CServiceBase::CServiceBase(PWSTR, BOOL, BOOL, BOOL)
            //
            //   PURPOSE: The constructor of CServiceBase. It initializes a new instance 
            //   of the CServiceBase class. The optional parameters (fCanStop, 
            ///  fCanShutdown and fCanPauseContinue) allow you to specify whether the 
            //   service can be stopped, paused and continued, or be notified when system 
            //   shutdown occurs.
            //
            //   PARAMETERS:
            //   * pszServiceName - the name of the service
            //   * fCanStop - the service can be stopped
            //   * fCanShutdown - the service is notified when system shutdown occurs
            //   * fCanPauseContinue - the service can be paused and continued
            //
            CServiceBase::CServiceBase(
                const wchar_t* aServiceName,
                bool aCanStop,
                bool aCanShutdown,
                bool aCanPauseContinue)
            {
                // Service name must be a valid string and cannot be NULL.
                m_Name = (aServiceName == nullptr) ? L"" : aServiceName;

                m_StatusHandle = nullptr;

                // The service runs in its own process.
                m_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

                // The service is starting.
                m_Status.dwCurrentState = SERVICE_START_PENDING;

                // The accepted commands of the service.
                UINT32 vControlsAccepted = 0;
                if (aCanStop)
                    vControlsAccepted |= SERVICE_ACCEPT_STOP;
                if (aCanShutdown)
                    vControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
                if (aCanPauseContinue)
                    vControlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;
                m_Status.dwControlsAccepted = vControlsAccepted;

                m_Status.dwWin32ExitCode = NO_ERROR;
                m_Status.dwServiceSpecificExitCode = 0;
                m_Status.dwCheckPoint = 0;
                m_Status.dwWaitHint = 0;
            }


            //
            //   FUNCTION: CServiceBase::~CServiceBase()
            //
            //   PURPOSE: The virtual destructor of CServiceBase. 
            //
            CServiceBase::~CServiceBase(void)
            {
            }

#pragma endregion


#pragma region Service Start, Stop, Pause, Continue, and Shutdown

            //
            //   FUNCTION: CServiceBase::Start(DWORD, PWSTR *)
            //
            //   PURPOSE: The function starts the service. It calls the OnStart virtual 
            //   function in which you can specify the actions to take when the service 
            //   starts. If an error occurs during the startup, the error will be logged 
            //   in the Application event log, and the service will be stopped.
            //
            //   PARAMETERS:
            //   * dwArgc   - number of command line arguments
            //   * lpszArgv - array of command line arguments
            //
            UINT32 CServiceBase::Start(UINT32 aArgc, const wchar_t** aArgv)
            {
                UINT32 vDosError = ERROR_CALL_NOT_IMPLEMENTED;

                try
                {
                    // Tell SCM that the service is starting.
                    SetServiceStatus(SERVICE_START_PENDING);

                    // Perform service-specific initialization.
                    vDosError = OnStart(aArgc, aArgv);
                    if (NOERROR != vDosError)
                    {
                        // Log the error.
                        WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Start", vDosError);
                        // Set the service status to be stopped.
                        SetServiceStatus(SERVICE_STOPPED);
                    }
                    else
                    {
                        // Tell SCM that the service is started.
                        SetServiceStatus(SERVICE_RUNNING);
                    }
                }
                catch (UINT32 vDosError2)
                {
                    // Log the error.
                    WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Start", vDosError2);

                    vDosError = vDosError2;

                    // Set the service status to be stopped.
                    SetServiceStatus(SERVICE_STOPPED, vDosError2);
                }
                catch (...)
                {
                    vDosError = GetLastError();
                    if (NOERROR == vDosError)
                    {
                        vDosError = _doserrno;
                    }

                    // Log the error.
                    WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service failed to start.", vDosError);

                    // Set the service status to be stopped.
                    SetServiceStatus(SERVICE_STOPPED);
                }

                return vDosError;
            }


            //
            //   FUNCTION: CServiceBase::OnStart(DWORD, PWSTR *)
            //
            //   PURPOSE: When implemented in a derived class, executes when a Start 
            //   command is sent to the service by the SCM or when the operating system 
            //   starts (for a service that starts automatically). Specifies actions to 
            //   take when the service starts. Be sure to periodically call 
            //   CServiceBase::SetServiceStatus() with SERVICE_START_PENDING if the 
            //   procedure is going to take long time. You may also consider spawning a 
            //   new thread in OnStart to perform time-consuming initialization tasks.
            //
            //   PARAMETERS:
            //   * dwArgc   - number of command line arguments
            //   * lpszArgv - array of command line arguments
            //
            UINT32 CServiceBase::OnStart(UINT32 /*aArgc*/, const wchar_t** /*aArgv*/)
            {
                return ERROR_CALL_NOT_IMPLEMENTED;
            }


            //
            //   FUNCTION: CServiceBase::Stop()
            //
            //   PURPOSE: The function stops the service. It calls the OnStop virtual 
            //   function in which you can specify the actions to take when the service 
            //   stops. If an error occurs, the error will be logged in the Application 
            //   event log, and the service will be restored to the original state.
            //
            UINT32 CServiceBase::Stop()
            {
                UINT32 vDosError = ERROR_CALL_NOT_IMPLEMENTED;

                UINT32 dwOriginalState = m_Status.dwCurrentState;
                try
                {
                    // Tell SCM that the service is stopping.
                    SetServiceStatus(SERVICE_STOP_PENDING);

                    // Perform service-specific stop operations.
                    vDosError = OnStop();
                    if (NOERROR != vDosError)
                    {
                        WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Stop.", vDosError);
                        SetServiceStatus(dwOriginalState);
                    }
                    else
                    {
                        // Tell SCM that the service is stopped.
                        SetServiceStatus(SERVICE_STOPPED);
                    }
                }
                catch (UINT32 vDosError2)
                {
                    // Log the error.
                    WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Stop.", vDosError2);

                    vDosError = vDosError2;

                    // Set the orginal service status.
                    SetServiceStatus(dwOriginalState);
                }
                catch (...)
                {
                    vDosError = GetLastError();

                    // Log the error.
                    WriteEventLogEntry(L"Service failed to stop.", EVENTLOG_ERROR_TYPE);

                    // Set the orginal service status.
                    SetServiceStatus(dwOriginalState);
                }

                return vDosError;
            }


            //
            //   FUNCTION: CServiceBase::OnStop()
            //
            //   PURPOSE: When implemented in a derived class, executes when a Stop 
            //   command is sent to the service by the SCM. Specifies actions to take 
            //   when a service stops running. Be sure to periodically call 
            //   CServiceBase::SetServiceStatus() with SERVICE_STOP_PENDING if the 
            //   procedure is going to take long time. 
            //
            UINT32 CServiceBase::OnStop()
            {
                return ERROR_CALL_NOT_IMPLEMENTED;
            }


            //
            //   FUNCTION: CServiceBase::Pause()
            //
            //   PURPOSE: The function pauses the service if the service supports pause 
            //   and continue. It calls the OnPause virtual function in which you can 
            //   specify the actions to take when the service pauses. If an error occurs, 
            //   the error will be logged in the Application event log, and the service 
            //   will become running.
            //
            UINT32 CServiceBase::Pause()
            {
                UINT32 vDosError = ERROR_CALL_NOT_IMPLEMENTED;

                try
                {
                    // Tell SCM that the service is pausing.
                    SetServiceStatus(SERVICE_PAUSE_PENDING);

                    // Perform service-specific pause operations.
                    vDosError = OnPause();
                    if (NOERROR != vDosError)
                    {
                        WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Pause.", vDosError);
                        SetServiceStatus(SERVICE_RUNNING);
                    }
                    else
                    {
                        // Tell SCM that the service is paused.
                        SetServiceStatus(SERVICE_PAUSED);
                    }
                }
                catch (UINT32 vDosError2)
                {
                    // Log the error.
                    WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Pause.", vDosError2);

                    vDosError = vDosError2;

                    // Tell SCM that the service is still running.
                    SetServiceStatus(SERVICE_RUNNING);
                }
                catch (...)
                {
                    vDosError = GetLastError();

                    // Log the error.
                    WriteEventLogEntry(L"Service failed to pause.", EVENTLOG_ERROR_TYPE);

                    // Tell SCM that the service is still running.
                    SetServiceStatus(SERVICE_RUNNING);
                }

                return vDosError;
            }


            //
            //   FUNCTION: CServiceBase::OnPause()
            //
            //   PURPOSE: When implemented in a derived class, executes when a Pause 
            //   command is sent to the service by the SCM. Specifies actions to take 
            //   when a service pauses.
            //
            UINT32 CServiceBase::OnPause()
            {
                return ERROR_CALL_NOT_IMPLEMENTED;
            }


            //
            //   FUNCTION: CServiceBase::Continue()
            //
            //   PURPOSE: The function resumes normal functioning after being paused if
            //   the service supports pause and continue. It calls the OnContinue virtual 
            //   function in which you can specify the actions to take when the service 
            //   continues. If an error occurs, the error will be logged in the 
            //   Application event log, and the service will still be paused.
            //
            UINT32 CServiceBase::Continue()
            {
                UINT32 vDosError = ERROR_CALL_NOT_IMPLEMENTED;

                try
                {
                    // Tell SCM that the service is resuming.
                    SetServiceStatus(SERVICE_CONTINUE_PENDING);

                    // Perform service-specific continue operations.
                    vDosError = OnContinue();
                    if (NOERROR != vDosError)
                    {
                        WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Continue.", vDosError);
                        SetServiceStatus(SERVICE_PAUSED);
                    }
                    else
                    {
                        // Tell SCM that the service is running.
                        SetServiceStatus(SERVICE_RUNNING);
                    }
                }
                catch (UINT32 vDosError2)
                {
                    // Log the error.
                    WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Continue.", vDosError2);

                    vDosError = vDosError2;

                    // Tell SCM that the service is still paused.
                    SetServiceStatus(SERVICE_PAUSED);
                }
                catch (...)
                {
                    vDosError = GetLastError();

                    // Log the error.
                    WriteEventLogEntry(L"Service failed to resume.", EVENTLOG_ERROR_TYPE);

                    // Tell SCM that the service is still paused.
                    SetServiceStatus(SERVICE_PAUSED);
                }

                return vDosError;
            }


            //
            //   FUNCTION: CServiceBase::OnContinue()
            //
            //   PURPOSE: When implemented in a derived class, OnContinue runs when a 
            //   Continue command is sent to the service by the SCM. Specifies actions to 
            //   take when a service resumes normal functioning after being paused.
            //
            UINT32 CServiceBase::OnContinue()
            {
                return ERROR_CALL_NOT_IMPLEMENTED;
            }


            //
            //   FUNCTION: CServiceBase::Shutdown()
            //
            //   PURPOSE: The function executes when the system is shutting down. It 
            //   calls the OnShutdown virtual function in which you can specify what 
            //   should occur immediately prior to the system shutting down. If an error 
            //   occurs, the error will be logged in the Application event log.
            //
            UINT32 CServiceBase::Shutdown()
            {
                UINT32 vDosError = ERROR_CALL_NOT_IMPLEMENTED;

                try
                {
                    // Perform service-specific shutdown operations.
                    vDosError = OnShutdown();
                    if (NOERROR != vDosError)
                    {
                        WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Shutdown.", vDosError);
                    }
                    else
                    {
                        // Tell SCM that the service is stopped.
                        SetServiceStatus(SERVICE_STOPPED);
                    }
                }
                catch (UINT32 vDosError2)
                {
                    vDosError = vDosError2;

                    // Log the error.
                    WriteEventLog(EVENTLOG_ERROR_TYPE, L"DosError: %08u, Service Shutdown.", vDosError2);
                }
                catch (...)
                {
                    vDosError = GetLastError();

                    // Log the error.
                    WriteEventLogEntry(L"Service failed to shut down.", EVENTLOG_ERROR_TYPE);
                }

                return vDosError;
            }


            //
            //   FUNCTION: CServiceBase::OnShutdown()
            //
            //   PURPOSE: When implemented in a derived class, executes when the system 
            //   is shutting down. Specifies what should occur immediately prior to the 
            //   system shutting down.
            //
            UINT32 CServiceBase::OnShutdown()
            {
                return ERROR_CALL_NOT_IMPLEMENTED;
            }


            //
            //   FUNCTION: CServiceBase::Other()
            //
            //   PURPOSE: The function executes when the other command. It 
            //   calls the OnOther virtual function. If an error occurs, 
            //   the error will be logged in the Application event log.
            //
            UINT32 CServiceBase::Other(
                UINT32 dwCtrl,
                UINT32 dwEventType,
                LPVOID lpEventData,
                LPVOID lpContext)
            {
                UINT32 vDosError = ERROR_CALL_NOT_IMPLEMENTED;

                try
                {
                    // Perform service-specific other operations.
                    vDosError = OnOther(dwCtrl, dwEventType, lpEventData, lpContext);
                    if (NOERROR != vDosError)
                    {
                        WriteEventLog(EVENTLOG_ERROR_TYPE,
                            L"DosError: %08u, Service handling device event notify.", vDosError);
                    }
                }
                catch (UINT32 vDosError2)
                {
                    vDosError = vDosError2;

                    // Log the error.
                    WriteEventLog(EVENTLOG_ERROR_TYPE,
                        L"DosError: %08u, Service handling device event notify.", vDosError2);
                }
                catch (...)
                {
                    vDosError = GetLastError();

                    // Log the error.
                    WriteEventLogEntry(L"Service failed to device event notify.", EVENTLOG_ERROR_TYPE);
                }

                return vDosError;
            }


            //
            //   FUNCTION: CServiceBase::OnOther()
            //
            //   PURPOSE: When implemented in a derived class, executes when the 
            //   other command.
            //
            UINT32 CServiceBase::OnOther(
                UINT32 /*aCtrl*/,
                UINT32 /*aEventType*/,
                LPVOID /*aEventData*/,
                LPVOID /*aContext*/)
            {
                return ERROR_CALL_NOT_IMPLEMENTED;
            }

#pragma endregion


#pragma region Helper Functions

            //
            //   FUNCTION: CServiceBase::SetServiceStatus(DWORD, DWORD, DWORD)
            //
            //   PURPOSE: The function sets the service status and reports the status to 
            //   the SCM.
            //
            //   PARAMETERS:
            //   * dwCurrentState - the state of the service
            //   * dwWin32ExitCode - error code to report
            //   * dwWaitHint - estimated time for pending operation, in milliseconds
            //
            void CServiceBase::SetServiceStatus(
                UINT32 aCurrentState,
                UINT32 aWin32ExitCode,
                UINT32 aWaitHint)
            {
                static UINT32 dwCheckPoint = 1;

                // Fill in the SERVICE_STATUS structure of the service.

                m_Status.dwCurrentState = aCurrentState;
                m_Status.dwWin32ExitCode = aWin32ExitCode;
                m_Status.dwWaitHint = aWaitHint;

                m_Status.dwCheckPoint =
                    ((aCurrentState == SERVICE_RUNNING) ||
                    (aCurrentState == SERVICE_STOPPED)) ?
                    0 : dwCheckPoint++;

                // Report the status of the service to the SCM.
                ::SetServiceStatus(m_StatusHandle, &m_Status);
            }


            //
            //   FUNCTION: CServiceBase::WriteEventLogEntry(PWSTR, WORD)
            //
            //   PURPOSE: Log a message to the Application event log.
            //
            //   PARAMETERS:
            //   * pszMessage - string message to be logged.
            //   * wType - the type of event to be logged. The parameter can be one of 
            //     the following values.
            //
            //     EVENTLOG_SUCCESS
            //     EVENTLOG_AUDIT_FAILURE
            //     EVENTLOG_AUDIT_SUCCESS
            //     EVENTLOG_ERROR_TYPE
            //     EVENTLOG_INFORMATION_TYPE
            //     EVENTLOG_WARNING_TYPE
            //
            void CServiceBase::WriteEventLogEntry(const wchar_t* aMessage, UINT16 aType)
            {
                HANDLE vEventSourceHandle = nullptr;
                const wchar_t* vStrings[2] = { nullptr, nullptr };

                vEventSourceHandle = RegisterEventSource(nullptr, m_Name);
                if (vEventSourceHandle)
                {
                    vStrings[0] = m_Name;
                    vStrings[1] = aMessage;

                    ReportEvent(
                        vEventSourceHandle, // Event log handle
                        aType,              // Event type
                        0,                  // Event category
                        0,                  // Event identifier
                        nullptr,            // No security identifier
                        2,                  // Size of lpszStrings array
                        0,                  // No binary data
                        vStrings,           // Array of strings
                        nullptr             // No binary data
                    );

                    DeregisterEventSource(vEventSourceHandle);
                }
            }

            void CServiceBase::WriteEventLogEntry(
                UINT16 aType, const wchar_t * aFormats, ...)
            {
                HRESULT hr = S_OK;
                wchar_t vMessage[2048]{};

                for (;;)
                {
                    va_list vArglist;
                    va_start(vArglist, aFormats);
                    hr = StringCchVPrintfW(vMessage, ARRAYSIZE(vMessage), aFormats, vArglist);
                    va_end(vArglist);

                    if (FAILED(hr))
                    {
                        break;
                    }

                    return WriteEventLogEntry(vMessage, aType);
                }
            }

            HANDLE CServiceBase::GetServiceStatusHandle()
            {
                return m_StatusHandle;
            }

#pragma endregion

        }
    }
}
