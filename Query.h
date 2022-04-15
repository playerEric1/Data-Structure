#ifndef HW9_QUERY_H
#define HW9_QUERY_H

#include <string>
#include "MovieData.h"

class Query {
public:

    Query(const std::string &atitle, const std::string &ayear, const std::string &aruntime, const std::list<std::string> &aGenre,
          const std::list<std::string> &aActor, const std::list<std::string> &aRole) :
            title(atitle), year(ayear), runtime(aruntime),
            GenreList(aGenre), ActorList(aActor), RoleList(aRole) {};

    explicit Query(std::istream &stream);

    std::pair<Query, std::list<MovieData *> > compare(std::list<MovieData> &movielist);

    std::string long_str();

private:
    //REPRESENTATION
    std::string title, year, runtime;
    std::list<std::string> GenreList, ActorList, RoleList;
};

#endif