#include "session.hpp"

int main()
{
    if (!bank::make_connection())
    {
        std::cerr << "Failed to connect to the server...\n";
        return 0;
    }
    bank::preinit();
    bank::start_bank();
    bank::println("");
    bank::msg m;
    m.as_bytes[0] = _CLOSE_CONN;
    bank::send_msg(m);
    bank::close_connection();
    return 0;
}
