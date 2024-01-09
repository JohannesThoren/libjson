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

const char *TEST_STR = "{\"x\":10,\"y\":20,\"dat\":{\"name\":\"karlstad\"}}";

int main()
{
    String str = new_string();
    string_append(&str, TEST_STR);

    if (InitialCheck(&str))
    {
        printf("%s",ChoppObjectString(&str)[0].ptr);
    }
    else
        return 1;

    return 0;
}