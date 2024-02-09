//
// Created by gonzalezext on 09.02.24.
//

#ifndef STREAM_EXAMPLE_SERVER_H
#define STREAM_EXAMPLE_SERVER_H

#include <iostream>

void runServer(std::istream &in, std::ostream &out){
    while(!in.eof()) {
        char a;
        in.get(a);
        if (a == 'a') {
            out << "a letter" << std::endl;
        }
    }
}

#endif //STREAM_EXAMPLE_SERVER_H
