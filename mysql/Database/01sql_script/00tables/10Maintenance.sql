-- @author 巷北  
-- @time 2025-12-31 19:34:54  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS Maintenance (
    maintenanceId INT AUTO_INCREMENT PRIMARY KEY COMMENT '维修记录编号, 主键',
    roomId INT NOT NULL COMMENT '房间编号, 外键，关联 Room 表',
    employeeId INT NOT NULL COMMENT '维修工员工编号, 外键，关联 Employee 表',
    startTime DATETIME NOT NULL COMMENT '开始时间',
    endTime DATETIME COMMENT '结束时间',
    problemDescription VARCHAR(255) COMMENT '问题描述',
    status ENUM('进行中','已完成') DEFAULT '进行中' COMMENT '维修状态',
    CONSTRAINT fk_maintenance_room FOREIGN KEY (roomId)
        REFERENCES Room(roomId)
        ON DELETE RESTRICT
        ON UPDATE CASCADE,
    CONSTRAINT fk_maintenance_employee FOREIGN KEY (employeeId)
        REFERENCES Employee(employeeId)
        ON DELETE RESTRICT
        ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='维修记录表';
