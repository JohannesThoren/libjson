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
const char *TEST_STR = "{\"x\":10,\"y\":20,\"dat\":{\"name\":\"karlstad\",\"location\":\"varmland\"}}";

int main()
{
    String str = new_string();
    string_append(&str, TEST_STR);
    String *choppedObjects = malloc(sizeof(String) * 1024);

    if (InitialCheck(&str))
    {
        choppedObjects = ChoppObjectString(&str);
    }
    else
        return 1;

    for (int i = 0; i < 1024; i++)
    {
        if (choppedObjects[i].ptr == NULL)
            break;

        printf("%s\n", choppedObjects[i].ptr);
    }

    return 0;
}