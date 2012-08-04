#ifndef __IPLAYERINPUT_H__
#define __IPLAYERINPUT_H__

#include <string>

#include "Common.h"

namespace Othello
{
    class IPlayerInput
    {
    public:
        virtual Position GetMove() = 0;
        virtual void Alert(std::string msg) = 0;
    };
}

#endif
