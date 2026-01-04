-- @author 巷北  
-- @time 2025-12-31 18:34:40  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS Stay(
  checkInId INT AUTO_INCREMENT PRIMARY KEY COMMENT '入住编号, 主键',
  customerId INT NOT NULL COMMENT '客户编号, 外键',
  roomId INT NOT NULL COMMENT '房间编号, 外键',
  checkInTime DATETIME NOT NULL COMMENT '入住时间',
  plannedCheckOutTime DATETIME NOT NULL COMMENT '计划退房时间',
  actualCheckOutTime DATETIME COMMENT '实际退房时间',
  guestCount INT NOT NULL COMMENT '入住人数',
  depositAmount DECIMAL(10, 2) DEFAULT 0.00 COMMENT '押金金额',
  status ENUM('在住', '已退房') DEFAULT '在住' COMMENT '入住状态',
  staffId INT COMMENT '操作员工编号, 外键',
  CONSTRAINT fk_stay_customer FOREIGN KEY (customerId)
    REFERENCES Customer(customerId)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT fk_stay_room FOREIGN KEY (roomId)
    REFERENCES Room(roomId)
    ON DELETE RESTRICT
    ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='入住表';
