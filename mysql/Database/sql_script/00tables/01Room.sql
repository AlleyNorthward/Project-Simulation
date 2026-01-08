-- @author 巷北  
-- @time 2025-12-31 16:48:18  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS Room(
  roomId INT PRIMARY KEY COMMENT '房间编号, 主键',
  roomTypeId INT NOT NULL COMMENT '房型编号, 外键',
  floor INT NOT NULL COMMENT '楼层',
  status ENUM('空闲', '已预订', '已入住', '维修中') DEFAULT '空闲' COMMENT '房态信息',
  discountEligible BOOLEAN DEFAULT FALSE COMMENT '是否打折',
  description VARCHAR(255) COMMENT '描述信息',
  CONSTRAINT fk_room_roomType FOREIGN KEY(roomTypeId)
    REFERENCES RoomType(roomTypeId)
    ON DELETE RESTRICT
    ON UPDATE CASCADE
)ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='房间表';

-- INSERT INTO Room(roomId, roomTypeId, floor, status, discountEligible, description)
-- VALUES
-- (101, 1, 1, '空闲', FALSE, '标准间101号'),
-- (102, 1, 1, '空闲', FALSE, '标准间102号'),
-- (103, 1, 1, '空闲', FALSE, '标准间103号'),
-- (104, 1, 1, '空闲', FALSE, '标准间104号'),
-- (105, 1, 1, '空闲', FALSE, '标准间105号'),
-- (106, 1, 1, '空闲', FALSE, '标准间106号'),
-- (107, 1, 1, '空闲', FALSE, '标准间107号'),
-- (108, 1, 1, '空闲', FALSE, '标准间108号'),
-- (109, 1, 1, '空闲', TRUE, '标准间109号打折房'),
-- (110, 1, 1, '空闲', TRUE, '标准间110号打折房'),
-- (201, 2, 2, '空闲', FALSE, '大床房201号'),
-- (202, 2, 2, '空闲', FALSE, '大床房202号'),
-- (203, 2, 2, '空闲', FALSE, '大床房203号'),
-- (204, 2, 2, '空闲', FALSE, '大床房204号'),
-- (205, 2, 2, '空闲', FALSE, '大床房205号'),
-- (206, 2, 2, '空闲', FALSE, '大床房206号'),
-- (207, 2, 2, '空闲', FALSE, '大床房207号'),
-- (208, 2, 2, '空闲', FALSE, '大床房208号'),
-- (209, 2, 2, '空闲', TRUE, '大床房209号打折房'),
-- (210, 2, 2, '空闲', TRUE, '大床房210号打折房'),
-- (301, 3, 3, '空闲', FALSE, '豪华套房301号'),
-- (302, 3, 3, '空闲', FALSE, '豪华套房302号'),
-- (303, 3, 3, '空闲', FALSE, '豪华套房303号'),
-- (304, 3, 3, '空闲', FALSE, '豪华套房304号'),
-- (305, 3, 3, '空闲', FALSE, '豪华套房305号'),
-- (306, 3, 3, '空闲', FALSE, '豪华套房306号'),
-- 复杂又繁琐. 所以, 我问了问ai, 有没有其它方案快速写入呢? 写个函数就行. 其实, 昨天我就想写函数了, 但是在sql这里, 应用场景又不知道, 以为只有在查询的时候才会用到函数. 原来创建的时候也可以使用, 这样感觉就还不错. 但是下面的是PROCEDURE, 并不是函数, 哈哈.

-- 这个必须要加, 昨天就在这里吃了一个坑.
-- DELIMITER //
--
-- DROP PROCEDURE IF EXISTS GenerateRooms;
-- TRUNCATE TABLE Room;
--
-- CREATE PROCEDURE GenerateRooms()
-- BEGIN
--   DECLARE i INT DEFAULT 101;
--
--   WHILE i <= 110 DO
--     INSERT INTO Room(roomId, roomTypeId, floor, status, discountEligible, description)
--     VALUES (i, 1, 1, '空闲', i >= 109, CONCAT('标准间', i, '号', IF(i >= 109, '打折房', '')));
--     SET i = i + 1;
--   END WHILE;
--
--   SET i = 201;
--   WHILE i <= 210 DO
--     INSERT INTO Room(roomId, roomTypeId, floor, status, discountEligible, description)
--     VALUES (i, 2, 2, '空闲', i >= 209, CONCAT('大床房', i, '号', IF(i >= 209, '打折房', '')));
--     SET i = i + 1;
--   END WHILE;
--
--   SET i = 301;
--   WHILE i <= 306 DO
--     INSERT INTO Room(roomId, roomTypeId, floor, status, discountEligible, description)
--     VALUES (i, 3, 3, '空闲', FALSE, CONCAT('豪华套房', i, '号'));
--     SET i = i + 1;
--   END WHILE;
-- END;
--
-- //
--
-- DELIMITER ;
--
-- CALL GenerateRooms();

-- 思考. Room跟RoomType的关系, 和汽车跟引擎的关系好像啊. 什么关系来着? 聚合? 一开始考虑RoomType的时候, 想着是否要添加其它东西, 比如是否靠窗, 等等, 但是想了想不合理, 却又说不上来什么感觉. 往后面一写, 发现还有个房间(Room), 那么是否靠窗, 其实跟房间关系很大, 跟房间类型关系不大, 有些本末倒置了. 本质还是做的少了, 还得是在实战中获取知识啊 . 
