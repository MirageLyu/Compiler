#include "reportError.h"

void reportSemanticError(int typeno, string msg, int lineno){
    cout<<"Error type "<<typeno<<" at line "<<lineno<<": "<<msg<<endl;
}