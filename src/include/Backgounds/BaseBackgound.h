#pragma once

#include "include/Interfaces/IBackground.h"
#include "include/Types.h"

class BaseBackgound : public IBackground
{
public:
	BaseBackgound(WHandle handle);

protected:
	WHandle m_WHandle;
};
