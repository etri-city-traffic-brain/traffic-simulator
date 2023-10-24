//
// Created by hwonsong on 2018. 5. 18..
//

#ifndef SALT_SIMSCENARIO_H
#define SALT_SIMSCENARIO_H

#include <utils/config.h>
#include <string>
#include <tuple>
#include <list>
#include <comm/CmdMessage.hpp>
#include <utility>

namespace SALT{

class SimScenarioInfo {
public:
    SimScenarioInfo();

    explicit SimScenarioInfo(string scenariofile); 
    explicit SimScenarioInfo(string scenariofile, string subSimID); // sub simulation in distributed mode
    virtual ~SimScenarioInfo();

    void setOutDirPrefix(string outdirPrefix);

    //Simulation ID
    string simid;

    //VisServer's port
    string simhost = "127.0.0.1";
    int simport = 0;
    int siminterval = 1;

    //Simulation Begin time & End time
    SALTTime simbegin;
    SALTTime simend;

    //PartitionInfo
    string partitionid;
    tuple<float,float,float,float> parea;

    //InputParameters - paths to read file
    string scenarioFile;
    string fileType;
    string dataDir;

    string nodeFile;
    string linkFile;
    string connectionFile;
    string tlsFile;
    list<string> routeFileList;

    string eventFile;
    string weatherFile;

    //ModelParameters
    Length minCellLength = 30.0;
    Length vehLength = 7.0;

    //OutputParameters
    string outputDir;
    string outputPrefix;
    string outputlevel = "cell";
    int outputperiod = 0;
    int outputsave = 0;

    // debug
    string purpose = "normal"; // normal / debug / test
};

}
#endif //SALT_SIMSCENARIO_H





// example1 standalone scenario json file format
// {
//   "scenario": {
//     "id": "test-salt",
//     "time": {
//       "begin": "0",
//       "end": "3599"
//     },
//     "input": {
//       "fileType" : "SALT",
//       "node": "Gangdong-181130/standalone/monday/before/node.xml",
//       "link": "Gangdong-181130/standalone/monday/before/edge.xml",
//       "connection": "Gangdong-181130/standalone/monday/before/connection.xml",
//       "route": "routes/gd_trips_odpair_weighted_selection_mon.rou.xml",
//       "trafficLightSystem": "Gangdong-181130/standalone/monday/before/tss.xml"
      
//     },
//     "parameter":{
//       "minCellLength": "30.0",
//       "vehLength": "3.0"
//     },
//     "output": {
//       "fileDir": "output/test-salt/",
//       "save": "1",
//       "period": "600",
//       "level": "cell"
    
//     }
//   }
// }  






// example 2 distributed mode sceanrio json file
// {
//   "scenario": {
//     "id": "2019-dist-test",
//     "time": {
//       "begin": "0",
//       "end": "43199"
//     },
//     "inputs": {
//       "input": [
//         {
//           "sub-id": "1",
//           "fileType": "SALT",
//           "node": "191104-4-gu-dist-4/sub0.node.xml",
//           "link": "191104-4-gu-dist-4/sub0.edge.xml",
//           "connection": "191104-4-gu-dist-4/sub0.connection.xml",
//           "route": "routes/2019/gd_mon.xml",
//           "trafficLightSystem": "191104-4-gu-dist-4/sub0.tss.xml"
//         },
//         {
//           "sub-id": "2",
//           "fileType": "SALT",
//           "node": "191104-4-gu-dist-4/sub1.node.xml",
//           "link": "191104-4-gu-dist-4/sub1.edge.xml",
//           "connection": "191104-4-gu-dist-4/sub1.connection.xml",
//           "route": "routes/2019/gd_mon.xml",
//           "trafficLightSystem": "191104-4-gu-dist-4/sub1.tss.xml"
//         },
//         {
//           "sub-id": "3",
//           "fileType": "SALT",
//           "node": "191104-4-gu-dist-4/sub2.node.xml",
//           "link": "191104-4-gu-dist-4/sub2.edge.xml",
//           "connection": "191104-4-gu-dist-4/sub2.connection.xml",
//           "route": "routes/2019/gd_mon.xml",
//           "trafficLightSystem": "191104-4-gu-dist-4/sub2.tss.xml"
//         },
//         {
//           "sub-id": "4",
//           "fileType": "SALT",
//           "node": "191104-4-gu-dist-4/sub3.node.xml",
//           "link": "191104-4-gu-dist-4/sub3.edge.xml",
//           "connection": "191104-4-gu-dist-4/sub3.connection.xml",
//           "route": "routes/2019/gd_mon.xml",
//           "trafficLightSystem": "191104-4-gu-dist-4/sub3.tss.xml"
//         }
//       ]
//     },
//     "parameter": {
//       "minCellLength": "30.0",
//       "vehLength": "3.0"
//     },
//     "output": {
//       "fileDir": "output/2019-dist-test/",
//       "save": "1",
//       "period": "600",
//       "level": "cell"
//     }
//   }
// }