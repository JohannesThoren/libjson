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

/**
 * @file
 */
#ifndef JSON_H
#define JSON_H

#include "../lib/DynVec/vector.h"
#include "../lib/DynamicString/dynamic_string.h"

#define JSON_L_CB '{'
#define JSON_R_CB '}'
#define JSON_COMMA ','
#define JSON_L_SB '['
#define JSON_R_SB ']'
#define JSON_DQ '"'

#define JSON_TYPE_BOOL 0
#define JSON_TYPE_STR 1
#define JSON_TYPE_NUM 3
#define JSON_TYPE_OBJECT 4
#define JSON_TYPE_DOUBLE 5

#define TRUE !0
#define FALSE 0


typedef char *JStr;
typedef enum JBool { T = !0, F = 0 } JBool;
typedef enum JError { J_UNDEFINED, J_TYPE_MISMATCH } JError;

typedef enum {
  BOOL = JSON_TYPE_BOOL,
  STR = JSON_TYPE_STR,
  NUM = JSON_TYPE_NUM,
  OBJECT = JSON_TYPE_OBJECT,
  DOUBLE = JSON_TYPE_DOUBLE
} JType;

struct JKVPair;
struct JObject;

typedef union JValue {
  JBool boolean;
  int integer;
  double decimal;
  struct JObject* object;
  JStr string;
  JError error;
} JValue;

typedef struct JKVPair {
  JType type;
  JStr key;
  JValue value;
} JKVPair;

DECLARE_VECTOR_TYPE(JObject, j, JKVPair)

/**
 *Allaocates memmoryspace for a new json object and returns the pointer
 * 
 */
JObject *j_new_object();

void j_add_bool(JObject *object, JStr key, JBool value);
void j_add_int(JObject *object, JStr key, int value);
void j_add_str(JObject *object, JStr key, JStr value);
void j_add_obj(JObject *object, JStr key, JObject *value);
void j_add_double(JObject *object, JStr key, double value);

JBool j_get_bool(JObject *object, JStr key);
int j_get_int(JObject *object, JStr key);
JStr j_get_str(JObject *object, JStr key);
JObject *j_get_obj(JObject *object, JStr key);
double j_get_double(JObject *object, JStr key);

JStr j_obj_to_str(JObject *obj);
JStr __obj_to_str(JObject *obj);

JObject *j_str_to_obj(const String json_str);

#endif