FAQ
--

### Q: Why is it called slothjson? ###
Because I'm lazy.   
"Sloth" means lazy, and this toolkit works with JSON—so, `sloth` + `JSON` = `slothjson`.  

**I believe a bit of laziness can make a programmer more productive.** ^_^  

### Q: Why use `RapidJSON`? Any plans to support `JsonCpp`? ###
We chose `RapidJSON` for performance: in our benchmarks, `JsonCpp` takes roughly **twice** as long for encoding/decoding in production tests.    
That said, `JsonCpp` does offer a more user-friendly API. I'd consider adding support if it matched or exceeded `RapidJSON`'s speed. (I actually wrote the `JsonCpp` version but kept it hidden.) ^_^  
**If you understand `slothjson`'s design, you could add `JsonCpp` support in just a few hundred lines of code**. The earliest version of `slothjson` was actually based on `JsonCpp`.  

### Q: Why use `JSON` for the schema instead of a DSL like [protobuf](https://github.com/google/protobuf) ? ###
Because I'm lazy and didn't want to write a custom parser — and I didn't have to. `JSON` is already the de-facto standard: **easy to learn, universally supported**, and familiar to most developers.  
Plus, `JSON`'s tree-like structure is **incredibly flexible**: you can model `functions`, `classes`, SQL-like `select/where` clauses, Makefile directives like `target/depends`, and more. Why reinvent the wheel?  
Finally, powerful DSLs (e.g., `Protobuf`, [`Flatbuffers`](https://github.com/google/flatbuffers)) are backed by large teams (Google) that can maintain and evolve them. As a solo developer, I don’t have those resources — and even if you create your own DSL, others may still prefer plain JSON over a custom format.  

>  
**When you need a schema language, why not just use JSON?**   
– No parser to write (JSON is native to Python and many languages)  
– Full flexibility without extra tooling  
– Broad familiarity among developers  

### Q: Why generate separate .h and .cpp files instead of a single header like [`flatbuffers`](https://github.com/google/flatbuffers) ? ###
To speed up your builds. If your schema defines **500+ types** each with **5+ fields**, stuffing everything into one header can make rebuilds excruciating.  
In one project I saw, with headers and implementations combined, a full rebuild took **160 minutes** and an incremental build took **20 minutes**. After splitting interfaces (`.h`) and implementations (`.cpp`), full rebuilds dropped to **40 minutes** and incremental builds to **5 minutes** — an **80%** time savings. That's a game-changer for productivity!  
My experience shows that collecting all implementations in a single `.cpp` file minimizes build time.  

### Q: Why not support more types like `std::list`, `enum` or `union` ###
`slothjson` once supported those (even `boost::uuid`), but they were removed, leaving only the core types [here](schema/types.md). (Honestly, `float` could probably go too.)  
I aim to keep `slothjson` as **lightweight** as possible. You can **adapt** any third-party or custom type to the built-in ones—for example, map an `enum` to an `int32_t`.  
The entire library is about **700 lines** (excluding `RapidJSON`), with no extra dependencies. If you want to add `std::list` support, just create a wrapper header (e.g., `slothjson_wrap.h`) and implement:  

    namespace slothjson
    {
	    template<typename T>
	    bool encode(const std::list<T>& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
	    template<typename T>
	    bool decode(const rapidjson::Value& json_val, std::list<T>& obj_val);
    }
Seamless integration is straightforward, but these types aren't required for the core library. **Good design isn't when there’s nothing more to add, but when there's nothing left to remove**.  

### Q: Any plans to support C++ features like `namespace`, `inheritance`, or `templates` ? ###
`slothjson` once included those features, but they were dropped due to complexity and low usage. In practice, **they're rarely needed**, and they bloated the code generator by hundreds of lines. Now the generator is about **300 lines**.  
Sometimes knowing **what not to include is as important as knowing what to include**. If you need namespaces, inheritance, or templates, you can extend `slothjson` yourself:  
- **namespace**: use naming prefixes  
- **inheritance**: prefer composition  
- **templates**: give distinct type names  

What you really need is already supported — any missing functionality can often be worked around.  

### Q: How can I add more types or advanced features ? ###
Feel free to fork the project and experiment. Just be warned — you might regret how much work that is! ^_^  

[Previous](../../README.md)

[Home](../../README.md)
