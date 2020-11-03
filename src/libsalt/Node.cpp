/****************************************************************************/
// C++ libsalt.node API implementation
/****************************************************************************/

#include <algorithm>
#include <json/json.h>
#include <Object/NetworkManager.h>
#include <utils/ParserSALT.h>

#include <libsalt/LibsaltDefs.h>
#include "Node.h"
#include "Simulation.h"

namespace libsalt {

    // ===========================================================================
    // Getter Implementation
    // ===========================================================================

    std::vector<LibsaltNode>
    Node::getNodeList() {
        std::vector<LibsaltNode> nodes;
        std::vector<SALT::Node*> rs = Simulation::getNetworkManager()->getNodeList();
        for (SALT::Node* r : rs) {
            LibsaltNode node = LibsaltNode(r);
            nodes.push_back(node);
        }

        return nodes;
    }

    LibsaltNode
    Node::getNode(const std::string& nodeId) {
        std::vector<LibsaltNode> nodes = getNodeList();
        LibsaltNode node = std::find(nodes.begin(), nodes.end(), nodeId).operator*();
        return node;
    }

    std::string
    Node::getIntersectionType(const std::string& nodeID) {
        return (std::string)Simulation::getNetworkManager()->findNodeByID(nodeID)->getMyIntersectionType();
    }

    LibsaltPosition
    Node::getLocation(const std::string& nodeID) {
        SALT::Position p = Simulation::getNetworkManager()->findNodeByID(nodeID)->getMyLocation();
        LibsaltPosition r;
        r.x = p.x;
        r.y = p.y;
        //r.z = INVALID_FLOAT_VALUE;
        return r;
    }

    std::vector<LibsaltConnection>
    Node::getOrderedConnectionList(const std::string& nodeID) {
        std::vector<LibsaltConnection> conns;
        std::vector<SALT::Connection*> rs = Simulation::getNetworkManager()->findNodeByID(nodeID)->getOrderedConnectionList();
        for (auto r : rs) {
            LibsaltConnection conn = LibsaltConnection(r);
            conns.push_back(conn);
        }

        return conns;
    }

    std::map<int, LibsaltConnection>
    Node::getMapOrderConnection(const std::string& nodeID) {
        std::map<int, LibsaltConnection> conns;
        std::map<int, SALT::Connection*> rs = Simulation::getNetworkManager()->findNodeByID(nodeID)->getMapOrderConnection();
        for (auto r : rs) {
            LibsaltConnection conn = LibsaltConnection(r.second);
            conns[r.first] = conn;
        }

        return conns;
    }

    std::string
    Node::getShape(const std::string& nodeID) {
        return Simulation::getNetworkManager()->findNodeByID(nodeID)->getShape();
    }

    std::string
    Node::getCurrentTrafficSignalState(const std::string& nodeID) {
        return Simulation::getNetworkManager()->findNodeByID(nodeID)->getCurrentTrafficSignalState();
    }

    LibsaltConnection
    Node::findConnection(const std::string& nodeID, const std::string& fromLink, const std::string& toLink, int fromLane, int toLane) {
        SALT::Link* from = Simulation::getNetworkManager()->findLinkByID(fromLink);
        SALT::Link* to = Simulation::getNetworkManager()->findLinkByID(toLink);
        LibsaltConnection conn = LibsaltConnection(Simulation::getNetworkManager()->findNodeByID(nodeID)->findConnection(from, to, fromLane, toLane));
        return conn;
    }

    std::string
    Node::getOwner(const std::string& nodeID) {
        return Simulation::getNetworkManager()->findNodeByID(nodeID)->getOwner();
    }

    // ===========================================================================
    // Setter Implementation
    // ===========================================================================

} // end of namespace libsalt

/****************************************************************************/