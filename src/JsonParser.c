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

#define KEY_VALUE_PAIR_CAP 1024
#define MODE_SEEK 0
#define MODE_BUILD_KVP_STR 1

String *ChoppObjectString(const String *src)
{

    String *result = malloc(sizeof(String) * KEY_VALUE_PAIR_CAP);
    int count = 0;
    String tmp = new_string();

    int mode = MODE_SEEK;

    for (int i = 0; i < src->length - 1; i++)
    {
        char c = *(src->ptr + i);
        if (mode == MODE_SEEK && (c != '"' || c == "{" || c == "}"))
            continue;

        if (c == '"')
            mode = MODE_BUILD_KVP_STR;

        int index = 0;
        while (mode == MODE_BUILD_KVP_STR && i + index < src->length - 1)
        {
            char tmpC = *(src->ptr + i + index);

            if (tmpC == ',')
            {
                mode = MODE_SEEK;
                break;
            }

            string_push_char(&tmp, tmpC);

            index++;
        }

        printf("%s\n", tmp.ptr);
        result[count] = string_copy(tmp);

        count++;

        string_clear(&tmp);

        i += index;
    }

    return result;
}
