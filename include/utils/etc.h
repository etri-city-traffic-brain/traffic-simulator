/*
 * etc.h
 *
 *  Created on: 2018. 2. 2.
 *      Author: SDJ
 */

#ifndef ETC_H_
#define ETC_H_

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include <json/json.h>
#include <utils/config.h>
#include <string>

#include <list>
//parse json file using jsoncpp library
std::string joinPath(std::string _dir, std::string _fileName);

SALT::Result getJSONObject(std::string const& file_path, Json::Value& root);
std::string convertJSONObject2string(const Json::Value&  _obj);
std::string jv2string(Json::Value& _input, std::string defaultValue="");
int jv2int(Json::Value& _input, int defaultValue=0);
float jv2float(Json::Value& _input, float defaultValue=0.0);
int str2int(string _input);

string generateRandomNode(int numNode);
string generateRandomLink(int numNode);
string generateRandomConnection(int numNode);
string generateRandomRoute(int numNode, int numVeh);
void generateRandomData(int numNode, int numVeh, int numPartition=1, string resultDataRootDirPath="/data/");

string generateRandomCutPartition2(int numNode);
vector<string> generateRandomNodePartition2(int numNode);
vector<string> generateRandomLinkPartition2(int numNode);
vector<string> generateRandomConnectionPartition2(int numNode);
vector<string> generateRandomTLSPartition2(int numNode);



//parse json file using boost




//


void unifyWorkerResult(string const& outputFilePath, list<string> const&  workerOutputFilePathList);
#endif /* ETC_H_ */
