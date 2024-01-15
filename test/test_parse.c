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
#include "../src/Json.h"

int main(int argc, char **argv)
{
    String s = new_string();
    printf("object from string\n\n");
    string_append(&s, "{\"test1\":\"test string\",\"test2\":420.690000,\"test3\":true,\"test4\":false}");
    JsonObject obj = ObjectFromString(s);
    printf("\n\n\n\n\n\nobject to string\n\n");
    printf("%s", ObjectToString(&obj));

    return 0;
}