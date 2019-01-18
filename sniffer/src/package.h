//
// Created by 苏畅 on 2019/1/15.
//
#include <mysql/mysql.h>
#pragma once
struct RequestInfo{
    int id;
    int type;
    char src[100];
    char dest[100];
   char host[1024];
    int created_at;
} ;

struct RequestHeader{
    int id;
    int request_id;
    char header_key[100];
    char header_value[1024];
};