#include "Application.hpp"

int main(int argc, char* argv[]) 
{     
    std::vector<std::string> objectsPaths;
    for (size_t i = 1; i < argc; ++i)
    {
        objectsPaths.emplace_back(argv[i]);
    }
    Application app(objectsPaths);
    app.Run();
}