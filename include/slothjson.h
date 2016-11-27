#ifndef __slothjson_20160529042409_h__
#define __slothjson_20160529042409_h__

#ifdef WIN32
#pragma warning(disable: 4305)
#pragma warning(disable: 4996)
#endif // WIN32

#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <typeinfo>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <rapidjson.h>
#include <document.h>
#include <stringbuffer.h>
#include <writer.h>
#include <prettywriter.h>

namespace slothjson
{
    typedef rapidjson::Document::AllocatorType allocator_t;
}

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

namespace slothjson
{
    template<typename T>
    bool encode_field(const T& field, const char * field_name, allocator_t& alloc, rapidjson::Value& json_val);
    template<typename T>
    bool decode_field(const rapidjson::Value& json_val, const char * field_name, T& field, bool& field_in_json);
}

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

// implement
namespace slothjson
{
    template<typename T>
    bool encode(const std::map<std::string, T>& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetObject();
        typename std::map<std::string, T>::const_iterator it;
        for (it = obj_val.begin(); it != obj_val.end(); ++it)
        {
            rapidjson::Value val;
            if (!encode(it->second, alloc, val))
            {
                return false;
            }
            json_val.AddMember(rapidjson::StringRef(it->first.c_str()), val.Move(), alloc);
        }
        return true;
    }
    template<typename T>
    bool decode(const rapidjson::Value& json_val, std::map<std::string, T>& obj_val)
    {
        if (!json_val.IsObject())
        {
            return false;
        }
        for (rapidjson::Value::ConstMemberIterator it = json_val.MemberBegin(); it != json_val.MemberEnd(); ++it)
        {
            T val;
            if (!decode(it->value, val))
            {
                return false;
            }
            obj_val[it->name.GetString()] = val;
        }
        return true;
    }

    template<typename T>
    bool encode(const std::vector<T>& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetArray();
        typename std::vector<T>::const_iterator it;
        for (it = obj_val.begin(); it != obj_val.end(); ++it)
        {
            rapidjson::Value temp;
            if (!encode(*it, alloc, temp))
            {
                return false;
            }
            json_val.PushBack(temp.Move(), alloc);
        }
        return true;
    }
    template<typename T>
    bool decode(const rapidjson::Value& json_val, std::vector<T>& obj_val)
    {
        if (!json_val.IsArray())
        {
            return false;
        }
        rapidjson::SizeType size = json_val.Size();
        obj_val.reserve(size);

        for (rapidjson::SizeType i = 0; i < size; ++i)
        {
            T tmp_val;
            if (!decode(json_val[i], tmp_val))
            {
                return false;
            }
            obj_val.push_back(tmp_val);
        }
        return true;
    }

} // slothjson

namespace slothjson
{
    template<typename T>
    bool encode_field(const T& field, const char * field_name, allocator_t& alloc, rapidjson::Value& json_val)
    {
        if (!field_name)
        {
            return false;
        }
        rapidjson::Value tmp_json_val;
        if (!encode(field, alloc, tmp_json_val))
        {
            return false;
        }
        rapidjson::Value key(field_name, alloc);
        json_val.AddMember(key, tmp_json_val.Move(), alloc);
        return true;
    }

    template<typename T>
    bool decode_field(const rapidjson::Value& json_val, const char * field_name, T& field, bool& field_in_json)
    {
        field_in_json = false;
        if (!json_val.IsObject() || !field_name)
        {
            return false;
        }
        if (json_val.HasMember(field_name))
        {
            field_in_json = true;
            const rapidjson::Value &tmp_json_val = json_val[field_name];
            if (!decode(tmp_json_val, field))
            {
                return false;
            }
        }
        return true;
    }

} // slothjson

namespace slothjson
{
    template <bool pretty> struct json_writer_t;

    template <>
    struct json_writer_t <true>
    {
        typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> writer_t;
    };

    template <>
    struct json_writer_t <false>
    {
        typedef rapidjson::Writer<rapidjson::StringBuffer> writer_t;
    };

    template<bool pretty>
    bool encode(const rapidjson::Document& doc, std::string& json_val)
    {
        rapidjson::StringBuffer buffer;
        typename json_writer_t<pretty>::writer_t writer(buffer);
        doc.Accept(writer);
        json_val = buffer.GetString();
        return true;
    }

    bool encode_document(rapidjson::Value& dst, rapidjson::Document& src);
    bool decode_document(rapidjson::Document& src, rapidjson::Value& dst);

    template<bool pretty, typename T>
    bool encode(const T& obj_val, std::string& json_val)
    {
        rapidjson::Document doc;
        rapidjson::Value value;
        if (!encode(obj_val, doc.GetAllocator(), value))
        {
            return false;
        }
        if (!encode_document(value, doc))
        {
            return false;
        }
        return encode <pretty> (doc, json_val);
    }
    template<typename T>
    bool decode(const char * json_val, T& obj_val)
    {
        rapidjson::Document doc;
        doc.Parse(json_val);
        rapidjson::Value value;
        if (!slothjson::decode_document(doc, value))
        {
            return false;
        }
        return decode(value, obj_val);
    }
    template<typename T>
    bool decode(const std::string& json_val, T& obj_val)
    {
        return decode(json_val.c_str(), obj_val);
    }

} // slothjson

namespace slothjson
{
    template<bool pretty, typename T>
    bool dump(const T& obj_val, const char * path)
    {
        std::string json_val;
        if (!encode <pretty, T> (obj_val, json_val))
        {
            return false;
        }
        std::ofstream os;
        try
        {
            os.open(path, std::ios::binary);
        }
        catch (...)
        {
            return false;
        }
        os << json_val;
        os.close();
        return true;
    }

    struct json_loader_t
    {
        json_loader_t(const char * path);
        ~json_loader_t();
        const char * c_str();
    private:
        char * data_;
    };

    template<typename T>
    bool load(const char * path, T& obj_val)
    {
        json_loader_t loader(path);
        if (!loader.c_str())
        {
            return false;
        }
        return decode(loader.c_str(), obj_val);
    }
};

#endif // __slothjson_20160529042409_h__
