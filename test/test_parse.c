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
 * limitations under the License.<
 */
#include <stdio.h>
#include "../build/include/JsonParser.h"
#include "malloc.h"
#include "../src/JsonFile.h"

int main(int argc, char **argv)
{
    String str = ReadStringFromFile("test.json");
    String *choppedObjects = malloc(sizeof(String) * 1024);
    JObject *obj = j_new_object();

    if (InitialCheck(&str))
    {
        choppedObjects = ChoppObjectString(&str);
        BuildJsonObject(obj, &choppedObjects[0]);
    }

    else
        return 1;

    for (int i = 0; i < 1024; i++)
    {
        if (choppedObjects[i].ptr == NULL)
            break;

        printf("%s\n", choppedObjects[i].ptr);
    }

    printf("%s\n", j_obj_to_str(obj));

    return 0;
}