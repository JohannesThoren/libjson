
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
 * @file
 */

#ifndef __JSON_H
#define __JSON_H

#include "../lib/DynVec/vector.h"
#include "../lib/DynamicString/dynamic_string.h"
#include "stdbool.h"

DECLARE_VECTOR_TYPE(StringVec, StringVec, String);


/*******************************************************************************
 *                              TYPE DEFINITIONS                               *
 *******************************************************************************/

struct JsonObject;
struct JsonArray;

/// @brief The type of a JsonValue
typedef enum JsonType
{
    BOOL = 1,
    STR = 2,
    OBJECT = 3,
    DOUBBLE = 4,
    ARRAY = 5,
} JsonType;

/// @brief The value of a JsonElement
typedef union JsonValue
{
    bool boolean;
    double doubble;
    struct JsonObject *object;
    String string;
    struct JsonArray *array;
} JsonValue;

typedef struct JsonArrayValue
{
    JsonType type;
    JsonValue value;
} JsonArrayValue;

/// @brief A JsonElement in a JsonObject
typedef struct JsonElement
{
    JsonType type;
    String key;
    JsonValue value;
} JsonElement;

DECLARE_VECTOR_TYPE(JsonObject, JsonObject, JsonElement);
DECLARE_VECTOR_TYPE(JsonArray, JsonArray, JsonArrayValue);

/*******************************************************************************
 *                            FUNCTION DEFINITIONS                             *
 *******************************************************************************/

/// @brief
/// @param type
/// @param key
/// @param value
/// @return
JsonElement NewElement(JsonType type, const char *key, JsonValue value);

/// @brief Add JsonElement To a JsonObject
/// @param obj a pointer to a JsonObject
/// @param element
void AddElement(JsonObject *obj, JsonElement element);

/// @brief
/// @param element
/// @return
String ElementToString(JsonElement *element);

/// @brief
/// @param object
/// @return
String ObjectToString(JsonObject *object);

/// @brief
/// @param src
/// @return
JsonObject ObjectFromString(String src);

/// @brief
/// @param obj
/// @param key
/// @return
JsonElement *GetElement(JsonObject *obj, const char *key);

#endif