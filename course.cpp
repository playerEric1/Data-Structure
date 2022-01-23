#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "course.h"

Course::Course()
{
  title = "";
  CRN = "";
  dept = "";
  code = ""; //course code
  day = "";
  start = "";
  end = "";
  room = "";
}
Course::Course(std::string atitle, std::string aCRN, std::string adept, std::string acode, std::string aday, std::string astart,
               std::string aend, std::string aroom)
{
  title = atitle;
  CRN = aCRN;
  dept = adept;
  code = acode; //course code
  day = aday;
  start = astart;
  end = aend;
  room = aroom;
}
// Read information about a student, returning true if the information was read correctly.
bool Course::read(std::istream &in_str)
{

  // If we don't find an id, we've reached the end of the file & silently return false.
  if (!(in_str >> CRN >> dept >> code >> title >> day >> start >> end >> room))
    return false;
  // Once we have an id number, any other failure in reading is treated as an error.
  return true; // everything was fine
}

//ACCESSORS
std::string Course::gettitle() const
{
  return title;
}
std::string Course::getday() const
{
  return day;
}
std::string Course::getroom() const
{
  return room;
}
std::string Course::getcode() const
{
  return code;
}
std::string Course::getdept() const
{
  return dept;
}
std::string Course::gettime() const
{
  return start;
}
//return several entries that are different in time in a vector
std::string Course::print_room(int space) const
{
  char abbrev = day[0];
  std::string singleentry = dept + "  " + code + "    " + title + std::string(space - (int)title.size(), ' ');
  if (abbrev == 'M')
  {
    singleentry += "Monday     ";
  }
  else if (abbrev == 'T')
  {
    singleentry += "Tuesday    ";
  }
  else if (abbrev == 'W')
  {
    singleentry += "Wednesday  ";
  }
  else if (abbrev == 'R')
  {
    singleentry += "Thursday   ";
  }
  else if (abbrev == 'F')
  {
    singleentry += "Friday     ";
  }
  singleentry += start + "     " + end + "\n";
  return singleentry;
}
std::string Course::print_dept(int space) const
{
  char abbrev = day[0];
  std::string singleentry = code + "    " + title + std::string(space - (int)title.size(), ' ');
  if (abbrev == 'M')
  {
    singleentry += "Monday     ";
  }
  else if (abbrev == 'T')
  {
    singleentry += "Tuesday    ";
  }
  else if (abbrev == 'W')
  {
    singleentry += "Wednesday  ";
  }
  else if (abbrev == 'R')
  {
    singleentry += "Thursday   ";
  }
  else if (abbrev == 'F')
  {
    singleentry += "Friday     ";
  }
  singleentry += start + "     " + end + "\n";
  return singleentry;
}

std::vector<Course> Course::expand() const
{
  std::vector<Course> tmp;
  for (int i = 0; i < (int)day.size(); i++)
  {
    std::string abbrev = std::string(1, day[i]);
    Course singlecourse(title, CRN, dept, code, abbrev, start, end, room);
    tmp.push_back(singlecourse);
  }
  return tmp;
}

//sort based on room name, day, time, code and then dept name
bool EarlierThan_room(const Course &c1, const Course &c2)
{
  int d1 = 0, d2 = 0, t1 = 0, t2 = 0;
  if (c1.getday()[0] == 'M')
  {
    d1 = 1;
  }
  else if (c1.getday()[0] == 'T')
  {
    d1 = 2;
  }
  else if (c1.getday()[0] == 'W')
  {
    d1 = 3;
  }
  else if (c1.getday()[0] == 'R')
  {
    d1 = 4;
  }
  else if (c1.getday()[0] == 'F')
  {
    d1 = 5;
  }
  if (c2.getday()[0] == 'M')
  {
    d2 = 1;
  }
  else if (c2.getday()[0] == 'T')
  {
    d2 = 2;
  }
  else if (c2.getday()[0] == 'W')
  {
    d2 = 3;
  }
  else if (c2.getday()[0] == 'R')
  {
    d2 = 4;
  }
  else if (c2.getday()[0] == 'F')
  {
    d2 = 5;
  }
  if (d1 < d2)
  {
    return true;
  }
  //Courses on the same day should be sorted by what hour the course starts at, with
  //the earlier courses coming first.
  if (c1.gettime().substr(0, 2) == "12")
  {
    t1 = 12;
  }
  else if (c1.gettime()[5] == 'P')
  {
    t1 = std::stoi(c1.gettime().substr(0, 2)) + 12;
  }
  else
  {
    t1 = std::stoi(c1.gettime().substr(0, 2));
  }
  if (c2.gettime().substr(0, 2) == "12")
  {
    t2 = 12;
  }
  else if (c2.gettime()[5] == 'P')
  {
    t2 = std::stoi(c2.gettime().substr(0, 2)) + 12;
  }
  else
  {
    t2 = std::stoi(c2.gettime().substr(0, 2));
  }

  if (d1 == d2 && t1 < t2)
  {
    return true;
  }
  //If two or more courses happen in the same time and place, you should sort those courses by course code
  else if (d1 == d2 && t1 == t2 && c1.getcode() < c2.getcode())
  {
    return true;
  }
  //If the course numbers also match, you should sort by department name
  else if (d1 == d2 && t1 == t2 && c1.getcode() == c2.getcode() && c1.getdept() < c2.getdept())
  {
    return true;
  }
  return false;
}

//sort based on code, day, and then latest time
bool EarlierThan_dept(const Course &c1, const Course &c2)
{
  int d1 = 0, d2 = 0, t1 = 0, t2 = 0, code1 = 0, code2 = 0;
  code1 = std::stoi(c1.getcode().substr(0, 4)+c1.getcode().substr(5, 2));
  code2 = std::stoi(c2.getcode().substr(0, 4)+c2.getcode().substr(5, 2));
  if (code1 < code2)
  {
    return true;
  }
  if (c1.getday()[0] == 'M')
  {
    d1 = 1;
  }
  else if (c1.getday()[0] == 'T')
  {
    d1 = 2;
  }
  else if (c1.getday()[0] == 'W')
  {
    d1 = 3;
  }
  else if (c1.getday()[0] == 'R')
  {
    d1 = 4;
  }
  else if (c1.getday()[0] == 'F')
  {
    d1 = 5;
  }
  if (c2.getday()[0] == 'M')
  {
    d2 = 1;
  }
  else if (c2.getday()[0] == 'T')
  {
    d2 = 2;
  }
  else if (c2.getday()[0] == 'W')
  {
    d2 = 3;
  }
  else if (c2.getday()[0] == 'R')
  {
    d2 = 4;
  }
  else if (c2.getday()[0] == 'F')
  {
    d2 = 5;
  }
  if (d1 < d2 && code1 == code2)
  {
    return true;
  }
  if (c1.gettime().substr(0, 2) == "12")
  {
    t1 = 12;
  }
  else if (c1.gettime()[5] == 'P')
  {
    t1 = std::stoi(c1.gettime().substr(0, 2)) + 12;
  }
  else
  {
    t1 = std::stoi(c1.gettime().substr(0, 2));
  }
  if (c2.gettime().substr(0, 2) == "12")
  {
    t2 = 12;
  }
  else if (c2.gettime()[5] == 'P')
  {
    t2 = std::stoi(c2.gettime().substr(0, 2)) + 12;
  }
  else
  {
    t2 = std::stoi(c2.gettime().substr(0, 2));
  }

  if (d1 == d2 && code1 == code2 && t1 > t2)
  {
    return true;
  }
  return false;
}