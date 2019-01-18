//
// Created by 鑻忕晠 on 2019/1/15.
//

#include "package.h"	
#include <iostream>
using namespace std;

MYSQL conn;

RequestHeader* InitRequestHeader(RequestHeader * rh){
    rh->id = -1;
    rh->request_id = -1;
    bzero(rh->header_key, sizeof(rh->header_key));
    bzero(rh->header_value, sizeof(rh->header_value));
    return rh;
}

RequestInfo* InitRequestInfo(RequestInfo * rh){
    bzero(rh->src, sizeof(rh->src));
    bzero(rh->dest,sizeof(rh->dest	));
    return rh;
}

void  connection ()
{

	const char *server="localhost";
    const char *user="root";
    const char *password="Root!!2019";
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


int SaveRequestInfo(RequestInfo *req)
{
    // 锟斤拷一锟斤拷RequestInfo锟斤拷锟斤拷锟斤拷锟� 锟斤拷锟斤拷id
    // -1锟斤拷锟斤拷失锟斤拷
    // id锟皆讹拷锟斤拷涞斤拷锟斤拷锟斤拷req
    MYSQL_RES *res;
    MYSQL_ROW row;
    int id;
    char query[8000]={0};
    int t,r;
    char str[2]={0};
	sprintf(str,"%d",req->type);
	sprintf(query,"insert into request_infos(src,dest,type,host)values(");
	strcat(query,"\"");
	strcat(query,req->src);
	strcat(query,"\"");
	strcat(query,",");
	strcat(query,"\"");
	strcat(query,req->dest);
	strcat(query,"\"");
	strcat(query,",");
	strcat(query,str);
	strcat(query,",");
	strcat(query,"\"");
	strcat(query,req->host);
	strcat(query,"\"");
	strcat(query,")");
	printf("%s\n",query);
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
			req->id=id;
				
		}
		mysql_free_result(res);
	}
	return id;
	    
};


int SaveRequestHeader(RequestHeader * h){
    // 锟斤拷一锟斤拷RequestHeader锟斤拷锟斤拷锟斤拷锟� 锟斤拷锟斤拷id
    // -1锟斤拷锟斤拷失锟斤拷
    // id锟皆讹拷锟斤拷涞斤拷锟斤拷锟斤拷req
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[5000]={0}; 
    int t,r;
    int id;

	char str[1000]={0};
	sprintf(str,"%d",h->request_id);
	sprintf(query,"insert into request_headers(request_id,header_key,header_value)values(");
	strcat(query,str);
	strcat(query,",");
	strcat(query,"\"");
	strcat(query,h->header_key);
	strcat(query,"\"");
	strcat(query,",");
	strcat(query,"\"");
	strcat(query,h->header_value);
	strcat(query,"\"");
	strcat(query,")");
	printf("%s\n",query);
	t=mysql_query(&conn,query);
	if(t)
	{
	    cout<<"Error:"<<mysql_error(&conn);
		return -1;
	
	}
	else
	{
	   cout<<t<<endl;
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
