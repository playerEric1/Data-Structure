#include "ds_hashset.h"

int main()
{
    std::string filename, token;
    int table_size = 100;
    std::list<MovieData> movielist;
    std::map<std::string, std::string> actorlist;
    ds_hashset htable(table_size);

    std::set<std::string> title_all, year_all, runtime_all;
    std::set<std::list<std::string> > Genre_all, Actor_all, Role_all;


    std::ifstream command("top250_example_input.txt"); // Temporary

    while (command >> token)
    {
        if (token == "movies")
        {
            command >> filename;
            std::ifstream infile(filename);
            if (!infile) {
                std::cerr << "cannot open file";
            }
            while (!infile.eof()) {
                movielist.push_back(MovieData(infile, title_all, year_all, runtime_all,
                                              Genre_all, Actor_all, Role_all));
            }

            std::list<std::string> emptylist;
            Genre_all.insert(emptylist);
            Actor_all.insert(emptylist);
            Role_all.insert(emptylist);
            infile.close();
            year_all.insert("?");
            runtime_all.insert("?");
            title_all.insert("?");
        }

        else if (token == "actors")
        {
            command >> filename;
            std::ifstream infile(filename);
            if (!infile) {
                std::cerr << "cannot open file";
            }
            std::string nconst, aname;
            while (!infile.eof()) {
                infile >> nconst >> aname;
                actorlist[nconst] = aname;
            }
            infile.close();

            for (std::set<std::string>::const_iterator it_title = title_all.cbegin();
                 it_title!=title_all.cend();it_title++){
                for (std::set<std::string>::const_iterator it_year = year_all.cbegin();
                     it_year!=year_all.cend();it_year++){
                    for (std::set<std::string>::const_iterator it_runt = runtime_all.cbegin();
                         it_runt!=runtime_all.cend(); it_runt++){
                        for (std::set<std::list<std::string>>::const_iterator it_G = Genre_all.cbegin();
                             it_G!=Genre_all.cend(); it_G++){
                            for (std::set<std::list<std::string>>::const_iterator it_A = Actor_all.cbegin();
                                 it_A!=Actor_all.cend(); it_A++){
                                for (std::set<std::list<std::string>>::const_iterator it_R = Role_all.cbegin();
                                     it_R!=Role_all.cend(); it_R++){

                                    int debug=htable.getsize();
                                    assert(debug!=0);

                                    Query one=Query(*it_title,*it_year,
                                                    *it_runt,*it_G,*it_A, *it_R);

                                    std::pair<Query, std::list<MovieData*> > *one_value = new std::pair<Query, std::list<MovieData*> >(
                                            one.compare(movielist));

                                    if (one_value->second.size()!=0)
                                        htable.insert(one_value); // ignore query with no match
                                }
                            }
                        }
                    }
                }
            }
        }

        else if (token == "table_size")
        {
            command >> table_size;
            htable.change_size(table_size);
        }

        else if (token == "occupancy")
        {
            float f;
            command >> f;
            htable.change_occupancy_level(f);
        }

        else if (token == "query")
        {
            Query current_query(command);
            htable.print(current_query, actorlist);
        }

        else if (token == "quit")
        {
            break;
        }

        else{
            std::cerr << "Invalid input\n";
        }
    }
    return 0;
}