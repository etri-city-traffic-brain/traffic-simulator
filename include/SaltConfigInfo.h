//
// Created by hwonsong on 2018. 8. 27..
//

#ifndef WORK_SALTCONFIGFILE_H
#define WORK_SALTCONFIGFILE_H


#include <string>
#include <vector>
#include <utils/config.h>

namespace SALT {

//struct DBConnInfo {
//    std::string host;
//    std::string dbname;
//    std::string user;
//    std::string passwd;
//};

struct SaltMasterConfig {
	//    std::string masterip;
	int mport;
	std::vector<std::string> workerIPList;
	std::vector<int> workerPortList;
	//    int simnumber;
	//        std::vector<std::string> simscenarios;
	//    DBConnInfo dbinfo;
	//    std::string dbinfo;
};

//struct SaltWorkerConfig {
//    std::string masterip;
//    std::string masterport;
//    std::string workerport;
////    int simnumber;
////    DBConnInfo dbinfo;
////    std::string dbinfo;
//};



class SaltConfigInfo {
public:
	SaltConfigInfo(std::string config);

	//    std::string dbconn;
	struct SaltMasterConfig saltmconf;
	//    SaltWorkerConfig saltwconf;
	//
	//        //Simulation ID
	//        std::string simid;
	//
	//        //Simulation Begin time & End time
	//        SALTTime simbegin;
	//        SALTTime simend;
	//
	//        //Scenario file info
	//        std::string scenario;
	//
	//        //Partitions info
	//        int pnum;
	//        std::vector<std::string> partitions;
	//
	//        //ModelParameters
	//        Length minCellLength = 30.0;
	//        Length vehLength = 7.0;
	//
	//        //OutputParameters
	//        std::string outputPrefix;
	//        std::string outputFile;
	//        std::string outputlevel = "cell";
	//        int outputperiod = 0;
	//        int outputsave = 0;
private:
	//        void getSimTimeInfo(std::string scenariofile);
};


}

#endif //WORK_SALTCONFIGFILE_H
