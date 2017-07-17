#pragma once

namespace MBox
{
    namespace ShimsApi
    {

        //
        // Shims api name
        //

#define ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName) \
    aFunctionName ## Shims

        //
        // Shims api parameter declare
        //

#define ShimsApi$CompileTime$ShimsApiParameter$1$Macro(aT1, aP1) \
        aT1 aP1

#define ShimsApi$CompileTime$ShimsApiParameter$2$Macro(aT1, aP1, aT2, aP2) \
        aT1 aP1, \
        aT2 aP2

#define ShimsApi$CompileTime$ShimsApiParameter$3$Macro(aT1, aP1, aT2, aP2, aT3, aP3) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3

#define ShimsApi$CompileTime$ShimsApiParameter$4$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4

#define ShimsApi$CompileTime$ShimsApiParameter$5$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4, \
        aT5 aP5

#define ShimsApi$CompileTime$ShimsApiParameter$6$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4, \
        aT5 aP5, \
        aT6 aP6

#define ShimsApi$CompileTime$ShimsApiParameter$7$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4, \
        aT5 aP5, \
        aT6 aP6, \
        aT7 aP7

#define ShimsApi$CompileTime$ShimsApiParameter$8$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4, \
        aT5 aP5, \
        aT6 aP6, \
        aT7 aP7, \
        aT8 aP8

#define ShimsApi$CompileTime$ShimsApiParameter$9$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4, \
        aT5 aP5, \
        aT6 aP6, \
        aT7 aP7, \
        aT8 aP8, \
        aT9 aP9

#define ShimsApi$CompileTime$ShimsApiParameter$10$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4, \
        aT5 aP5, \
        aT6 aP6, \
        aT7 aP7, \
        aT8 aP8, \
        aT9 aP9, \
        aT10 aP10

#define ShimsApi$CompileTime$ShimsApiParameter$11$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4, \
        aT5 aP5, \
        aT6 aP6, \
        aT7 aP7, \
        aT8 aP8, \
        aT9 aP9, \
        aT10 aP10, \
        aT11 aP11

#define ShimsApi$CompileTime$ShimsApiParameter$12$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4, \
        aT5 aP5, \
        aT6 aP6, \
        aT7 aP7, \
        aT8 aP8, \
        aT9 aP9, \
        aT10 aP10, \
        aT11 aP11, \
        aT12 aP12

#define ShimsApi$CompileTime$ShimsApiParameter$13$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12, aT13, aP13) \
        aT1 aP1, \
        aT2 aP2, \
        aT3 aP3, \
        aT4 aP4, \
        aT5 aP5, \
        aT6 aP6, \
        aT7 aP7, \
        aT8 aP8, \
        aT9 aP9, \
        aT10 aP10, \
        aT11 aP11, \
        aT12 aP12, \
        aT13 aP13

        //
        // Shims api declare
        //

#define ShimsApi$CompileTime$DeclareApi$0$Macro(aFunctionName, aReturnType, aCallType) \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)()

#define ShimsApi$CompileTime$DeclareApi$1$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$1$Macro(aT1, aP1))

#define ShimsApi$CompileTime$DeclareApi$2$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$2$Macro(aT1, aP1, aT2, aP2))

#define ShimsApi$CompileTime$DeclareApi$3$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$3$Macro(aT1, aP1, aT2, aP2, aT3, aP3))

#define ShimsApi$CompileTime$DeclareApi$4$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$4$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4))

#define ShimsApi$CompileTime$DeclareApi$5$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$5$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5))

#define ShimsApi$CompileTime$DeclareApi$6$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$6$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6))

#define ShimsApi$CompileTime$DeclareApi$7$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$7$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7))

#define ShimsApi$CompileTime$DeclareApi$8$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$8$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8))

#define ShimsApi$CompileTime$DeclareApi$9$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$9$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9))

#define ShimsApi$CompileTime$DeclareApi$10$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$10$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10))

#define ShimsApi$CompileTime$DeclareApi$11$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$11$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11))

#define ShimsApi$CompileTime$DeclareApi$12$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$12$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12))

#define ShimsApi$CompileTime$DeclareApi$13$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12, aT13, aP13)  \
    aReturnType aCallType ShimsApi$CompileTime$ShimsApiName$Macro(aFunctionName)( \
        ShimsApi$CompileTime$ShimsApiParameter$13$Macro(aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12, aT13, aP13))

        //
        // Shims api define
        //

#define ShimsApi$CompileTime$EmptyReturn$Macro    


#define _ShimsApi$CompileTime$Str2WStr$Macro(aStr) \
    L ## aStr

#define ShimsApi$CompileTime$Str2WStr$Macro(aStr) \
    _ShimsApi$CompileTime$Str2WStr$Macro(aStr)


#define ShimsApi$CompileTime$MakeStrA$Macro(aStr) \
    #aStr


#define ShimsApi$CompileTime$MakeStrW$Macro(aStr) \
    ShimsApi$CompileTime$Str2WStr$Macro( ShimsApi$CompileTime$MakeStrA$Macro(aStr) )


#define ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) \
    aFunctionName ## $Fun


#define ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) \
    s ## aFunctionName


#define ShimsApi$CompileTime$DefineFunctionType$0$Macro(aFunctionName, aReturnType, aCallType) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)()

#define ShimsApi$CompileTime$DefineFunctionType$1$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1)

#define ShimsApi$CompileTime$DefineFunctionType$2$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2)

#define ShimsApi$CompileTime$DefineFunctionType$3$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3)

#define ShimsApi$CompileTime$DefineFunctionType$4$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4)

#define ShimsApi$CompileTime$DefineFunctionType$5$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4, aT5 aP5)

#define ShimsApi$CompileTime$DefineFunctionType$6$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4, aT5 aP5, aT6 aP6)

#define ShimsApi$CompileTime$DefineFunctionType$7$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4, aT5 aP5, aT6 aP6, aT7 aP7)

#define ShimsApi$CompileTime$DefineFunctionType$8$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4, aT5 aP5, aT6 aP6, aT7 aP7, aT8 aP8)

#define ShimsApi$CompileTime$DefineFunctionType$9$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4, aT5 aP5, aT6 aP6, aT7 aP7, aT8 aP8, aT9 aP9)

#define ShimsApi$CompileTime$DefineFunctionType$10$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4, aT5 aP5, aT6 aP6, aT7 aP7, aT8 aP8, aT9 aP9, aT10 aP10)

#define ShimsApi$CompileTime$DefineFunctionType$11$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4, aT5 aP5, aT6 aP6, aT7 aP7, aT8 aP8, aT9 aP9, aT10 aP10, aT11 aP11)

#define ShimsApi$CompileTime$DefineFunctionType$12$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4, aT5 aP5, aT6 aP6, aT7 aP7, aT8 aP8, aT9 aP9, aT10 aP10, aT11 aP11, aT12 aP12)

#define ShimsApi$CompileTime$DefineFunctionType$13$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12, aT13, aP13) \
    using ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) = aReturnType(aCallType *)(aT1 aP1, aT2 aP2, aT3 aP3, aT4 aP4, aT5 aP5, aT6 aP6, aT7 aP7, aT8 aP8, aT9 aP9, aT10 aP10, aT11 aP11, aT12 aP12, aT13 aP13)


#define ShimsApi$CompileTime$DefineApi$0$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType) \
    ShimsApi$CompileTime$DeclareApi$0$Macro(aFunctionName, aReturnType, aCallType) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$0$Macro(aFunctionName, aReturnType, aCallType);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$1$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1) \
    ShimsApi$CompileTime$DeclareApi$1$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$1$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$2$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2) \
    ShimsApi$CompileTime$DeclareApi$2$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$2$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$3$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3) \
    ShimsApi$CompileTime$DeclareApi$3$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$3$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$4$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4) \
    ShimsApi$CompileTime$DeclareApi$4$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$4$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$5$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5) \
    ShimsApi$CompileTime$DeclareApi$5$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$5$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4, aP5); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$6$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6) \
    ShimsApi$CompileTime$DeclareApi$6$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$6$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4, aP5, aP6); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$7$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7) \
    ShimsApi$CompileTime$DeclareApi$7$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$7$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4, aP5, aP6, aP7); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$8$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8) \
    ShimsApi$CompileTime$DeclareApi$8$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$8$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4, aP5, aP6, aP7, aP8); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$9$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9) \
    ShimsApi$CompileTime$DeclareApi$9$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$9$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4, aP5, aP6, aP7, aP8, aP9); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$10$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10) \
    ShimsApi$CompileTime$DeclareApi$10$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$10$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4, aP5, aP6, aP7, aP8, aP9, aP10); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$11$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11) \
    ShimsApi$CompileTime$DeclareApi$11$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$11$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4, aP5, aP6, aP7, aP8, aP9, aP10, aP11); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$12$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12) \
    ShimsApi$CompileTime$DeclareApi$12$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$12$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4, aP5, aP6, aP7, aP8, aP9, aP10, aP11, aP12); \
        } \
        \
        return aDefaultReturn; \
    }

#define ShimsApi$CompileTime$DefineApi$13$Macro(aDefaultReturn, aGetRoutine, aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12, aT13, aP13) \
    ShimsApi$CompileTime$DeclareApi$13$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12, aT13, aP13) \
    { \
        ShimsApi$CompileTime$DefineFunctionType$13$Macro(aFunctionName, aReturnType, aCallType, aT1, aP1, aT2, aP2, aT3, aP3, aT4, aP4, aT5, aP5, aT6, aP6, aT7, aP7, aT8, aP8, aT9, aP9, aT10, aP10, aT11, aP11, aT12, aP12, aT13, aP13);\
        static ShimsApi$CompileTime$FunctionTypeName$Macro(aFunctionName) ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName) = nullptr; \
        \
        if (nullptr == ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            auto vRoutine = aGetRoutine( ShimsApi$CompileTime$MakeStrA$Macro(aFunctionName) ); \
            InterlockedExchangePointer((void* volatile *)(& ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)), (void*)vRoutine); \
        } \
        if (ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)) \
        { \
            return ShimsApi$CompileTime$FunctionVarName$Macro(aFunctionName)(aP1, aP2, aP3, aP4, aP5, aP6, aP7, aP8, aP9, aP10, aP11, aP12, aP13); \
        } \
        \
        return aDefaultReturn; \
    }

    }
}
