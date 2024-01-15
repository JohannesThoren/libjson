// /**
//  * Copyright 2024 johannes
//  *
//  * Licensed under the Apache License, Version 2.0 (the "License");
//  * you may not use this file except in compliance with the License.
//  * You may obtain a copy of the License at
//  *
//  *     http://www.apache.org/licenses/LICENSE-2.0
//  *
//  * Unless required by applicable law or agreed to in writing, software
//  * distributed under the License is distributed on an "AS IS" BASIS,
//  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  * See the License for the specific language governing permissions and
//  * limitations under the License.
//  */

// /**
//  * { = new object
//  * } = end of object
//  * : = kvp separator
//  * " = start and end of a key
//  * , = separates key value pairs
//  */

// #include "JsonParser.h"
// #include <stdio.h>
// #include <malloc.h>
// #include "Json.h"
// #include <string.h>
// DEFINE_VECTOR_TYPE(StringVec, StringVec, String);

// /**
//  *
//  * OAC = Opening And Closing
//  *
//  */

// typedef struct OAC
// {
//     int o;
//     int c;
// } OAC;

// OAC __get_opening_and_closing_count(const String *src)
// {
//     int opening = 0;
//     int closing = 0;

//     for (int i = 0; i < src->length; i++)
//     {
//         char c = *(src->ptr + i);
//         switch (c)
//         {
//         case '{':
//             opening++;
//             break;

//         case '}':
//             closing++;
//             break;
//         }
//     }

//     return (OAC){opening, closing};
// }

// int InitialCheck(const String *src)
// {

//     OAC oac = __get_opening_and_closing_count(src);

//     if (oac.o != oac.c)
//         printf("[ERROR] opening and closing bracket missmatch\n");
//     else
//         return 1;

//     return 0;
// }

// void __push_char(String *str, const char c)
// {
//     if (c == '\n' || c == '\t')
//         return;
//     string_push_char(str, c);
// }

// int __handle_opening_closing(const char openingC, const char closingC, const String src, String *tmp, int si)
// {
//     int index = si;
//     int opening = 1;
//     int closing = 0;
//     do
//     {

//         char c = *(src.ptr + index);
//         if (c == openingC)
//             opening++;
//         if (c == closingC)
//             closing++;

//         __push_char(tmp, c);
//         index++;

//     } while (closing != opening);
//     return index;
// }

// #define KEY_VALUE_PAIR_CAP 1024
// #define MODE_SEEK 0
// #define MODE_BUILD_KVP_STR 1
// #define MODE_BUILD_KVP_STR_BUILD_SUBOBJ 2
// #define MODE_BUILD_KVP_STR_BUILD_ARRAY 3

// String __build_kvp_string(int *mode, int *i, const String src)
// {
//     int index = *i;
//     String tmp = new_string();
//     char c = *(src.ptr + index);

//     while (*mode == MODE_BUILD_KVP_STR && index < src.length - 1)
//     {

//         if (index == src.length)
//             break;

//         c = *(src.ptr + index);

//         switch (c)
//         {
//         case ',':
//             *mode = MODE_SEEK;
//             break;

//         case '{':
//             __push_char(&tmp, c);
//             *mode = MODE_BUILD_KVP_STR_BUILD_SUBOBJ;
//             index++;
//             break;

//         case '[':
//             __push_char(&tmp, c);
//             *mode = MODE_BUILD_KVP_STR_BUILD_ARRAY;
//             index++;
//             break;
//         default:
//             __push_char(&tmp, c);
//             index++;
//             break;
//         }

//         switch (*mode)
//         {
//         case MODE_BUILD_KVP_STR_BUILD_SUBOBJ:
//             index = __handle_opening_closing('{', '}', src, &tmp, index);
//             *mode = MODE_SEEK;
//             break;

//         case MODE_BUILD_KVP_STR_BUILD_ARRAY:
//             index = __handle_opening_closing('[', ']', src, &tmp, index);
//             *mode = MODE_SEEK;
//             break;
//         }
//     }

//     *i = index;
//     return tmp;
// }

// StringVec __ChoppObjectString(const String src)
// {
//     StringVec result = StringVec_new();
//     int count = 0;

//     int mode = MODE_SEEK;

//     for (int i = 0; i < src.length - 1; i++)
//     {
//         char c = *(src.ptr + i);

//         if (mode == MODE_SEEK && (c != '"' || c == '{' || c == '}'))
//             continue;

//         if (mode == MODE_SEEK && c == '"')
//         {

//             mode = MODE_BUILD_KVP_STR;
//             StringVec_push(&result, __build_kvp_string(&mode, &i, src));

//             count++;
//         }
//     }

//     return result;
// }

// JsonType __CheckType(const String src)
// {

//     for (int i = 0; i < src.length; i++)
//     {
//         const char c = *(src.ptr + i);
//         if (c == ' ')
//             continue;

//         if (c == '"')
//             return STR;

//         if (c == '{')
//             return OBJECT;

//         if (atof(src.ptr) == 0 && c == (char)48 || atof(src.ptr) != 0)
//             return DOUBBLE;

//         if (strcmp(src.ptr, "true") == 0 || strcmp(src.ptr, "false") == 0)

//             return BOOL;
//     }

//     return -1;
// }

// String __remove_star_space(const String src)
// {
//     String tmp = new_string();

//     if (*src.ptr == ' ')
//     {
//         for (int i = 0; i < src.length; i++)
//         {
//             string_push_char(&tmp, *(src.ptr + i));
//         }
//     }

//     return tmp;
// }

// String __clean_value(const String value)
// {
//     String tmp = new_string();
//     int i = 0;
//     while (*(value.ptr + i) != '"')
//     {
//         if (i == value.length)
//         {
//             i = 0;
//             break;
//         }
//         i++;
//     }

//     for (int j = i + 1; j < (*(value.ptr + value.length - 1) == '"' ? value.length - 1 : value.length); j++)
//     {
//         string_push_char(&tmp, *(value.ptr + j));
//     }

//     return tmp;
// }

// String __clean_key(const String key)
// {
//     String tmp = new_string();

//     for (int i = 1; i < key.length - 1; i++)
//         string_push_char(&tmp, *(key.ptr + i));

//     return tmp;
// }

// void BuildJsonObject(JsonObject *obj, const String src)
// {

//     String key = new_string();
//     String value = new_string();

//     int index = 0;

//     while (1)
//     {
//         if (*(src.ptr + index) == ':' || index == src.length)
//             break;

//         string_push_char(&key, *(src.ptr + index));

//         index++;
//     }

//     index++;

//     while (1)
//     {
//         if (index == src.length)
//             break;

//         string_push_char(&value, *(src.ptr + index));

//         index++;
//     }

//     key = __clean_key(key);

//     switch (__CheckType(__remove_star_space(value)))
//     {
//     case STR:
//         value = __clean_value(value);
//         AddElement(obj, NewElement(DOUBBLE, key.ptr, (JsonValue){.string = value}));
//         break;
//     case DOUBBLE:
//         value = __clean_value(value);
//         AddElement(obj, NewElement(DOUBBLE, key.ptr, (JsonValue){.doubble = atof(value.ptr)}));
//         break;
//     case BOOL:
//         value = __clean_value(value);
//         if (strcmp(value.ptr, "false") == 0)
//             AddElement(obj, NewElement(BOOL, key.ptr, (JsonValue){.boolean = false}));
//         else
//             AddElement(obj, NewElement(BOOL, key.ptr, (JsonValue){.boolean = true}));
//         break;

//     case OBJECT:
//     {
//         JsonObject tmp = JsonObject_new();
//         tmp = ObjectFromString(value);
//         AddElement(obj, NewElement(OBJECT, key.ptr, (JsonValue){.object = &tmp}));
//     }
//     break;
//     default:
//         break;
//     }
// }
