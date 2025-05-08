[简体中文](README_ZH.md)

# slothjson - object serialization tool for the lazy #
![slothjson logo](res/logo.png)

`slothjson` is a powerful toolkit for object serialization and deserialization with **full automation**, built on top of [rapidjson](https://github.com/miloyip/rapidjson). 

With this tool, you can write less code and say goodbye to overtime. ^_^

Here's the architecture of slothjson: 
![design](res/design.png)

## What is slothjson ? ##

Want to serialize any C++ object (literally **any type**) to JSON, or deserialize it from JSON, with **just a single line of code** ? Like this:  

    template <typename T>
    bool encode(const T& obj_val, std::string& json_val);
    
    template <typename T>
    bool decode(const std::string& json_val, T& obj_val);

If so, congratulations! Slothjson is designed just for you. Instead of repeatedly writing serialization/deserialization code for **every type**, you simply define a SIMPLE schema. Then, with a SIMPLE command-line call, the code is generated for you — **instantly**. Your hands (both of them!) will thank you. ^_^

So,
> 
`slothjson` is NOT another `rapidjson`. `rapidjson` already delivers great performance — there's no need to reinvent the wheel;  
What `slothjson` brings is automation, which rapidjson lacks;  
`rapidjson` solves problems **from 0 to 1**;  
`slothjson` solves problems **from 1 to 100**. In other words, **batch code generation**.


## Features ##
* Simple interface (everything can be done with just a single line of code)
* Powerful code generator with full automation (no need to write serialization code manually)
* Support optional fields (easily include or skip fields during serialization/deserialization)
* Flexible schema (supports arrays, dictionaries, nested objects, and **nested arrays/dictionaries**)
* Clean design (no complex C++ template tricks — easy to understand), reusable (can be extended to XML), extensible (easy to support new types)
* Cross-Platform (Windows & Linux & OS X)

## Usage ##

First, add the following dependencies to your project:

* `rapidjson`: refer to `include/rapidjson`, the fastest json parser in the world
* `slothjson`: refer to `include/slothjson.h` and `include/slothjson.cpp`, the slothjson library

That’s it! Easy, right? ^_^  

Next, take a few seconds to learn the most important slothjson interfaces:  

	namespace slothjson
	{
        // encode C++ object to json string
	    template<bool pretty, typename T>
	    bool encode(const T& obj_val, std::string& json_val);

        // decode json string to C++ object
	    template<typename T>
	    bool decode(const std::string& json_val, T& obj_val);
	}
	
	namespace slothjson
	{
        // encode C++ object to json string, then dump it to file
	    template<bool pretty, typename T>
	    bool dump(const T& obj_val, const char * path);

	    // load json string from file, then decode it to C++ object
	    template<typename T>
	    bool load(const char * path, T& obj_val);
	};

The `pretty` argument controls the formatting style:

* `true` - outputs human-readable JSON (with indentation and line breaks);  
* `false` - outputs compact JSON (no indentation or line breaks, everything on one line)

These interfaces should cover most of your needs.  

**Example**: Create a schema file named fxxx_gfw.json:  

	{
	    "structs": 
	    [
	        {
	            "type": "fxxx_gfw_t",
	            "members": 
	            [
                    ["bool", "bool_val", "true"],
                    ["int8_t", "int8_val", "'a'"],
	                ["int32_t", "int32_val", "-111111"],
	                ["uint64_t", "uint64_val", "0xffff"],
	                ["double", "double_val", "111111.111111"],
	                ["string", "str_val", "test"],
	                ["[int32_t]", "vec_val"],
	                ["{string}", "dict_val"]
	            ]
	        }
	    ]
	}

Then run the command:  

    python generator/slothjson.py -f src/fxxx_gfw.json

This generate `fxxx_gfw.h` and `fxxx_gfw.cpp`, which you can include in your project.  
Then write code like this:  

    slothjson::fxxx_gfw_t obj_val;
    // set the value of "obj_val"
    ......
    // output as string
	std::string json_val;
	bool rc = slothjson::encode <false> (obj_val, json_val);
    // output as file
    std::string path = "fxxx_gfw_t.json";
    bool rc = slothjson::dump <false> (obj_val, path);

Want to skip serializing certain fields? Just do:  

    obj_val.skip_dict_val(); // call "skip_xxx"
Deserialization works the same way:  

    // load from string
	std::string json_val;
    // set the value of "json_val"
    ......
    slothjson::fxxx_gfw_t obj_val;
	bool rc = slothjson::decode(json_val, obj_val);

    // load from file
	std::string path = "fxxx_gfw_t.json";
    slothjson::fxxx_gfw_t obj_val;
	bool rc = slothjson::load(path, obj_val);

After deserialization, you can check **whether a field exists in the JSON** like this:  

    if (obj_val.json_has_dict_val()) // call "json_has_xxx()"
    {
         ......
    }

That’s it — simple and straightforward, isn’t it?  

PS: If you're a Linux developer and still don’t understand Makefiles... I might judge you a little. ^_^

## Platforms ##

Tested on the following platforms:  

Platform | Description
---------|----------------------------------------------------------
Linux    | CentOS 6.x & Ubuntu 10.04 (kernel 2.6.32) GCC 4.4.7
Win32    | Windows 7, MSVC 10.0
OS X     | Mac OS X EI Capitan, GCC 4.2.1, Apple LLVM version 7.3.0

## Details ##
* [Index](doc/en/index.md)
* [Design](doc/en/design.md)
* [Schema](doc/en/schema.md)
* [FAQ](doc/en/FAQ.md)

## License ##

`slothjson` is licensed under [New BSD License](https://opensource.org/licenses/BSD-3-Clause), which is very permissive and easy to use.

## Author ##

* chengzhuo (jobs, yao050421103@163.com)  

## More ##

- Yet Another Schema - [yas](https://github.com/jobs-github/yas)  
- Sister Project - [slothxml](https://github.com/jobs-github/slothxml)  
- Scalable & Efficient Serialization Library - [rawbuf](https://github.com/jobs-github/rawbuf)  
- High-performance Distributed Storage - [huststore](https://github.com/Qihoo360/huststore)  
