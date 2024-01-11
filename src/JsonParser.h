// Copyright 2024 johannes
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef JSONPARSER_H
#define JSONPARSER_H
#include "json.h"

#include "../lib/DynamicString/dynamic_string.h"


/**
 * does the initial check of the provided json,
 * 
 * 1.   check if there are an equal amount of opening
 *      and closing curlybracets
 * 
 * returns 1 if ok 0 if error
*/
int InitialCheck(const String * src);

/**
 * Returns a string representing an object
 */
String * ChoppObjectString(const String *src);
void *BuildJsonObject(JObject * obj, const String *src);

#endif