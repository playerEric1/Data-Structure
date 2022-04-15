HOMEWORK 9: IMDB SEARCH



NAME:  Qicheng Cao


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

Completely by myself
GeekforGeeks
CPPreference
W3School
StackOverflow
Class slides
Google
ALAC tutors

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  13


HASH FUNCTION DESCRIPTION

It first make a string that consists of Query's title, year, runtime, genres and actor.

The hash funtion is the one we used in the lab
http://www.partow.net/programming/hashfunctions/

    unsigned int hash = 1315423911;
    for (unsigned int i = 0; i < key.length(); i++)
        hash ^= ((hash << 5) + key[i] + (hash >> 2));
    return hash % size;


MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


