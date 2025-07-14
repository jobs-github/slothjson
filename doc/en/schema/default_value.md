## default_value ##

**Type:** `string`

**Value:** `<value-str>`

**Attribute:** optional

**Parent:** [member](member.md)

Used to describe the default value of a C++ object field, which must be described in **string** format.

The following definitions are invalid:

    ...
    ["bool", "bool_val", true],
    ["int8_t", "int8_val", 'a'],
    ["int32_t", "int32_val", -128]
    ...
    

Should be changed to:

    ...
    ["bool", "bool_val", "true"],
    ["int8_t", "int8_val", "'a'"],
    ["int32_t", "int32_val", "-128"]
    ...

Default values for `string` type can be written directly like this:

    ["string", "str_val", "test"]

[Previous](../schema.md)

[Home](../../../README.md)