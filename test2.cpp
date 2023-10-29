#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>

int main()
{
    const char *pipeName = "my_pipe"; // Choose a suitable name
    std::ofstream pipe(pipeName);

    if (!pipe.is_open())
    {
        std::cerr << "Failed to open the named pipe." << std::endl;
        return 1;
    }

    while (true)
    {
        // Your C++ program generates messages
        std::string message = "Hello from C++!";
        pipe << message << std::endl;
    }

    pipe.close();
    return 0;
}
