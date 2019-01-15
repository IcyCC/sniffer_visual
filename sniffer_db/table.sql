create table request_headers
(
	id int null,
	request_id int not null comment '请求id',
	header_key varchar(255) default '' not null comment '请求头的值',
	header_value varchar(1024) default '' not null comment '请求头的值'
)
;

create table request_infos
(
	id int not null
		primary key comment '本次请求的id',
	src varchar(128) default '' not null comment '本次请求的源地址',
	dest varchar(128) default '' null comment '本次请求的目的地址',
	created_at timestamp default CURRENT_TIMESTAMP not null comment '入库时间'
)
;

