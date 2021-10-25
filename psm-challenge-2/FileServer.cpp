
#include "FileServer.h"
#include "Win32Helper.h"

#include <thread>
#include <mutex>
#include <fstream>
#include <iostream>
#include <stdexcept>

#define BASE_FOLDER "files\\"
#define BUFFER_SIZE 4096

void FileServer::run()
{
    while (true)
    {
        HANDLE pipe_server_handle = Win32Helper::create_named_pipe("\\\\.\\pipe\\file_server");
        output("[+] File server pipe created");

        Win32Helper::connect_named_pipe(pipe_server_handle);
        output("[+] Client connected");

        std::thread client_thread([this, pipe_server_handle]()
        {
            client_handler(pipe_server_handle);
        });

        client_thread.detach();
    }
}

void FileServer::client_handler(HANDLE pipe_server_handle)
{
    bool impersonated = false;
    
    try
    {
        int bad_attempts = 0;
        
        while (true)
        {
            try
            {
                char buf[BUFFER_SIZE] = { 0 };
                DWORD bytes_read = Win32Helper::read_file(pipe_server_handle, buf, BUFFER_SIZE - 1);

                if (!impersonated)
                {
                    Win32Helper::impersonate_named_pipe_client(pipe_server_handle);
                    impersonated = true;
                }

                buf[bytes_read] = '\0';
                std::string request(buf);

                if (request.find("..") != std::string::npos)
                {
                    throw std::invalid_argument("Path traversal attempt");
                }

                if (!request.empty())
                {
                    std::string response = get_file_content(BASE_FOLDER + request);
                    Win32Helper::write_file(pipe_server_handle, response.c_str(), (DWORD)response.length());
                }
            }
            catch (const std::invalid_argument& ex)
            {
                output("[!] ERROR: " + std::string(ex.what()), true);
                std::string response = "Invalid request";
                Win32Helper::write_file(pipe_server_handle, response.c_str(), (DWORD)response.length());
                Win32Helper::revert_to_self();

                if (++bad_attempts == 2)
                {
                    break;
                }
            }
        }

        Win32Helper::revert_to_self();
    }
    catch (const std::exception& ex)
    {
        output("[!] ERROR: " + std::string(ex.what()), true);
        Win32Helper::revert_to_self();
    }

    Win32Helper::close_handle(pipe_server_handle);
}

std::string FileServer::get_file_content(const std::string& path)
{
    std::ifstream input(path);
    if (!input.good())
    {
        return "Could not open file";
    }

    std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    return content;
}

void FileServer::output(const std::string& str, bool error)
{
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);

    if (error)
    {
        std::cerr << str << std::endl;
    }
    else
    {
        std::cout << str << std::endl;
    }
}
