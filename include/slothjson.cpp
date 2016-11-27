#include "slothjson.h"

namespace slothjson
{
    // bool
    bool encode(const bool& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetBool(obj_val);
        return true;
    }
    bool decode(const rapidjson::Value& json_val, bool& obj_val)
    {
        if (json_val.IsBool())
        {
            obj_val = json_val.GetBool();
            return true;
        }
        else if (json_val.IsInt())
        {
            int tmp = json_val.GetInt();
            if (!tmp)
            {
                obj_val = false;
            }
            else
            {
                obj_val = true;
            }
            return true;
        }
        return false;
    }
    // std::string
    bool encode(const std::string& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetString(rapidjson::StringRef(obj_val.c_str()));
        return true;
    }
    bool decode(const rapidjson::Value& json_val, std::string& obj_val)
    {
        if (json_val.IsString())
        {
            obj_val = json_val.GetString();
            return true;
        }
        return false;
    }
    // int32_t
    bool encode(const int32_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetInt(obj_val);
        return true;
    }
    bool decode(const rapidjson::Value& json_val, int32_t& obj_val)
    {
        if (json_val.IsInt())
        {
            obj_val = json_val.GetInt();
            return true;
        }
        return false;
    }
    // uint32_t
    bool encode(const uint32_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetUint(obj_val);
        return true;
    }
    bool decode(const rapidjson::Value& json_val, uint32_t& obj_val)
    {
        if (json_val.IsInt())
        {
            obj_val = json_val.GetInt();
            return true;
        }
        else if (json_val.IsUint())
        {
            obj_val = json_val.GetUint();
            return true;
        }
        return false;
    }
    // double
    bool encode(const double& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetDouble(obj_val);
        return true;
    }
    bool decode(const rapidjson::Value& json_val, double & obj_val)
    {
        if (json_val.IsDouble())
        {
            obj_val = json_val.GetDouble();
            return true;
        }
        return false;
    }

    // int64_t & uint64_t
    bool encode(const int64_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetInt64(obj_val);
        return true;
    }
    bool decode(const rapidjson::Value& json_val, int64_t& obj_val)
    {
        if (json_val.IsInt64())
        {
            obj_val = json_val.GetInt64();
            return true;
        }
        else if (json_val.IsInt())
        {
            obj_val = json_val.GetInt();
            return true;
        }
        else if (json_val.IsUint())
        {
            obj_val = json_val.GetUint();
            return true;
        }
        return false;
    }
    bool encode(const uint64_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        json_val.SetUint64(obj_val);
        return true;
    }
    bool decode(const rapidjson::Value& json_val, uint64_t& obj_val)
    {
        if (json_val.IsUint64())
        {
            obj_val = json_val.GetUint64();
            return true;
        }
        else if (json_val.IsInt())
        {
            obj_val = json_val.GetInt();
            return true;
        }
        else if (json_val.IsUint())
        {
            obj_val = json_val.GetUint();
            return true;
        }
        return false;
    }
}

namespace slothjson
{
    // int8_t & uint8_t
    bool encode(const int8_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        int32_t tmp_obj_val = static_cast<int32_t>(obj_val);
        return encode(tmp_obj_val, alloc, json_val);
    }
    bool decode(const rapidjson::Value& json_val, int8_t& obj_val)
    {
        int32_t tmp_obj_val = 0;
        if (!decode(json_val, tmp_obj_val))
        {
            return false;
        }
        obj_val = static_cast<int8_t>(tmp_obj_val);
        return true;
    }

    bool encode(const uint8_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        uint32_t tmp_obj_val = static_cast<uint32_t>(obj_val);
        return encode(tmp_obj_val, alloc, json_val);
    }
    bool decode(const rapidjson::Value& json_val, uint8_t& obj_val)
    {
        uint32_t tmp_obj_val = 0;
        if (!decode(json_val, tmp_obj_val))
        {
            return false;
        }
        obj_val = static_cast<uint8_t>(tmp_obj_val);
        return true;
    }
    // int16_t & uint16_t
    bool encode(const int16_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        int32_t tmp_obj_val = static_cast<int32_t>(obj_val);
        return encode(tmp_obj_val, alloc, json_val);
    }
    bool decode(const rapidjson::Value& json_val, int16_t& obj_val)
    {
        int32_t tmp_obj_val = 0;
        if (!decode(json_val, tmp_obj_val))
        {
            return false;
        }
        obj_val = static_cast<int16_t>(tmp_obj_val);
        return true;
    }

    bool encode(const uint16_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        uint32_t tmp_obj_val = static_cast<uint32_t>(obj_val);
        return encode(tmp_obj_val, alloc, json_val);
    }
    bool decode(const rapidjson::Value& json_val, uint16_t& obj_val)
    {
        uint32_t tmp_obj_val = 0;
        if (!decode(json_val, tmp_obj_val))
        {
            return false;
        }
        obj_val = static_cast<uint16_t>(tmp_obj_val);
        return true;
    }

    // float
    bool encode(const float& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
    {
        double tmp_obj_val = obj_val;
        return encode(tmp_obj_val, alloc, json_val);
    }
    bool decode(const rapidjson::Value& json_val, float & obj_val)
    {
        double tmp_obj_val = 0;
        if (!decode(json_val, tmp_obj_val))
        {
            return false;
        }
        obj_val = static_cast<float>(tmp_obj_val);
        return true;
    }

} // slothjson

namespace slothjson
{
    template <typename src_t, typename dst_t>
    bool __convert(const src_t& src, dst_t& dst)
    {
        if (src.IsBool())
        {
            dst.SetBool(src.GetBool());
        }
        else if (src.IsInt())
        {
            dst.SetInt(src.GetInt());
        }
        else if (src.IsUint())
        {
            dst.SetUint(src.GetUint());
        }
        else if (src.IsInt64())
        {
            dst.SetInt64(src.GetInt64());
        }
        else if (src.IsUint64())
        {
            dst.SetUint64(src.GetUint64());
        }
        else if (src.IsDouble())
        {
            dst.SetDouble(src.GetDouble());
        }
        else if (src.IsString())
        {
            dst.SetString(rapidjson::StringRef(src.GetString()));
        }
        else if (src.IsNull())
        {
            dst.SetNull();
        }
        else
        {
            return false;
        }
        return true;
    }

    bool encode_document(rapidjson::Value& src, rapidjson::Document& dst)
    {
        rapidjson::Document::AllocatorType& allocator = dst.GetAllocator();
        if (src.IsArray())
        {
            dst.SetArray();
            rapidjson::SizeType size = src.Size();
            for (rapidjson::SizeType i = 0; i < size; ++i)
            {
                dst.PushBack(src[i].Move(), allocator);
            }
            return true;
        }
        else if (src.IsObject())
        {
            dst.SetObject();
            for (rapidjson::Value::MemberIterator it = src.MemberBegin(); it != src.MemberEnd(); ++it)
            {
                dst.AddMember(rapidjson::StringRef(it->name.GetString()), it->value.Move(), allocator);
            }
            return true;
        }
        return __convert(src, dst);
    }
    bool decode_document(rapidjson::Document& src, rapidjson::Value& dst)
    {
        rapidjson::Document::AllocatorType& allocator = src.GetAllocator();
        if (src.IsArray())
        {
            dst.SetArray();
            rapidjson::SizeType size = src.Size();
            for (rapidjson::SizeType i = 0; i < size; ++i)
            {
                dst.PushBack(src[i].Move(), allocator);
            }
            return true;
        }
        else if (src.IsObject())
        {
            dst.SetObject();
            for (rapidjson::Document::MemberIterator it = src.MemberBegin(); it != src.MemberEnd(); ++it)
            {
                dst.AddMember(rapidjson::StringRef(it->name.GetString()), it->value.Move(), allocator);
            }
            return true;
        }
        return __convert(src, dst);
    }

    char * __load(const char * path)
    {
        if (!path)
        {
            return NULL;
        }
        FILE * f = fopen(path, "rb");
        if (!f)
        {
            return NULL;
        }
        fseek(f, 0, SEEK_END);
        long len = ftell(f);
        fseek(f, 0, SEEK_SET);
        char * data = (char *)malloc(len + 1);
        fread(data, 1, len, f);
        data[len] = '\0';
        fclose(f);
        return data;
    }

    json_loader_t::json_loader_t(const char * path)
    {
        data_ = __load(path);
    }
    json_loader_t::~json_loader_t()
    {
        if (data_)
        {
            free(data_);
            data_ = NULL;
        }
    }
    const char * json_loader_t::c_str()
    {
        return data_;
    }

}
