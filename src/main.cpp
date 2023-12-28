#include <Arln.hpp>

int main()
{
    arln::Window window({});

    while (!window.shouldClose())
    {
        window.pollEvents();
    }
}
