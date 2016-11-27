## default_value ##

**类型:** `string`

**值:** `<value-str>`

**属性:** 可选

**父节点:** [member](member.md)

用于描述 `C++` 对象的字段的默认值，用 **字符串** 的形式来描述。

这样的定义是不合法的：

    ...
    ["bool", "bool_val", true],
    ["int8_t", "int8_val", 'a'],
    ["int32_t", "int32_val", -128]
    ...
    

应当更改为这样：

    ...
    ["bool", "bool_val", "true"],
    ["int8_t", "int8_val", "'a'"],
    ["int32_t", "int32_val", "-128"]
    ...

字符串类型的默认值可以直接这样写：

    ["string", "str_val", "test"]

[上一级](../schema.md)

[根目录](../../../README_ZH.md)