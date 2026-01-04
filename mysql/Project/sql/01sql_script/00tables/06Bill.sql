-- @author 巷北  
-- @time 2025-12-31 18:58:51  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS Bill(
  billId INT AUTO_INCREMENT PRIMARY KEY COMMENT '账单编号, 主键',
  checkInId INT NOT NULL COMMENT '入住编号, 外键',
  roomCharge DECIMAL(10, 2) DEFAULT 0.00 COMMENT '房费金额',
  diningCharge DECIMAL(10, 2) DEFAULT 0.00 COMMENT '餐饮金额',
  otherCharge DECIMAL(10, 2) DEFAULT 0.00 COMMENT '其它金额',
  totalAmount DECIMAL(10, 2) AS (roomCharge + diningCharge + otherCharge) STORED COMMENT '总金额',
  discountInfo VARCHAR(255) COMMENT '折扣信息',
  paymentMethod ENUM('现金', '微信', '支付宝', '银行卡') COMMENT '计算方式',
  paymentTime DATETIME COMMENT '结算时间',
  CONSTRAINT fk_bill_stay FOREIGN KEY (checkInId)
    REFERENCES Stay(checkInId)
    ON DELETE RESTRICT
    ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='账单表';
