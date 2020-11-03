/****************************************************************************/
// C++ libsalt.connection API implementation
/****************************************************************************/
#pragma once

#include <vector>
#include <libsalt/LibsaltDefs.h>
#include <Object/Network/CellInterface.h>
#include <Object/NetworkManager.h>

/**
 * @class Connection
 * @brief C++ libsalt.connection API implementation
 */
namespace libsalt {
    class Connection {
    private:
        // Invalidate standard constructor
        Connection() = delete;

    public:
        // --------------------------------------------------------------
        // Getter
        // --------------------------------------------------------------

        static std::vector<LibsaltConnection> getConnectionList();

        // --------------------------------------------------------------
        // Setter
        // --------------------------------------------------------------

    }; // End of class Network

} // End of namespace libsalt

/****************************************************************************/