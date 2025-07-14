Schema
--

`slothjson` uses [yas](https://github.com/jobs-github/yas) as the specification for schema files.  

### Usage Example ###

The schema file is the input for the code generator, used as follows:

    usage:
        python slothjson.py [option] filelist
            [option]
                -f: use file in filelist
                -p: use dir in filelist, will be parsed recursive
                
    sample:
        python slothjson.py -f file1.json file2.json file3.json
        python slothjson.py -p ./dir1/ ./dir2/ ./dir3/

Notes:

* Schema files must use `.json` as the extension, for example: `perf_test.json`.
* Use `-f` as the input option, followed by a list of schema files (at least one); use `-p` as the input option, followed by a list of folders (at least one), each folder **will be recursively expanded and processed**.
* After processing by `slothjson.py`, implementation files with the same name will be generated in the same directory with `.h` and `.cpp` extensions. For example, after processing `perf_test.json`, it will generate `perf_test.h` and `perf_test.cpp`.

### Configuration Example ###

The following is a complete configuration file containing all fields and configuration syntax supported by `slothjson.py`:

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


### Field Structure ###
 
[`structs`](schema/structs.md)  
　　[`struct`](schema/struct.md)  
　　　　[`type`](schema/type.md)  
　　　　[`members`](schema/members.md)  
　　　　　　[`member`](schema/member.md)  
　　　　　　　　[`field_type`](schema/field_type.md)  
　　　　　　　　[`field_name`](schema/field_name.md)  
　　　　　　　　[`default_value`](schema/default_value.md)  

### Syntax Constraints ###

#### Build-in Types ####
Refer to [here](schema/types.md).

#### Optional Fields ####
When defining a [`member`](schema/member.md), [`default_value`](schema/default_value.md) is an optional field.

#### Array ####
The syntax for describing `array` is:

    [type-str]
where `type-str` can be any type.

#### Dict ####
The syntax for describing `dict` is:

    {type-str}
where `type-str` can be any type.

#### Nested Objects ####
* Objects of **different types** can be nested with each other, for example, `object_t` can contain nested objects of type `object_base_t`.
* **Nested objects must be defined first**. For example, `object_base_t` must be defined before `object_t`, otherwise the generated code will not compile.
* **Self-nested** types are not allowed.
* **`array` and `dict` can be nested with each other, with unlimited nesting levels**.

For example:

    [{[object_base_t]}]
After expansion, this type generates the following type:

    std::vector < std::map < std::string, std::vector <object_base_t> > >

#### More ####
By now, almost all secrets of `slothjson` have been revealed. If you still have questions, [here](FAQ.md) records bits and pieces related to `slothjson`, hope it can help you ^_^

[Previous](../../README.md)

[Home](../../README.md)