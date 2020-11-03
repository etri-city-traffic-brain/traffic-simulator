/****************************************************************************/
// C++ libsalt.node API implementation
/****************************************************************************/
#pragma once

#include <vector>
#include <libsalt/LibsaltDefs.h>
#include <Object/Network/CellInterface.h>
#include <Object/NetworkManager.h>


/**
 * @class Node
 * @brief C++ libsalt.node API implementation
 */
namespace libsalt {
    class Node {
    private:
        // Invalidate standard constructor
        Node() = delete;

    public:
        // --------------------------------------------------------------
        // Getter
        // --------------------------------------------------------------
        static std::vector<LibsaltNode> getNodeList();
        static LibsaltNode getNode(const std::string& nodeID);

        static std::string getIntersectionType(const std::string& nodeID);
        static LibsaltPosition getLocation(const std::string& nodeID);
        static std::vector<LibsaltConnection> getOrderedConnectionList(const std::string& nodeID);
        static std::map<int, LibsaltConnection> getMapOrderConnection(const std::string& nodeID);

        static std::string getShape(const std::string& nodeID);
        static std::string getCurrentTrafficSignalState(const std::string& nodeID);
        static LibsaltConnection findConnection(const std::string& nodeID, const std::string& fromLink, const std::string& toLink, int fromLane, int toLane);
        static std::string getOwner(const std::string& nodeID);

        // --------------------------------------------------------------
        // Setter
        // --------------------------------------------------------------

    }; // End of class Node

} // End of namespace libsalt

/****************************************************************************/