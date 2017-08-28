schema
--

`slothjson` uses [yas](https://github.com/jobs-github/yas) as specification of schema.  

### Usage ###

The code generator use the schema as input:

    usage:
        python slothjson.py [option] filelist
            [option]
                -f: use file in filelist
                -p: use dir in filelist, will be parsed recursive
                
    sample:
        python slothjson.py -f file1.json file2.json file3.json
        python slothjson.py -p ./dir1/ ./dir2/ ./dir3/

Comments:

* The `schema` must end with `.json`, for example: perf_test.json
* If use `-f` as option, you can add `schema` file list after it (at least one); if use `-p`  as option, you can add folder list after it (at least one). Each of the folders will be processed **recursively**.
* After parsed by `slothjson.py`, the implements will be generated at the same folder with schema, including `.h` and `.cpp` (the same name as schema). For example, `perf_test.json` as input, `perf_test.h` and `perf_test.cpp` as output.

### Configuration ###

This is a complete configuration, including all supported tags and syntax by `slothjson.py`: 

	{
	    "structs": 
	    [
	        {
	            "type": "object_base_t",
	            "members": 
	            [
                    ["bool", "bool_val", "true"],
	                ["int8_t", "int8_val"],
	                ["uint8_t", "uint8_val"],
	                ["int16_t", "int16_val"],
	                ["uint16_t", "uint16_val"],
	                ["int32_t", "int32_val", "-128"],
	                ["uint32_t", "uint32_val", "65536"],
	                ["int64_t", "int64_val"],
	                ["uint64_t", "uint64_val", "0xffffffffffffffff"],
	                ["float", "float_val"],
	                ["double", "double_val"],
	                ["string", "str_val", "test"],
	                ["[int32_t]", "vec_val"],
	                ["{string}", "dict_val"]
	            ]
	        },
            {
	            "type": "object_t",
	            "members": 
	            [
	                ["object_base_t", "obj_val"],
                    ["[object_base_t]", "obj_vec_val"],
                    ["{object_base_t}", "obj_dict_val"],
                    ["{[object_base_t]}", "obj_vec_dict_val"],
                    ["[{object_base_t}]", "obj_dict_vec_val"],
                    ["[[object_base_t]]", "obj_vec_vec_val"],
                    ["{{object_base_t}}", "obj_dict_dict_val"],
                    ["[{[object_base_t]}]", "obj_vec_dict_vec_val"]
	            ]
	        }
	    ]
	}


### Structure ###
 
[`structs`](schema/structs.md)  
　　[`struct`](schema/struct.md)  
　　　　[`type`](schema/type.md)  
　　　　[`members`](schema/members.md)  
　　　　　　[`member`](schema/member.md)  
　　　　　　　　[`field_type`](schema/field_type.md)  
　　　　　　　　[`field_name`](schema/field_name.md)  
　　　　　　　　[`default_value`](schema/default_value.md)  

### Syntax ###

#### Build-in types ####
Click [here](schema/types.md) for reference.

#### Optional tag ####
When defines [`member`](schema/member.md) , [`default_value`](schema/default_value.md) is optional.

#### Array ####
You can define `array` like this:

    [type-str]
The type of `type-str` is unlimited.

#### Dict ####
You can define `dict` like this:

    {type-str}
The type of `type-str` is unlimited.

#### Nested object ####
* **Different** types of objects could have nested structure, for example, you can define `object_base_t` inside of `object_t` .
* **Nested object is supposed to be defined first**. For instance, `object_base_t` should be defined before `object_t`, otherwise it will not compile.
* **Self-nested** object is NOT allowed.
* **`array` and `dict` could be nested with each other. The nested level is unlimited** .

For example: 

    [{[object_base_t]}]
It will be parsed as the following type:

    std::vector < std::map < std::string, std::vector <object_base_t> > >

#### More ####
You've got almost all secrets of slothjson so far. If you still have questions, please click [here](FAQ.md) to know more about slothjson. Hope it's helpful for you ^_^

[Previous](../../README.md)

[Home](../../README.md)