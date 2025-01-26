#ifndef PUBLIC_TRANSPORT_SYSTEM_HPP
#define PUBLIC_TRANSPORT_SYSTEM_HPP

#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>

using std::pair;
using std::tuple;
using std::string;
using std::vector;
using std::list;
using std::unordered_set;
using std::unordered_map;

class PublicTransportSystem
{
private:
	using StopNumber = int;
	using BusNumber = int;
	using Time = int;
	using TimeTable = vector<pair<Time, Time>>;
	using TimePath = vector<tuple<StopNumber, StopNumber, BusNumber, Time, Time>>;
	using LeastTransferPath = vector<tuple<StopNumber, StopNumber, BusNumber>>;

	static const Time MAX_TIME = 24 * 60;
	static const Time INVALID_TIME = -1;
	static const StopNumber INVALID_STOPNUMBER = -1;
	static const BusNumber INVALID_BUSNUMBER = -1;
	static const int INVALID_INDEX = -1;
	const string FILENAME = "transport.txt";

	//Автобусна линия със списък от номера на спирки
	struct BusLine
	{
		list<StopNumber> path;
	};
	// Автобусна спирка с име и множество от автобусни линии, които минават през нея 
	// със съответните времена на пристигане. Ако автобус тръгва от и свършва курса си на една
	// и съща спирка, се използва втората част от наредената двойка във вектора за указване
	// на второто време за пристигане. Иначе е същата като първата.
	struct BusStop
	{
		string name;
		unordered_map<BusNumber, vector<pair<Time, Time>>> busLines;
	};

	//data
	vector<BusStop> stops;
	unordered_map<BusNumber, BusLine> buses;

	void deserialize();

	static Time correctTotalTime(Time firstTime, Time secondTime);
	static void convertAndPrintTime(Time time);
	static bool isDigit(char c);
	static Time extractTime(const string& time);
	static BusNumber extractNumber(const string& strNumber);
	
	static size_t findInsertIndex(const vector<pair<Time, Time>>& timeTable, Time firstTime);
	static void insertArriveTime(vector<pair<Time, Time>>& timeTable, const pair<Time, Time>& newTimes, size_t insertIndex);	
	
	static int findRemoveIndex(const vector<pair<Time, Time>>& timeTable, Time courseStartTime);
	static void deleteTime(vector<pair<Time, Time>>& timeTable, size_t removeIndex);
	static bool isOnPath(const list<StopNumber>& path, StopNumber sn);
	static bool askForConfirmation(const unordered_set<BusNumber>& busLinesThroughStop, 
		const unordered_set<BusNumber>& busLinesToBeDeleted, const string& name);
	static void deleteStopFromPath(list<StopNumber>& path, StopNumber sn);
	
	//static size_t binarySearchFirstTime(const vector<pair<Time, Time>>& timeTable, int start, int end, Time currTime, Time previousTime);
	size_t findNextDepartureTimeIndex(StopNumber sn, BusNumber bn, Time currTime) const;
	static StopNumber findNextStop(StopNumber sn, const list<StopNumber>& path);

	StopNumber findStop(const string& name) const;
	TimePath findFastestRoute(StopNumber start, BusNumber end, Time startTime) const;
	LeastTransferPath findRouteWithFewestTransfers(StopNumber start, StopNumber end) const;
	TimePath findLeastWaitingRoute(StopNumber start, StopNumber end, Time startTime) const;

	static bool validPath(StopNumber start, StopNumber end, Time startTime);
	void printRoute(const TimePath& reversedPath, Time startTime, Time& totalWaitingTime, Time& totalTravelTime) const;
	static void printTotalTime(Time time);
	static void printTotalTravelTime(Time totalTravelTime);
	static void printTotalWaitingTime(Time totalWaitingTime);

	void deleteBusLine(BusNumber bn);
public:
	PublicTransportSystem();
	PublicTransportSystem(const PublicTransportSystem&) = delete;
	PublicTransportSystem& operator=(const PublicTransportSystem&) = delete;
	
	void printFastestRoute(const string& start, const string& end, const string& startTime) const;
	void printRouteWithFewestTransfers(const string& start, const string& end) const;
	void printLeastWaitingRoute(const string& start, const string& end, const string& strTime) const;
	
	void addBusStop();
	void addCourse();
	void addBusLine();

	void deleteBusStop(const string& name);
	void deleteBusLine(const string& bn);
	void deleteCourse(const string& bn);

	void serialize() const;

	void printBusStopNames() const;
	void printBusLines() const;
	void printBusStopTimeTable(const string& name) const;
	void printBusLineTimeTable(const string& bn) const;
};

#endif