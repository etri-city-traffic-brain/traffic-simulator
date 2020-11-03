/****************************************************************************/
// C++ libsalt.connection API implementation
/****************************************************************************/

#include <algorithm>
#include <json/json.h>
#include <Object/NetworkManager.h>
#include <utils/ParserSALT.h>

#include <libsalt/LibsaltDefs.h>
#include "Connection.h"
#include "Simulation.h"

namespace libsalt {

// ===========================================================================
// Getter Implementation
// ===========================================================================

std::vector<LibsaltConnection>
Connection::getConnectionList() {
    std::vector<LibsaltConnection> conns;

    const auto rs = Simulation::getNetworkManager()->getConnectionList();
    for (auto r: rs) {
        LibsaltConnection conn = LibsaltConnection(r);
        conns.push_back(conn);
    }
    return conns;
}

// ===========================================================================
// Setter Implementation
// ===========================================================================

}

/****************************************************************************/