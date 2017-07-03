#pragma once

#ifndef UnitTest$CompileTime$Log$Macro
#define UnitTest$CompileTime$Log$Macro(aLevel, aFormat, ...) \
do{ \
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, aLevel,     \
        "MBox [%04u, %s]-> " aFormat "\n",      \
        __LINE__, __FUNCTION__, __VA_ARGS__);   \
}while(0)
#endif
