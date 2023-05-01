// Datastructures.hh
//
// Student name: Huy Doan
// Student email: huy.doan@tuni.fi
// Student number: 151221835

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

using RegionStationBelong = std::vector<RegionID>;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

struct Station
{
    StationID stationID;
    Name station_name;
    Coord station_coordinate;
    std::vector<std::pair<Time,TrainID>> Train_Departures;

};

struct Region
{
    RegionID regionID;
    Name region_name;
    std::vector<Coord> region_coordinates;
    std::vector<RegionID> vectorOfSubregions;
    std::vector<StationID> vectorOfStations;
    RegionID parentRegion = NO_REGION;
    bool isSubRegion = false;
};

struct Train
{
    TrainID trainID;
    std::vector<std::pair<StationID, Time>> station_times;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    //
    // Old assignment 1 operations
    //

    // Estimate of performance: O(1)
    // Short rationale for estimate: it just returns the size of the unordered map and not looping through any containers
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: it will either clear all elements in the map making the map size of 0
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: it depends on the size of the station maps
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: since it requires looping through finding if station already found, insert is constant time so does not affect the whole function
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: "If the given key exists in unordered_map it returns an iterator to that element otherwise it returns the end of the map iterator."
    Name get_station_name(StationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: "If the given key exists in unordered_map it returns an iterator to that element otherwise it returns the end of the map iterator."
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n^(2)logn)
    // Short rationale for estimate: I think because first adding and looping through vectors = O(n) then sorting is O nlogn
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n^(2)logn)
    // Short rationale for estimate: there are a lot of complex functions going on such as first looping and calculating distance then sorting then looping then insert into final output vector
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: looping through the Station map seeing if matching station id with coord
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find is constant and I assume the same thing for changing coord
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find and add should be constant time
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n^3)
    // Short rationale for estimate: looping through 2 times + std::erase
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: find and push back = constant, loop and sort = O(N)
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: loop to check if region exist and insert = constant time
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: looping and sort
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(1)
    // Short rationale for estimate: https://www.geeksforgeeks.org/unordered_map-find-in-c-stl/
    Name get_region_name(RegionID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: https://www.geeksforgeeks.org/unordered_map-find-in-c-stl/
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(n^4)
    // Short rationale for estimate: there are a lot of loops
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: there are loops and bunch of find and push back functions
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n^4)
    // Short rationale for estimate: there are at least 4 for loops
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(1)
    // Short rationale for estimate: Instantly return the vector of subregions that region has
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n^3)
    // Short rationale for estimate: there are about 3 for loops
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: erase is constant
    bool remove_station(StationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: 2 recursive functions calls
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

    //
    // New assignment 2 operations
    //

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> next_stations_from(StationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> train_stations_from(StationID stationid, TrainID trainid);

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_trains();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Distance>> route_any(StationID fromid, StationID toid);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Distance>> route_least_stations(StationID fromid, StationID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> route_with_cycle(StationID fromid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Distance>> route_shortest_distance(StationID fromid, StationID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Time>> route_earliest_arrival(StationID fromid, StationID toid, Time starttime);

private:
    // Add stuff needed for your class implementation here
    std::unordered_map<StationID,Station>Stations_Map;
    std::unordered_map<RegionID,Region>Regions_Map;
    std::unordered_map<TrainID,Train>Trains_Map;

    std::unordered_map<StationID,RegionStationBelong> Station_Region_Maps;
    RegionID find_biggest_parent(RegionID id);
    bool find_parent_region_and_add(RegionID id);
    void finding_routes(std::unordered_map<TrainID,Train> &trainmap, std::vector<StationID> &Station_Path, StationID fromid, StationID toid);
    void finding_routes2(std::vector<StationID> &Station_Path, StationID fromid, StationID toid);

};

#endif // DATASTRUCTURES_HH
