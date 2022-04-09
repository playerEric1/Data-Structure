#ifndef HW9_QUERY_H
#define HW9_QUERY_H
#include <string>
#include "MovieData.h"

class Query {
public:
    explicit Query(std::istream& stream);
    std::string gettitle(){return title;}
    std::pair<Query, std::list<MovieData> > compare(std::list<MovieData> &movielist);
    std::string long_str();
private:
    std::string title;
    int year, runtime;
    std::list<std::string> GenreList,ActorList,RoleList;
};



#endif //HW9_QUERY_H
