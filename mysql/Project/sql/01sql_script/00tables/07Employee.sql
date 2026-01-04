-- @author 巷北  
-- @time 2025-12-31 19:09:43  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS Employee(
  employeeId INT AUTO_INCREMENT PRIMARY KEY COMMENT '员工编号, 主键',
  name VARCHAR(50) NOT NULL COMMENT '姓名',
  gender ENUM('男', '女', '其它') DEFAULT '其它' COMMENT '性别',
  phone VARCHAR(20) COMMENT '联系方式',
  position VARCHAR(50) COMMENT '职位',
  department VARCHAR(50) COMMENT '部门',
  hireDate DATE COMMENT '入职时间',
  username VARCHAR(50) UNIQUE NOT NULL COMMENT '用户名',
  password VARCHAR(255) NOT NULL COMMENT '密码',
  permissionLevel ENUM('普通员工', '主管', '管理员') DEFAULT '普通员工' COMMENT '权限等级'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='员工表';

-- 这些肯定会跟上面有联系的, 我先整体地写出来, 然后再根据报告的后面去找找联系.
