//
// Created by hwonsong on 2018. 8. 10..
//

#ifndef WORK_SALTSIMMANAGER_H
#define WORK_SALTSIMMANAGER_H

#include <string>
#include <utils/config.h>
#include <set>
#include <tuple>
#include <map>
#include <comm/CmdMessage.hpp>
#include <SaltConfigInfo.h>


namespace SALT {
class SimScenarioManager {

public:
	//        SimScenarioManager(){};
	//        SimScenarioManager(SaltConfigInfo * saltconf);
	SimScenarioManager(std::string scenarioFilePath);

//	const std::vector<salt_comm::ScenarioInfoContent>& getScenarioInfoContentList(){;
//		return mySubScenarioList;
//	}


	const std::vector<salt_comm::SubScenarioMessage>& getSubScenarioMessageList(){;
		return mySubScenarioMessageList;
	}
	//        set <SALT::ID> getPartitionIDs();

	string getSimulationID() const {
		return simid;
	}

	SALTTime getSimend() const {
		return simend;
	}

	const std::string& getUnifiedOutputPath() const {
		return myUnifiedOutputPath;
	}

	const list<std::string>& getWorkerOutputPathList() const {
		return myWorkerOutputPathList;
	}

	//        void assignJobs(string workerIP, int workerPort);

	//	const map<SALT::ID, tuple<string, int> >& getJobMapper() const {
	//		return mapJob2Worker;
	//	}

private:

	//        //Simulation ID
	//        string simid;
	//
	//        //Simulation Begin time & End time
	//
	//        SALT::SALTTime simbegin;
	//        SALT::SALTTime simend;
	//
	//        //Simulation Traget Area Info
	//        int areatype;
	//        string area;
	//        tuple<float, float, float, float> areagps;
	//
	//        //Number of Partition
	//        int numPartitions;
	//
	//        //Simulation Model Parameters
	//        float minCellLength;
	//        float vehLength;
	//
	//        //Output Parameters
	//        int outputSave;
	//        string outputPrefix;
	//        string outputlevel;
	//        int outputperiod;
	//
	//        //DB Connection Info
	//        string dbhost;
	//        string dbport;
	//        string dbusers;
	//        string dbpasswd;

	//Simulation ID
	string simid="";

	//Simulation Begin time & End time
	SALTTime simbegin=0;
	SALTTime simend=0;

	//PartitionInfo
	int pnumber=1;
	//
	//        //InputParameters
	//        string scenario;
	//
	//        //ModelParameters
	//        Length minCellLength = 30.0;
	//        Length vehLength = 7.0;
	//
	//        //OutputParameters
	//        string outputPrefix;
	//        string outputlevel = "cell";
	//        int outputperiod = 0;
	//        int outputsave = 0;
	//    map <SALT::ID, string> partitions;
	//    map <SALT::ID, tuple<float,float,float,float>> partitionsgps;
	vector<salt_comm::SubScenarioMessage> mySubScenarioMessageList;

	list<std::string> myWorkerOutputPathList;
	std::string myUnifiedOutputPath="";


//	vector<salt_comm::ScenarioInfoContent> mySubScenarioList;
	// map[subScenarioID]=<worker ip,port>
	//        map<SALT::ID, tuple<string, int>> mapJob2Worker;
	//        map<SALT::ID, salt_comm::SimScenarioInfo> wscenarioinfos;

	//        map<SALT::ID, tuple<float, float, float, float>> getPartitionInfoGPS(int _pnum);

	//        map<SALT::ID, string> getPartitionInfo(int _pnum);
//	void buildSubScenarioList();
	void genSimTimeInfo(string starttime, string endtime);
	//        void genPidSet();

};

}


#endif //WORK_SALTSIMMANAGER_H
