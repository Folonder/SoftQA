#include <iostream>
using namespace std;

class KinshipDegreeException : public std::exception {

private:
    string message;


public:

    KinshipDegreeException(string msg) {
        this->message = msg;
    }


    string what() {
        return message;
    }

};