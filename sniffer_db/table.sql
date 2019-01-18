CREATE TABLE `request_headers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `request_id` int(11) NOT NULL COMMENT '请求的id',
  `header_key` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '' COMMENT '请求头的值',
  `header_value` varchar(4096) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '' COMMENT '请求头的值',
  PRIMARY KEY (`id`),
  KEY `request_headers_request_id_index` (`request_id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci

CREATE TABLE `request_infos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `src` varchar(128) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `dest` varchar(128) COLLATE utf8mb4_unicode_ci DEFAULT '',
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '类型 0 为请求 1为响应',
  `host` varchar(1024) COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '' COMMENT '请求的url',
  PRIMARY KEY (`id`),
  KEY `request_infos_src_index` (`src`(10))
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci

