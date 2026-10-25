#include "float.h"

using namespace std;

// Check system
void check_system() {
    if(sizeof(_Float16) != 2) {
        cerr << "This system does not support _Float16. "
             << "Contact the TA for help." << endl;
        exit(1);
    }
}

