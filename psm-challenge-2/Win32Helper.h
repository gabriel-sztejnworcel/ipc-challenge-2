#pragma once

#include <Windows.h>

#include <string>

class Win32Helper
{
public:
    static HANDLE create_named_pipe(const std::string& name);
    static void connect_named_pipe(HANDLE pipe_server_handle);

    static void security_descriptor_access_for_everyone(SECURITY_DESCRIPTOR& security_descriptor);

    static DWORD read_file(HANDLE file_handle, char* buf, DWORD len);
    static DWORD write_file(HANDLE file_handle, const char* buf, DWORD len);

    static void impersonate_named_pipe_client(HANDLE pipe_server_handle);
    static void revert_to_self();
};
