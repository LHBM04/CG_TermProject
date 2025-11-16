#pragma once

#include "Object.h"

class GameObject final
	: public Object
{
public:
    /**
     * @brief 소멸자.
     */
    virtual ~GameObject() noexcept override = default;

private:

};
