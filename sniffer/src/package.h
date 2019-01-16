//
// Created by 苏畅 on 2019/1/15.
//
#pragma once

#include "stdio.h"
#include "string.h"

struct RequestInfo{
    int id;
    char src[100];
    char dest[100];
    int created_at;
} ;

struct RequestHeader{
    int id;
    int request_id;
    char header_key[100];
    char header_value[1024];
};