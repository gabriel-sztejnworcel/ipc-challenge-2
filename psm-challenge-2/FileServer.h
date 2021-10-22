#pragma once

#include <Windows.h>

#include <string>

class FileServer
{
public:
    void run();

private:
    void client_handler(HANDLE pipe_server_handle);
    std::string get_file_content(const std::string& path);
    void output(const std::string& str, bool error = false);
};
