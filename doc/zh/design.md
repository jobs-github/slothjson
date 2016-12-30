设计方法
--

关键词：第三方 `json` 解析库 + 基础库 + 代码生成器

![design](../../res/design.png)

### 原理 ###
`slothjson` 的设计基于这样一个原理：任何以 `C++` 结构体形式所定义的类型，其对象实例均可以 **递归地展开为一颗“对象树”** 。

例如，有一个结构体 `root_t` 定义如下：

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

可以看到，当实例化一个 `root_t` 的对象：

    root_t root_obj;

它展开后的“对象树”是这样：

`root_obj (root_t)`  
　　`int64_val (int64_t)`  
　　`bool_val (bool)`  
　　`base_val (base_t)`  
　　　　`int32_val (int32_t)`  
　　　　`str_val (std::string)`  

其中，`root_obj` 和 `base_val` 是自定义类型的实例，在“对象树”中的地位相当于 **树枝节点**；  `int64_val`、`bool_val` 等是 `C++` 原生支持的类型的实例，在“对象树”中的地位相当于 **叶子节点** 。

### 推演 ###

基于以上原理，要解决 `C++` 对象与 `json` 序列化的问题，同理可以分解为以下两个子问题：

1. 实现 **叶子节点对象** 与 `json` 序列化的接口；
2. 在 1 的基础上，实现 **树枝节点对象** 与 `json` 序列化的接口。

第一个子问题可具体化为，实现 `C++` 原生支持的类型的 `json` 编解码接口，包括：  

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

其中 `std::string` 严格意义上属于 `STL` 的内置类型，由于它用的太普遍了，所以也将其归为叶子节点。

第二个子问题可具体化为，实现 `C++` 自定义类型的 `json` 编解码接口。进一步可细化为：

* 实现类似 `root_t`、`base_t` 这样的树枝节点类型的 `json` 编解码接口
* 实现数组的 `json` 编解码接口（任意类型）
* 实现字典的 `json` 编解码接口（任意类型）

### 设计方法 ###

第一个子问题的解决办法很简单，因为 `rapidjson` 已经帮我们做好了其中大部分的工作 ^_^  
第二个子问题，要实现类似 `root_t` 这样的树枝节点类型的 `json` 编解码接口，最简单的方案便是将其对象编码为 `json` 的 `object` 类型，以成员变量的名字作为 `key`（名字是不可重复的），值作为 `value`。 这个做法的关键是要 **在运行时遍历对象的成员** 。对于这类问题，有三种自动化的解决方案：

1. 反射
2. 宏
2. 代码生成器

`C++` 由于天生不支持反射，因此基本告别第一种解法。  
PS: 即使 `C++` 支持反射，作者也坚持不用这种做法。因为 **性能太差** 。

宏听起来不错。但是宏是出了名的难调试，难扩展，难维护。而且构建一个充满着宏的庞大系统，其时间开销将是一场噩梦。

所以，解决办法便是用代码生成器。

数组和字典的 `json` 编解码接口，可以借助 `STL` 的内置类型来解决

* `std::vector <T>`
* `std::map <std::string, T>`

总结下来，`slothjson` 的整体设计方法是：

* 使用第三方 `json` 解析库（`rapidjson`）来解决原生的 `C++` 类型的 `json` 编解码问题
* 对于第三方 `json` 解析库不直接支持的原生类型（例如，`int8_t`），将其适配为其支持的类型（强制类型转换）
* 使用代码生成器解决自定义类型的 `json` 编解码问题
* 使用 `STL` 的内置类型来解决数组和字典的 `json` 编解码问题

以下用 `slothjson.h` 中定义的接口来描述完整的设计。

`rapidjson` 中 `json` 的类型如下：

    rapidjson::Value

因此，任何 `C++` 类型，只需要实现从它到 `rapidjson::Value` 编解码的接口，即可支持 `json` 序列化。

    namespace slothjson
	{
        template<typename T>
	    bool encode(const T& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
        template<typename T>
	    bool decode(const rapidjson::Value& json_val, T& obj_val);
    }

说明：`rapidjson::Value` 的实例化依赖于如下对象：

	namespace slothjson
	{
	    typedef rapidjson::Document::AllocatorType allocator_t;
	}
因此 `encode` 函数依赖于 `alloc` 对象。

首先，实现 `rapidjson` 原生支持的类型：

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

然后，将 `rapidjson` 不直接支持的原生类型 **适配** 为它支持的类型：

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

由于 `C++` 有模板这一神器，可以预先支持数组和字典的接口：

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

接下来，提供对象成员变量的编解码接口，方便代码生成器的编写：

    namespace slothjson
	{
	    template<typename T>
	    bool encode_field(const T& field, const char * field_name, allocator_t& alloc, rapidjson::Value& json_val);
	    template<typename T>
	    bool decode_field(const rapidjson::Value& json_val, const char * field_name, T& field, bool& field_in_json);
	}

最后，统一为用户接口：

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

以上便完成了基础库的工作。

进一步，对于用户自定义类型，以如下结构体为例：

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

要实现其编解码，只需要支持如下接口即可：

    bool encode(const perf_object_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
    bool decode(const rapidjson::Value& json_val, perf_object_t& obj_val);

其典型实现如下：

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

其中 `__skip_xxx` 用于支持可选字段编码，`__json_has_xxx` 用于支持可选字段解码。  
以上的解法便是遍历对象的所有成员，以名字作为 `key`，以值作为 `value` 将对象编码为 `json` 的 `object` 实例。这一过程可以借助 [数据描述文件](schema.md) 以及代码生成器实现全自动化。

[上一级](../../README_ZH.md)

[根目录](../../README_ZH.md)