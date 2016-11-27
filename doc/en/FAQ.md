FAQ
--

### Q: Why named slothjson? ###
Because I'm a lazy man.   
For one thing, sloth stands for "lazy"; for another thing, this tool kit is designed for json, so that's why it is named "slothjson".  

**Lazy brings about the growth of programmer**, that's my belief ^_^

### Q: Why rapidjson? Any plan for jsoncpp? ###
For performance.  Compared with rapidjson, jsoncpp costs **double** time for encoding/decoding in production environment from performance test.  
However, the interface of jsoncpp is more friendly compared to rapidjson. I will support it only if it has better performance, or the same performance as rapidjson at least. I hide the code although I've written it ^_^  
PS: **it's easy to code one for jsoncpp if you've really understood the design of slothjson**, which requires only several hundred lines of code. In fact, the oldest version of slothjson was based on jsoncpp, not rapidjson. 

### Q: Why use json as schema? Why not DSL like [protobuf](https://github.com/google/protobuf) ? ###
Because I'm so lazy that I can't be bothered to write the parser of DSL. Besides, it's unnecessary to do so.  
In the first place, json is the de-facto industrial standard, **very easy to study and use**, and well-known by most programmers. That is very important.   
In the second place,  json's grammar is tree-like, which brings about great flexibility for semantic construction. It's easy to construct semantic like `function`, `variable`, `if-else`, as well as `class`, `property`, `method`, even `select`, `where` in relationship model, `target`, `depends` in makefile, and so on.  
Why reinvent the wheel to increase the mental cost when we already have such great artifact?   
In the third place, while you see the powerful DSL of protobuf, please do not forget google, its "father", **where a powerful team keeps working on this project**. Google can do it easily, rapidly, and continuously, because it's google; I can't, because this project is too immense to me. No one can self-willed design and implement a language except a company like google. For example, the latest works: [`flatbuffers`](https://github.com/google/flatbuffers). In this industry, its easy to become the standard if only marked by google, which results in so many DSLs regardless of json.  
So, this question can be upgraded to the following one:  
**When required DSL, why not json?**   
My answer is json. Firstly it's not need to write the grammar parser (python is friendly with json), secondly it's not need to worry about the flexibility. Above all, as another point of view, if you decide to reinvent another DSL, for you, it's a heavy task to implement the grammar parser; for other people, **maybe it's easier to use json compared to your DSL**. So **maybe nobody will use your DSL in the end, only because you are not google**, then your works doesn't pay...

### Q: Why generate two files (.h & .cpp) from one schema ? Why not join together just like [flatbuffers](https://github.com/google/flatbuffers) ? ###
To save your time on compilation and link process, otherwise you may smash your computer for long wait. ^_^  
If you ever joined a project which included huge message module with **more than 500 types** of message, and **more than 5 fields** in each of type, then you know that the schema would include more than 500 types of struct, and more than 5 fields in each type of struct. If all of codes are generated just in a single header file (or in several files, but the interfaces and implements are together), then you can nearly take a nap when you rebuild the project. If you make changes to a header file, then rebuild, congratulations, you can drink cups of coffee before it ends.   
I've ever participated in such project. In the beginning, the interfaces and implements are together. In consequence, it took **160 minutes** to completely rebuild the project, and **20 minutes** for incremental build. I redesigned it with separable solution, which splited the interfaces and implements into different files. As a result, it took only **40 minutes** to completely rebuild the project, and **5 minutes** for incremental build, which saved nearly **80 percent** of time. What greate liberation for productive force!~~~  
My experience tells me that it cost least time to build if all implements are joined into a single cpp file.

### Q: Why don't support more types? Such as `std::list`, `enum`, `union` ###
Once upon a time slothjson supported them, even included `boost::uuid` , which were all taken away afterwards, except [these types](schema/types.md) (Now, in my eyes, `float` could be removed too)   
Because I'm a lazy man.    
Besides, the base library of slothjson should be kept **as succinct as possible**. All third-party types could be **adapted** to the built-in types of  slothjson to  achieved the aim.  
For example, `enum` could be adapted to `int32_t`.   
As a result, I used about only **700 lines** of code to implement `slothjson.h` and `slothjson.cpp`, without any other third-party libraries (except `rapidjson`), very lightweight. No deletion, no gains. (For example, to support `boost::uuid`, you know what it depends on ^_^)  
If you can not wait to support more types, such as `std::list`, my suggestion is to create a new file (for example, `slothjson_wrap.h`), and implement the two interfaces:

    namespace slothjson
    {
	    template<typename T>
	    bool encode(const std::list<T>& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    template<typename T>
	    bool decode(const rapidjson::Value& json_val, std::list<T>& obj_val);
    }
You can do it with the help of `slothjson.h` and `slothjson.cpp`.  
Seamless integration into slothjson is easy for all third-party types if you implement them. However, these types are not **required** in slothjson. For example, `union`.  
Good design is not achieved when there is nothing left to add, **but when there is nothing left to take away**. That's my belief.  

### Q: Any plan to support C++ features such as `namespace`, `inheritance`, `template` and so on ? ###
Once upon a time slothjson supported these features, which were all taken away afterwards for complexity and frequency of utilization. For one thing, **in practice, these features are rarely used**; for another thing, to add these features, the implement of slothjson became so complicated that it was nearly out of control. What slothjson benefits from such deletion is that it costs only about **300 lines** of code to implement the generator. To promise the succinct of slothjson, it is necessary to give up such fancy but impractical features.  
In my eyes, to design such infrastructure, **what not to do is as important as what to do**.  
Just in case you require these features, please redesign.  
Because what you **really** require is already in slothjson.  
What slothjson lacks is not so important. You can get solution by workround.    
For example, `namespace` (give a long name with prefix such as project), `inheritance` (use composition instead), `template` (give different names for different types)  

### Q: What should I do to support more types, more fancy features ? ###
Please fork one to do it. I promise you will regret to add them ^_^  

[Previous](../../README.md)

[Home](../../README.md)