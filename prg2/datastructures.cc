// Datastructures.cc
//
// Student name: Huy Doan
// Student email: huy.doan@tuni.fi
// Student number: 151221835

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <map>

#include <math.h>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

/**
 * @brief station_count returns the number of stations in the station map
 * @return size of station unordered map
 */
unsigned int Datastructures::station_count()
{
    if(Stations_Map.size() != 0)
        return Stations_Map.size();
}

/**
 * @brief clear_all clears out the data structures: station and region map
 */
void Datastructures::clear_all()
{
    Stations_Map.clear();
    Regions_Map.clear();
}


/**
 * @brief all_stations returns all the stations in Station map
 * @return a vector of Station IDs
 */
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> vect_of_StationID;
    for (auto& x: Stations_Map)
    {
        vect_of_StationID.push_back(x.first);
    }
    return vect_of_StationID;
}

/**
 * @brief add_station Adds a station to the data structure with the given unique id, name, type, and coordinates.
 * if the program cannot find the passed in station id, return false, else add in the data structure of station maps
 * in this function I also add in another data sturcture map of key station id and region id in order to be used in station in regions function later (this function will return as the output for that function)
 * @param id the station id to be added to the data structure
 * @param name name of the station to be added
 * @param xy coordinates of the station to be added
 * @return true or false depending on if found the station successfully
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    //first loop thorugh station map station already existed?
    for(auto element:Stations_Map){
        if(element.first == id){
            return false;
        }
    }

    //if not exist then insert in the station map
    Station station_obj = Station{id, name, xy, {}};
    std::pair<StationID,Station> new_station = std::make_pair(id, station_obj);
    Stations_Map.insert(new_station);

    //add new key for station regions map
    std::pair<StationID,RegionStationBelong> new_value (id,{});
    Station_Region_Maps.insert(new_value);

    return true;
}

/**
 * @brief get_station_name Returns the name and type of the station with the given ID, or NO_NAME if such a station doesn't exist
 * @param id the station id to fetch information
 * @return the name of wanted station
 */
Name Datastructures::get_station_name(StationID id)
{
    //find if station exist
    std::unordered_map<StationID,Station>::const_iterator map_iter = Stations_Map.find(id);

    //if station cannot be found in map then return no name
    if ( map_iter == Stations_Map.end() )
      return NO_NAME;
    else
      return map_iter->second.station_name;
}

/**
 * @brief get_station_coordinates Returns the name of the station with the given ID, or value NO_COORD, if such a station doesn't exist.
 * @param id station to fetch coordinate information
 * @return the coordinate of wanted station
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    std::unordered_map<StationID,Station>::const_iterator map_iter = Stations_Map.find(id);

    if ( map_iter == Stations_Map.end() )
      return NO_COORD;
    else
      return map_iter->second.station_coordinate;
}

/**
 * @brief String_compareFunction acts as condition in std::sort in ascending order for Station ID
 * @param a name or station ID of first element sorting
 * @param b name or station ID of second element sorting
 * @return the ascending order a<b
 */
bool String_compareFunction (Name a, Name b) { return a<b;}

/**
 * @brief stations_alphabetically Returns station IDs sorted according to alphabetical order of station names. Stations with the same name can be in any order with respect to each other.
 * @return a vector of station id sorted alphabetically based on its station name
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<Name> vect;
    std::vector<StationID> final_output;

    //O(n)
    for (auto& it: Stations_Map) {
        vect.push_back(it.second.station_name);
    }

    //n log n
    std::sort(vect.begin(), vect.end());

    //adding matching names in station map and vect of names into final output
    //O(n^2)
    for(auto element:vect){
        for(auto map_element:Stations_Map){
            if(map_element.second.station_name==element){
                final_output.push_back(map_element.first);
            }
        }
    }

    return final_output;
}

/**
 * @brief compare acts as condition in std::sort in ascending order for Station coordinate
 * @param a coord of first element sorting
 * @param b coord of second element sorting
 * @return the ascending order a<b
 */
bool compare(std::pair<StationID, float>& a,
         std::pair<StationID, float>& b)
{
    return a.second < b.second;
}

/**
 * @brief stations_distance_increasing Returns station IDs sorted according to their coordinates
 * @return a vector of station IDs
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> final_output;

    //create a map of station id and coordinate to help us sort by coordinate icnreasing
    std::map<StationID, float> map;

    //O(n) + insert = O(1)
    for (auto& it: Stations_Map) {
        float distance = sqrt(pow((it.second.station_coordinate.x - 0),2) +  pow((it.second.station_coordinate.y - 0),2));
        map.insert({it.first, distance});
    }

    //O(n)
    std::vector<std::pair<StationID, float> > vector_pairs;
    for (auto& it : map) {
        vector_pairs.push_back(it);
    }

    //O n log n
    sort(vector_pairs.begin(), vector_pairs.end(),compare);

    //O n --> inserting into an output of vector of station id after sorting the vector of pairs
    for (auto& iter : vector_pairs) {
        final_output.push_back(iter.first);
    }

    return final_output;
}

/**
 * @brief find_station_with_coord Returns a station with the given coordinate, return NO_STATION, if no such station exists
 * @param xy the coordinate of the station to be found
 * @return the station id with that coordinate
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    for (auto& it: Stations_Map) {
        if(it.second.station_coordinate.x == xy.x && it.second.station_coordinate.y == xy.y)
        {
            return it.first;
        }
    }
    return NO_STATION;
}

/**
 * @brief change_station_coord Changes the location of the station with given ID. If such station doesn't exist, returns false, otherwisetrue.
 * @param id of station ID to be changed
 * @param newcoord the new coord for that station id
 * @return true or false depending on if the operation succeeds
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    try {
        auto it = Stations_Map.find(id);
        if(it != Stations_Map.end())
        {
          it->second.station_coordinate = newcoord;
          return true;
        }
    }  catch (...) {
        return false;
    }
}

/**
 * @brief add_departure Adds information that the given train leaves from the
given station at the given time. If such station doesn't
exist or the departure has already been added (train
already leaves from given station at the given time),
returns false, otherwise true.
 * @param stationid which train the station belongs to
 * @param trainid id of the train to be departed
 * @param time time of the train departure
 * @return true or false depnding on the success of the operation
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    try {
        const auto& it = Stations_Map.find(stationid);
        std::pair<Time,TrainID> new_train_time (time,trainid);
        if(it != Stations_Map.end())
        {
            auto vector_of_train_departures = it->second.Train_Departures;
            it->second.Train_Departures.push_back(new_train_time);
            return true;
        }
        else{
            return false;
        }
    }  catch (...) {
        return false;
    }
}



/**
 * @brief remove_departure Removes the given train departure from the given
station at the given time. If such a station or such
departure from the station doesn't exist, returns
false, otherwise true.
 * @param stationid of the station to remove train departure
 * @param trainid trainid of the removing departure
 * @param time time of the removing departure
 * @return true or false depending on the success of operations (such as if station or train id exsists)
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    try {
        const auto& it = Stations_Map.find(stationid);

        //if station is found
        if(it != Stations_Map.end())
        {
            auto& train_vector = it->second.Train_Departures;
            if(train_vector.size()==0){
                return false;
            }

            bool found = false;
            int i = 0;

            //checking logic if the train departure can be found O(n)
            for(auto train_departure: train_vector){
                if(train_departure.first == time && train_departure.second == trainid){
                    found = true;
                }
            }

            if(found == false){
                return false;
            }

            //logic for deleting train departure taking into account one train many time departures
            for(auto train_departure: train_vector){
                if(train_departure.first == time && train_departure.second == trainid){
                    try {
                        train_vector.erase(train_vector.begin()+i);
                        return true;
                    }  catch (...) {
                        return false;
                    }
                }else{
                    i++;
                }
            }

        } else {
            return false;
        }
    }
    catch (...) {
        return false;
    }
}

/**
 * @brief sortbysec sorting the vector of pairs based on pair value, not key
 * @param a first pair for comparing
 * @param b second pair for comparing
 * @return true or false
 */
bool sortbysec(const std::pair<Time,TrainID> &a,
              const std::pair<Time,TrainID> &b)
{
    return (a.first < b.first);
}

/**
 * @brief station_departures_after Lists all train departures from the given station at or
after the given time. Departures should be sorted based
on departure time. If trains have the same departure
time, they should be sorted based on their ids. If the
given station doesn't exist, pair {NO_TIME,
NO_TRAIN} is returned.
 * @param stationid station to list all departures of
 * @param time condition of what time departure to be outputed
 * @return a vector of pairs time and train id sorting according to time asc
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time, TrainID>> output_list;
    auto it = Stations_Map.find(stationid);
    //std::pair<TrainID,Time> new_train_time (trainid,time);
    if(it != Stations_Map.end())
    {
        auto train_vector = it->second.Train_Departures;
        for(auto element : train_vector){
            if(element.first >= time){
                output_list.push_back(element);
            }
        }
        sort(output_list.begin(), output_list.end(), sortbysec);
    } else {
        return {{NO_TIME, NO_TRAIN}};
    }

    return output_list;

//    if(output_list.size()!=0){

//    }
//    else{
////        std::vector<std::pair<Time, TrainID>> no_output = {{NO_TIME, NO_TRAIN}};
//        return {{NO_TIME, NO_TRAIN}};
//    }
}

/**
 * @brief add_region Adds a region to the data structure with given unique id, name and polygon (coordinates).
 * @param id of region to be added
 * @param name of the region
 * @param coords vector of coordinates regions to be added
 * @return true or false depending on the success of the operation
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    //if region already exist O(n)
    for(auto element : Regions_Map){
        if(element.first == id){
            return false;
        }
    }

    Region region_obj = Region{id, name, coords, {}, {}, false};
    std::pair<RegionID,Region> new_region = std::make_pair(id, region_obj);
    Regions_Map.insert(new_region);
    return true;
}

/**
 * @brief all_regions helps in sorting the region alphabetically
 * @param a region 1
 * @param b region 2
 * @return the smaller region alphabetically
 */
bool compareFunction (RegionID a, RegionID b) {return a<b;}

/**
 * @brief all_regions Returns all the regions in any (arbitrary) order
 * @return a vector of Region ID
 */
std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> vect_of_RegionID;
    for (auto& x: Regions_Map)
    {
        vect_of_RegionID.push_back(x.first);
    }
    std::sort(vect_of_RegionID.begin(),vect_of_RegionID.end(),compareFunction);

    return vect_of_RegionID;
}


/**
 * @brief get_region_name Returns the name of the region with the given ID, or NO_NAME if such region doesn't exist.
 * @param id name of the region to be found
 * @return Name of the region
 */
Name Datastructures::get_region_name(RegionID id)
{
    std::unordered_map<RegionID,Region>::const_iterator map_iter = Regions_Map.find(id);
    if ( map_iter == Regions_Map.end() )
      return NO_NAME;
    else
      return map_iter->second.region_name;
}

/**
 * @brief get_region_coords Returns the coordinate vector of the region with the given ID, or a vector with single item NO_COORD, if such region doesn't exist.
 * @param id name of the region to be found
 * @return Vector of region coordinates
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    std::unordered_map<RegionID,Region>::const_iterator map_iter = Regions_Map.find(id);
    std::vector<Coord> output_coords;
    if ( map_iter == Regions_Map.end() )
    {
        output_coords.push_back(NO_COORD);
        return output_coords;
    }
    else
      return map_iter->second.region_coordinates;
}

/**
 * @brief find_parent_region_and_add is a recursive function - if parent region also has parent region - then add all subregions from parent regions to its parent
 * @param id region to be found
 * @return boolean values
 */
bool Datastructures::find_parent_region_and_add(RegionID id){
    std::unordered_map<RegionID,Region>::iterator region_id_iter = Regions_Map.find(id);
    if (region_id_iter->second.parentRegion!=0){
        //iterator to find parent of parentid region
        std::unordered_map<RegionID,Region>::iterator parent_regionid_iter = Regions_Map.find(region_id_iter->second.parentRegion);
        //add all the subregions from parent regions to parent of parent regions
        for(auto region_element : region_id_iter->second.vectorOfSubregions){
            parent_regionid_iter->second.vectorOfSubregions.push_back(region_element);
        }
        std::sort( parent_regionid_iter->second.vectorOfSubregions.begin(), parent_regionid_iter->second.vectorOfSubregions.end() );
        parent_regionid_iter->second.vectorOfSubregions.erase( std::unique( parent_regionid_iter->second.vectorOfSubregions.begin(), parent_regionid_iter->second.vectorOfSubregions.end() ), parent_regionid_iter->second.vectorOfSubregions.end() );
        find_parent_region_and_add(parent_regionid_iter->first);
        return true;
    }
    else{
        return false;
    }
}


/**
 * @brief add_subregion_to_region Adds the first given region as a subregion to the second
region. If no regions exist with the given IDs, or if the
first region is already a subregion of some region,
nothing is done and false is returned, otherwise
true is returned. - Also Handling stations in subregions also belonging to parent region
 * @param id subregion id to be added
 * @param parent id parent region id
 * @return boolean values
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    std::unordered_map<RegionID,Region>::iterator parentid_iter = Regions_Map.find(parentid);
    std::unordered_map<RegionID,Region>::iterator sub_region_id = Regions_Map.find(id);

    //settting subregion attributes
    if (sub_region_id != Regions_Map.end())
    {
        sub_region_id->second.isSubRegion = true;
        sub_region_id->second.parentRegion = parentid;
    }
    else {
        return false;
    }

    if (parentid_iter != Regions_Map.end())
    {
        //first insert the subregion id to the parent region id
        parentid_iter->second.vectorOfSubregions.push_back(id);

        //if sub region also has a vector of subregions, also add that to the parent region vector of subregion
        if(sub_region_id->second.vectorOfSubregions.size()>0){
            for(auto subregion_of_subregion : sub_region_id->second.vectorOfSubregions){
                parentid_iter->second.vectorOfSubregions.push_back(subregion_of_subregion);
            }
        }

        //set subregion parent region = functopm parent id
        sub_region_id->second.parentRegion = parentid;

        //see if parentid region also a subregion of another region then add it
        find_parent_region_and_add(parentid_iter->first);


        //Handling stations in subregions also belonging to parent region
        if(sub_region_id->second.vectorOfStations.size()>0){
            for(auto station_in_subregion_iter : sub_region_id->second.vectorOfStations){
                parentid_iter->second.vectorOfStations.insert(parentid_iter->second.vectorOfStations.end(),station_in_subregion_iter);
            }
        }

    }
    else{
        return false;
    }

    return true;
}

/**
 * @brief add_station_to_region Adds the first given region as a subregion to the second
region. If no regions exist with the given IDs, or if the
first region is already a subregion of some region,
nothing is done and false is returned, otherwise
true is returned.
 * @param id of the station to be added
 * @param parentid id of the region the station belongs to
 * @return true or false
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    std::unordered_map<RegionID,Region>::iterator region_iter = Regions_Map.find(parentid);
    std::unordered_map<StationID, Station>::iterator station_iter = Stations_Map.find(id);

    //check if that station already been added
    if(region_iter != Regions_Map.end()){
        for(auto element : region_iter->second.vectorOfStations){
            if(element == id){
                return false;
            }
        }
    }


    if (region_iter != Regions_Map.end() && station_iter!=Stations_Map.end())
    {
        region_iter->second.vectorOfStations.push_back(id);

        //Update the value in Station Region Maps
        auto station_region_map_it = Station_Region_Maps.find(id);
        if(station_region_map_it != Station_Region_Maps.end())
            station_region_map_it->second.push_back(parentid);

        //if parentid-region also has parents - recursive called function
        auto parent_of_region_iter = region_iter->second.parentRegion;
        if(parent_of_region_iter != 0){
            std::unordered_map<RegionID,Region>::iterator also_add_to_parent_region_iter = Regions_Map.find(region_iter->second.parentRegion);
            station_region_map_it->second.push_back(also_add_to_parent_region_iter->first);
            add_station_to_region(id, also_add_to_parent_region_iter->first);
        }

        std::string debug = "debugging";
        return true;
    }
    else{
        return false;
    }


}
/**
 * @brief station_in_regions Adds the given station to the given region. If no station
or region exist with the given IDs, or if the station
already belongs to some region, nothing is done and
false is returned, otherwise true is returned.
 * @param id of the station
 * @return vector of region ids  that station belong to, including sub and parent regions
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> output;

    std::vector<Region> swap_output;

    std::unordered_map<StationID, Station>::iterator station_iter = Stations_Map.find(id);
    if(station_iter == Stations_Map.end()){
        output.push_back(NO_REGION);
        return output;
    }

    for (auto element : Regions_Map){
        auto vector_of_station_ids = element.second.vectorOfStations;
        if (std::find(vector_of_station_ids.begin(), vector_of_station_ids.end(), id) != vector_of_station_ids.end()) {
            swap_output.push_back(element.second);
        }
    }


    for(int i=0; i < swap_output.size(); i++){
        for(int j = 1; j < swap_output.size();j++){
            if(swap_output[j].parentRegion == swap_output[i].regionID){
                       std::iter_swap(swap_output.begin() + i, swap_output.begin() + j);
            }
       }
    }

    for(auto k_element : swap_output){
        output.push_back(k_element.regionID);
    }

    if(output.size()>0){
        return output;
    }
    else{
//        output.push_back(NO_REGION);
        return output;
    }

}

/**
 * @brief all_subregions_of_region Returns a list of regions which belong either directly or
indirectly to the given region. The order of regions in
the returned vector can be arbitrary (the main program
sorts them in increasing ID order). If no region with
the given ID exists, a vector with a single element
NO_REGION is returned.
 * @param id of the region
 * @return a vector of RegionIDs
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    std::unordered_map<RegionID,Region>::iterator parentid_iter = Regions_Map.find(id);
    if (parentid_iter != Regions_Map.end())
    {
        return parentid_iter->second.vectorOfSubregions;
    }
    else{
        std::vector<RegionID> null_output;
        null_output.push_back(NO_REGION);
        return null_output;
    }

}

/**
 * @brief stations_closest_to Returns the three stations closest to the given
coordinate in order of increasing distance (based on the
ordering of coordinates described earlier). If there are
less than three stations in total, of course less stations
are returned. Implementing this command is not
compulsory (but is taken into account in the grading of
the assignment).
 * @param xy coordinate
 * @return a vector of station ids
 */

std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::vector<StationID> final_output;
    std::map<StationID, float> map;
    for (auto& it: Stations_Map) {
        float distance = sqrt(pow((it.second.station_coordinate.x - xy.x),2) +  pow((it.second.station_coordinate.y - xy.y),2));
        map.insert({it.first, distance});
    }

    std::vector<std::pair<StationID, float> > vector_pairs;
    for (auto& it : map) {
        vector_pairs.push_back(it);
    }

    sort(vector_pairs.begin(), vector_pairs.end(),compare);
    for (auto& iter : vector_pairs) {
        final_output.push_back(iter.first);
    }

    if(final_output.size()>3){
        final_output.resize(3);
        return final_output;
    }
    else{
        return final_output;
    }

}


/**
 * @brief remove_station Removes the station with the given id. If a station with
the given id does not exist, does nothing and returns
false, otherwise returns true. The performance of
this operation is not critical (it is not expected to be
called often), so it's not part of default performance
tests. Implementing this command is not compulsory
(but is taken into account in the grading of the
assignment).
 * @param id of station to be removed
 * @return true or false based on the success of operation
 */
bool Datastructures::remove_station(StationID id)
{
    std::unordered_map<StationID,Station>::iterator parentid_iter = Stations_Map.find(id);
    if(parentid_iter!=Stations_Map.end()){
        try {
            Stations_Map.erase(id);
            return true;
        }  catch (...) {
            return false;
        }
    } else {
        return false;
    }
}

/**
 * @brief find_biggest_parent recursive to find biggest common parent of a region id
 * @param id region id
 * @return parent region id
 */
RegionID Datastructures::find_biggest_parent(RegionID id){

    std::unordered_map<RegionID,Region>::iterator region_iter = Regions_Map.find(id);
    if(region_iter->second.parentRegion == 0){
        return region_iter->first;
    } else{
        find_biggest_parent(region_iter->second.parentRegion);
    }
}

/**
 * @brief common_parent_of_regions Returns the “nearest” region in the subregion
hierarchy, to which both given regions belong either
directly or indirectly. The returned region must be the
"nearest" in the following sense: it does not have a
subregion containing both given regions.
If either of the region ids do not correspond to any
region, or if no common region exists, returns
NO_REGION.
 * @param id of region 1
 * @param id of region 2
 * @return common parent of 2 regions
 */
RegionID Datastructures::common_parent_of_regions(RegionID id, RegionID id2)
{
    std::unordered_map<RegionID,Region>::iterator region1_iter = Regions_Map.find(id);
    std::unordered_map<RegionID,Region>::iterator region2_iter = Regions_Map.find(id2);

    if(region1_iter==Regions_Map.end() || region2_iter == Regions_Map.end()){
        return NO_REGION;
    }

    RegionID biggest_parent_of_region_1 = find_biggest_parent(id);
    RegionID biggest_parent_of_region_2 = find_biggest_parent(id2);

    if(biggest_parent_of_region_1 == biggest_parent_of_region_2){
        return biggest_parent_of_region_1;
    } else {
        return NO_REGION;
    }
}

/**
 * @brief add_train Adds a train to the data structure. If there
already is a train with the given id or some station id is
not found, nothing is done and false is returned,
otherwise true is returned.
 * @param trainid of the train to be added
 * @param stationtimes a vector of pairs station id and times (the route the train will travel through and time arrive at stations)
 * @return boolean values
 */
bool Datastructures::add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes)
{
    //If there already is a train with given id
    std::unordered_map<TrainID, Train>::iterator train_iter = Trains_Map.find(trainid);
    if(train_iter != Trains_Map.end())
    {
        return false;
    }

    //check if any of the station ids are not found
    for(auto element : stationtimes){
        std::unordered_map<StationID,Station>::iterator station_map_iter = Stations_Map.find(element.first);
        if(station_map_iter==Stations_Map.end()){
            return false;
        }
    }

    //Adding train to map
    Train train_object = Train{trainid,stationtimes};
    std::pair<TrainID,Train> new_train = std::make_pair(trainid, train_object);
    Trains_Map.insert(new_train);
    return true;

}

/**
 * @brief Returns the stations that are immediate next stations on
trains running through the given station.
 * @param id of the station to be found
 * @return vector of Station IDs
 */
std::vector<StationID> Datastructures::next_stations_from(StationID id)
{
    std::vector<StationID> outupt_stations;
    //If no trains leave from the station, an empty vector is returned
    //If a station with given id doesn't exist, a vector with single element NO_STATION is returned
    std::unordered_map<StationID,Station>::iterator station_map_iter = Stations_Map.find(id);
    if(station_map_iter==Stations_Map.end()){
        outupt_stations.push_back(NO_STATION);
        return outupt_stations;
    }

    //finding station immediately after for any train
    for(auto train_element : Trains_Map){
        auto vector_station_time_train_element = train_element.second.station_times;
        for(size_t i = 0; i < vector_station_time_train_element.size()-1; ++i){
            if(vector_station_time_train_element[i].first==id){
                outupt_stations.push_back(vector_station_time_train_element[i+1].first);
            }
        }
    }

    return outupt_stations;
}


/**
 * @brief train_stations_from Returns a list of stations, through which the given train
runs after leaving the given station
 * @param stationid to be found
 * @param trainid of the train route
 * @return vector of Station IDs
 */
std::vector<StationID> Datastructures::train_stations_from(StationID stationid, TrainID trainid)
{
    std::vector<StationID> output_stations;
    //if there is no station or train with the given ID
    std::unordered_map<TrainID, Train>::iterator train_iter = Trains_Map.find(trainid);
    std::unordered_map<StationID,Station>::iterator station_check = Stations_Map.find(stationid);
    if(train_iter == Trains_Map.end() || station_check == Stations_Map.end()){
        output_stations.push_back(NO_STATION);
        return output_stations;
    }

    bool found = false;
    if(train_iter != Trains_Map.end()){
        auto vector_station_times = train_iter->second.station_times;
        for(size_t i = 0; i < vector_station_times.size(); ++i){
            if(found==true){
                output_stations.push_back(vector_station_times[i].first);
            }
            if(vector_station_times[i].first == stationid){
                found = true;
            }
        }
    }

    //the train does not leave from the given station
    if(found == false){
        output_stations.push_back(NO_STATION);
        return output_stations;
    }

    return output_stations;
}

/**
 * @brief Clears out all trains
 */
void Datastructures::clear_trains()
{
    Trains_Map.clear();
}


void Datastructures::finding_routes(std::unordered_map<TrainID,Train> &trainmap, std::vector<StationID> &Station_Path, StationID fromid, StationID toid)
{
    StationID FromID = fromid;
    StationID ToID = toid;

    for(std::unordered_map<TrainID,Train>::iterator train_iter = trainmap.begin(); train_iter!=trainmap.end(); ++train_iter){

        auto vector_station_time_train_element = train_iter->second.station_times;

        for(size_t i = 1; i < vector_station_time_train_element.size(); ++i){

            if(vector_station_time_train_element[i].first==ToID){

                Station_Path.push_back(vector_station_time_train_element[i].first);

                Station_Path.push_back(vector_station_time_train_element[i-1].first);

                Station_Path.erase( unique( Station_Path.begin(), Station_Path.end() ), Station_Path.end() );

//                if(!Station_Path.empty() && Station_Path.back() != FromID){
//                      trainmap.erase(train_iter->first);
//                      ToID = vector_station_time_train_element[i-1].first;
//                      finding_routes(trainmap, Station_Path, FromID, ToID);
//                }

                if(!Station_Path.empty() && Station_Path.back() != FromID){
                      ToID = vector_station_time_train_element[i-1].first;
                      finding_routes(trainmap, Station_Path, FromID, ToID);
                } else {
                    break;
                }
            }
        }
    }
}

void Datastructures::finding_routes2(std::vector<StationID> &Station_Path, StationID fromid, StationID toid)
{
    StationID FromID = fromid;
    StationID ToID = toid;

    //looping through train map
    for(std::unordered_map<TrainID,Train>::iterator train_iter = Trains_Map.begin(); train_iter!=Trains_Map.end(); ++train_iter){
        //accessing the station times vector of pairs
        auto vector_station_time_train_element = train_iter->second.station_times;

        //checking that train route
        for(size_t i = 1; i < vector_station_time_train_element.size(); ++i){
            //if that station was found on that route
            if(vector_station_time_train_element[i].first==ToID){
                Station_Path.push_back(vector_station_time_train_element[i].first);
                Station_Path.push_back(vector_station_time_train_element[i-1].first);
                ToID = Station_Path.back();
                Station_Path.erase( unique( Station_Path.begin(), Station_Path.end() ), Station_Path.end() );
            }
        }
    }

    if(!Station_Path.empty() && Station_Path.back() != FromID){
          finding_routes2(Station_Path, FromID, ToID);
    }
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID fromid, StationID toid)
{

  //Calculating the StationIDs that would be on the path
  auto trainmap = Trains_Map;
  std::vector<StationID> Station_Path;

//  finding_routes(trainmap, Station_Path, fromid, toid);
  finding_routes2(Station_Path, fromid, toid);

  //if either of the station ids is not found, {NO_STATION, NO_DISTANCE} is returned
  std::unordered_map<StationID,Station>::iterator station_from_iter = Stations_Map.find(fromid);
  std::unordered_map<StationID,Station>::iterator station_to_iter = Stations_Map.find(toid);
  if(station_from_iter==Stations_Map.end()||station_to_iter==Stations_Map.end()){
      std::vector<std::pair<StationID, Distance>> nulloutput {{NO_STATION, NO_DISTANCE}};
      return nulloutput;
  }


  //If no route can be found between the stations, an empty vector is returned
  if(Station_Path.empty()){
      std::vector<std::pair<StationID, Distance>> empty_vector;
      return empty_vector;
  }

  reverse(Station_Path.begin(), Station_Path.end());

  //Calculating the Distance associate with the Stations IDs that appear on the path

  std::vector<std::pair<StationID, int> > vector_pairs;
  std::vector<std::unordered_map<StationID,Station>::iterator> vector_of_station_iters;
  std::vector<Distance> dist_vect;

  for(auto station_path_id:Station_Path){
      std::unordered_map<StationID,Station>::iterator station_map_iter = Stations_Map.find(station_path_id);
      vector_of_station_iters.push_back(station_map_iter);
  }

  for(size_t i = 0; i < vector_of_station_iters.size(); ++i){
        if(i == 0){
            float distance = sqrt(pow((vector_of_station_iters[i]->second.station_coordinate.x - vector_of_station_iters[i]->second.station_coordinate.x),2) +  pow((vector_of_station_iters[i]->second.station_coordinate.y - vector_of_station_iters[i]->second.station_coordinate.y),2));
            dist_vect.push_back((int)distance);
        } else{
            float sum_so_far = 0.0;
            float next_distance = sqrt(pow((vector_of_station_iters[i]->second.station_coordinate.x - vector_of_station_iters[i-1]->second.station_coordinate.x),2) +  pow((vector_of_station_iters[i]->second.station_coordinate.y - vector_of_station_iters[i-1]->second.station_coordinate.y),2));
            for(std::vector<int>::iterator it = dist_vect.begin(); it != dist_vect.end(); ++it)
                sum_so_far += *it;
            float distance = next_distance + sum_so_far;
            dist_vect.push_back((int)distance);
        }
  }

  for(size_t i = 0; i < vector_of_station_iters.size(); ++i){
      vector_pairs.push_back(std::make_pair(Station_Path[i], dist_vect[i]));
  }

  //Clear all
  vector_of_station_iters.clear();
  Station_Path.clear();
  dist_vect.clear();
  trainmap.clear();

  //Return Final output of vector pairs
  return vector_pairs;

}

std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_least_stations()");
}

std::vector<StationID> Datastructures::route_with_cycle(StationID /*fromid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_with_cycle()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_shortest_distance()");
}

std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival(StationID /*fromid*/, StationID /*toid*/, Time /*starttime*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_earliest_arrival()");
}
