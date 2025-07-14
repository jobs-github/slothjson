FAQ
--

### Q: Why is it called slothjson? ###
Because the author is lazy.  
Sloths are almost synonymous with laziness, and this toolkit is for `json`, so it's called `slothjson`.  
The author firmly believes: **Laziness is the driving force behind programmer improvement** ^_^.

### Q: Why use rapidjson? Are there plans to support jsoncpp? ###
For performance. Testing shows that `rapidjson` has **more than half** the time overhead compared to `jsoncpp` for encoding/decoding in production environments.  
Of course, `jsoncpp` provides a much more user-friendly interface than `rapidjson`. If its performance could match or exceed the latter, the author would add support for `jsoncpp`. I have the code, but I'm not releasing it. ^_^  
PS: **If you understand the design approach of `slothjson`, implementing a `jsoncpp` version is very easy**, just a few hundred lines of code. In fact, the original version of `slothjson` was based on `jsoncpp`.

### Q: Why use json as the schema? Why not use an approach similar to [protobuf](https://github.com/google/protobuf)? ###
Because the author is lazy and didn't want to write a `DSL` parser. And there's no need.  
First, `json` as the de facto industrial standard has **virtually no learning curve**, which is very important.  
Second, `json` has **tree-like syntax** with **extremely strong "flexibility"** in semantic construction. We can easily use it to construct `function`, `variable`, and `conditional` judgment semantics; `class`, `property`, and `method` semantics; even relational model `select` and `where` semantics, `make`'s `target` and `depends` semantics, and countless other semantics. Why reinvent the wheel?  
Third, `protobuf`'s `DSL` is very good and powerful, but don't forget, its author is `google`, **backed by a powerful team to carefully craft it**. This poses no difficulty for `google`. But for us ordinary folk developers, it's too massive a project. You should know that **designing and implementing a language can only be done so capriciously by companies at `google`'s level**. For example, the latest [`flatbuffers`](https://github.com/google/flatbuffers). Basically in this industry, anything tagged with `google` finds it hard not to become an industrial standard, so it can ignore `json` and capriciously create many `DSL`s.  
So this question can even be escalated to:  
**When we need to design a `DSL`, why not just use `json`?**  
The author chooses to use `json` directly. First, because there's no need to write a syntax parser (`python` is naturally friendly to `json`), second, no need to worry about the flexibility of semantic construction. Most importantly, if you choose to reinvent a `DSL`, first you need to implement syntax parsing yourself, which will be an arduous task, second, its learning cost **may not be lower than using `json` directly**, third, **do you think you're `google`, inventing a `DSL` and expecting people to use it**? This is the most problematic part...

### Q: Why does one data description file generate both .h and .cpp files? Why not follow [flatbuffers](https://github.com/google/flatbuffers)' approach of merging into one header file? ###
To save your compilation and linking time. To prevent you from smashing your computer due to excessive waiting time. ^_^  
If you've worked on projects like this: message systems that are extremely large, with message types **exceeding 500**, and each message **averaging more than 5 fields**, then the data description file corresponds to more than 500 `struct`s, each containing more than 5 fields. If all the generated code is in one header file (or scattered across many headers, but interface and implementation are not separated), then when you rebuild the project, you could almost take an afternoon nap. Or when you modify a header file and plan to rebuild the project once, congratulations, this time might be enough for you to slowly drink several cups of coffee.  
The author once participated in such a project. Initially, the approach was not to separate implementation, everything was done in header files. The result was that a complete project build took **160 minutes**, and an incremental build took **20 minutes**. Later, after switching to a separated approach, complete builds only took **40 minutes**, and incremental builds only took **5 minutes**. This compressed nearly **80%** of the time. What a great liberation of productivity~~~  
The author's summarized experience is: merging all implementations in one `cpp` results in relatively the shortest build time.

### Q: Why not support more data types? Like `std::list`, `enum`, `union` ###
Many types were once supported, even `boost::uuid`, but they were later removed, keeping only the essential [12 types](schema/types.md) (the author now thinks it could be further reduced, like `float`).  
Because the author is lazy.  
Moreover, `slothjson`'s base library should remain **as simple as possible**. All third-party types can achieve their purpose through **adaptation** to types supported by `slothjson`.  
For example, `enum` can achieve its purpose by adapting to `int32_t`.  
Thanks to this, `slothjson.h` and `slothjson.cpp` **together total only about 700 lines of code**, without depending on any third-party libraries (except `rapidjson`), very lightweight. This is the benefit of subtraction. (For example, the previously supported `boost::uuid` required dependencies, you know ^_^)  
If you must support more data types, like `std::list`, create a new file yourself (for example, `slothjson_wrap.h`) and implement the following two interfaces:

    namespace slothjson
    {
	    template<typename T>
	    bool encode(const std::list<T>& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    template<typename T>
	    bool decode(const rapidjson::Value& json_val, std::list<T>& obj_val);
    }
Implementation methods can refer to `slothjson.h` and `slothjson.cpp`.
All third-party types, as long as they implement this pair of interfaces, can be seamlessly integrated into `slothjson`. However, these types are not **essential** for `slothjson`. For example, `union`.  
The author firmly believes: **good design isn't when there’s nothing more to add, but when there's nothing left to remove**.  

### Q: Any plans to support C++ features like `namespace`, `inheritance`, or `templates` ? ###
It once did. Later, it was discovered that **they are used extremely rarely**, and the implementation became extremely complex, so they were removed. Thanks to this, the code generator implementation became extremely simple, completed in less than 300 lines. There are definitely losses, these fancy features are no longer supported. But precisely because of this subtraction, `slothjson` is guaranteed to be simple enough.  
The author believes that when designing a basic component, **deciding what not to do is as important as deciding what to do**.  
What if you need these features? Consider redesigning.  
Because the functionality you really need, `slothjson` basically has it all;  
The functionality `slothjson` lacks is basically unimportant, or can be solved through workarounds.  
For example, namespaces (make names longer, add project prefixes), inheritance (use composition instead), templates (define several types with different names).

### Q: I must make slothjson support more types and more cool features, what should I do? ###
Fork one yourself to complete it. After you finish adding them, you'll find that not adding them was better. ^_^

[Previous](../../README.md)

[Home](../../README.md)
