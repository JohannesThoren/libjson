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

#include "Json.h"
#include "malloc.h"
#include "string.h"

DEFINE_VECTOR_TYPE(JsonObject, JsonObject, JsonElement);
DEFINE_VECTOR_TYPE(JsonArray, JsonArray, JsonArrayValue);
DEFINE_VECTOR_TYPE(StringVec, StringVec, String);

JsonElement NewElement(JsonType type, const char *key, JsonValue value)
{
    JsonElement elem;
    String keyStr = new_string();
    string_append(&keyStr, key);

    elem.key = keyStr;
    elem.type = type;
    elem.value = value;

    return elem;
}

void AddElement(JsonObject *obj, JsonElement element)
{
    JsonObject_push(obj, element);
}

JsonElement *GetElement(JsonObject *obj, const char *key)
{
    for (int i = 0; i < obj->length; i++)
    {
        JsonElement *elem = (obj->ptr + i);
        if (strcmp(elem->key.ptr, key) == 0)
        {
            return elem;
        }
    }
}

String JsonArrayToString(JsonArray *array)
{
    String str = new_string();
    string_push_char(&str, '[');
    for (int i = 0; i < array->length; i++)
    {
        JsonArrayValue *arrayValue = (array->ptr + i);
        switch (arrayValue->type)
        {
        case ARRAY:
            string_append(&str, JsonArrayToString(arrayValue->value.array).ptr);
            break;
        case BOOL:
            if (arrayValue->value.boolean == true)
                string_append(&str, "true");
            else
                string_append(&str, "false");
            break;
        case OBJECT:
            string_append(&str, ObjectToString(arrayValue->value.object).ptr);
            break;
        case DOUBBLE:
            string_printf(&str, "%lf", arrayValue->value.doubble);
            break;
        case STR:
            string_printf(&str, "\"%s\"", arrayValue->value.string);
        default:
            break;
        }

        if (i != array->length - 1)
        {
            string_push_char(&str, ',');
        }
    }
    string_push_char(&str, ']');

    return str;
}

String ElementToString(JsonElement *element)
{
    String str = new_string();
    string_printf(&str, "\"%s\":", element->key.ptr);

    printf("ElementToString element type: %d element key: %s\n", element->type, element->key.ptr);

    switch (element->type)
    {
    case BOOL:
        if (element->value.boolean == true)
            string_append(&str, "true");
        else
            string_append(&str, "false");
        break;
    case STR:
        string_printf(&str, "\"%s\"", element->value.string.ptr);
        break;
    case DOUBBLE:
        string_printf(&str, "%lf", element->value.doubble);
        break;
    case OBJECT:
        string_printf(&str, "%s", ObjectToString(element->value.object).ptr);
        break;
    case ARRAY:
        string_printf(&str, "%s", JsonArrayToString(element->value.array).ptr);
        break;
    default:
        break;
    }

    return str;
}

String ObjectToString(JsonObject *object)
{

    printf("ObjectToString size %zu\n", object->length);
    String str = new_string();
    string_push_char(&str, '{');
    for (int i = 0; i < object->length; i++)
    {
        String elementString = ElementToString(object->ptr + i);
        printf("ObjectToString element: %s\n", elementString.ptr);

        string_append(&str, elementString.ptr);
        printf("ObjectToString new full string %s\n", str.ptr);

        if (i != object->length - 1)
        {
            string_push_char(&str, ',');
        }
    }
    string_push_char(&str, '}');
    return str;
}

#define MODE_SEEK 0
#define MODE_BUILD_KVP_STR 1
#define MODE_BUILD_KVP_STR_BUILD_SUBOBJ 2
#define MODE_BUILD_KVP_STR_BUILD_ARRAY 3

void __push_char(String *str, const char c)
{
    if (c == '\n' || c == '\t')
        return;
    string_push_char(str, c);
}

int __handle_opening_closing(const char openingC, const char closingC, const String src, String *tmp, int si)
{
    int index = si;
    int opening = 1;
    int closing = 0;
    do
    {

        char c = *(src.ptr + index);
        if (c == openingC)
            opening++;
        if (c == closingC)
            closing++;

        __push_char(tmp, c);
        index++;

    } while (closing != opening);
    return index;
}

String __build_kvp_string(int *mode, int *i, const String src)
{
    int index = *i;
    String tmp = new_string();
    char c = *(src.ptr + index);

    while (*mode == MODE_BUILD_KVP_STR && index < src.length - 1)
    {

        if (index == src.length)
            break;

        c = *(src.ptr + index);

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

StringVec __ChoppObjectString(const String src)
{
    StringVec result = StringVec_new();
    int count = 0;

    int mode = MODE_SEEK;

    for (int i = 0; i < src.length - 1; i++)
    {
        char c = *(src.ptr + i);

        if (mode == MODE_SEEK && (c != '"' || c == '{' || c == '}'))
            continue;

        if (mode == MODE_SEEK && c == '"')
        {

            mode = MODE_BUILD_KVP_STR;
            StringVec_push(&result, __build_kvp_string(&mode, &i, src));

            count++;
        }
    }

    return result;
}

typedef struct JsonElementStrings
{
    String key;
    String value;
} JsonElementStrings;

JsonElementStrings __SplitKeyAndValue(String src)
{

    String key = new_string();
    String value = new_string();
    char c = *(src.ptr);

    int i = 1;
    while (true)
    {
        c = *(src.ptr + i);

        if (c == '\"')
        {
            i++;
            break;
        }

        string_push_char(&key, c);
        i++;
    }

    c = *(src.ptr + i);
    if (c == ':')
    {
        i++;
    }

    c = *(src.ptr + i);
    if (c == ' ')
    {
        i++;
    }

    for (; i < src.length; i++)
    {
        c = *(src.ptr + i);
        string_push_char(&value, c);
    }

    return (JsonElementStrings){.key = key, .value = value};
}

JsonElement __ElementFromString(String src)
{
    JsonElementStrings jes = __SplitKeyAndValue(src);
    printf("KEY = %s, VALUE = %s\n", jes.key.ptr, jes.value.ptr);

    if (*(jes.value.ptr) == '\"' && *(jes.value.ptr + jes.value.length - 1) == '\"')
    {
        String str = string_copy(jes.value);
        string_pop_front(&str, NULL);
        string_pop_back(&str, NULL);
        return NewElement(STR, jes.key.ptr, (JsonValue){.string = str});
    }
    else if (strcmp(jes.value.ptr, "false") == 0)
        return NewElement(BOOL, jes.key.ptr, (JsonValue){.boolean = false});
    else if (strcmp(jes.value.ptr, "true") == 0)
        return NewElement(BOOL, jes.key.ptr, (JsonValue){.boolean = true});
    else if (*(jes.value.ptr) == '{' && *(jes.value.ptr + jes.value.length - 1) == '}')
    {
        JsonObject obj = ObjectFromString(jes.value);
        return NewElement(OBJECT, jes.key.ptr, (JsonValue){.object = &obj});
    }
    else if (atof(jes.value.ptr))
    {
        return NewElement(DOUBBLE, jes.key.ptr, (JsonValue){.doubble = atof(jes.value.ptr)});
    }

    else if (*(jes.value.ptr) == '[' && *(jes.value.ptr + jes.value.length - 1) == ']')
    {
    }
}

JsonObject ObjectFromString(String src)
{
    printf("ObjectFromString %s\n", src.ptr);
    JsonObject obj = JsonObject_new();
    StringVec chopp = __ChoppObjectString(src);

    for (int i = 0; i < chopp.length; i++)
    {
        JsonElement elem = __ElementFromString(*(chopp.ptr + i));
        AddElement(&obj, elem);
    }

    return obj;
}