-- @author 巷北  
-- @time 2025-12-31 19:43:05  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS MembershipLevel (
    levelId INT AUTO_INCREMENT PRIMARY KEY COMMENT '会员等级编号, 主键',
    levelName VARCHAR(50) NOT NULL COMMENT '等级名称, 如普通会员/白银会员/金卡会员',
    discountRate DECIMAL(4,2) NOT NULL COMMENT '折扣比例, 如 0.95 / 0.90',
    requiredSpending DECIMAL(10,2) DEFAULT 0.00 COMMENT '升级所需累计消费金额',
    privileges VARCHAR(255) COMMENT '会员权益说明'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='会员等级表';
