#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include "course.h"

int title_length(std::vector<Course> courses){
  int i;
  unsigned int max_length = 0;
  for (i = 0; i < (int)courses.size(); ++i)
  {
    unsigned int tmp_length = courses[i].gettitle().size();
    max_length = std::max(max_length, tmp_length);
  }
  return max_length+2;// account for the output padding with ", "
}

int main(int argc, char *argv[])
{
  if (argc != 4 && argc != 5)
  {
    std::cerr << "Wrong argument numbers\n";
    return 1;
  }
  std::string option = "";
  //add optional variable if there are four arguments
  if (argc == 5)
  {
    option = argv[4];
  }

  std::ifstream in_str(argv[1]);
  if (!in_str)
  {
    std::cerr << "Could not open " << argv[1] << " to read\n";
    return 1;
  }
  std::ofstream out_str(argv[2]);
  if (!out_str)
  {
    std::cerr << "Could not open " << argv[2] << " to write\n";
    return 1;
  }
  std::vector<Course> courses;
  Course one_course;
  while (one_course.read(in_str))
  {
    courses.push_back(one_course);
  }
  int max_length=title_length(courses);
  //room
  if (std::string(argv[3]) == "room")
  {
    //print a specific room
    if (argc == 5)
    {
      std::vector<Course> fullentries;
      int counter = 0;
      for (int i = 0; i < (int)courses.size(); ++i)
      {
        std::vector<Course> tmp = courses[i].expand();
        for (int j = 0; j < (int)tmp.size(); ++j)
        {

          if (tmp[j].getroom() == option)
          {
            fullentries.push_back(tmp[j]);
            counter += 1;
          }
        }
      }
      if (counter == 0)
      {
        out_str << "No data available.\n";
        out_str.close();
        return 0;
      }

      // Output a header...
      const std::string header = "Dept  Coursenum  Class Title" + std::string(max_length - 11, ' ') + "Day        Start Time  End Time";
      const std::string underline = "----  ---------  " + std::string(max_length - 2, '-') + "  ---------  ----------  --------";
      out_str << "Room " << option << '\n'
              << header << '\n'
              << underline << '\n';
      //nested loop to get full entry list
      sort(fullentries.begin(), fullentries.end(), EarlierThan_room);
      for (int i = 0; i < (int)fullentries.size(); ++i)
      {
        out_str << fullentries[i].print_room(max_length);
      }
      out_str << counter << " entries\n\n";
    }

    //print all room
    else if (argc == 4)
    {

      std::vector<Course> fullentries;
      std::vector<std::string> roomlist;
      int counter = 0;
      for (int i = 0; i < (int)courses.size(); ++i)
      {
        std::string roomname = courses[i].getroom();
        if (std::find(roomlist.begin(), roomlist.end(), roomname) == roomlist.end())
        {
          roomlist.push_back(roomname);
          counter += 1;
        }
        if (counter == 0)
        {
          out_str << "No data available.\n";
          out_str.close();
          return 0;
        }
      }
      sort(roomlist.begin(), roomlist.end());
      for (int k = 0; k < (int)roomlist.size(); ++k)
      {
        counter = 0;
        fullentries.clear();
        option = roomlist[k];
        for (int i = 0; i < (int)courses.size(); ++i)
        {
          std::vector<Course> tmp = courses[i].expand();
          for (int j = 0; j < (int)tmp.size(); ++j)
          {
            if (tmp[j].getroom() == option)
            {
              fullentries.push_back(tmp[j]);
              counter += 1;
            }
          }
        }

        //a loop to print each room

        // Output a header...
        //recalculate table length
        int max_length=title_length(fullentries);
        const std::string header = "Dept  Coursenum  Class Title" + std::string(max_length - 11, ' ') + "Day        Start Time  End Time";
        const std::string underline = "----  ---------  " + std::string(max_length - 2, '-') + "  ---------  ----------  --------";
        out_str << "Room " << option << '\n'
                << header << '\n'
                << underline << '\n';
        //nested loop to get full entry list
        sort(fullentries.begin(), fullentries.end(), EarlierThan_room);
        for (int i = 0; i < (int)fullentries.size(); ++i)
        {
          out_str << fullentries[i].print_room(max_length);
        }
        out_str << counter << " entries\n\n";
      }
    }
  }

  //dept
  else if (std::string(argv[3]) == "dept")
  {
    if (argc != 5)
    {
      std::cerr << "Wrong argument numbers\n";
      return 1;
    }
    std::vector<Course> fullentries;
    // Output a header 
    // This cannot be integrated into a function because the table format is different for dept and room
    const std::string header = "Coursenum  Class Title" + std::string(max_length - 11, ' ') + "Day        Start Time  End Time";
    const std::string underline = "---------  " + std::string(max_length - 2, '-') + "  ---------  ----------  --------";
    out_str << "Room " << option << '\n'
            << header << '\n'
            << underline << '\n';
    //nested loop to get full entry list
    int counter = 0;
    for (int i = 0; i < (int)courses.size(); ++i)
    {
      std::vector<Course> tmp = courses[i].expand();
      for (int j = 0; j < (int)tmp.size(); ++j)
      {
        if (tmp[j].getdept() == option)
        {
          fullentries.push_back(tmp[j]);
          counter += 1;
        }
      }
      sort(fullentries.begin(), fullentries.end(), EarlierThan_dept);
    }
    for (int i = 0; i < (int)fullentries.size(); ++i)
    {
      out_str << fullentries[i].print_dept(max_length);
    }
    out_str << counter << " entries\n\n";
  }

  //custom
  else if (std::string(argv[3]) == "custom")
  {
  }
  in_str.close();
  out_str.close();
  return 0;
}
