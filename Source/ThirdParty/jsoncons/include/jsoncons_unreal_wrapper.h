#pragma once

#pragma warning(push)
	#pragma warning(disable:4582) //Constructor is not implicitly called
	#pragma warning(disable:4583) //Destructor is not implicitly called
	#pragma warning(disable:4668) //_GNUC_ is not defined as a preprocessor macro
	#include "jsoncons/json.hpp"
	#include "jsoncons_ext/bson/bson.hpp"
	#include "jsoncons_ext/jsonpointer/jsonpointer.hpp"
#pragma warning(pop)