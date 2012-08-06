#ifndef __IPLAYERINPUT_H__
#define __IPLAYERINPUT_H__

#include <string>

#include "common.h"

namespace othello
{
    class IPlayerInput
    {
    public:
        virtual Position get_move() = 0;
        virtual void alert(std::string msg) = 0;
    };
}

#endif
