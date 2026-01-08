-- @author 巷北  
-- @time 2026-01-01 19:00:54  

SET NAMES utf8mb4;

CREATE DATABASE IF NOT EXISTS hotel_management
  CHARACTER SET utf8mb4
  COLLATE utf8mb4_unicode_ci;
USE hotel_management;

-- 房型表
CREATE TABLE IF NOT EXISTS RoomType (
  roomTypeId INT AUTO_INCREMENT PRIMARY KEY COMMENT '房型编号, 唯一标识',
  name VARCHAR(50) NOT NULL COMMENT '房间名称, 如标准间/大床房/套房',
  bedType VARCHAR(20) NOT NULL COMMENT '床型',
  maxOccupancy INT NOT NULL COMMENT '可住人数',
  basePrice DECIMAL(10,2) NOT NULL COMMENT '基础房价',
  area DECIMAL(6,2) COMMENT '面积(㎡)',
  breakfastIncluded BOOLEAN DEFAULT FALSE COMMENT '是否含早',
  remarks VARCHAR(255) COMMENT '备注',
  UNIQUE KEY uq_roomtype_name(name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='房型表';

-- 房间表
CREATE TABLE IF NOT EXISTS Room (
  roomId INT PRIMARY KEY COMMENT '房间编号/房号(可用实际房号)',
  roomTypeId INT NOT NULL COMMENT '房型外键',
  floor INT NOT NULL,
  status ENUM('空闲','已预订','已入住','维修中') DEFAULT '空闲' COMMENT '房态',
  discountEligible BOOLEAN DEFAULT FALSE COMMENT '是否可打折',
  description VARCHAR(255) COMMENT '房间描述',
  CONSTRAINT fk_room_roomType FOREIGN KEY (roomTypeId)
    REFERENCES RoomType(roomTypeId)
    ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='房间表';

-- 员工表
CREATE TABLE IF NOT EXISTS Employee (
  employeeId INT AUTO_INCREMENT PRIMARY KEY COMMENT '员工编号',
  name VARCHAR(50) NOT NULL,
  gender ENUM('男','女','其它') DEFAULT '其它',
  phone VARCHAR(20),
  position VARCHAR(50),
  department VARCHAR(50),
  hireDate DATE,
  username VARCHAR(50) UNIQUE NOT NULL,
  password VARCHAR(255) NOT NULL,
  permissionLevel ENUM('普通员工','主管','管理员') DEFAULT '普通员工'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='员工表';

-- 会员等级表
CREATE TABLE IF NOT EXISTS MembershipLevel (
  levelId INT AUTO_INCREMENT PRIMARY KEY,
  levelName VARCHAR(50) NOT NULL,
  discountRate DECIMAL(4,2) NOT NULL COMMENT '例如 0.95 表示95折',
  requiredSpending DECIMAL(10,2) DEFAULT 0.00 COMMENT '升级所需累计消费',
  privileges VARCHAR(255)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='会员等级表';

-- 客户表
CREATE TABLE IF NOT EXISTS Customer (
  customerId INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(50) NOT NULL,
  idNumber VARCHAR(50) UNIQUE COMMENT '身份证/证件号',
  phone VARCHAR(20),
  gender ENUM('男','女','其它') DEFAULT '其它',
  membershipLevelId INT DEFAULT NULL COMMENT '外键 -> MembershipLevel',
  registrationDate DATETIME DEFAULT CURRENT_TIMESTAMP,
  remarks VARCHAR(255),
  CONSTRAINT fk_customer_membership FOREIGN KEY (membershipLevelId)
    REFERENCES MembershipLevel(levelId)
    ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='客户表';

-- 预定表
CREATE TABLE IF NOT EXISTS Reservation (
  reservationId INT AUTO_INCREMENT PRIMARY KEY,
  customerId INT NOT NULL,
  roomTypeId INT NOT NULL,
  reservedRoomCount INT NOT NULL,
  checkInDate DATE NOT NULL,
  checkOutDate DATE NOT NULL,
  status ENUM('已预订','已取消','已入住') DEFAULT '已预订',
  depositAmount DECIMAL(10,2) DEFAULT 0.00,
  remark VARCHAR(255),
  CONSTRAINT fk_reservation_customer FOREIGN KEY (customerId)
    REFERENCES Customer(customerId) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT fk_reservation_roomType FOREIGN KEY (roomTypeId)
    REFERENCES RoomType(roomTypeId) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='预定表';

-- 入住表
CREATE TABLE IF NOT EXISTS Stay (
  checkInId INT AUTO_INCREMENT PRIMARY KEY,
  reservationId INT DEFAULT NULL COMMENT '若来源于预约则指向 Reservation',
  customerId INT NOT NULL,
  roomId INT NOT NULL,
  checkInTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  plannedCheckOutTime DATETIME NOT NULL,
  actualCheckOutTime DATETIME,
  guestCount INT NOT NULL,
  depositAmount DECIMAL(10,2) DEFAULT 0.00,
  status ENUM('在住','已退房') DEFAULT '在住',
  staffId INT DEFAULT NULL COMMENT '办理入住的员工',
  CONSTRAINT fk_stay_reservation FOREIGN KEY (reservationId)
    REFERENCES Reservation(reservationId) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT fk_stay_customer FOREIGN KEY (customerId)
    REFERENCES Customer(customerId) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT fk_stay_room FOREIGN KEY (roomId)
    REFERENCES Room(roomId) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT fk_stay_staff FOREIGN KEY (staffId)
    REFERENCES Employee(employeeId) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='入住表';

-- 餐饮/商品的商品表
CREATE TABLE IF NOT EXISTS Product (
  productId INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(100) NOT NULL,
  price DECIMAL(10,2) NOT NULL,
  unit VARCHAR(20),
  description VARCHAR(255),
  status ENUM('在售','下架') DEFAULT '在售'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='商品表';

-- 服务表
CREATE TABLE IF NOT EXISTS Service (
  serviceId INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(50) NOT NULL,
  price DECIMAL(10,2) NOT NULL,
  unit ENUM('次','小时','件') DEFAULT '次',
  description VARCHAR(255),
  status ENUM('启用','停用') DEFAULT '启用'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='服务表';

-- 消费记录
CREATE TABLE IF NOT EXISTS ConsumptionRecord (
  recordId INT AUTO_INCREMENT PRIMARY KEY,
  checkInId INT NOT NULL,
  type ENUM('餐饮','商品','服务','其它') NOT NULL,
  amount DECIMAL(10,2) NOT NULL,
  time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  remarks VARCHAR(255),
  serviceId INT DEFAULT NULL,
  productId INT DEFAULT NULL,
  CONSTRAINT fk_consumption_stay FOREIGN KEY (checkInId)
    REFERENCES Stay(checkInId) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT fk_consumption_service FOREIGN KEY (serviceId)
    REFERENCES Service(serviceId) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT fk_consumption_product FOREIGN KEY (productId)
    REFERENCES Product(productId) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='消费记录表';

-- 账单表
CREATE TABLE IF NOT EXISTS Bill (
  billId INT AUTO_INCREMENT PRIMARY KEY,
  checkInId INT NOT NULL,
  roomCharge DECIMAL(10,2) DEFAULT 0.00,
  consumptionCharge DECIMAL(10,2) DEFAULT 0.00 COMMENT '结算时汇总的消费金额（餐饮/商品/服务）',
  otherCharge DECIMAL(10,2) DEFAULT 0.00,
  discountAmount DECIMAL(10,2) DEFAULT 0.00 COMMENT '折扣金额（绝对值）',
  totalAmount DECIMAL(12,2) NOT NULL COMMENT '结账后写入的总金额（room+consumption+other - discount）',
  paymentStatus ENUM('未支付','部分支付','已支付') DEFAULT '未支付',
  createdTime DATETIME DEFAULT CURRENT_TIMESTAMP,
  settledTime DATETIME DEFAULT NULL,
  CONSTRAINT fk_bill_stay FOREIGN KEY (checkInId)
    REFERENCES Stay(checkInId) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='账单表';

-- 支付记录
CREATE TABLE IF NOT EXISTS Payment (
  paymentId INT AUTO_INCREMENT PRIMARY KEY,
  billId INT NOT NULL,
  paidAmount DECIMAL(10,2) NOT NULL,
  paymentMethod ENUM('现金','微信','支付宝','银行卡') NOT NULL,
  paymentTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  operatorId INT DEFAULT NULL,
  remarks VARCHAR(255),
  CONSTRAINT fk_payment_bill FOREIGN KEY (billId)
    REFERENCES Bill(billId) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT fk_payment_operator FOREIGN KEY (operatorId)
    REFERENCES Employee(employeeId) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='支付记录表';

-- 维修记录
CREATE TABLE IF NOT EXISTS Maintenance (
  maintenanceId INT AUTO_INCREMENT PRIMARY KEY,
  roomId INT NOT NULL,
  employeeId INT NOT NULL,
  startTime DATETIME NOT NULL,
  endTime DATETIME DEFAULT NULL,
  problemDescription VARCHAR(255),
  status ENUM('进行中','已完成') DEFAULT '进行中',
  CONSTRAINT fk_maintenance_room FOREIGN KEY (roomId)
    REFERENCES Room(roomId) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT fk_maintenance_employee FOREIGN KEY (employeeId)
    REFERENCES Employee(employeeId) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='维修记录表';
