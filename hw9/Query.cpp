#include "Query.h"
Query::Query(std::istream& movie_file) {
    std::string str;
    int tmpint;
    movie_file>>title>>year>>runtime;
    std::cout<<title<<" ";

    movie_file>>tmpint;
    for (int i=0;i<tmpint;i++){
        movie_file>>str;
        GenreList.push_back(str);
    }
    movie_file>>tmpint;
    for (int i=0;i<tmpint;i++){
        movie_file>>str;
        ActorList.push_back(str);
    }
    movie_file>>tmpint;
    for (int i=0;i<tmpint;i++){
        movie_file>>str;
        RoleList.push_back(str);
    }
}

// find movies that satisfy Query and make them a value of the hashtable
std::pair<Query, std::list<MovieData> > Query::compare(std::list<MovieData>& movielist){
    std::list<MovieData> result;
    for (std::list<MovieData>::iterator itr=movielist.begin();itr!=movielist.end();itr++){
        if (title!=itr->gettitle()){
            continue;
        }
        result.push_back(*itr);
        return std::make_pair(*this,result);
    }
}

// generate a string for itself to make a hash
std::string Query::long_str(){
    std::string ret="";
    ret+=title+std::to_string(year)+std::to_string(runtime);
    for(std::list<std::string>::iterator iter = GenreList.begin();
        iter != GenreList.end(); iter++){
        ret+=*iter;
    }

    for(std::list<std::string>::iterator iter = ActorList.begin();
        iter != ActorList.end(); iter++){
        ret+=*iter;
    }
}

/*
The recursive function will generate every combination of 0/1 for 6 positions
in a vector. Whenever pos == 6, the vector is complete.
Hint: There are 6 different "fields" in a query, this may be useful in constructing
your hash table.
*/
// Wrapping a class around a function turns a function into a functor
// (We'll talk about this more in Lecture 22.  You can just ignore
// this wrapper part for now.)
void permute_filters(int pos, std::vector<int> filter){
    if(pos == 6){
        //Add other code here if you want to use the completed vectors
        return;
    }

    filter.push_back(0);
    std::vector<int> filter_new = filter;
    filter_new.back() = 1;
    permute_filters(pos+1, filter_new);
    permute_filters(pos+1, filter);
}

//Driver function, feel free to modify to pass more arguments
void permute_filters(){
    permute_filters(0, std::vector<int>());
}