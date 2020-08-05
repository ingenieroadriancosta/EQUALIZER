LONG WINAPI OurCrashHandler(EXCEPTION_POINTERS * exceptionInfo ){
    msgboxf((char*)"An exception occurred which wasn't handled!\nCode: 0x%08X\nAddress: 0x%08X",
             exceptionInfo->ExceptionRecord->ExceptionCode, exceptionInfo->ExceptionRecord->ExceptionAddress);
    INPROCESS = FALSE;
    Shell_NotifyIconA( NIM_DELETE , &nid);
    ExitThread(0);
    return EXCEPTION_CONTINUE_EXECUTION;//g_showCrashDialog ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
}//EXCEPTION_CONTINUE_EXECUTION
