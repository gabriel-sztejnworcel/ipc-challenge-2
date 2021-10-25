
#include "Win32Helper.h"

#define BUFFER_SIZE 4096

HANDLE Win32Helper::create_named_pipe(const std::string& name)
{
    std::wstring name_wstr(name.begin(), name.end());

    SECURITY_DESCRIPTOR security_descriptor = { 0 };
    security_descriptor_access_for_everyone(security_descriptor);

    SECURITY_ATTRIBUTES security_attributes = { sizeof(SECURITY_ATTRIBUTES) };
    security_attributes.bInheritHandle = FALSE;
    security_attributes.lpSecurityDescriptor = &security_descriptor;
    
    HANDLE pipe_server_handle = CreateNamedPipe(
        name_wstr.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
        PIPE_UNLIMITED_INSTANCES,
        BUFFER_SIZE,
        BUFFER_SIZE,
        NMPWAIT_USE_DEFAULT_WAIT,
        &security_attributes
    );

    if (pipe_server_handle == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error(
            "CreateNamedPipe failed with error code "
            + std::to_string(GetLastError()));
    }

    return pipe_server_handle;
}

void Win32Helper::connect_named_pipe(HANDLE pipe_server_handle)
{
    BOOL result = ConnectNamedPipe(pipe_server_handle, nullptr);
    if (!result)
    {
        throw std::runtime_error(
            "ConnectNamedPipe failed with error code "
            + std::to_string(GetLastError()));
    }
}

void Win32Helper::security_descriptor_access_for_everyone(SECURITY_DESCRIPTOR& security_descriptor)
{
    BOOL result = InitializeSecurityDescriptor(
        &security_descriptor, SECURITY_DESCRIPTOR_REVISION);

    if (!result)
    {
        throw std::runtime_error(
            "InitializeSecurityDescriptor failed with error code "
            + std::to_string(GetLastError()));
    }

    result = SetSecurityDescriptorDacl(
        &security_descriptor, TRUE, nullptr, FALSE);

    if (!result)
    {
        throw std::runtime_error(
            "SetSecurityDescriptorDacl failed with error code "
            + std::to_string(GetLastError()));
    }
}

DWORD Win32Helper::read_file(HANDLE file_handle, char* buf, DWORD len)
{
    DWORD bytes_read = 0;

    BOOL result = ReadFile(file_handle, buf, BUFFER_SIZE, &bytes_read, nullptr);
    if (!result)
    {
        throw std::runtime_error(
            "ReadFile failed with error code "
            + std::to_string(GetLastError()));
    }

    return bytes_read;
}

DWORD Win32Helper::write_file(HANDLE file_handle, const char* buf, DWORD len)
{
    DWORD bytes_written = 0;

    BOOL result = WriteFile(file_handle, buf, len, &bytes_written, nullptr);
    if (!result)
    {
        throw std::runtime_error(
            "WriteFile failed with error code "
            + std::to_string(GetLastError()));
    }

    return bytes_written;
}

void Win32Helper::impersonate_named_pipe_client(HANDLE pipe_server_handle)
{
    BOOL result = ImpersonateNamedPipeClient(pipe_server_handle);
    if (!result)
    {
        throw std::runtime_error(
            "ImpersonateNamedPipeClient failed with error code "
            + std::to_string(GetLastError()));
    }
}

void Win32Helper::revert_to_self()
{
    RevertToSelf();
}

void Win32Helper::close_handle(HANDLE handle)
{
    CloseHandle(handle);
}
