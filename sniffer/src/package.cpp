//
// Created by 苏畅 on 2019/1/15.
//

#include "package.h"	
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
    connection();
    MYSQL_RES *res;
    MYSQL_ROW row;
    int id;
    char query[200];
    int t,r;
	sprintf(query,"insert into request_infos(src,dest) values(req->src,req->dest)");
	t=mysql_query(&conn,query);
	if(t)
	{
    	printf("Error:%s\n",mysql_error(&conn));
    	return -1;
	}
	else
	{
    	printf("success insert\n");
	}
	char find_id[200];
	sprintf(find_id,"select last_insert_id()");
	r=mysql_query(&conn,find_id);
	if(r)
	{
		printf("Error:%s\n",mysql_error(&conn));
		return -1;
	}
	else
	{
		res=mysql_store_result(&conn);
		if(res)
		{
		  
			row=mysql_fetch_row(res)
			printf("%s\n",row[0]);
			id=atoi(row[0]);
			req->id=id;
				
		}
		mysql_free_result(res);
	}
	return id;
	    
};


int SaveRequestHeader(RequestHeader * h){
    // 把一个RequestHeader对象入库 返回id
    // -1代表失败
    // id自动填充到传入的req
    connection();
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[200];
    int t,r;
    int id;
	sprintf(query,"insert into request_headers(request_id,header_key,header_value) values(h->request_id,h->header_key,req->header_value)");
	t=mysql_query(&conn,query);
	if(t)
	{
	   printf("Error:%s\n",mysql_error(&conn));
       return -1;
	
	}
	else
	{
	   printf("success insert\n");
	}
    
	char find_id[200];
	sprintf(find_id,"select last_insert_id()");
	r=mysql_query(&conn,find_id);
	if(r)
	{
		printf("Error:%s\n",mysql_error(&conn));
		return -1;
	}
	else
	{
		res=mysql_store_result(&conn);
		if(res)
		{
		  
			row=mysql_fetch_row(res);
			printf("%s\n",row[0]);
			id=atoi(row[0]);
			h->id=id;
				
		}
	mysql_free_result(res);
	}
	return id;
   
    
};