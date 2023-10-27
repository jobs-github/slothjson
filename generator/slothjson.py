# -*- coding: UTF-8 -*-
#/usr/bin/python3
import sys
import os.path
import datetime
import json

CTYPE = 0
NAME = 1
VAL = 2

FILTER = '@null_string_place_holder'
joins = lambda l, s: s.join(l)
merge = lambda l: '\n'.join(filter(lambda item: FILTER != item, l))
json_filter = lambda f: (lambda f, l: os.path.splitext(f)[1] in l)(f, ['.json'])

def manual(): 
    print("""
    usage:
        python3 slothjson.py [option] filelist
            [option]
                -f: use file in filelist
                -p: use dir in filelist, will be parsed recursive
                
    sample:
        python3 slothjson.py -f file1.json file2.json file3.json
        python3 slothjson.py -p ./dir1/ ./dir2/ ./dir3/
        """)

def get_file_list(cur_dir, path_filter):
    def __get(cur_dir, path_filter, file_list):
        for root, dirs, files in os.walk(cur_dir): 
            for f in files:
                if path_filter(f):
                    file_list.append(os.path.join(root, f))
    file_list = []
    __get(cur_dir, path_filter, file_list)
    return file_list

def get_files(file_filter, op, urls):
    if '-f' == op:
        return filter(file_filter, urls)
    elif '-p' == op:
        file_set = set()
        for url in urls:
            for item in get_file_list(url, file_filter):
                if not item in file_set:
                    file_set.add(item)
        return list(file_set)
    return []

def write_file(url, data):
    with open(url, 'w') as f:
        f.writelines(data)

def get_scalar_dict():
    return {
        'bool': 'false',
        'int8_t': '0',
        'uint8_t': '0',
        'short': '0',
        'int16_t': '0',
        'uint16_t': '0',
        'int': '0',
        'int32_t': '0',
        'uint32_t': '0',
        'long': '0',
        'int64_t': '0',
        'uint64_t': '0',
        'float': '0.0',
        'double': '0.0'
        }

def write_interface(path, header, details):
    tm = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
    head_def = '__%s_%s_h__' % (os.path.basename(path).lower(), tm)
    write_file(path + ".h", merge([
        header,
        '#ifndef %s' % head_def,
        '#define %s' % head_def,
        '',
        details,
        '#endif // %s' % head_def
        ]))

def get_header():
    return merge([
        '////////////////////////////////////////////////////////////////////////////////',
        '// NOTE : Generated by slothjson. It is NOT supposed to modify this file.',
        '////////////////////////////////////////////////////////////////////////////////'
        ])

def get_member_methods():
    __do_while_frame = lambda imp: merge([
        '    do',
        '    {',
        imp,
        '',
        '        return true;',
        '    } while (0);',
        '',
        '    return false;'
        ])
    __get_val = lambda member: '"%s"' % member[VAL] if 'std::string' == member[CTYPE] else member[VAL]
    __construct_val = lambda member: FILTER if (len(member) < VAL + 1) else '    %s = %s;' % (member[NAME], __get_val(member))
    __construct = {
        'dec': lambda declare, ctype: ('    %s();' % ctype) if declare else (
            '%s::%s()' % (ctype, ctype)
            ),
        'imp': lambda ctype, members: merge([merge([
            '' if 0 != members.index(member) else FILTER,
            __construct_val(member),
            '    __skip_%s = false;' % member[NAME],
            '    __json_has_%s = false;' % member[NAME],
            ]) for member in members])
        }
    __assign = {
        'dec': lambda declare, ctype: ('    %s& operator=(const %s& obj_val);' % (ctype, ctype)) if declare else (
            '%s& %s::operator=(const %s& obj_val)' % (ctype, ctype, ctype)
            ),
        'imp': lambda ctype, members: merge([
            merge(['    this->%s = obj_val.%s;' % (member[NAME], member[NAME]) for member in members]),
            '    return *this;'
            ])
        }
    __eq = {
        'dec': lambda declare, ctype: '    bool operator==(const %s& obj_val) const;' % ctype if declare else (
            'bool %s::operator==(const %s& obj_val) const' % (ctype, ctype)
            ),
        'imp': lambda ctype, members: merge([
            merge(['    if (!(this->%s == obj_val.%s)) return false;' % (member[NAME], member[NAME]) for member in members]),
            '    return true;'
            ])
        }
    __encode = {
        'dec': lambda declare, ctype: '    bool encode(allocator_t& alloc, rapidjson::Value& json_val) const;' if declare else (
            'bool %s::encode(allocator_t& alloc, rapidjson::Value& json_val) const' % ctype
            ),
        'imp': lambda ctype, members: __do_while_frame(merge([
            '        json_val.SetObject();',
            merge(['        if (!__skip_%s && !encode_field(%s, "%s", alloc, json_val)) break;' % (
                member[NAME], member[NAME], member[NAME]) for member in members])
            ]))
        }
    __decode = {
        'dec': lambda declare, ctype: '    bool decode(const rapidjson::Value& json_val);' if declare else (
            'bool %s::decode(const rapidjson::Value& json_val)' % ctype
            ),
        'imp': lambda ctype, members: __do_while_frame(merge([
            '        if (!decode_field(json_val, "%s", %s, __json_has_%s)) break;' % (
                member[NAME], member[NAME], member[NAME]) for member in members
            ]))
        }
    return [__construct, __assign, __eq, __encode, __decode]

def get_object_methods():
    __encode = {
        'dec': lambda ctype: 'bool encode(const %s& obj_val, allocator_t& alloc, rapidjson::Value& json_val)' % ctype,
        'imp': '    return obj_val.encode(alloc, json_val);'
        }
    __decode = {
        'dec': lambda ctype: 'bool decode(const rapidjson::Value& json_val, %s& obj_val)' % ctype,
        'imp': '    return obj_val.decode(json_val);'
        }
    return [__encode, __decode]

def gen_from_schema(path, obj, scalar_dict, member_methods, object_methods):
    def __check(structs):
        for struct in structs:
            ctype = struct['type']
            if not ctype.endswith('_t'):
                print('"%s" is not end with "_t"' % ctype)
                return False
            for member in struct['members']:
                if len(member) < VAL or len(member) > VAL + 1:
                    print('invalid members: "%s" in struct "%s"' % (str(member), ctype))
                    return False
        return True
    def __get_ctype(key):
        is_array = lambda key: key.startswith('[') and key.endswith(']')
        is_dict = lambda key: key.startswith('{') and key.endswith('}')
        base_type = lambda key: key[1:len(key) - 1]
        if is_array(key):
            return 'std::vector<%s >' % __get_ctype(base_type(key))
        if is_dict(key):
            return 'std::map<std::string, %s >' % __get_ctype(base_type(key))
        if 'string' == key:
            return 'std::string'
        return key
    def __update_object(obj, scalar_dict):
        for struct in obj['structs']:
            struct['ctype'] = struct['type']
            for member in struct['members']:
                member[CTYPE] = __get_ctype(member[CTYPE])
                if len(member) > VAL:
                    continue
                if member[CTYPE] in scalar_dict:
                    member.append(scalar_dict[member[CTYPE]])
    def __gen_interface(path, obj, member_methods, object_methods):
        def __gen_struct(struct, member_methods, object_methods):
            __gen_def = lambda ctype, members: merge([
                'struct %s' % ctype,
                '{',
                merge(['    %s %s;' % (member[CTYPE], member[NAME]) for member in members]),
                '',
                merge(['    void skip_%s() { __skip_%s = true; }' % (member[NAME], member[NAME]) for member in members]),
                '',
                merge(['    bool json_has_%s() const { return __json_has_%s; }' % (member[NAME], member[NAME]) for member in members]),
                '',
                merge([method['dec'](True, ctype) for method in member_methods]),
                '',
                'private:',
                merge(['    bool __skip_%s;' % member[NAME] for member in members]),
                '',
                merge(['    bool __json_has_%s;' % member[NAME] for member in members]),
                '};',
                ''
                ])
            return merge([
                __gen_def(struct['ctype'], struct['members']),
                merge([method['dec'](struct['ctype']) + ';' for method in object_methods]),
                ''
                ])
        write_interface(path, get_header(), merge([
            '#include "slothjson.h"',
            '',
            'namespace slothjson {',
            '',
            merge([__gen_struct(struct, member_methods, object_methods) for struct in obj['structs']]),
            '} // namespace slothjson',
            ''
            ]))
    def __gen_implement(path, obj, member_methods, object_methods):
        def __gen_struct(struct, member_methods, object_methods):
            __gen_member_imp = lambda ctype, members, method: merge([
                method['dec'](False, ctype),
                '{',
                method['imp'](ctype, members),
                '}',
                ''
                ])
            __gen_imp = lambda ctype, method: merge([
                method['dec'](ctype),
                '{',
                method['imp'],
                '}',
                ''
                ])
            __gen_object_implement = lambda ctype, members, member_methods, object_methods: merge([
                merge([__gen_member_imp(ctype, members, method) for method in member_methods]),
                merge([__gen_imp(ctype, method) for method in object_methods])
                ])
            return merge([
                __gen_object_implement(struct['ctype'], struct['members'], member_methods, object_methods)
                ])
        write_file(path + ".cpp", merge([
            get_header(),
            '#include "%s.h"' % os.path.basename(path),
            '',
            'namespace slothjson {',
            '',
            merge([__gen_struct(struct, member_methods, object_methods) for struct in obj['structs']]),
            '} // namespace slothjson'
            ]))
    if not __check(obj['structs']):
        return False
    __update_object(obj, scalar_dict)
    __gen_interface(path, obj, member_methods, object_methods)
    __gen_implement(path, obj, member_methods, object_methods)
    return True

def gen(file_list, scalar_dict, member_methods, object_methods):
    for path_item in file_list:
        with open(path_item, 'r') as f:
            obj = json.load(f)
            gen_from_schema(
                path_item[:path_item.rfind('.')], 
                obj, 
                scalar_dict,
                member_methods,
                object_methods)
    return True

def parse_shell(argv):
    size = len(argv)
    if 1 == size or 2 == size:
        return False
    op = argv[1]
    file_list = get_files(json_filter, op, argv[2:])
    return gen(
        file_list, 
        get_scalar_dict(), 
        get_member_methods(),
        get_object_methods()
        ) if len(file_list) > 0 else False

if __name__ == "__main__":
    if not parse_shell(sys.argv):
        manual()