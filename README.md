[简体中文](README_ZH.md)

# slothjson - object serialization artifact for lazy man #
![slothjson logo](res/logo.png)

`slothjson` is a powerful tool kit used in object serialization and deserialization with **full automation** feature, based on [rapidjson](https://github.com/miloyip/rapidjson). 

With this guy, you can code less, then say goodbye to overtime work. ^_^

The following is the design of slothjson: 
![design](res/design.png)

## What is slothjson ? ##

Do you want to serialize an object (**any type** in C++, remember, **any type**) to JSON, or deserialize it from JSON, with **just a single line of code** ? Like this:  

    template <typename T>
    bool encode(const T& obj_val, std::string& json_val);
    
    template <typename T>
    bool decode(const std::string& json_val, T& obj_val);

If YES, congratulations! It can be well-done by slothjson, which is tailored for you. To avoid writing the JSON serialize/deserialize methods for all types of objects **time and time again** , all you need to do is just to write a SIMPLE schema, then all of the implement will be generated **instantly** by a SIMPLE command line. I think it's a good news for your right-hand (and left-hand) as your productive forces can be liberated. ^_^

So,
> 
`slothjson` is NOT another `rapidjson`. The performance of `rapidjson` is so great that it's unnecessary to reinvent the wheel;  
What the `slothjson` has done is the **automation**, lacking by `rapidjson`;  
`rapidjson` is aimed at the problems **from 0 to 1**;  
`slothjson` is aimed at the problems **from 1 to 100**. Another word, **batch code**.


## Features ##
* Succinct interface for people (everything can be done with just a single line of code)
* Simple, powerful code generator with full automation (not need to implement serialize/deserialize interfaces manually)
* Support optional field (easy to serialize/deserialize field optionally)
* Flexible schema (support array, dict, nested object and **nested array & dict**)
* Succinct design (no tricky C++ template technology, easy to understand), reusable (the same design for XML), extensible (easy to support new types)
* Cross-Platform (Windows & Linux & OS X)

## Usage ##

In the beginning, you need to add the following items to your project:

* `rapidjson`: refer to `include/rapidjson`, the fastest json parser in the world
* `slothjson`: refer to `include/slothjson.h` and `include/slothjson.cpp`, the library of slothjson

**That's all the dependency** , very easy, isn't it ? ^_^

Then, you need to take a few seconds to know the most important interfaces of slothjson: 

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

Argument `pretty` is used to set the style of encoded json string:

* when set `true`, object is encoded humanized ( **with indent and line break** );  
* when set `false`, object is encoded by the fastest way (no indent & line break, everything in a single line)

I believe the interfaces metioned above can meet most of the requirements.

**For example** , write the schema named `fxxx_gfw.json`:  

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

Run command line:  

    python generator/slothjson.py -f src/fxxx_gfw.json

It will generate `fxxx_gfw.h` and `fxxx_gfw.cpp`, which you need to add to your project.  
Then you can code like this: 

    slothjson::fxxx_gfw_t obj_val;
    // set the value of "obj_val"
    ......
    // output as string
	std::string json_val;
	bool rc = slothjson::encode <false> (obj_val, json_val);
    // output as file
    std::string path = "fxxx_gfw_t.json";
    bool rc = slothjson::dump <false> (obj_val, path);

If you don't want to serialize all fields, code like this: 

    obj_val.skip_dict_val(); // call "skip_xxx"
The same as deserialize:

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

After deserialized, if you need to know **whether a field is in JSON or not**, code like this:

    if (obj_val.json_has_dict_val()) // call "json_has_xxx()"
    {
         ......
    }

That's all about the usage, simple & stupid, isn't it ?  

PS: if you are a linux developer, but know nothing about Makefile, I will "despise" you ^_^

## Platforms ##

Tested platforms so far:

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

`slothjson` is licensed under [New BSD License](https://opensource.org/licenses/BSD-3-Clause), a very flexible license to use.

## Author ##

* chengzhuo (jobs, yao050421103@163.com)  

## More ##

- Yet Another Schema - [yas](https://github.com/jobs-github/yas)  
- Sister - [slothxml](https://github.com/jobs-github/slothxml)  
- Scalable & Efficient Serialization Library - [rawbuf](https://github.com/jobs-github/rawbuf)  
- High-performance Distributed Storage - [huststore](https://github.com/Qihoo360/huststore)  