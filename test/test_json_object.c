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

#include "../lib/DynamicString/dynamic_string.h"
#include "../src/Json.h"
#include <stdio.h>
int main()
{

    JsonObject obj = JsonObject_new();
    String s = new_string();
    string_append(&s, "test string");
    AddElement(&obj, NewElement(STR, "test1", (JsonValue){.string = s}));

    JsonObject subObj = JsonObject_new();
    String ss = new_string();

    AddElement(&subObj, NewElement(STR, "test1", (JsonValue){.string = s}));

    AddElement(&obj, NewElement(OBJECT, "test5", (JsonValue){.object = &subObj}));

    String str = ObjectToString(&obj);
    JsonObject obj2 = ObjectFromString(str);
    String sssssss = ObjectToString(&obj2);

    printf("result %s", sssssss.ptr);

    return 0;
}