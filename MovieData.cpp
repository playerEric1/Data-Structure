#include "MovieData.h"

MovieData::MovieData(std::ifstream &movie_file, std::set<std::string> &title_all,
                     std::set<std::string>&year_all, std::set<std::string>&runtime_all,
                     std::set<std::list<std::string> > &Genre_all, std::set<std::list<std::string> > &Actor_all,
                     std::set<std::list<std::string> > &Role_all) {
    std::string str;
    int tmpint;
    movie_file >> title >> year >> runtime;
    title_all.insert(title);
    year_all.insert(year);
    runtime_all.insert(runtime);

    movie_file >> tmpint;
    for (int i = 0; i < tmpint; i++) {
        movie_file >> str;
        GenreList.push_back(str);
    }
    movie_file >> tmpint;
    for (int i = 0; i < tmpint; i++) {
        movie_file >> str;
        ActorList.push_back(str);
    }
    movie_file >> tmpint;
    for (int i = 0; i < tmpint; i++) {
        movie_file >> str;
        RoleList.push_back(str);
    }

    Genre_all.insert(GenreList);
    Actor_all.insert(ActorList);
    Role_all.insert(RoleList);
}

// called by ds_hashset::print()
void MovieData::print(std::map<std::string, std::string> &actornamelist) {
    std::cout << title << std::endl << year << std::endl << runtime << std::endl
              << GenreList.size();
    for (std::list<std::string>::iterator iter = GenreList.begin();
         iter != GenreList.end(); iter++) {
        std::cout << ' ' << *iter;
    }
    std::cout << std::endl << ActorList.size();
    std::list<std::string>::iterator iter2 = RoleList.begin();
    std::list<std::string>::iterator iter = ActorList.begin();
    for (; iter != ActorList.end(); iter++, iter2++) {
        std::cout << ' ' << actornamelist[*iter] << " (" << *iter2 << ')';
    }
    std::cout << std::endl;
}