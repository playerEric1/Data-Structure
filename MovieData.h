#include <iostream>
#include <string>
#include <list>
#include <map>
#include <fstream>
#include <vector>
#include <cassert>
#include <set>

#ifndef MDATA_H
#define MDATA_H

//Title Year of Release Runtime in Minutes GenreCount GenreList ActorCount ActorList RoleCount RoleList
class MovieData {
public:
    //CONSTRUCTOR
    MovieData(std::ifstream &movie_file, std::set<std::string>& title_all,
              std::set<std::string>& year_all, std::set<std::string>& runtime_all,
    std::set<std::list<std::string> >& Genre_all, std::set<std::list<std::string> >& Actor_all,
              std::set<std::list<std::string> >& Role_all);

    //ACCESSORS
    std::string gettitle() { return title; }

    std::string getyear() { return year; }

    std::string getruntime() { return runtime; }

    std::list<std::string> getgenre() { return GenreList; }

    std::list<std::string> getactor() { return ActorList; }

    std::list<std::string> getrole() { return RoleList; }

    //PRINT HELPER
    void print(std::map<std::string, std::string> &actornamelist);

private:
    //REPRESENTATION
    std::string title, year, runtime;
    std::list<std::string> GenreList, ActorList, RoleList;
};

#endif
