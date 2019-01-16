//
// Created by 苏畅 on 2019/1/15.
//
#pragma once
#include <stdio.h>
#include <pcap.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <>


struct RequestInfo{
    int id;
    int type;   //0请求，1响应
    char src[100];
    char dest[100];
    int created_at;
} ;

struct RequestInfo reqinfo;

struct RequestHeader{
    int id;
    int request_id;
    char header_key[100];
    char header_value[1024];
};

struct RequestHeader reqhdl;
