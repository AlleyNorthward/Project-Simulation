-- @author 巷北  
-- @time 2025-12-31 19:26:25  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS Payment (
  paymentId INT AUTO_INCREMENT PRIMARY KEY COMMENT '支付记录编号, 主键',
  billId INT NOT NULL COMMENT '账单编号, 外键，关联 Bill 表',
  paidAmount DECIMAL(10,2) NOT NULL COMMENT '支付金额',
  paymentMethod ENUM('现金','微信','支付宝','银行卡') NOT NULL COMMENT '支付方式',
  paymentTime DATETIME NOT NULL COMMENT '支付时间',
  operatorId INT COMMENT '操作员工编号, 外键，关联 Employee 表',
  remarks VARCHAR(255) COMMENT '备注信息',
  CONSTRAINT fk_payment_bill FOREIGN KEY (billId)
    REFERENCES Bill(billId)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT fk_payment_operator FOREIGN KEY (operatorId)
    REFERENCES Employee(employeeId)
    ON DELETE RESTRICT
    ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='支付记录表';
