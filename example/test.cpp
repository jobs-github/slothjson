//
// Created by kang on 2018/7/22.
//
#include "sample.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv){
    string jIn = R"({
                        "json":"{\"error\":true,\"message\":\"404 not found\",\"code\":0,\"nonce\":\"0.8054632351656703\",\"timestamp\":1532525070907}",
                        "signature":"Q5s1tDJVXWcNXp"
                    })";
    slothjson::root_t rootObj;
    slothjson::root_t in;
    slothjson::json_t jsonObj;
    string jOut;
    bool ret;

    ret = slothjson::decode(jIn, rootObj);
    if (!ret){
        cout <<__func__ <<":"<<__LINE__<<", decode() failed！"<<endl;
        return -1;
    }

    ret = slothjson::decode(rootObj.json, jsonObj);
    if (!ret){
        cout <<__func__ <<":"<<__LINE__<<", decode() failed！"<<endl;
        return -1;
    }

    ret = slothjson::encode<false>(jsonObj, in.json);
    if (!ret){
        cout <<__func__ <<":"<<__LINE__<<", encode() failed！"<<endl;
        return -1;
    }

    ret = slothjson::encode<false>(rootObj.signature, in.signature);
    if (!ret){
        cout <<__func__ <<":"<<__LINE__<<", encode() failed！"<<endl;
        return -1;
    }

    ret = slothjson::encode<false>(in, jOut);
    if (!ret){
        cout <<__func__ <<":"<<__LINE__<<", encode() failed！"<<endl;
        return -1;
    }

    cout <<jOut <<endl;

    return 0;
}
