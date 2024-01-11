/**
 * Copyright 2024 johannes
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * { = new object
 * } = end of object
 * : = kvp separator
 * " = start and end of a key
 * , = separates key value pairs
 */

#include "JsonParser.h"
#include <stdio.h>
#include <malloc.h>
#include "json.h"
#include <string.h>

/**
 *
 * OAC = Opening And Closing
 *
 */

typedef struct OAC
{
    int o;
    int c;
} OAC;

OAC __get_opening_and_closing_count(const String *src)
{
    int opening = 0;
    int closing = 0;

    for (int i = 0; i < src->length; i++)
    {
        char c = *(src->ptr + i);
        switch (c)
        {
        case '{':
            opening++;
            break;

        case '}':
            closing++;
            break;
        }
    }

    return (OAC){opening, closing};
}

int InitialCheck(const String *src)
{

    OAC oac = __get_opening_and_closing_count(src);

    if (oac.o != oac.c)
        printf("[ERROR] opening and closing bracket missmatch\n");
    else
        return 1;

    return 0;
}

void __push_char(String *str, const char c)
{
    if (c == '\n' || c == '\t')
        return;
    string_push_char(str, c);
}

int __handle_opening_closing(const char openingC, const char closingC, const String *src, String *tmp, int si)
{
    int index = si;
    int opening = 1;
    int closing = 0;
    do
    {

        char c = *(src->ptr + index);
        // printf("i=%d c=%c o=%d c=%d\n", index, c, opening, closing);
        if (c == openingC)
            opening++;
        if (c == closingC)
            closing++;

        __push_char(tmp, c);
        index++;

    } while (closing != opening);
    return index;
}

#define KEY_VALUE_PAIR_CAP 1024
#define MODE_SEEK 0
#define MODE_BUILD_KVP_STR 1
#define MODE_BUILD_KVP_STR_BUILD_SUBOBJ 2
#define MODE_BUILD_KVP_STR_BUILD_ARRAY 3

String __build_kvp(int *mode, int *i, const String *src)
{
    int index = *i;
    String tmp = new_string();
    char c = *(src->ptr + index);

    while (*mode == MODE_BUILD_KVP_STR && index < src->length - 1)
    {

        if (index == src->length)
            break;

        c = *(src->ptr + index);

        switch (c)
        {
        case ',':
            *mode = MODE_SEEK;
            break;

        case '{':
            __push_char(&tmp, c);
            *mode = MODE_BUILD_KVP_STR_BUILD_SUBOBJ;
            index++;
            break;

        case '[':
            __push_char(&tmp, c);
            *mode = MODE_BUILD_KVP_STR_BUILD_ARRAY;
            index++;
            break;
        default:
            __push_char(&tmp, c);
            index++;
            break;
        }

        switch (*mode)
        {
        case MODE_BUILD_KVP_STR_BUILD_SUBOBJ:
            index = __handle_opening_closing('{', '}', src, &tmp, index);
            *mode = MODE_SEEK;
            break;

        case MODE_BUILD_KVP_STR_BUILD_ARRAY:
            index = __handle_opening_closing('[', ']', src, &tmp, index);
            *mode = MODE_SEEK;
            break;
        }
    }

    *i = index;
    return tmp;
}

String *ChoppObjectString(const String *src)
{
    String *result = malloc(sizeof(String) * KEY_VALUE_PAIR_CAP);
    int count = 0;

    int mode = MODE_SEEK;

    for (int i = 0; i < src->length - 1; i++)
    {
        char c = *(src->ptr + i);

        if (mode == MODE_SEEK && (c != '"' || c == '{' || c == '}'))
            continue;

        if (mode == MODE_SEEK && c == '"')
        {

            mode = MODE_BUILD_KVP_STR;
            result[count] = __build_kvp(&mode, &i, src);

            count++;
        }
    }

    return result;
}

JType __CheckType(const String *src)
{

    for (int i = 0; i < src->length; i++)
    {
        const char c = *(src->ptr + i);
        if (c == ' ')
            continue;

        if (c == '"')
            return JSON_TYPE_STR;

        if (c == '{')
            return JSON_TYPE_OBJECT;

        if ((atoi(src->ptr) == 0 && c == (char)48) || atoi(src->ptr) != 0)
            return JSON_TYPE_NUM;

        if (atof(src->ptr))
            return JSON_TYPE_DOUBLE;

        if (strcmp(src->ptr, "true") == 0 || strcmp(src->ptr, "false") == 0)
            return JSON_TYPE_BOOL;
    }

    return J_UNDEFINED;
}

void *BuildJsonObject(JObject *obj, const String *src)
{
    String key = new_string();
    String value = new_string();

    int index = 0;

    while (1)
    {
        if (*(src->ptr + index) == ':' || index == src->length)
            break;

        string_push_char(&key, *(src->ptr + index));

        index++;
    }

    index++;

    while (1)
    {
        if (index == src->length)
            break;

        string_push_char(&value, *(src->ptr + index));

        index++;
    }

    printf("%d", __CheckType(&value));

    switch (__CheckType(&value))
    {
    case JSON_TYPE_STR:
        printf("string\n");
        j_add_str(obj, key.ptr, value.ptr);
        break;
    case JSON_TYPE_NUM:
        printf("num\n");
        j_add_int(obj, key.ptr, atoi(value.ptr));
        break;

    default:
        break;
    }
}
