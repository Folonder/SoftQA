#include <iostream>
using namespace std;

class FileNotFoundException : public std::exception {

private:
    string message;


public:

    FileNotFoundException(string msg){
        this->message = msg;
    }


    string what() {
        return "File not found: " + message;
    }

};