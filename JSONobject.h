#pragma once

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <cassert>
#include <map>

class keyValuePair;
class json_type;

/* class which has the purpose of being created every time we try to append
something on an array, and changing append_mode back to false for the
dummy_type after we successfully append our data into it. */

class append_mode_changer_class {
public:
    bool* obj;
    ~append_mode_changer_class();
};

class json_type {
public:

    json_type* parent = nullptr;
    int indexToThis = -1;
    std::string keyToThis;

    bool append_mode = false;

    std::string type = "Null";

    std::string get_type () const {
        std::string str = type;
        char first = str[0];

        str[0] = std::tolower(first);

        return str + "\n";
    }

    bool is_empty () const {
        
        if (this->type == "Array")
            return this->a.arr_var.empty();

        if (this->type == "Object")
            return this->a.obj_var.empty();

        return false;
    }

    int size_of () const {

        if (this->type == "Array")
            return this->a.arr_var.size();

        if (this->type == "Object")
            return this->a.obj_var.size();

        return 1;
    }

    bool has_key (std::string key) const {

        if (this->type == "Object")
            return this->a.obj_var.find(key) != this->a.obj_var.end();

        return false;
    }

    static std::vector<std::vector<json_type>> stack;

    union uni {
        std::string str_var;
        double double_var;
        bool bool_var;
        std::map<std::string, json_type> obj_var;
        std::vector<json_type> arr_var;

        uni() : str_var(""){}  // Default constructor does nothing
        ~uni() {}
    } a;

    enum dummy_type { some_val };

    // Default constructor (initializes as null)
    json_type() : type("Object") {
        new (&(this->a.obj_var)) std::map<std::string, json_type>();
    }

    json_type(dummy_type useless) : type("Array") {

        std::vector<json_type> stack_frame = std::vector<json_type>();
        stack.push_back(std::move(stack_frame));
        new (&a.arr_var) std::vector<json_type>();
    }

    json_type(void* null) : type("Null") {}

    // Constructor for string
    explicit json_type (const std::string& str) : type("String") {
        new (&a.str_var) std::string(str); // Construct string in the union
    }

    // Constructor for double
    explicit json_type (const double num) : type("Double") {
        a.double_var = num; // Assign to double union member
    }

    // Constructor for bool
    explicit json_type (const bool b) : type("Bool") {
        a.bool_var = b; // Assign to bool union member
    }

    json_type(std::initializer_list<keyValuePair> init);

    ~json_type() {
        clear_union();
    }

    // Clear the union based on the current type
    void clear_union() {
        if (type == "String"){
            a.str_var.~basic_string();
        }
        type = "Null"; // Reset type to null
    }

    json_type (const json_type& other) : type(other.type) {
        this->json_type::operator= (other);
    }

    // Copy assignment operator
    json_type& operator= (const json_type& other) {
        if (this != &other) {
            clear_union(); // Clean up current value
            type = other.type;
            if (type == "String"){
                new (&a.str_var) std::string(other.a.str_var);
            }else if (type == "Double"){
                a.double_var = other.a.double_var;
            }else if (type == "Bool"){
                a.bool_var = other.a.bool_var;
            }else if (type == "Object"){
                new (&a.obj_var) std::map<std::string, json_type>(other.a.obj_var);
            }else if (type == "Array"){
                new (&a.arr_var) std::vector<json_type>(other.a.arr_var);
                append_mode = other.append_mode;
            }
        }
        return *this;
    }

    // indexing an object
    json_type& operator[] (const std::string& key){
        assert(type == "Object");

        if (a.obj_var.find(key) == a.obj_var.end())
            a.obj_var[key] = json_type(nullptr);

        a.obj_var[key].keyToThis = key;
        a.obj_var[key].parent = this;

        return a.obj_var[key];
    }

    // indexing an array
    json_type& operator[] (int i){
        assert(type == "Array" && i < a.arr_var.size());
        auto it = a.arr_var.begin() + i;
        it->parent = this;
        it->indexToThis = i;
        return *it;
    }

    // add an element to the array
    json_type operator[](json_type obj) {

        assert(type == "Array");
        int index = stack.size() - 1;
        assert(index >= 0);

        stack[index].push_back(std::move(obj));
        new (&a.arr_var) std::vector<json_type>(stack[index]);
        stack.pop_back();

        return *this;
    }

    // takes lvalue
    json_type& operator,(json_type& element) {

        if (append_mode) {
            assert(type == "Array");
            a.arr_var.push_back(std::move(json_type(element)));
            return *this;
        }

        assert(stack.size() >= 1);
        stack[stack.size() - 1].push_back(std::move(json_type(*this)));
        return element;
    }

    // takes rvalue
    json_type& operator,(json_type&& element) {

        if (append_mode) {
            assert(type == "Array");
            a.arr_var.push_back(std::move(element));
            return *this;
        }

        assert(stack.size() >= 1);
        stack[stack.size() - 1].push_back(std::move(*this));
        return element;
    }

    friend std::ostream& operator<< (std::ostream& os, json_type obj) {
        os << obj.toString();
        return os;
    }

    // default of print
    std::string toString() {
        return toString(1);
    }

    std::string toString (int indentation_level) {
        std::string indentation;

        for (int i = 0; i < indentation_level; i++)
            indentation += "    ";

        if (type == "String")
            return a.str_var + "\n";

        if (type == "Double")
            return std::to_string(a.double_var) + "\n";

        if (type == "Bool")
            return a.bool_var ? std::string ("true\n") : std::string("false\n");

        if (type == "Object") {
            std::string result = type + "{\n";
            for (auto& it : a.obj_var){
                result += indentation + "KEY(" + it.first + ") : " + it.second.toString(indentation_level + 1);
            }
            return result + indentation.substr(0, (indentation_level-1) * 4) + "}\n";
        }

        if (type == "Array") {
            std::string result = type + "[\n";
            for (auto it : a.arr_var){
                result += indentation + it.toString(indentation_level + 1);
            }
            return result +  indentation.substr(0, (indentation_level-1) * 4) + "]\n";
        }

        if (type == "Null")
            return type + "\n";

        throw std::runtime_error("invalid type");
    }

    // append first element to array
    json_type& operator>> (const json_type& obj) {
        if (type == "Array"){
            append_mode = true;
            a.arr_var.push_back(obj);
        }

        return *this;
    }

    json_type& operator>> (append_mode_changer_class&& temp) {
        temp.obj = &append_mode;
        return *this;
    }

    json_type operator+(json_type obj) {
        if (type != obj.type)
            assert(false);

        if (type == "Array") {
            obj.a.arr_var.insert(obj.a.arr_var.begin(), a.arr_var.begin(), a.arr_var.end());
        } else if (type == "Double") {
            obj.a.double_var += a.double_var;
        } else if (type == "String") {
            obj.a.str_var = a.str_var + obj.a.str_var;
        } else if (type == "Object") {
            obj.a.obj_var.insert(a.obj_var.begin(), a.obj_var.end());
        } else {
            assert(false);
        }

        return obj;
    }

    json_type operator-(json_type obj) const {
        if (type == "Double" && obj.type == "Double"){
            obj.a.double_var = a.double_var - obj.a.double_var;
            return obj;
        }
        assert(false);
    }

    json_type operator*(json_type obj) const {
        if (type == "Double" && obj.type == "Double"){
            obj.a.double_var *= a.double_var;
            return obj;
        }
        assert(false);
    }

    json_type operator/(json_type obj) const {
        if (type == "Double" && obj.type == "Double"){
            obj.a.double_var = a.double_var / obj.a.double_var;
            return obj;
        }
        assert(false);
    }

    json_type operator%(json_type obj) const {
        if (type == "Double" && obj.type == "Double"){
            obj.a.double_var = static_cast<int>(a.double_var) % static_cast<int>(obj.a.double_var);
            return obj;
        }
        assert(false);
    }

    json_type operator>(json_type obj) {
        if (type == "Double" && obj.type == "Double")
            return json_type(static_cast<bool>(a.double_var > obj.a.double_var));

        assert(false);
    }

    json_type operator<(const json_type obj) const {
        if (type == "Double" && obj.type == "Double")
            return json_type(static_cast<bool>(a.double_var < obj.a.double_var));

        assert(false);
    }

    json_type operator<=(const json_type obj) const {
        if (type == "Double" && obj.type == "Double")
            return json_type(static_cast<bool>(a.double_var <= obj.a.double_var));

        assert(false);
    }

    json_type operator>=(const json_type obj) const {
        if (type == "Double" && obj.type == "Double")
            return json_type(static_cast<bool>(a.double_var >= obj.a.double_var));

        assert(false);
    }

    json_type operator&&(const json_type obj) const {
        if (type == "Bool" && obj.type == "Bool")
            return json_type(a.bool_var && obj.a.bool_var);

        assert(false);
    }

    json_type operator||(const json_type obj) const {
        if (type == "Bool" && obj.type == "Bool")
            return json_type(a.bool_var || obj.a.bool_var);

        assert(false);
    }

    json_type operator!() const {
        if (type == "Bool")
            return json_type(!a.bool_var);

        assert(false);
    }

    json_type operator==(json_type obj) {
        if (type != obj.type)
            return json_type(false);

        if (type == "String")
            return json_type(obj.a.str_var == a.str_var);

        if (type == "Double")
            return json_type(obj.a.double_var == a.double_var);

        if (type == "Bool")
            return json_type(obj.a.bool_var == a.bool_var);

        if (type == "Array") {
            if (a.arr_var.size() != obj.a.arr_var.size())
                return json_type(false);

            for (int i = 0; i < a.arr_var.size(); i++){
                if (((*this)[i] != obj[i]).a.bool_var)
                    return json_type(false);
            }

            return json_type(true);
        }

        if (type == "Object") {
            if (a.obj_var.size() != obj.a.obj_var.size())
                return json_type(false);

            auto it1 = a.obj_var.begin();
            auto it2 = obj.a.obj_var.begin();

            while (it1 != a.obj_var.end() && it2 != obj.a.obj_var.end()) {
                if (
                    it1->first != it2->first || 
                    (it1->second != it2->second).a.bool_var
                ) return json_type(false);

                ++it1;
                ++it2;
            }

            return json_type(true);
        }

        return json_type(false);
    }

    json_type operator!=(json_type obj) {
        return !((*this) == obj);
    }

    void operator-(){
        if (parent == nullptr){

            if (type == "Array")
                *this = json_type(some_val);

            else if (type == "Object")
                *this = json_type{};

            else
                assert(false);

            return;

        }else if (parent->type == "Array"){

                assert(indexToThis >= 0 && indexToThis < parent->a.arr_var.size());
                parent->a.arr_var.erase(parent->a.arr_var.begin() + indexToThis);
                return;

        }else if (parent->type == "Object"){

            assert(!keyToThis.empty() && (parent->a.obj_var.find(keyToThis) != parent->a.obj_var.end()));
            parent->a.obj_var.erase(keyToThis);
            return;

        }

        assert(false);
    }

};

std::vector<std::vector<json_type>> json_type::stack = std::vector<std::vector<json_type>>(0);

class keyValuePair {
public:
    std::string key;
    json_type value = nullptr;

    explicit keyValuePair (std::string key) {
        this->key = std::move(key);
    }

    keyValuePair& operator= (const json_type& new_value) {
        this->value = new_value;
        return *this;
    }

    keyValuePair& operator= (const long long null) {
        return *this;
    }
};

json_type::json_type(std::initializer_list<keyValuePair> init) : type("Object") {
    std::vector<std::pair<std::string, json_type>> vec;

    for (const auto &kv: init)
        vec.emplace_back(kv.key, kv.value);

    new (&(this->a.obj_var)) std::map<std::string, json_type>(vec.begin(), vec.end());
}

append_mode_changer_class::~append_mode_changer_class(){
    *obj = false;
}
