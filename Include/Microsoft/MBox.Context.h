#pragma once


namespace MBox
{
#ifdef _X86_

    struct FloatingSaveArea 
    {
        enum : UINT32
        {
            SizeOf80387Registers = 80,
        };

        UINT32   ControlWord;
        UINT32   StatusWord;
        UINT32   TagWord;
        UINT32   ErrorOffset;
        UINT32   ErrorSelector;
        UINT32   DataOffset;
        UINT32   DataSelector;
        UINT8   RegisterArea[SizeOf80387Registers];
        UINT32   Spare0;
    };

#pragma pack(push,4)
    struct Context
    {
        enum : UINT32
        {
            MaximumSupportedExtension = 512,
        };

        //
        // The flags values within this flag control the contents of
        // a CONTEXT record.
        //
        // If the context record is used as an input parameter, then
        // for each portion of the context record controlled by a flag
        // whose value is set, it is assumed that that portion of the
        // context record contains valid context. If the context record
        // is being used to modify a threads context, then only that
        // portion of the threads context will be modified.
        //
        // If the context record is used as an IN OUT parameter to capture
        // the context of a thread, then only those portions of the thread's
        // context corresponding to set flags will be returned.
        //
        // The context record is never used as an OUT only parameter.
        //

        UINT32 ContextFlags;

        //
        // This section is specified/returned if CONTEXT_DEBUG_REGISTERS is
        // set in ContextFlags.  Note that CONTEXT_DEBUG_REGISTERS is NOT
        // included in CONTEXT_FULL.
        //

        UINT32   Dr0;
        UINT32   Dr1;
        UINT32   Dr2;
        UINT32   Dr3;
        UINT32   Dr6;
        UINT32   Dr7;

        //
        // This section is specified/returned if the
        // ContextFlags word contians the flag CONTEXT_FLOATING_POINT.
        //

        FloatingSaveArea FloatSave;

        //
        // This section is specified/returned if the
        // ContextFlags word contians the flag CONTEXT_SEGMENTS.
        //

        UINT32   SegGs;
        UINT32   SegFs;
        UINT32   SegEs;
        UINT32   SegDs;

        //
        // This section is specified/returned if the
        // ContextFlags word contians the flag CONTEXT_INTEGER.
        //

        UINT32   Edi;
        UINT32   Esi;
        UINT32   Ebx;
        UINT32   Edx;
        UINT32   Ecx;
        UINT32   Eax;

        //
        // This section is specified/returned if the
        // ContextFlags word contians the flag CONTEXT_CONTROL.
        //

        UINT32   Ebp;
        UINT32   Eip;
        UINT32   SegCs;              // MUST BE SANITIZED
        UINT32   EFlags;             // MUST BE SANITIZED
        UINT32   Esp;
        UINT32   SegSs;

        //
        // This section is specified/returned if the ContextFlags word
        // contains the flag CONTEXT_EXTENDED_REGISTERS.
        // The format and contexts are processor specific
        //

        UINT8   ExtendedRegisters[MaximumSupportedExtension];
    };
#pragma pack(pop)

#endif

#ifdef _AMD64_

    struct __declspec(align(16)) Context
    {

        //
        // Register parameter home addresses.
        //
        // N.B. These fields are for convience - they could be used to extend the
        //      context record in the future.
        //

        UINT64 P1Home;
        UINT64 P2Home;
        UINT64 P3Home;
        UINT64 P4Home;
        UINT64 P5Home;
        UINT64 P6Home;

        //
        // Control flags.
        //

        UINT32 ContextFlags;
        UINT32 MxCsr;

        //
        // Segment Registers and processor flags.
        //

        UINT16 SegCs;
        UINT16 SegDs;
        UINT16 SegEs;
        UINT16 SegFs;
        UINT16 SegGs;
        UINT16 SegSs;
        UINT32 EFlags;

        //
        // Debug registers
        //

        UINT64 Dr0;
        UINT64 Dr1;
        UINT64 Dr2;
        UINT64 Dr3;
        UINT64 Dr6;
        UINT64 Dr7;

        //
        // Integer registers.
        //

        UINT64 Rax;
        UINT64 Rcx;
        UINT64 Rdx;
        UINT64 Rbx;
        UINT64 Rsp;
        UINT64 Rbp;
        UINT64 Rsi;
        UINT64 Rdi;
        UINT64 R8;
        UINT64 R9;
        UINT64 R10;
        UINT64 R11;
        UINT64 R12;
        UINT64 R13;
        UINT64 R14;
        UINT64 R15;

        //
        // Program counter.
        //

        UINT64 Rip;

        //
        // Floating point state.
        //

        union
        {
            XMM_SAVE_AREA32 FltSave;
            struct
            {
                M128A Header[2];
                M128A Legacy[8];
                M128A Xmm0;
                M128A Xmm1;
                M128A Xmm2;
                M128A Xmm3;
                M128A Xmm4;
                M128A Xmm5;
                M128A Xmm6;
                M128A Xmm7;
                M128A Xmm8;
                M128A Xmm9;
                M128A Xmm10;
                M128A Xmm11;
                M128A Xmm12;
                M128A Xmm13;
                M128A Xmm14;
                M128A Xmm15;
            };
        };

        //
        // Vector registers.
        //

        M128A VectorRegister[26];
        UINT64 VectorControl;

        //
        // Special debug control registers.
        //

        UINT64 DebugControl;
        UINT64 LastBranchToRip;
        UINT64 LastBranchFromRip;
        UINT64 LastExceptionToRip;
        UINT64 LastExceptionFromRip;
    };

#endif

}
