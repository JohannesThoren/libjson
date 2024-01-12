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

#include "JsonFile.h"
#include "JsonParser.h"
#include <stdio.h>
#include <stdlib.h>

String ReadStringFromFile(const char *path)
{
    String str = new_string();

    FILE *fp = fopen(path, "rb");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *cstr = (char *)malloc(fsize + 2);
    fread(cstr, fsize, 1, fp);
    fclose(fp);

    string_append(&str, cstr);

    return str;
}



JObject * JsonFromFile(const String src) {
    JObject * obj = j_new_object();
    ChoppObjectString(src);
    

    return obj;
}