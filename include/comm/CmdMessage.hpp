#ifndef CMDMESSAGE_HPP
#define CMDMESSAGE_HPP

#include <string>
#include <tuple>
#include "SerializeTuple.hpp"

namespace salt_comm{

enum CmdMessageType {
	START_SIM,
	PAUSE_SIM,
	RESUME_SIM,
	STOP_SIM,
	TERMINATE_WORKER
};


struct CmdMessage {
	CmdMessageType type_;
	std::string simId_;
	long timeStep_;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & type_;
		ar & simId_;
		ar & timeStep_;
	}
};

struct SubScenarioMessage{
	std::string partitionID;
	std::string subScenarioFile_;
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & partitionID;
		ar & subScenarioFile_;
	}
};


//
////SimScenrioInfo  structure
//struct ScenarioInfoContent {
//	//simulation ID
//	std::string simId_;
//
//	//simulation begin time & end time
//	int startTimeStep_;
//	int endTimeStep_;
//
//	//partition info
//	//	std::string partitionId_;
//	//	std::tuple<float, float, float, float> partitionArea_;
//
//	//input parameters
//	std::string fileType;
//	std::string dataDir_;
//	std::string nodeFile_;
//	std::string linkFile_;
//	std::string connectionFile_;
//	std::string routeFile_;
//	std::string tlsFile_;
//	//	std::string eventFile_;
//	//	std::string weatherFile_;
//
//	//	std::string scenarioFile_;
//
//	//model parameters
//	float minCellLength_;
//	float vehicleLength_;
//
//
//	//OutputParameters
//	//	std::string outputPrefix_;
//	//	std::string outputFile_;
//	std::string outputLevel_;
//	int outputPeriod_;
//	//	int outputSave_;
//
//	template <typename Archive>
//	void serialize (Archive& ar, const unsigned int version) {
//		ar & simId_;
//		ar & startTimeStep_;
//		ar & endTimeStep_;
//		//		ar & partitionId_;
//		//		ar & partitionArea_;
//		ar & fileType;
//		ar & dataDir_;
//		ar & nodeFile_;
//		ar & linkFile_;
//		ar & connectionFile_;
//		ar & routeFile_;
//		ar & tlsFile_;
//		//		ar & eventFile_;
//		//		ar & weatherFile_;
//		//        ar & scenarioFile_;
//		ar & minCellLength_;
//		ar & vehicleLength_;
//		//		ar & outputPrefix_;
//		//		ar & outputFile_;
//		ar & outputLevel_;
//		ar & outputPeriod_;
//		//		ar & outputSave_;
//	}
//}; // endo of struct SimScenarioInfo



} // end of namespace salt_comm 

#endif
