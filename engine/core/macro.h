#pragma once
#include<iostream>
#include<stdexcept>
#include<memory>

#define MAGE_THROW(message) throw std::runtime_error(#message);

#define MAGE_TRY try{ 
#define MAGE_CATCH }catch(std::exception& excep){ \
	std::cout<<excep.what()<<std::endl; \
}
