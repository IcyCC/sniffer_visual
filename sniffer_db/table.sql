create table request_info
(
	id int null,
	request_id int not null comment '请求的id',
	header_key varchar(255) default '' not null comment '请求头的值',
	header_value varchar(1024) default '' not null comment '请求头的值'
)
;
