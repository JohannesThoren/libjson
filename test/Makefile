# Copyright 2024 johannes
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
CC:=clang
CFLAGS:= -I../build/
LIBS:=-l:libjson.a -L../build/

test_parse: test_parse.c libjson.a
	$(CC) $(CFLAGS) test_parse.c $(LIBS) -o test_parse

test_json_object: test_json_object.c libjson.a
	$(CC) $(CFLAGS) test_json_object.c $(LIBS) -o test_json_object

libjson.a:
	cd .. && make

clean:
	rm test_parse
