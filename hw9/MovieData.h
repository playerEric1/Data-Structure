#include <iostream>
#include <string>
#include <list>
#include <map>
#include <fstream>
#include <vector>
#include <cassert>

#ifndef HW9_MOVIEDATA_H
#define HW9_MOVIEDATA_H

//Title Year of Release Runtime in Minutes GenreCount GenreList ActorCount ActorList RoleCount RoleList
class MovieData {
public:
    MovieData(std::ifstream& fname);
    std::string gettitle(){return title;}
    void print(std::map<std::string,std::string>& actornamelist);
private:
    std::string title;
    int year, runtime;
    std::list<std::string> GenreList,ActorList,RoleList;
};

#endif //HW9_MOVIEDATA_H
