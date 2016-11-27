目录
--

`include`  
　　`slothjson.h`  
　　`slothjson.cpp`  
　　`rapidjson`  
`generator`  
　　`rapidjsongen.py`  
`src`  
　　`sample_test.json`  
　　`sample_test.h`  
　　`sample_test.cpp`  
　　`perf_test.json`  
　　`perf_test.h`  
　　`perf_test.cpp`  
　　`main.cpp`  
`vs2010`  
`Makefile`

* `include/rapidjson` 包含了 `rapidjson-1.1.0` 的源代码。如果需要支持最新的 `rapidjson`， **直接进行代码覆盖即可** 。
* `include/slothjson.h` 和 `include/slothjson.cpp` 是 `slothjson` 的基础库，定义了如下基本类型与 `json` 对象之间的编解码接口：
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
	* `std::vector <T>`
	* `std::map <std::string, T>`  
* `generator/slothjson.py` 用于生成自定义数据类型的 `json` 编解码代码。
* `src/sample_test.json` 和 `src/perf_test.json` 分别是用于单元测试和性能测试的数据描述文件 (schema)；  
`src/sample_test.h` 和 `src/sample_test.cpp` 是 `generator/slothjson.py` 以 `src/sample_test.json` 作为输入生成的代码；  
`src/perf_test.h` 和 `src/perf_test.cpp` 是 `generator/slothjson.py` 以 `src/perf_test.json` 作为输入生成的代码。
* `src/main.cpp` 包含了所有 `slothjson` 的测试代码。
* `vs2010` 是 `slothjson` 在 `windows` 平台下的工程文件；  
`Makefile` 是 `slothjson` 在 `linux` 平台下的工程文件。

接下来，如果您对 `slothjson` 的设计有兴趣，可以点击 [这里](design.md)。

[上一级](../../README_ZH.md)

[根目录](../../README_ZH.md)