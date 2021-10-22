
#include "FileServer.h"

#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        FileServer file_server;
        file_server.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[!] ERROR: " << ex.what() << std::endl;
    }
}
