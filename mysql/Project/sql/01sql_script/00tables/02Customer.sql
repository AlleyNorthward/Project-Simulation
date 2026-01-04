-- @author 巷北  
-- @time 2025-12-31 18:00:59  

SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS Customer(
  customerId INT AUTO_INCREMENT PRIMARY KEY COMMENT '客户编号, 主键',
  name VARCHAR(50) NOT NULL COMMENT '客户姓名',
  idNumber VARCHAR(20) UNIQUE COMMENT '身份证号, 唯一',
  phone VARCHAR(20) COMMENT '联系方式',
  gender ENUM('男', '女', '其它') DEFAULT '其它' COMMENT '性别',
  membershipLevel ENUM ('普通', '银卡', '金卡', 'VIP') DEFAULT '普通' COMMENT '会员等级',
  totalSpending DECIMAL(10, 2) DEFAULT 0.00 COMMENT '累计消费金额',
  registrationDate DATETIME DEFAULT CURRENT_TIMESTAMP COMMENT '注册日期',
  remarks VARCHAR(255) COMMENT '备注'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='客户表';

-- 添加几个测试用户吧
-- INSERT INTO Customer(name, idNumber, phone, gender, membershipLevel, totalSpending, remarks)
-- VALUES
-- ('张三', '110101199001011234', '13800000001', '男', '普通', 0.00, '测试客户1'),
-- ('李四', '110101199002022345', '13800000002', '女', '银卡', 1200.50, '测试客户2'),
-- ('王五', '110101199003033456', '13800000003', '男', '金卡', 3500.00, '测试客户3'),
-- ('赵六', '110101199004044567', '13800000004', '女', 'VIP', 7800.75, '测试客户4'),
-- ('孙七', '110101199005055678', '13800000005', '其它', '普通', 0.00, '测试客户5');
