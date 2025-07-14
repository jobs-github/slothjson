Design Approach
--

Keywords: `third-party json parser` & `base library` & `code generator`

![design](../../res/design.png)

### Principle ###
The design of `slothjson` is based on this principle: any type defined as a `C++` struct can have its object instances **recursively expanded into an "object tree"**.

For example, given a struct `root_t` defined as follows:

	struct base_t
	{
	    int32_t int32_val;
	    std::string str_val;
	}
	
	struct root_t
	{
	    int64_t int64_val;
	    bool bool_val;
	    base_t base_val;
	}

When instantiating a `root_t` object:

    root_t root_obj;

The expanded "object tree" looks like this:

`root_obj (root_t)`  
　　`int64_val (int64_t)`  
　　`bool_val (bool)`  
　　`base_val (base_t)`  
　　　　`int32_val (int32_t)`  
　　　　`str_val (std::string)`  

Here, `root_obj` and `base_val` are instances of custom types, acting as **branch nodes** in the "object tree"; while `int64_val`, `bool_val`, etc., are instances of `C++` native types, acting as **leaf nodes** in the "object tree".

### Deduction ###

Based on the above principle, solving the `C++` object to `json` serialization problem can be decomposed into the following two sub-problems:

1. Implement the `json` serialization interface for **leaf node objects**;
2. Based on 1, implement the `json` serialization interface for **branch node objects**.

The first sub-problem can be specified as implementing `json` encoding/decoding interfaces for `C++` native types, including:  

* `bool`
* `int8_t`  
* `uint8_t`  
* `int16_t`  
* `uint16_t`  
* `int32_t`  
* `uint32_t`  
* `int64_t`  
* `uint64_t`  
* `float`  
* `double`  
* `std::string`  

`std::string` strictly belongs to `STL` built-in types, but since it's so commonly used, we also classify it as a leaf node.

The second sub-problem can be specified as implementing `json` encoding/decoding interfaces for `C++` custom types. This can be further refined into:

* Implementing `json` encoding/decoding interfaces for branch node types like `root_t` and `base_t`
* Implementing `json` encoding/decoding interfaces for arrays (any type)
* Implementing `json` encoding/decoding interfaces for dictionaries (any type)

### Design Approach ###

The solution to the first sub-problem is simple, as `rapidjson` has already done most of the work for us ^_^  
For the second sub-problem, to implement `json` encoding/decoding interfaces for branch node types like `root_t`, the simplest approach is to encode their objects as `json` `object` types, using member variable names as `key` (names are unique) and values as `value`. The key to this approach is **traversing object members at runtime**. For such problems, there are three automated solutions:

1. Reflection
2. Macros
3. Code generator

Since `C++` doesn't natively support reflection, we basically rule out the first solution.  
PS: Even if `C++` supported reflection, the author would insist on not using this approach because **performance is too poor**.

Macros sound good, but they're notoriously difficult to debug, extend, and maintain. Moreover, building a large system full of macros would be a nightmare in terms of time overhead.

So, the solution is to use a code generator.

The `json` encoding/decoding interfaces for arrays and dictionaries can be solved using `STL` built-in types:

* `std::vector <T>`
* `std::map <std::string, T>`

In summary, `slothjson`'s overall design approach is:

* Use a third-party `json` parsing library (`rapidjson`) to solve `json` encoding/decoding for native `C++` types
* For native types not directly supported by the third-party `json` parsing library (e.g., `int8_t`), adapt them to supported types (type casting)
* Use a code generator to solve `json` encoding/decoding for custom types
* Use `STL` built-in types to solve `json` encoding/decoding for arrays and dictionaries

The following describes the complete design using interfaces defined in `slothjson.h`.

The `json` type in `rapidjson` is defined as:

    rapidjson::Value

Therefore, any `C++` type only needs to implement the encoding/decoding interface to and from `rapidjson::Value` to support `json` serialization.

    namespace slothjson
	{
        template<typename T>
	    bool encode(const T& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
        template<typename T>
	    bool decode(const rapidjson::Value& json_val, T& obj_val);
    }

Note: Instantiation of `rapidjson::Value` depends on the following object:

	namespace slothjson
	{
	    typedef rapidjson::Document::AllocatorType allocator_t;
	}
Therefore, the `encode` function depends on the `alloc` object.

First, implement types natively supported by `rapidjson`:

	namespace slothjson
	{
	    bool encode(const bool& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, bool& obj_val);
	
	    bool encode(const std::string& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, std::string& obj_val);
	
	    bool encode(const int32_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, int32_t& obj_val);
	
	    bool encode(const uint32_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, uint32_t& obj_val);
	
	    bool encode(const double& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, double & obj_val);
	
	    bool encode(const int64_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, int64_t& obj_val);
	
	    bool encode(const uint64_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, uint64_t& obj_val);
	}

Then, **adapt** native types not directly supported by `rapidjson` to supported types:

    namespace slothjson
	{
	    bool encode(const int8_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, int8_t& obj_val);
	
	    bool encode(const uint8_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, uint8_t& obj_val);
	
	    bool encode(const int16_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, int16_t& obj_val);
	
	    bool encode(const uint16_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, uint16_t& obj_val);
	
	    bool encode(const float& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    bool decode(const rapidjson::Value& json_val, float & obj_val);
	}

Thanks to `C++` templates, we can predefine interfaces for arrays and dictionaries:

	namespace slothjson
	{
	    template<typename T>
	    bool encode(const std::vector<T>& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    template<typename T>
	    bool decode(const rapidjson::Value& json_val, std::vector<T>& obj_val);
	
	    template<typename T>
	    bool encode(const std::map<std::string, T>& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    template<typename T>
	    bool decode(const rapidjson::Value& json_val, std::map<std::string, T>& obj_val);
	} 

Next, provide encoding/decoding interfaces for object member variables to facilitate code generator writing:

    namespace slothjson
	{
	    template<typename T>
	    bool encode_field(const T& field, const char * field_name, allocator_t& alloc, rapidjson::Value& json_val);
	    template<typename T>
	    bool decode_field(const rapidjson::Value& json_val, const char * field_name, T& field, bool& field_in_json);
	}

Finally, unify the user interface:

	namespace slothjson
	{
	    template<bool pretty, typename T>
	    bool encode(const T& obj_val, std::string& json_val);
	    template<typename T>
	    bool decode(const std::string& json_val, T& obj_val);
	}
	
	namespace slothjson
	{
	    template<bool pretty, typename T>
	    bool dump(const T& obj_val, const char * path);
	
	    template<typename T>
	    bool load(const char * path, T& obj_val);
	};

This completes the base library work.

Furthermore, for user-defined types, using the following struct as an example:

	struct perf_object_t
	{
	    bool bool_val;
	    int8_t int8_val;
	    uint8_t uint8_val;
	    int16_t int16_val;
	    uint16_t uint16_val;
	    int32_t int32_val;
	    uint32_t uint32_val;
	    int64_t int64_val;
	    uint64_t uint64_val;
	    float float_val;
	    double double_val;
	    std::string str_val;
	    std::vector<int32_t > vec_val;
	    std::map<std::string, std::string > dict_val;
	}

To implement its encoding/decoding, you only need to support the following interface:

    bool encode(const perf_object_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
    bool decode(const rapidjson::Value& json_val, perf_object_t& obj_val);

A typical implementation would be:

    bool perf_object_t::encode(allocator_t& alloc, rapidjson::Value& json_val) const
	{
	    do
	    {
	        json_val.SetObject();
	        if (!__skip_bool_val && !encode_field(bool_val, "bool_val", alloc, json_val)) break;
	        if (!__skip_int8_val && !encode_field(int8_val, "int8_val", alloc, json_val)) break;
	        if (!__skip_uint8_val && !encode_field(uint8_val, "uint8_val", alloc, json_val)) break;
	        if (!__skip_int16_val && !encode_field(int16_val, "int16_val", alloc, json_val)) break;
	        if (!__skip_uint16_val && !encode_field(uint16_val, "uint16_val", alloc, json_val)) break;
	        if (!__skip_int32_val && !encode_field(int32_val, "int32_val", alloc, json_val)) break;
	        if (!__skip_uint32_val && !encode_field(uint32_val, "uint32_val", alloc, json_val)) break;
	        if (!__skip_int64_val && !encode_field(int64_val, "int64_val", alloc, json_val)) break;
	        if (!__skip_uint64_val && !encode_field(uint64_val, "uint64_val", alloc, json_val)) break;
	        if (!__skip_float_val && !encode_field(float_val, "float_val", alloc, json_val)) break;
	        if (!__skip_double_val && !encode_field(double_val, "double_val", alloc, json_val)) break;
	        if (!__skip_str_val && !encode_field(str_val, "str_val", alloc, json_val)) break;
	        if (!__skip_vec_val && !encode_field(vec_val, "vec_val", alloc, json_val)) break;
	        if (!__skip_dict_val && !encode_field(dict_val, "dict_val", alloc, json_val)) break;
	
	        return true;
	    } while (0);
	
	    return false;
	}
	
	bool perf_object_t::decode(const rapidjson::Value& json_val)
	{
	    do
	    {
	        if (!decode_field(json_val, "bool_val", bool_val, __json_has_bool_val)) break;
	        if (!decode_field(json_val, "int8_val", int8_val, __json_has_int8_val)) break;
	        if (!decode_field(json_val, "uint8_val", uint8_val, __json_has_uint8_val)) break;
	        if (!decode_field(json_val, "int16_val", int16_val, __json_has_int16_val)) break;
	        if (!decode_field(json_val, "uint16_val", uint16_val, __json_has_uint16_val)) break;
	        if (!decode_field(json_val, "int32_val", int32_val, __json_has_int32_val)) break;
	        if (!decode_field(json_val, "uint32_val", uint32_val, __json_has_uint32_val)) break;
	        if (!decode_field(json_val, "int64_val", int64_val, __json_has_int64_val)) break;
	        if (!decode_field(json_val, "uint64_val", uint64_val, __json_has_uint64_val)) break;
	        if (!decode_field(json_val, "float_val", float_val, __json_has_float_val)) break;
	        if (!decode_field(json_val, "double_val", double_val, __json_has_double_val)) break;
	        if (!decode_field(json_val, "str_val", str_val, __json_has_str_val)) break;
	        if (!decode_field(json_val, "vec_val", vec_val, __json_has_vec_val)) break;
	        if (!decode_field(json_val, "dict_val", dict_val, __json_has_dict_val)) break;
	
	        return true;
	    } while (0);
	
	    return false;
	}
    
    bool encode(const perf_object_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
	{
	    return obj_val.encode(alloc, json_val);
	}
	
	bool decode(const rapidjson::Value& json_val, perf_object_t& obj_val)
	{
	    return obj_val.decode(json_val);
	}

Here, `__skip_xxx` is used to support optional field encoding, and `__json_has_xxx` is used to support optional field decoding.  
The above solution traverses all members of the object, encoding the object as a `json` `object` instance using names as `key` and values as `value`. This process can be fully automated using [schema](schema.md) and a code generator.

[Previous](../../README.md)

[Home](../../README.md)