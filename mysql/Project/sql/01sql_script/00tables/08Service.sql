-- @author 巷北  
-- @time 2025-12-31 19:19:01  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS Service(
  serviceId INT AUTO_INCREMENT PRIMARY KEY COMMENT '服务编号, 主键',
  name VARCHAR(50) NOT NULL COMMENT '服务名称',
  price DECIMAL(10, 2) NOT NULL COMMENT '单价',
  unit ENUM('次', '小时', '件') DEFAULT '次' COMMENT '计费单位',
  description VARCHAR(255) COMMENT '服务描述',
  status ENUM('启用', '停用') DEFAULT '启用' COMMENT '状态'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='服务表';
