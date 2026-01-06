-- @author 巷北  
-- @time 2025-12-31 14:54:57  
-- 首先, 需要先创建数据库. 由于这是第一个脚本, 我们只在这一个脚本里写就好了. 

SET NAMES utf8mb4; -- 我服了, 脚本运行, 还必须添加这句, 否则就会中文乱码...... 这运行一次就行, 有没有方式避免重复运行呢? 
CREATE Database IF NOT EXISTS hotel_management
CHARACTER SET utf8mb4
COLLATE utf8mb4_unicode_ci;

-- 切换数据库(我觉得还是叫模式吧, mysql中没有SCHEMA)
USE hotel_management;

-- 创建表. 这是个房型表
CREATE Table IF NOT EXISTS RoomType(
  roomTypeId INT AUTO_INCREMENT PRIMARY KEY COMMENT '房型编号, 唯一标识',
  name VARCHAR(50) NOT NULL COMMENT '房间名称, 比如标准间/大床房/套房',
  bedType VARCHAR(20) NOT NULL COMMENT '床型, 如单人床/双人床/大床',
  maxOccupancy INT NOT NULL COMMENT '可住人数', 
  basePrice DECIMAL(10, 2) NOT NULL COMMENT '基础房价',
  area DECIMAL(6, 2) COMMENT '房间面积, 单位平方米',
  breakfastIncluded BOOLEAN DEFAULT FALSE COMMENT '是否提供早餐',
  remarks VARCHAR(255) COMMENT '备注信息',
  UNIQUE KEY uq_name(name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='房型表';

-- 现在还没想好, 这里, 目前来看, 大致就这个样子吧.
-- INSERT INTO RoomType(name, bedType, maxOccupancy, basePrice, area, breakfastIncluded, remarks)
-- VALUES
-- ('标准间', '标准房', 3, 200.00, 35.00, FALSE, "标准房, 最多容纳三人"),
-- ('大床房', '大床', 4, 400, 50.00, FALSE, "大床房, 最多容纳四人"),
-- ('套房', '豪华大床',4 , 700, 60.00, TRUE, "豪华套房, 最多容纳四人")
