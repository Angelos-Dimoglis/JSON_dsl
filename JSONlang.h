/**
* @Author: Aggelos Titos Dimoglis csd5078
* @Author: Panagiwtis Antonakakis csd5137
*/

#pragma once

#include <map>
#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include "JSONobject.h"

#define PROGRAM_BEGIN int main() {
#define PROGRAM_END ;}

#define JSON(a) ;json_type a
#define STRING(a) json_type(std::string(a))
#define NUMBER(a) json_type((double)a)
#define TRUE json_type((bool)true)
#define FALSE json_type((bool)false)
#define NULL json_type(nullptr)

#define OBJECT json_type
#define KEY(val) keyValuePair(#val) = 0 ?

#define ARRAY json_type(json_type::dummy_type::some_val)

#define SET ;
#define ASSIGN =

#define APPEND >> append_mode_changer_class() >>
#define PRINT ;std::cout <<
#define ERASE ;-

#define SIZE_OF(a) a.size_of()
#define IS_EMPTY(a) a.is_empty()
#define HAS_KEY(a,key) a.has_key(key)
#define TYPE_OF(a) a.get_type()
