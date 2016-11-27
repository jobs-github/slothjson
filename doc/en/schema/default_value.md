## default_value ##

**Type:** `string`

**Value:** `<value-str>`

**Attribute:** optional

**Parent:** [member](member.md)

To specify the default value for field. It should be **string**.

It's incorrect to write like this:

    ...
    ["bool", "bool_val", true],
    ["int8_t", "int8_val", 'a'],
    ["int32_t", "int32_val", -128]
    ...
    

The right way:

    ...
    ["bool", "bool_val", "true"],
    ["int8_t", "int8_val", "'a'"],
    ["int32_t", "int32_val", "-128"]
    ...

`string` type could be defined like this:

    ["string", "str_val", "test"]

[Previous](../schema.md)

[Home](../../../README.md)