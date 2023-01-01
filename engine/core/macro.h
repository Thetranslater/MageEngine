#pragma once
#include<iostream>
#include<stdexcept>
#include<memory>

#define MAGE_TRUE  1
#define MAGE_FALSE 0

#define MAGE_THROW(message) throw std::runtime_error(#message);

#define ASSERT_RESULT(func) \
	if(VK_SUCCESS != func){\
		MAGE_THROW(failed to call func)\
	}\

#define MAGE_TRY try{ 
#define MAGE_CATCH }catch(std::exception& excep){ \
	std::cout<<excep.what()<<std::endl; \
}
