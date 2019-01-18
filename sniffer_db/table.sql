create table request_headers
(
	id int not null primary key auto_increment comment '本次请求的id',
	request_id int not null comment '请求id',
	header_key varchar(255) default '' not null comment '请求头的值',
	header_value varchar(1024) default '' not null comment '请求头的值'
)
;
create index request_headers_request_id_index on request_headers (request_id);
create table request_infos
(
	id int not null
		primary key auto_increment comment '本次请求的id',
	type tinytint ,default '0' not null comment '类型 0 为请求 1为响应',
	src varchar(128) default '' not null comment '本次请求的源地址',
	dest varchar(128) default '' null comment '本次请求的目的地址',
	created_at timestamp default CURRENT_TIMESTAMP not null comment '入库时间',
	host varchar(1024) default '' not null comment '请求的url'
)
;
create index request_infos_src_index on request_infos (src);

