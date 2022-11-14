//
// Created by hwonsong on 2018. 9. 17..
//

#ifndef SALT_OUTPUTWRITER_H
#define SALT_OUTPUTWRITER_H

#include <utils/config.h>
#include <set>
#include <map>
#include <tuple>
#include <SimScenarioInfo.h>
#include <Object/Network/CellInterface.h>


namespace SALT{

class NetworkManager;


class OutputWriter{
public:
	OutputWriter(SimScenarioInfo *scenario, NetworkManager* NM);
	virtual ~OutputWriter();
	void setup();
	// void initAnalysisTarget(string analysisTargetFile="");

	// @brief: 1) for all cells, compute its periodic result
	//			2) for all links, compute its periodic result based on cell results
	void computePeriodicResult(SALTTime curTimeStep);
    void computePeriodicResultV3(SALTTime curTimeStep);
	//
	void fillPeriodicResultBuffer(SALTTime tPeriodStart, SALTTime tPeriodEnd);
	void fillPeriodicResultBufferV3(SALTTime tPeriodStart, SALTTime tPeriodEnd);

	void writePeriodicResult();

	// @brief: order all roads to collect its periodic result
	//			(collect, compute stats, write)
	// @param: current timestep
	// @return: whether actually execute periodic process or not
	bool doPeriodicProcess(SALTTime curTimeStep);
	bool doPeriodicProcessV3(SALTTime curTimeStep);

	// @brief: get begin, end of the interval current time step belongs to
	// @param: current timestep
	// @return: <interval begin, interval end>
	std::tuple<SALTTime,SALTTime> getAvgInterval(SALTTime curTimeStep);

	void writeProgresStatus(string _progress);

	const string& getPeriodicFileFullPath() const {
		return myPeriodicFileFullPath;
	}

	const string& getOutputDir() const {
		return myOutputDir;
	}
	
private:
	NetworkManager* myNetworkManager=NULL;
	ID mySimID="";
	string myParitionID="";

	SALTTime mySimStarttime=-1;
	SALTTime mySimEndtime=-1;

	SALTTime myPeriod=-1;
	int myOutputSaveFlag=0; //0: default(not save the output file), 1: save the output file
	string myOutputLevel="link"; // link, cell

	string myOutputDir="";

	string myOutputPrefix="";


	string myProgressStatusFilePath = "";

	string myPeriodicFileFullPath="";

	// (variable)
//	string myPeriodicOutputHeader = "intervalbegin,intervalend,roadID,VehPassed,AverageSpeed,AverageDensity,WaitingQLength,WaitingTime,SumTravelLength,SumTravelTime\n";    // for V1 & V2
	string myPeriodicOutputHeader = "intervalbegin,intervalend,roadID,VehPassed,AverageSpeed,AverageDensity,WaitingQLength,WaitingTime,SumTravelLength,SumTravelTime\n";    // for V3
	string myPeriodicOutputBuffer="";

	std::ofstream* myProgressStatusFileStream=NULL;
	std::ofstream* myPeriodicOutputFileStream=NULL;
};

}



#endif //SALT_OUTPUTWRITER_H
