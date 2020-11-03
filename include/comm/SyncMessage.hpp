#ifndef SYNCMESSAGE_HPP
#define SYNCMESSAGE_HPP

#include <string>
#include <tuple>
#include <vector>
#include "SerializeTuple.hpp"


namespace salt_comm{

enum SyncMessageType{
	START_WORKER,
	STOP_WORKER,
	SYNC_DATA
	//AGGREGATION_DATA
};


struct SyncMessage {
	SyncMessageType type_;
	std::string workerIp_;
	int workerPort_;
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & type_;
		ar & workerIp_;
		ar & workerPort_;
	}
};


struct AggregationData {
	long startTimeStep_;
	long endTimeStep_;
	std::string roadId_;
	int avgVehicleNum_;
	float avgSpeed_;
	float avgDensity_;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & startTimeStep_;
		ar & endTimeStep_;
		ar & roadId_;
		ar & avgVehicleNum_;
		ar & avgSpeed_;
		ar & avgDensity_;
	}
};

struct SyncDataElement {
	//long timeStep_;
	std::string vehicleId_;
	std::string route_;
	std::string currentLinkId_;
	std::string nextLinkId_;
	std::string dstPartitionID;
	//std::tuple<std::string, std::string> boundaryMapKey_;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		//ar & timeStep_;
		ar & vehicleId_;
		ar & route_;
		ar & currentLinkId_;
		ar & nextLinkId_;
//		ar & boundaryMapKey_;
		ar & dstPartitionID;
	}
                                       
};

struct WorkerSyncReq {
	bool isFinal_;
	std::string simId_;
	std::string workerIp_;
	int workerPort_;
	std::string srcPartitionID;
	long timeStep_;


	std::vector<SyncDataElement> syncDataElements_;
	bool existAggregationData_;
	AggregationData aggregationData_;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & isFinal_;
		ar & simId_;
		ar & workerIp_;
		ar & workerPort_;
		ar & srcPartitionID;
		ar & timeStep_;
		ar & syncDataElements_;
		ar & existAggregationData_;
		ar & aggregationData_;
	}
};
                                       
	

struct MasterSyncRes {
	std::string simId_;
	std::string dstPartitionID;	
	long timeStep_;

	std::vector<SyncDataElement> syncDataElements_;
	
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & simId_;
		ar & dstPartitionID;
		ar & timeStep_;
		ar & syncDataElements_;
		//ar & boundaryMapKey_;
	}                                       
};


} //end of namespace
#endif
