#include "MovieData.h"

MovieData::MovieData(std::ifstream& movie_file) {
    std::string str;
    int tmpint;
    movie_file>>title>>year>>runtime;

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

// called by ds_hashset::print()
void MovieData::print(std::map<std::string,std::string>& actornamelist){
    std::cout<<title<<std::endl<<year<<std::endl<<runtime<<std::endl
    <<GenreList.size();
//    for(std::list<std::string>::iterator iter = GenreList.begin();
//    iter != GenreList.end(); iter++){
//        std::cout<<' '<<*iter<<std::endl;
//    }
//
//    std::cout<<ActorList.size();
//    for(std::list<std::string>::iterator iter = ActorList.begin();
//        iter != ActorList.end(); iter++){
//        std::cout<<' '<<actornamelist[*iter]<<std::endl;
//    }
}