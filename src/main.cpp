#include <stdio.h>

#ifdef WIN32
    #pragma warning(disable: 4005)
    #pragma warning(disable: 4244)
    #include <time.h>
    #include <Windows.h>
    #define INT64_MIN  LLONG_MIN
    #define UINT64_MAX ULLONG_MAX
#elif __APPLE__
    #include <sys/time.h>
    typedef suseconds_t __suseconds_t;
#else
    #include <sys/time.h>
    #define INT64_MIN   LONG_LONG_MIN
    #define UINT64_MAX  ULONG_LONG_MAX
#endif // WIN32

#include "sample_test.h"
#include "perf_test.h"

namespace slothjson
{
    // special case for template object, you can define such object as following code
    template<typename T>
    struct sample_template_t
    {
        T val_;
        bool has_val_;

        bool operator==(const sample_template_t<T> & obj) const
        {
            return val_ == obj.val_;
        }
    };

    // encode & decode for sample_template_t
    template<typename T>
    bool encode(const sample_template_t<T>& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetObject();
        return encode_field(obj_val.val_, "val_", alloc, json_val);
    }

    template<typename T>
    bool decode(const rapidjson::Value& json_val, sample_template_t<T>& obj_val)
    {
        return decode_field(json_val, "val_", obj_val.val_, obj_val.has_val_);
    }

    template <typename T>
    void init(std::vector<T>& obj)
    {
        for (int i = 0; i < 10; ++i)
        {
            obj.push_back(i);
        }
    }

    void init(std::map<std::string, std::string>& obj)
    {
        for (int i = 0; i < 10; ++i)
        {
            char key[32];
            sprintf(key, "key%d", i);
            char val[32];
            sprintf(val, "val%d", i);

            obj[key] = val;
        }
    }

    template<typename T>
    void set_template(T & obj)
    {
        init(obj.vec_val);
        init(obj.str_map_val);
    }

    template<typename T>
    bool test_base(const T& init_val)
    {
        std::string json_val;
        if (!slothjson::encode <false, T> (init_val, json_val))
        {
            return false;
        }
        T out;
        if (!slothjson::decode(json_val, out))
        {
            return false;
        }
        return init_val == out;
    }

    template<typename T>
    bool test_file(const T& init_val, const char * path)
    {
        if (!slothjson::dump <false, T> (init_val, path))
        {
            return false;
        }
        T out;
        if (!slothjson::load(path, out))
        {
            return false;
        }
        return init_val == out;
    }

    struct test_item_t
    {
        std::string name;
        bool ret;
        test_item_t() : ret(false) {}
        test_item_t(const std::string& _name, bool _ret)
            : name(_name), ret(_ret) {}
    };

    struct perf_tester_t
    {
        perf_tester_t()
        {
            obj_.bool_val = true;
            obj_.int8_val = SCHAR_MIN;
            obj_.uint8_val = UCHAR_MAX;
            obj_.int16_val = SHRT_MIN;
            obj_.uint16_val = USHRT_MAX;
            obj_.int32_val = INT_MIN;
            obj_.uint32_val = UINT_MAX;
            obj_.int64_val = INT64_MIN;
            obj_.uint64_val = UINT64_MAX;
            obj_.float_val = 1024.5;
            obj_.double_val = DBL_MAX;
            obj_.str_val = "test_string";
            init(obj_.vec_val);
            init(obj_.dict_val);
        }
        void operator()()
        {
            test_base<perf_object_t>(obj_);
        }
    private:
        perf_object_t obj_;
    };

#ifdef WIN32

    int64_t perf_test(int count)
    {
        LARGE_INTEGER freq={0};
        QueryPerformanceFrequency(&freq); 

        LARGE_INTEGER s = {0};
        QueryPerformanceCounter(&s);
        perf_tester_t obj;
        for (int i = 0; i < count; ++i)
        {
            obj();
        }
        LARGE_INTEGER e = {0};
        QueryPerformanceCounter(&e);

        return ( ((e.QuadPart - s.QuadPart) * 1000000)/freq.QuadPart);
    }
#else
    __suseconds_t get_current_time()
    {
        struct  timeval    tv;
        struct  timezone   tz;
        gettimeofday(&tv,&tz);

        return tv.tv_usec;
    }
    __suseconds_t get_delta(struct  timeval& s, struct  timeval& e)
    {
        time_t delta_sec = e.tv_sec - s.tv_sec;
        __suseconds_t delta = delta_sec * 1000000;
        delta += (e.tv_usec - s.tv_usec);
        return delta;
    }

    int64_t perf_test(int count)
    {
        perf_tester_t obj;
        struct  timeval    s, e;
        struct  timezone   tz;
        gettimeofday(&s, &tz);
        for (int i = 0; i < count; ++i)
        {
            obj();
        }
        gettimeofday(&e, &tz);

        return get_delta(s, e);
    }
#endif

    void test()
    {
        std::vector<test_item_t> items;

        items.push_back(test_item_t("test_base<bool>", test_base<bool>(true)));
        items.push_back(test_item_t("test_base<int8_t>", test_base<int8_t>(SCHAR_MIN)));
        items.push_back(test_item_t("test_base<uint8_t>", test_base<uint8_t>(UCHAR_MAX)));
        items.push_back(test_item_t("test_base<int16_t>", test_base<int16_t>(SHRT_MIN)));
        items.push_back(test_item_t("test_base<uint16_t>", test_base<uint16_t>(USHRT_MAX)));
        items.push_back(test_item_t("test_base<int32_t>", test_base<int32_t>(INT_MIN)));
        items.push_back(test_item_t("test_base<uint32_t>", test_base<uint32_t>(UINT_MAX)));
        items.push_back(test_item_t("test_base<int64_t>", test_base<int64_t>(INT64_MIN)));
        items.push_back(test_item_t("test_base<uint64_t>", test_base<uint64_t>(UINT64_MAX)));
        items.push_back(test_item_t("test_base<float>", test_base<float>(FLT_MIN_EXP)));
        items.push_back(test_item_t("test_base<double>", test_base<double>(DBL_MAX_EXP)));
        items.push_back(test_item_t("test_base<std::string>", test_base<std::string>("test")));

        std::vector<int> tmp_vec;
        items.push_back(test_item_t("test_base<std::vector<int>> empty", test_base< std::vector<int> >(tmp_vec)));
        tmp_vec.push_back(1);
        tmp_vec.push_back(2);
        tmp_vec.push_back(3);
        items.push_back(test_item_t("test_base<std::vector<int>>", test_base< std::vector<int> >(tmp_vec)));

        std::vector<std::string> tmp_str_vec;
        items.push_back(test_item_t("test_base<std::vector<std::string>> empty", test_base< std::vector<std::string> >(tmp_str_vec)));
        tmp_str_vec.push_back("test1");
        tmp_str_vec.push_back("test2");
        tmp_str_vec.push_back("test3");
        items.push_back(test_item_t("test_base<std::vector<std::string>>", test_base< std::vector<std::string> >(tmp_str_vec)));

        std::map<std::string, std::string> tmp_map;
        items.push_back(test_item_t("test_base<std::map<std::string, std::string>> empty", test_base< std::map<std::string, std::string> >(tmp_map)));
        tmp_map["key1"] = "value1";
        tmp_map["key2"] = "value2";
        tmp_map["key3"] = "value3";
        items.push_back(test_item_t("test_base<std::map<std::string, std::string>>", test_base< std::map<std::string, std::string> >(tmp_map)));

        sample_struct_t obj;
        set_template<sample_struct_t>(obj);
        items.push_back(test_item_t("test_base<sample_struct_t>", test_base<sample_struct_t>(obj)));

        slothjson::sample_struct_t sobj;
        set_template<slothjson::sample_struct_t>(sobj);
        items.push_back(test_item_t("test_base<sample_test::sample_struct_t>", test_base<slothjson::sample_struct_t>(sobj)));

        sample_template_t<sample_struct_t> tmpobj;
        tmpobj.val_ = obj;
        items.push_back(test_item_t("test_base<sample_template_t<sample_struct_t>>", test_base< sample_template_t<sample_struct_t> >(tmpobj)));

        items.push_back(test_item_t("test_file<sample_template_t<sample_struct_t>>", test_file< sample_template_t<sample_struct_t> >(tmpobj, "test.json")));

        bool all_passed = true;
        std::vector<test_item_t>::const_iterator it;
        for (it = items.begin(); it != items.end(); ++it)
        {
            if (it->ret)
            {
                printf("%s passed\n", it->name.data());
            }
            else
            {
                all_passed = false;
                printf("%s failed\n", it->name.data());
            }
        }
        if (all_passed)
        {
            printf("all cases passed\n");
        }
        else
        {
            printf("not all cases passed\n");
        }
    }
}


int main(int argc, char * argv[])
{
    if (2 == argc)
    {
        int count = atoi(argv[1]);
        if (count < 1)
        {
            printf("invalid argument\n");
            return 0;
        }
        int64_t us = slothjson::perf_test(count);
        int64_t qps = (int64_t)((double) 1000000 * (double) count / (double) us);
#ifdef __APPLE__
        printf("QPS: %lld\n\n", qps);
#else
        printf("QPS: %ld\n\n", qps);
#endif
        return 0;
    }
    
    slothjson::test();
    return 0;
}
