#pragma once
#include<stdexcept>
#include<memory>

#define MAGE_THROW(message) throw std::runtime_error(#message);
