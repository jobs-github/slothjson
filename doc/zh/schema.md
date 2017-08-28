数据描述文件
--

`slothjson` 使用 [yas](https://github.com/jobs-github/yas) 作为数据描述文件的规范。  

### 使用范例 ###

`slothjson` 的数据描述文件是代码生成器的输入，使用方法如下：

    usage:
        python slothjson.py [option] filelist
            [option]
                -f: use file in filelist
                -p: use dir in filelist, will be parsed recursive
                
    sample:
        python slothjson.py -f file1.json file2.json file3.json
        python slothjson.py -p ./dir1/ ./dir2/ ./dir3/

说明：

* 数据描述文件必须以 `.json` 作为扩展名，例如：`perf_test.json`。
* 以 `-f` 作为输入选项，后面跟数据描述文件的列表（最少一个）；以 `-p` 作为输入选项，后面跟文件夹列表（最少一个），每个文件夹 **将被递归展开处理** 。
* 数据描述文件经过 `slothjson.py` 的处理，将在同级目录下生成同名的 `.h` 和 `.cpp` 的实现文件。例如，`perf_test.json` 经过处理后，将生成 `perf_test.h` 和 `perf_test.cpp`。

### 配置范例 ###

以下是一个完整的配置文件，包含 `slothjson.py` 所支持的所有字段以及配置语法：

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


### 字段结构 ###
 
[`structs`](schema/structs.md)  
　　[`struct`](schema/struct.md)  
　　　　[`type`](schema/type.md)  
　　　　[`members`](schema/members.md)  
　　　　　　[`member`](schema/member.md)  
　　　　　　　　[`field_type`](schema/field_type.md)  
　　　　　　　　[`field_name`](schema/field_name.md)  
　　　　　　　　[`default_value`](schema/default_value.md)  

### 语法约束 ###

#### 数据类型 ####
参考[这里](schema/types.md)。

#### 可选字段 ####
在定义 [`member`](schema/member.md) 时，[`default_value`](schema/default_value.md) 为可选字段。

#### 数组 ####
描述数组的语法为：

    [type-str]
其中 `type-str` 可以为任意类型。

#### 字典 ####
描述字典的语法为：

    {type-str}
其中 `type-str` 可以为任意类型。

#### 嵌套对象 ####
* 各个 **不同类型** 的对象之间可以嵌套，例如 `object_t` 里面可以嵌套定义 `object_base_t` 的对象。
* **被嵌套的对象必须定义在前面** 。例如， `object_base_t` 必须定义在 `object_t` 的前面，否则生成的代码通不过编译。
* 不能定义 **自我嵌套** 的类型。
* **数组和字典之间可以相互嵌套，且嵌套层级不限**。

例如：

    [{[object_base_t]}]
该类型被展开后生成的类型如下：

    std::vector < std::map < std::string, std::vector <object_base_t> > >

#### 更多 ####
到这里为止， `slothjson` 几乎所有的秘密都公开了。如果您依然有疑问，[这里](FAQ.md) 记录了和 `slothjson` 相关的点点滴滴，希望它能帮助您 ^_^

[上一级](../../README_ZH.md)

[根目录](../../README_ZH.md)