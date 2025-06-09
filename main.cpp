#include "JSONlang.h"

PROGRAM_BEGIN

    // normal variables
    JSON(name) = STRING("aggelos")
    JSON(number) = NUMBER(3.14)
    JSON(boolean_1) = TRUE
    JSON(boolean_2) = FALSE

    // empty object
    JSON(empty_obj) = {}

    // object
    JSON(student) = {
        KEY(name) : STRING("bobo"),
        KEY(am) : NUMBER(1234),
    }

    // empty array
    JSON(test_array) = ARRAY

    // array
    JSON(week_temperatures) = ARRAY [
        NUMBER(20), NUMBER(19.5), NUMBER(19), NUMBER(20),
        NUMBER(19), NUMBER(18.5), NUMBER(19)
    ]

    JSON(students) = ARRAY [
        OBJECT {
            KEY(name) : STRING("Nikos Nikolaou"),
            KEY(id) : NUMBER(4444),
            KEY(grades) : ARRAY[
                OBJECT { KEY(hy100) : NUMBER(7.5) },
                OBJECT { KEY(hy150) : NUMBER(8) }
            ]
        },
        OBJECT {
            KEY(name) : STRING("Nikos Nikolaou"),
            KEY(id) : NUMBER(4444),
            KEY(grades) : ARRAY[
                OBJECT { KEY(hy100) : NUMBER(7.5) },
                OBJECT { KEY(hy150) : NUMBER(8) }
            ]
        }
    ]

    // assigning normal variables
    SET name ASSIGN STRING("some other name")
    SET number ASSIGN NUMBER(5.1)

    // assign an object
    SET empty_obj ASSIGN student

    // assing and index an array
    SET week_temperatures[2] ASSIGN NUMBER(22)

    // add new field in object
    SET students[0]["email"] ASSIGN STRING("csd444@csd.uoc.gr")

    // append
    SET week_temperatures APPEND NUMBER(1), NUMBER(2), NUMBER(3), NUMBER(4)
    
    // append new
    SET students[0]["grades"] APPEND OBJECT { 
        KEY(hy255) : NUMBER(9) 
    }

    JSON(book) = OBJECT {
        KEY(title) : STRING("Gone Girl"),
        KEY(published) : NUMBER(2012),
        KEY(type) : STRING("Thriller"),
        KEY(author) : OBJECT {
            KEY(firstname) : STRING("GILLIAN"),
            KEY(sirname) : STRING("FLYNN"),
            KEY(age) : NUMBER(45)
        }
    }

    // some prints
    PRINT book["title"]
    PRINT book["author"]
    PRINT book

    // operators
    JSON(num1) = NUMBER(1.2)
    JSON(num2) = NUMBER(3.4)

    JSON(res) = num1 + num2
    PRINT res

    SET res ASSIGN num1 - num2
    PRINT res

    SET res ASSIGN num1 * num2
    PRINT res

    SET res ASSIGN num2 / num1
    PRINT res

    SET res ASSIGN NUMBER(2) % NUMBER(4)
    PRINT res

    SET res ASSIGN num2 > num1
    PRINT res

    SET res ASSIGN num2 < num1
    PRINT res

    SET res ASSIGN num2 <= num2
    PRINT res

    SET res ASSIGN num2 >= num1
    PRINT res

    // string concatination
    JSON(str) = STRING("part1 ") + STRING("part2")
    PRINT str

    // join two arrays
    JSON(array1) = ARRAY [
        NUMBER(1),
        NUMBER(2)
    ]
    + 
    ARRAY [
        NUMBER(3),
        NUMBER(4)
    ]
    
    PRINT array1

    // add a new field in an object
    JSON(o1) = OBJECT {
        KEY(test1) : NUMBER(2.3)
    }
    +
    OBJECT {
        KEY(test2) : NUMBER(5.4)
    }

    PRINT o1

    // logical operators
    JSON(bobo) = !FALSE && (FALSE || FALSE || TRUE)
    PRINT bobo
    
    // equality and inequality

    // for arrays
    JSON(array2) = ARRAY [NUMBER(1), NUMBER(2), NUMBER(3), NUMBER(4)]
    PRINT array1
    PRINT array2
    JSON(eq) = array1 != array2
    PRINT eq

    // for objects
    JSON(o2) = OBJECT {
        KEY(test1) : NUMBER(1.3),
        KEY(test2) : NUMBER(5.4)
    }
    SET eq ASSIGN o1 != o2    
    PRINT eq

    // addition can be used during definition of objects
    JSON(just_another_obj) = OBJECT {
        KEY(f1) : NUMBER(2),
        KEY(f2) : NUMBER(3) + NUMBER(3.3)
    }

    PRINT just_another_obj

    // TYPE_OF
    PRINT TYPE_OF(name)
    PRINT TYPE_OF(NUMBER(2))
    PRINT TYPE_OF(bobo)
    PRINT TYPE_OF(array1)
    PRINT TYPE_OF(OBJECT {})
    JSON(bozo) = NULL
    PRINT TYPE_OF(NULL)
    PRINT TYPE_OF(bozo)

    // IS_EMPTY
    PRINT IS_EMPTY(array1)
    ;std::cout << std::endl;
    PRINT IS_EMPTY(book)

    ;std::cout << std::endl;

    // SIZE_OF
    PRINT SIZE_OF(number)
    ;std::cout << std::endl;
    PRINT SIZE_OF(array1)
    ;std::cout << std::endl;
    PRINT SIZE_OF(empty_obj)
    ;std::cout << std::endl;

    // HAS_KEY
    PRINT HAS_KEY(number, "some key")
    ;std::cout << std::endl;
    PRINT HAS_KEY(empty_obj, "some key")
    ;std::cout << std::endl;
    PRINT HAS_KEY(empty_obj, "name")
    ;std::cout << std::endl;

    // ERASE
    SET array1 APPEND ARRAY [ STRING("test") ]
    PRINT array1
    ERASE array1[4]
    PRINT array1

    JSON(otto) = OBJECT {
        KEY(another_field) : NUMBER(123122),
        KEY(my_field) : OBJECT {
            KEY(baba) : STRING("programming")
        }
    }

    PRINT otto
    ERASE otto["my_field"]
    PRINT otto

    JSON(hy352_nik) = OBJECT{
        KEY(exam):NUMBER(7),
        KEY(project):NUMBER(8)
    }

    JSON(students1) = ARRAY [
        OBJECT{
            KEY(name) : STRING("Nikos ") + STRING("Nikolaou"),
            KEY(id) : NUMBER(4444),
            KEY(grades) : ARRAY[
                OBJECT {
                    KEY(hy352) : hy352_nik["exam"] * NUMBER(0.75) + hy352_nik["project"] * NUMBER(0.25)
                }
            ]
        }
    ]

    PRINT students1
    
PROGRAM_END
