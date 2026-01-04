-- @author 巷北  
-- @time 2025-12-31 18:47:30  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS ConsumptionRecord(
  recordId INT AUTO_INCREMENT PRIMARY KEY COMMENT '消费记录编号, 主键',
  checkInId INT NOT NULL COMMENT '入住编号, 外键',
  type ENUM('餐饮', '商品', '服务', '其它') NOT NULL COMMENT '消费类型',
  amount DECIMAL(10, 2) NOT NULL COMMENT '消费金额',
  time DATETIME NOT NULL COMMENT '消费时间',
  remarks VARCHAR(255) COMMENT '备注',
   CONSTRAINT fk_consumption_stay FOREIGN KEY (checkInId)
    REFERENCES Stay(checkInId)
    ON DELETE RESTRICT
    ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='消费记录表';
