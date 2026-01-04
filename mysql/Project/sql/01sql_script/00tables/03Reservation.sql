-- @author 巷北  
-- @time 2025-12-31 18:22:34  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS Reservation(
  reservationId INT AUTO_INCREMENT PRIMARY KEY COMMENT '预定编号, 主键',
  customerId INT NOT NULL COMMENT '客户编号, 外键',
  roomTypeId INT NOT NULL COMMENT '房型编号, 外键',
  reservedRoomCount INT NOT NULL COMMENT '预定房间数',
  checkInDate DATE NOT NULL COMMENT '入住日期',
  checkOutDate DATE NOT NULL COMMENT '退房日期',
  status ENUM('已预订', '已取消', '已入住') DEFAULT '已预订' COMMENT '预定状态',
  depositAmount DECIMAL(10, 2) DEFAULT 0.00 COMMENT '定金金额',
  CONSTRAINT fk_reservation_customer FOREIGN KEY (customerId)
    REFERENCES Customer(customerId)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,

  CONSTRAINT fk_reservation_roomType FOREIGN KEY (roomTypeId)
    REFERENCES RoomType(roomTypeId)
    ON DELETE RESTRICT
    ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='预定表';

-- 插入几个预定
-- INSERT INTO Reservation(customerId, roomTypeId, reservedRoomCount, checkInDate, checkOutDate, status, depositAmount)
-- VALUES
-- (1, 1, 2, '2026-01-01', '2026-01-03', '已预订', 200.00),
-- (2, 2, 1, '2026-01-05', '2026-01-06', '已预订', 150.00),
-- (3, 3, 1, '2026-01-10', '2026-01-12', '已预订', 500.00),
-- (4, 1, 1, '2026-01-15', '2026-01-18', '已预订', 100.00),
-- (5, 2, 2, '2026-01-20', '2026-01-22', '已预订', 300.00);
