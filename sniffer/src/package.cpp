//
// Created by 苏畅 on 2019/1/15.
//

#include "package.h"
#include <iostream>
#include <string>
using namespace std;

MYSQL conn;
void  connection ()
{

	const char *server="localhost";
    const char *user="root";
    const char *password="123456";
    const char *database="web"; 
	mysql_init(&conn);
	if(!mysql_real_connect(&conn,server,user,password,database,0,NULL,0))
	{
		printf("%s\n", mysql_error(&conn));
	}
	else
	{
		printf("connected..\n");
	}	
}
int SaveRequestInfo(RequestInfo *req){
    // 把一个RequestInfo对象入库 返回id
    // -1代表失败
    // id自动填充到传入的req
    
    MYSQL_RES *res;
    MYSQL_ROW row;
    int t;
	int id = 0;
	char *query;

	query = (char *)malloc((strlen(req->src)+strlen(req->dest)+1)*100);
		
	sprintf(query, "%s%s%s%s%s","insert into request_infos(src,dest)values(",req->src,",",req->dest,");");
	// strcat(query,",");
	// strcat(query,"\"");
	// strcat(query,req->src);
	// strcat(query,"\"");
	// strcat(query,",");
	// strcat(query,"\"");
	// strcat(query,req->dest);
	// strcat(query,"\"");
	// strcat(query,")");
	//printf("%s\n",query);

	t = mysql_query(&conn,query);
	if(t)
	{
    	printf("Error:%s\n",mysql_error(&conn));
    	return -1;
	}
	else
	{
    	printf("success insert\n");
	}
	
	return id;    
};


int SaveRequestHeader(RequestHeader * h)
{
    // 把一个RequestHeader对象入库 返回id
    // -1代表失败
    // id自动填充到传入的req
    
    MYSQL_RES *res;
    MYSQL_ROW row;

    int t;
	int id = 0;
	char *query;
	query = (char *)malloc((strlen(h->header_key)+strlen(h->header_value)+1)*100);
		
	sprintf(query, "%s%d%s%s%s%s%s","insert into request_headers(request_id,header_key,header_value)values(",1,",",h->header_key,",",h->header_value,");");

	// strcat(query,",");
	// strcat(query,"\"");
	// strcat(query,h->header_key);
	// strcat(query,"\"");
	// strcat(query,",");
	// strcat(query,"\"");
	// strcat(query,h->header_value);
	// strcat(query,"\"");
	// strcat(query,")");
	// printf("%s\n",query);

	t=mysql_query(&conn,query);
	if(t)
	{
	    std::cout<<"Error:"<<mysql_error(&conn);
	
	}
	else
	{
	   std::cout<<t<<std::endl;
	}
    
	return id;    
};