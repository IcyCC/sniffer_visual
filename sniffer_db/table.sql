create table request_headers
(
	id int auto_increment
		primary key,
	request_id int not null comment '请求的id',
	header_key varchar(255) default '' not null comment '请求头的值',
	header_value varchar(4096) default '' not null comment '请求头的值'
)
;

create index request_headers_request_id_index
	on request_headers (request_id)
;

create table request_infos
(
	id int auto_increment
		primary key,
	src varchar(128) default '' not null,
	dest varchar(128) default '' null,
	created_at timestamp default CURRENT_TIMESTAMP not null,
	type tinyint default '0' not null comment '类型 0 为请求 1为响应',
	host varchar(1024) default '' not null comment '请求的url'
)
;

create index request_infos_src_index
	on request_infos (src)
;

