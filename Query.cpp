#include "Query.h"

Query::Query(std::istream &movie_file) {
    std::string str;
    int tmpint;
    movie_file >> title >> year >> runtime >> tmpint;

    for (int i = 0; i < tmpint; i++) {
        movie_file >> str;
        if (str != "")
            GenreList.push_back(str);
    }
    movie_file >> tmpint;
    for (int i = 0; i < tmpint; i++) {
        movie_file >> str;
        if (str != "")
            ActorList.push_back(str);
    }
    movie_file >> tmpint;
    for (int i = 0; i < tmpint; i++) {
        movie_file >> str;
        if (str != "")
            RoleList.push_back(str);
    }
}

// find movies that satisfy Query and make them a value of the hashtable
std::pair<Query, std::list<MovieData *> > Query::compare(std::list<MovieData> &movielist) {

    std::list<MovieData *> result;

    for (std::list<MovieData>::iterator itr = movielist.begin(); itr != movielist.end(); itr++) {

        bool fill_requirement = true;
        if (title != "?" && title != itr->gettitle()) {
            fill_requirement = false;
        }

        if (year != "?" && year != itr->getyear()) {
            fill_requirement = false;
        }

        if (runtime != "?" && runtime != itr->getruntime()) {
            fill_requirement = false;
        }

        std::list<std::string>::const_iterator old_iter, iter;
        if (GenreList.size() != 0) {
            std::list<std::string> tmpgenre(itr->getgenre());
            old_iter = tmpgenre.cbegin();
            iter = GenreList.cbegin();
            while (iter != GenreList.cend() && old_iter != tmpgenre.cend()) {
                if (*iter != *old_iter) {
                    fill_requirement = false;
                }
                old_iter++;
                iter++;
            }
        }

        if (RoleList.size() != 0) {
            std::list<std::string> tmprole(itr->getrole());
            old_iter = tmprole.cbegin();
            iter = RoleList.cbegin();
            while (iter != RoleList.cend() && old_iter != tmprole.cend()) {
                if (*iter != *old_iter) {
                    fill_requirement = false;
                }
                old_iter++;
                iter++;
            }
        }

        if (ActorList.size() != 0) {
            std::list<std::string> tmpactor(itr->getactor());
            old_iter = tmpactor.cbegin();
            iter = ActorList.cbegin();
            while (old_iter != tmpactor.cend() && iter != ActorList.cend()) {

                if (*iter != *old_iter) {
                    fill_requirement = false;
                }
                old_iter++;
                iter++;
            }
        }

        if (fill_requirement) result.push_back(&*itr);
    }
    return std::make_pair(*this, result);
}

// generate a string for itself to make a hash
std::string Query::long_str() {
    std::string ret;
    ret += title;
    ret += year;
    ret += runtime;
    for (std::list<std::string>::iterator iter = GenreList.begin();
         iter != GenreList.end(); iter++) {
        ret += *iter;
    }

    for (std::list<std::string>::iterator iter = ActorList.begin();
         iter != ActorList.end(); iter++) {
        ret += *iter;
    }

    for (std::list<std::string>::iterator iter = RoleList.begin();
         iter != RoleList.end(); iter++) {
        ret += *iter;
    }


    return ret;
}