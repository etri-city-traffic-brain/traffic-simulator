// protocol message between SALT and SALT-VIS
#ifndef VIS_MSG_H
#define VIS_MSG_H

#pragma pack(push, 1)

#define MAX_VEHICLES 48
#define MAX_ROAD_ID 16
#define MAX_ROADS 1024 * 2
#define MAX_SIMULATION_ID 17
#define EXTEND_LENGTH 4

// message type
#define MSG_INIT 0
#define MSG_DATA 1
#define MSG_STATUS 2
#define MSG_SET 10
#define MSG_STOP 11

// VisClient's status
enum VisClientState : unsigned char {
    RUNNING,
    FINISHED,
    FAILURE
};

// road type
enum VisRoadType : unsigned char {
    LINK,
    CELL
};

// signal type
enum VisSignalType : unsigned char {
    RED,
    GREEN
};

// class
enum VisVehicleClass : unsigned char {
    SMALL,
    LARGE
};

struct MsgHeader {
    unsigned int type; // INIT(0), DATA(1), STATUS(2), SET(10), STOP(11)
    long timestamp; // milliseconds
    unsigned int bodySize;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & type;
        ar & timestamp;
        ar & bodySize;
    }
};

// SALT-VIS -> SALT
struct SetMsg {
    struct MsgHeader header;
    float extend[EXTEND_LENGTH]; // minx, miny, maxx, maxy
    VisRoadType roadType; // LINK, CELL

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & header;
        ar & extend;
        ar & roadType;
    }
};

// SALT-VIS --> SALT
struct StopMsg  {
    struct MsgHeader header;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & header;
    }
};

// SALT --> SALT-VIS
struct InitMsg  {
   struct MsgHeader header;
   char simulationId[MAX_SIMULATION_ID];

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & header;
        ar & simulationId;
    }
};

// SALT --> SALT-VIS
struct StatusMsg {
    struct MsgHeader header;
    VisClientState status; // RUNNING, FINISHED, FAILURE
    unsigned int progress; // 0 ~ 100

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & header;
        ar & status;
        ar & progress;
    }
};

struct Road {
    char roadId[MAX_ROAD_ID]; // cellId or linkId
    unsigned int speed; // road's mean speed
    VisSignalType currentSignal; // RED, GREEN
    unsigned int numVehicles; // number of vehicles
    VisVehicleClass vehicles[MAX_VEHICLES] = { VisVehicleClass::SMALL, }; // SMALL, LARGE
    //VisVehicleClass* vehicles;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & roadId;
        ar & speed;
        ar & currentSignal;
        ar & numVehicles;
        ar & vehicles;
    }
};

// SALT --> SALT-VIS
struct DataMsg {
    struct MsgHeader header;
    unsigned int numRoads;
    struct Road roads[MAX_ROADS];

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & header;
        ar & numRoads;
        ar & roads;
    }
};

#pragma pack(pop)

#endif // end of #define VIS_MSG_H