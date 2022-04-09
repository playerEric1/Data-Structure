#include "ds_hashset.h"

int main()
{
    std::string filename, token;
    int table_size=100;
    float f=0.5;
    std::list<MovieData> movielist;
    std::map<std::string,std::string> actorlist;
    ds_hashset htable(table_size);
    std::ifstream command("small_example_input.txt");

    while (command >> token)
    {
        if (token == "movies")
        {
            command>>filename;
            std::ifstream infile(filename);
            if (!infile) {
                std::cerr << "cannot open file";
            }
            while (!infile.eof()){
                movielist.push_back(MovieData(infile));
            }
            movielist.pop_back(); //remove empty line at the end
            infile.close();
        }

        else if (token == "actors")
        {
            command>>filename;
            std::ifstream infile(filename);
            if (!infile) {
                std::cerr << "cannot open file";
            }
            std::string nconst, aname;
            while (!infile.eof()){
                infile>>nconst>>aname;
                actorlist[nconst]=aname;
            }
            infile.close();

        }

        else if (token == "table_size")
        {
            command>>table_size;
            htable.change_size(table_size);
        }

        else if (token == "occupancy")
        {
            command>>f;
        }

        else if (token == "query")
        {
            Query current_query(command);
            static std::pair<Query, std::list<MovieData> > one_value=current_query.compare(movielist);
            htable.insert(one_value);
            htable.print(current_query, actorlist);
        }

        else if (token == "quit")
        {
            break;
        }

        else{
            std::cerr<<"Invalid input";
        }
    }
    return 0;
}