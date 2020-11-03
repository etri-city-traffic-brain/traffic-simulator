/*
 * config.h
 *
 *  Created on: 2018. 1. 30.
 *      Author: SDJ
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <random>

#define NO_RANDOM
#define TSS_ERROR_IGNORE
#define NETWORK_ERROR_IGNORE
#define INPUT_ERROR_VERBOSE
//#define DISTRIBUTED
//#define DISTRIBUTED_DEBUG
//#define MASTER_WORKER_DEBUG

//#define TOTAL_SECTION_COMPUTE

using namespace std;

namespace SALT{

typedef int SALTTime;
typedef float Speed;
typedef float Length;
typedef std::string ID;
typedef std::string TypeName;
typedef std::string PhaseStatus;

enum Result{
	FAILURE,
	SUCCESS
};
enum SCOPE{
	MICRO,
	MESO,
	MACRO
};
enum VehicleClass {
    PASSENGER,
    BUS
};
enum ErrorCode{
	NODE_INSTANCE_NOT_EXIST,
	EDGE_INSTANCE_NOT_EXIST,
	LANE_INSTANCE_NOT_EXIST,
	CONNECTION_INSTANCE_NOT_EXIST,
	LANE_OUT_OF_INDEX
};
enum SimulationState{
	SIMULATION_NOT_READY,
	SIMULATION_READY,
	SIMULATION_RUNNING,
	SIMULATION_PAUSED,
	SIMULATION_END
};
#define NO_LIMIT 99999999
#define DEFAULT_SPEED 60
#define SIMULATION_STEP_LIMIT 20

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define ABS(X) ((X) > 0 ? (X) : (-X))


class Vector2D{
public:
	float x;
	float y;
	Vector2D(float x=0,float y=0) : x(x), y(y){}
	Vector2D(const Vector2D& v) : x(v.x), y(v.y) {}
	float computeDistance(const Vector2D & v) const;
	float computeLength() const;

	string to_string();

	Vector2D operator+(const Vector2D& v) const;
	Vector2D operator-(const Vector2D& v) const;
	Vector2D getTurnLeftVector() const;
	Vector2D getTurnRightVector() const;

	Vector2D& normalize();
};

typedef Vector2D Position;

enum EventType{
	ACCIDENT,
	CONSTRUCTION
};

enum TrafficSignalState{
	GREEN, // G
	WEAK_GREEN, //g
	RED, //r
	YELLOW //y
};

enum class SpreadType {
	RIGHT
	, CENTER
};
class RandomGenerator
{
public:
	//RandomGenerator() : m_mt((std::random_device())()){};
	RandomGenerator();
	int getSample(int low, int high);
	float getFloatSample(float low, float high);
	bool getTrueWithProb(float prob);
private:
	mt19937 m_mt;
};
}
#endif /* CONFIG_H_ */
