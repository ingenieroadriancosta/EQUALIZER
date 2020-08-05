LONG WINAPI OurCrashHandler(EXCEPTION_POINTERS * exceptionInfo ){
    INPROCESS = FALSE;
    msgboxf((char*)"An exception occurred which wasn't handled!\nCode: 0x%08X\nAddress: 0x%08X",
             exceptionInfo->ExceptionRecord->ExceptionCode, exceptionInfo->ExceptionRecord->ExceptionAddress);

    ExitThread(0);
    return EXCEPTION_CONTINUE_EXECUTION;//g_showCrashDialog ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
}//EXCEPTION_CONTINUE_EXECUTION
