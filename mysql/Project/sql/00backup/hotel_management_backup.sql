-- MySQL dump 10.13  Distrib 9.5.0, for Win64 (x86_64)
--
-- Host: localhost    Database: hotel_management
-- ------------------------------------------------------
-- Server version	9.5.0

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `bill`
--

DROP TABLE IF EXISTS `bill`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `bill` (
  `billId` int NOT NULL AUTO_INCREMENT,
  `checkInId` int NOT NULL,
  `roomCharge` decimal(10,2) DEFAULT '0.00',
  `consumptionCharge` decimal(10,2) DEFAULT '0.00' COMMENT '结算时汇总的消费金额（餐饮/商品/服务）',
  `otherCharge` decimal(10,2) DEFAULT '0.00',
  `discountAmount` decimal(10,2) DEFAULT '0.00' COMMENT '折扣金额（绝对值）',
  `totalAmount` decimal(12,2) NOT NULL COMMENT '结账后写入的总金额（room+consumption+other - discount）',
  `paymentStatus` enum('未支付','部分支付','已支付') COLLATE utf8mb4_unicode_ci DEFAULT '未支付',
  `createdTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `settledTime` datetime DEFAULT NULL,
  PRIMARY KEY (`billId`),
  KEY `fk_bill_stay` (`checkInId`),
  CONSTRAINT `fk_bill_stay` FOREIGN KEY (`checkInId`) REFERENCES `stay` (`checkInId`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='账单表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bill`
--

LOCK TABLES `bill` WRITE;
/*!40000 ALTER TABLE `bill` DISABLE KEYS */;
INSERT INTO `bill` VALUES (1,1,520.00,65.00,0.00,0.00,585.00,'已支付','2025-05-06 11:30:00','2025-05-06 11:30:00'),(2,2,700.00,150.00,0.00,70.00,780.00,'已支付','2025-04-03 11:00:00','2025-04-03 11:00:00'),(3,3,250.00,100.00,0.00,0.00,350.00,'未支付','2025-07-02 12:30:00',NULL),(4,4,360.00,235.00,0.00,0.00,595.00,'已支付','2025-03-07 12:00:00','2025-03-07 12:00:00'),(5,5,4800.00,350.00,0.00,480.00,4670.00,'部分支付','2025-06-12 10:00:00',NULL),(6,6,520.00,85.00,0.00,0.00,605.00,'未支付','2025-09-12 13:00:00',NULL),(7,7,5000.00,17.00,0.00,0.00,5017.00,'未支付','2025-07-22 12:00:00',NULL),(8,8,420.00,300.00,0.00,0.00,720.00,'未支付','2025-02-02 12:10:00','2025-02-02 12:10:00'),(9,9,480.00,10.00,0.00,0.00,490.00,'已支付','2025-07-02 12:10:00','2025-07-02 12:20:00'),(10,10,250.00,8.00,0.00,0.00,258.00,'已支付','2025-01-11 13:00:00','2025-01-11 13:10:00'),(11,11,1200.00,17.00,0.00,0.00,1217.00,'未支付','2025-07-22 12:00:00',NULL),(12,12,520.00,145.00,0.00,0.00,665.00,'已支付','2025-03-07 13:00:00','2025-03-07 13:30:00');
/*!40000 ALTER TABLE `bill` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `consumptionrecord`
--

DROP TABLE IF EXISTS `consumptionrecord`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `consumptionrecord` (
  `recordId` int NOT NULL AUTO_INCREMENT,
  `checkInId` int NOT NULL,
  `type` enum('餐饮','商品','服务','其它') COLLATE utf8mb4_unicode_ci NOT NULL,
  `amount` decimal(10,2) NOT NULL,
  `time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `remarks` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `serviceId` int DEFAULT NULL,
  `productId` int DEFAULT NULL,
  PRIMARY KEY (`recordId`),
  KEY `fk_consumption_stay` (`checkInId`),
  KEY `fk_consumption_service` (`serviceId`),
  KEY `fk_consumption_product` (`productId`),
  CONSTRAINT `fk_consumption_product` FOREIGN KEY (`productId`) REFERENCES `product` (`productId`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `fk_consumption_service` FOREIGN KEY (`serviceId`) REFERENCES `service` (`serviceId`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `fk_consumption_stay` FOREIGN KEY (`checkInId`) REFERENCES `stay` (`checkInId`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=45 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='消费记录表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `consumptionrecord`
--

LOCK TABLES `consumptionrecord` WRITE;
/*!40000 ALTER TABLE `consumptionrecord` DISABLE KEYS */;
INSERT INTO `consumptionrecord` VALUES (23,1,'餐饮',60.00,'2025-05-05 19:00:00','晚餐',16,NULL),(24,1,'商品',5.00,'2025-05-05 21:00:00','矿泉水',NULL,1),(25,2,'服务',30.00,'2025-04-02 09:00:00','房间清洁',13,NULL),(26,2,'餐饮',120.00,'2025-04-02 19:00:00','两人晚餐',16,NULL),(27,3,'商品',20.00,'2025-07-01 20:00:00','啤酒',NULL,2),(28,3,'服务',80.00,'2025-07-02 10:00:00','延迟退房',14,NULL),(29,4,'餐饮',180.00,'2025-03-06 12:30:00','家庭午餐',16,NULL),(30,4,'商品',15.00,'2025-03-06 13:00:00','巧克力',NULL,4),(31,5,'商品',150.00,'2025-06-11 15:00:00','机场接送',NULL,9),(32,5,'服务',200.00,'2025-06-11 16:00:00','房间布置',19,NULL),(33,6,'商品',60.00,'2025-09-10 08:00:00','早餐代购',NULL,8),(34,6,'服务',25.00,'2025-09-11 09:00:00','洗衣快洗',18,NULL),(35,7,'商品',12.00,'2025-07-20 10:00:00','薯片',NULL,12),(36,7,'商品',5.00,'2025-07-21 08:00:00','矿泉水',NULL,1),(37,8,'服务',300.00,'2025-03-11 10:00:00','会议室租赁',17,NULL),(38,9,'服务',0.00,'2025-02-16 08:00:00','叫醒服务',15,NULL),(39,10,'商品',8.00,'2025-01-10 14:00:00','牙刷套装',NULL,6),(40,11,'服务',50.00,'2025-07-21 09:00:00','儿童床位',21,NULL),(41,12,'商品',120.00,'2025-07-02 20:00:00','加床',NULL,11),(42,3,'商品',10.00,'2025-07-01 22:00:00','可乐',NULL,3),(43,5,'服务',60.00,'2025-06-12 09:00:00','早餐服务',16,NULL),(44,4,'商品',40.00,'2025-03-06 15:00:00','洗衣服务',NULL,7);
/*!40000 ALTER TABLE `consumptionrecord` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `customer`
--

DROP TABLE IF EXISTS `customer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `customer` (
  `customerId` int NOT NULL AUTO_INCREMENT,
  `name` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `idNumber` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '身份证/证件号',
  `phone` varchar(20) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `gender` enum('男','女','其它') COLLATE utf8mb4_unicode_ci DEFAULT '其它',
  `membershipLevelId` int DEFAULT NULL COMMENT '外键 -> MembershipLevel',
  `registrationDate` datetime DEFAULT CURRENT_TIMESTAMP,
  `remarks` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`customerId`),
  UNIQUE KEY `idNumber` (`idNumber`),
  KEY `fk_customer_membership` (`membershipLevelId`),
  CONSTRAINT `fk_customer_membership` FOREIGN KEY (`membershipLevelId`) REFERENCES `membershiplevel` (`levelId`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='客户表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `customer`
--

LOCK TABLES `customer` WRITE;
/*!40000 ALTER TABLE `customer` DISABLE KEYS */;
INSERT INTO `customer` VALUES (1,'顾客甲','ID00000001','15510000001','男',2,'2023-06-10 10:00:00','常住客户'),(2,'顾客乙','ID00000002','15510000002','女',3,'2022-02-15 09:30:00',''),(3,'顾客丙','ID00000003','15510000003','男',1,'2024-01-20 12:00:00',''),(4,'顾客丁','ID00000004','15510000004','女',4,'2021-11-05 08:10:00','家庭出游'),(5,'顾客戊','ID00000005','15510000005','男',5,'2020-07-22 14:20:00',''),(6,'顾客己','ID00000006','15510000006','女',6,'2019-03-03 16:00:00','高价值客户'),(7,'顾客庚','ID00000007','15510000007','男',7,'2024-09-01 11:11:00',''),(8,'顾客辛','ID00000008','15510000008','女',8,'2024-10-12 09:00:00',''),(9,'顾客壬','ID00000009','15510000009','男',9,'2023-04-18 10:05:00',''),(10,'顾客癸','ID00000010','15510000010','女',1,'2022-12-01 13:00:00',''),(11,'测试用户A','IDTEST00011','15610000011','男',10,'2025-01-05 09:00:00','试用客户'),(12,'测试用户B','IDTEST00012','15610000012','女',1,'2025-02-06 10:00:00',''),(13,'团队成员1','IDTEAM0001','15000000001','男',9,'2025-03-01 09:00:00','组内成员-客户视角'),(14,'团队成员2','IDTEAM0002','15000000002','女',9,'2025-03-01 09:05:00','组内成员-客户视角'),(15,'临时访客','IDTEMP000','15000000003','其它',NULL,'2025-04-01 12:00:00','无会员');
/*!40000 ALTER TABLE `customer` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `employee`
--

DROP TABLE IF EXISTS `employee`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `employee` (
  `employeeId` int NOT NULL AUTO_INCREMENT COMMENT '员工编号',
  `name` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `gender` enum('男','女','其它') COLLATE utf8mb4_unicode_ci DEFAULT '其它',
  `phone` varchar(20) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `position` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `department` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `hireDate` date DEFAULT NULL,
  `username` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `password` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL,
  `permissionLevel` enum('普通员工','主管','管理员') COLLATE utf8mb4_unicode_ci DEFAULT '普通员工',
  PRIMARY KEY (`employeeId`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='员工表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `employee`
--

LOCK TABLES `employee` WRITE;
/*!40000 ALTER TABLE `employee` DISABLE KEYS */;
INSERT INTO `employee` VALUES (1,'张伟','男','13800010001','前台接待','前台','2020-05-10','front_zhang','pwd_front_1','普通员工'),(2,'李婷','女','13800010002','前台主管','前台','2018-08-01','front_li','pwd_front_2','主管'),(3,'王强','男','13800010003','客房服务','客房','2019-03-15','room_wang','pwd_room_1','普通员工'),(4,'赵敏','女','13800010004','财务','财务','2017-11-20','acct_zhao','pwd_acct_1','主管'),(5,'陈磊','男','13800010005','维修工','维修','2016-07-07','maint_chen','pwd_maint_1','普通员工'),(6,'刘洋','男','13800010006','维修主管','维修','2015-02-01','maint_liu','pwd_maint_2','主管'),(7,'周超','男','13900020001','后端开发','开发组','2024-03-01','dev_zhou','pwd_dev_zhou','管理员'),(8,'吴倩','女','13900020002','前端开发','开发组','2024-03-01','dev_wu','pwd_dev_wu','普通员工'),(9,'高峰','男','13900020003','测试工程师','开发组','2024-03-01','dev_gao','pwd_dev_gao','普通员工'),(10,'何倩','女','13900020004','产品经理','开发组','2024-03-01','dev_he','pwd_dev_he','管理员'),(11,'孙丽','女','13800010006','客服','客服','2021-09-10','cs_sun','pwd_cs_1','普通员工'),(12,'朱峰','男','13800010007','库存主管','后勤','2019-12-05','ops_zhu','pwd_ops_1','普通员工');
/*!40000 ALTER TABLE `employee` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `maintenance`
--

DROP TABLE IF EXISTS `maintenance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `maintenance` (
  `maintenanceId` int NOT NULL AUTO_INCREMENT,
  `roomId` int NOT NULL,
  `employeeId` int NOT NULL,
  `startTime` datetime NOT NULL,
  `endTime` datetime DEFAULT NULL,
  `problemDescription` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `status` enum('进行中','已完成') COLLATE utf8mb4_unicode_ci DEFAULT '进行中',
  PRIMARY KEY (`maintenanceId`),
  KEY `fk_maintenance_room` (`roomId`),
  KEY `fk_maintenance_employee` (`employeeId`),
  CONSTRAINT `fk_maintenance_employee` FOREIGN KEY (`employeeId`) REFERENCES `employee` (`employeeId`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `fk_maintenance_room` FOREIGN KEY (`roomId`) REFERENCES `room` (`roomId`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='维修记录表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `maintenance`
--

LOCK TABLES `maintenance` WRITE;
/*!40000 ALTER TABLE `maintenance` DISABLE KEYS */;
INSERT INTO `maintenance` VALUES (1,204,5,'2025-03-10 09:00:00','2025-03-11 16:00:00','浴室水管渗漏','已完成'),(2,303,6,'2025-09-11 09:00:00',NULL,'空调噪音','进行中'),(3,401,5,'2025-05-02 08:00:00','2025-05-02 12:00:00','插座维修','已完成'),(4,402,5,'2025-06-20 10:00:00','2025-06-20 15:00:00','门锁更换','已完成'),(5,101,6,'2025-07-01 09:00:00','2025-07-01 11:00:00','电视无法开机','已完成'),(6,302,5,'2025-02-02 09:00:00','2025-02-02 11:30:00','床垫更换','已完成'),(7,503,6,'2025-08-10 08:30:00',NULL,'灯具故障','进行中'),(8,504,6,'2025-08-12 10:00:00','2025-08-12 12:00:00','窗帘轨道修理','已完成'),(9,506,5,'2025-09-01 09:00:00','2025-09-01 13:00:00','浴室门铰链','已完成'),(10,505,5,'2025-04-15 10:00:00','2025-04-15 12:30:00','空调滤网清洁','已完成');
/*!40000 ALTER TABLE `maintenance` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `membershiplevel`
--

DROP TABLE IF EXISTS `membershiplevel`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `membershiplevel` (
  `levelId` int NOT NULL AUTO_INCREMENT,
  `levelName` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `discountRate` decimal(4,2) NOT NULL COMMENT '例如 0.95 表示95折',
  `requiredSpending` decimal(10,2) DEFAULT '0.00' COMMENT '升级所需累计消费',
  `privileges` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`levelId`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='会员等级表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `membershiplevel`
--

LOCK TABLES `membershiplevel` WRITE;
/*!40000 ALTER TABLE `membershiplevel` DISABLE KEYS */;
INSERT INTO `membershiplevel` VALUES (1,'青铜会员',1.00,0.00,'无'),(2,'白银会员',0.98,500.00,'免费果盘/生日优惠'),(3,'黄金会员',0.95,2000.00,'延迟退房/房价9.5折'),(4,'白金会员',0.90,5000.00,'免费升级/优先预留'),(5,'钻石会员',0.85,15000.00,'专属客服/免费接送'),(6,'VIP',0.80,30000.00,'高级礼遇'),(7,'VIP_PLUS',0.78,50000.00,'更多权益'),(8,'企业客户',0.92,0.00,'协议价/账期'),(9,'员工内购',0.70,0.00,'员工专属折扣'),(10,'试用会员',0.99,0.00,'试用权');
/*!40000 ALTER TABLE `membershiplevel` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `payment`
--

DROP TABLE IF EXISTS `payment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `payment` (
  `paymentId` int NOT NULL AUTO_INCREMENT,
  `billId` int NOT NULL,
  `paidAmount` decimal(10,2) NOT NULL,
  `paymentMethod` enum('现金','微信','支付宝','银行卡') COLLATE utf8mb4_unicode_ci NOT NULL,
  `paymentTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `operatorId` int DEFAULT NULL,
  `remarks` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`paymentId`),
  KEY `fk_payment_bill` (`billId`),
  KEY `fk_payment_operator` (`operatorId`),
  CONSTRAINT `fk_payment_bill` FOREIGN KEY (`billId`) REFERENCES `bill` (`billId`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `fk_payment_operator` FOREIGN KEY (`operatorId`) REFERENCES `employee` (`employeeId`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='支付记录表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `payment`
--

LOCK TABLES `payment` WRITE;
/*!40000 ALTER TABLE `payment` DISABLE KEYS */;
INSERT INTO `payment` VALUES (1,1,585.00,'微信','2025-05-06 11:31:00',4,'线上支付'),(2,2,780.00,'银行卡','2025-04-03 11:02:00',4,'公司报销'),(3,3,0.00,'现金','2025-07-02 12:40:00',1,'待结算'),(4,4,595.00,'现金','2025-03-07 12:05:00',1,'现场支付'),(5,5,1000.00,'微信','2025-06-12 09:00:00',2,'部分预付'),(6,5,3670.00,'银行卡','2025-06-12 10:30:00',4,'余款'),(7,6,0.00,'现金','2025-09-12 13:15:00',1,'尚未支付'),(8,7,0.00,'微信','2025-07-22 12:10:00',1,'尚未支付'),(9,8,720.00,'支付宝','2025-02-02 12:12:00',2,'线上'),(10,9,490.00,'现金','2025-07-02 12:25:00',1,'现场支付'),(11,10,258.00,'微信','2025-01-11 13:05:00',2,'线上支付'),(12,12,665.00,'银行卡','2025-03-07 13:32:00',4,'公司结算');
/*!40000 ALTER TABLE `payment` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `product`
--

DROP TABLE IF EXISTS `product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `product` (
  `productId` int NOT NULL AUTO_INCREMENT,
  `name` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `unit` varchar(20) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `description` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `status` enum('在售','下架') COLLATE utf8mb4_unicode_ci DEFAULT '在售',
  PRIMARY KEY (`productId`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='商品表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `product`
--

LOCK TABLES `product` WRITE;
/*!40000 ALTER TABLE `product` DISABLE KEYS */;
INSERT INTO `product` VALUES (1,'瓶装水',5.00,'瓶','矿泉水500ml','在售'),(2,'迷你吧—啤酒',20.00,'瓶','小瓶啤酒','在售'),(3,'迷你吧—可乐',10.00,'罐','可乐330ml','在售'),(4,'巧克力',15.00,'块','进口巧克力','在售'),(5,'加湿器租赁',40.00,'天','便携式','在售'),(6,'牙刷套装',8.00,'套','一次性','在售'),(7,'洗衣服务-单件',12.00,'件','单件洗熨','在售'),(8,'早餐代购',60.00,'份','拼盘自选','在售'),(9,'机场接送-单程',150.00,'次','可预订','在售'),(10,'枕头升级',30.00,'个','记忆棉枕','在售'),(11,'加床服务',120.00,'次','含床品','在售'),(12,'迷你吧—薯片',12.00,'袋','小包装','在售');
/*!40000 ALTER TABLE `product` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `reservation`
--

DROP TABLE IF EXISTS `reservation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `reservation` (
  `reservationId` int NOT NULL AUTO_INCREMENT,
  `customerId` int NOT NULL,
  `roomTypeId` int NOT NULL,
  `reservedRoomCount` int NOT NULL,
  `checkInDate` date NOT NULL,
  `checkOutDate` date NOT NULL,
  `status` enum('已预订','已取消','已入住') COLLATE utf8mb4_unicode_ci DEFAULT '已预订',
  `depositAmount` decimal(10,2) DEFAULT '0.00',
  `remark` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`reservationId`),
  KEY `fk_reservation_customer` (`customerId`),
  KEY `fk_reservation_roomType` (`roomTypeId`),
  CONSTRAINT `fk_reservation_customer` FOREIGN KEY (`customerId`) REFERENCES `customer` (`customerId`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `fk_reservation_roomType` FOREIGN KEY (`roomTypeId`) REFERENCES `roomtype` (`roomTypeId`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='预定表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `reservation`
--

LOCK TABLES `reservation` WRITE;
/*!40000 ALTER TABLE `reservation` DISABLE KEYS */;
INSERT INTO `reservation` VALUES (1,1,2,1,'2025-06-10','2025-06-12','已预订',200.00,'商务出差'),(2,2,3,1,'2025-06-15','2025-06-17','已取消',0.00,'改期'),(3,3,1,1,'2025-07-01','2025-07-02','已预订',50.00,''),(4,4,5,1,'2025-08-20','2025-08-24','已预订',500.00,'家庭出游'),(5,5,8,1,'2025-09-10','2025-09-12','已预订',1000.00,'节假日'),(6,6,6,1,'2025-05-05','2025-05-06','已入住',100.00,'当天到店'),(7,7,2,2,'2025-04-01','2025-04-03','已入住',300.00,'团体'),(8,8,4,1,'2025-03-10','2025-03-12','已预订',200.00,''),(9,9,3,1,'2025-02-15','2025-02-16','已预订',150.00,''),(10,10,1,1,'2025-01-10','2025-01-11','已预订',50.00,''),(11,11,9,1,'2025-07-20','2025-07-22','已预订',0.00,'团队体验'),(12,12,5,1,'2025-07-01','2025-07-03','已预订',200.00,'');
/*!40000 ALTER TABLE `reservation` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `room`
--

DROP TABLE IF EXISTS `room`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `room` (
  `roomId` int NOT NULL COMMENT '房间编号/房号(可用实际房号)',
  `roomTypeId` int NOT NULL COMMENT '房型外键',
  `floor` int NOT NULL,
  `status` enum('空闲','已预订','已入住','维修中') COLLATE utf8mb4_unicode_ci DEFAULT '空闲' COMMENT '房态',
  `discountEligible` tinyint(1) DEFAULT '0' COMMENT '是否可打折',
  `description` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '房间描述',
  PRIMARY KEY (`roomId`),
  KEY `fk_room_roomType` (`roomTypeId`),
  CONSTRAINT `fk_room_roomType` FOREIGN KEY (`roomTypeId`) REFERENCES `roomtype` (`roomTypeId`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='房间表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `room`
--

LOCK TABLES `room` WRITE;
/*!40000 ALTER TABLE `room` DISABLE KEYS */;
INSERT INTO `room` VALUES (101,1,1,'空闲',1,'靠近电梯'),(102,1,1,'空闲',0,''),(103,2,1,'空闲',1,'窗外朝向内庭'),(104,2,1,'已预订',1,''),(201,3,2,'空闲',1,'带办公桌'),(202,3,2,'已入住',1,'靠窗'),(203,4,2,'空闲',1,''),(204,4,2,'维修中',0,'浴室检修'),(301,5,3,'空闲',1,'家庭房，大且宽敞'),(302,5,3,'已预订',1,''),(303,6,3,'空闲',0,''),(304,6,3,'空闲',0,''),(401,7,4,'空闲',1,''),(402,7,4,'空闲',1,''),(403,8,4,'空闲',0,'豪华套房'),(404,8,4,'空闲',0,''),(501,9,5,'空闲',1,''),(502,9,5,'空闲',1,''),(503,10,5,'空闲',0,'背包客房'),(504,10,5,'空闲',0,''),(505,2,5,'空闲',1,'备用标准双床'),(506,3,5,'空闲',1,'备用高级大床'),(507,1,5,'空闲',1,'');
/*!40000 ALTER TABLE `room` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `roomtype`
--

DROP TABLE IF EXISTS `roomtype`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `roomtype` (
  `roomTypeId` int NOT NULL AUTO_INCREMENT COMMENT '房型编号, 唯一标识',
  `name` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '房间名称, 如标准间/大床房/套房',
  `bedType` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '床型',
  `maxOccupancy` int NOT NULL COMMENT '可住人数',
  `basePrice` decimal(10,2) NOT NULL COMMENT '基础房价',
  `area` decimal(6,2) DEFAULT NULL COMMENT '面积(㎡)',
  `breakfastIncluded` tinyint(1) DEFAULT '0' COMMENT '是否含早',
  `remarks` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '备注',
  PRIMARY KEY (`roomTypeId`),
  UNIQUE KEY `uq_roomtype_name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='房型表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `roomtype`
--

LOCK TABLES `roomtype` WRITE;
/*!40000 ALTER TABLE `roomtype` DISABLE KEYS */;
INSERT INTO `roomtype` VALUES (1,'经济单人间','单人床',1,250.00,15.00,0,'适合单人短住'),(2,'标准双床房','双床',2,350.00,22.00,1,''),(3,'高级大床房','大床',2,480.00,28.00,1,'带办公区'),(4,'商务大床房','大床',2,520.00,30.00,1,'含高速办公桌'),(5,'家庭套房','双床/大床',4,750.00,55.00,1,'带儿童床选项'),(6,'豪华套房','大床',2,1200.00,80.00,1,'带客厅与厨房'),(7,'无障碍房','大床',2,420.00,25.00,1,'无障碍通道'),(8,'双人标准间','双床',2,400.00,20.00,1,''),(9,'总统套房','大床',2,5000.00,220.00,1,'顶层豪华'),(10,'青年背包间','上下铺',4,180.00,12.00,0,'床位式，经济型');
/*!40000 ALTER TABLE `roomtype` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `service`
--

DROP TABLE IF EXISTS `service`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `service` (
  `serviceId` int NOT NULL AUTO_INCREMENT,
  `name` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `unit` enum('次','小时','件') COLLATE utf8mb4_unicode_ci DEFAULT '次',
  `description` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `status` enum('启用','停用') COLLATE utf8mb4_unicode_ci DEFAULT '启用',
  PRIMARY KEY (`serviceId`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='服务表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `service`
--

LOCK TABLES `service` WRITE;
/*!40000 ALTER TABLE `service` DISABLE KEYS */;
INSERT INTO `service` VALUES (13,'客房清洁',30.00,'次','标准清洁服务','启用'),(14,'延迟退房',80.00,'小时','最多延迟至17:00','启用'),(15,'叫醒服务',0.00,'次','免费','启用'),(16,'早餐服务',60.00,'次','早餐送房','启用'),(17,'会议室租赁（半天）',300.00,'次','含茶歇','启用'),(18,'洗衣服务-快洗',25.00,'件','当日取回','启用'),(19,'房间布置（鲜花）',200.00,'次','节日/纪念日','启用'),(20,'租车服务',800.00,'次','日租','启用'),(21,'儿童床位',50.00,'次','含床品','启用'),(22,'宠物照料',100.00,'次','仅限小型宠物','启用'),(23,'私人管家',500.00,'次','24小时服务','停用'),(24,'桑拿/SPA',300.00,'次','单人SPA','启用');
/*!40000 ALTER TABLE `service` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `stay`
--

DROP TABLE IF EXISTS `stay`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stay` (
  `checkInId` int NOT NULL AUTO_INCREMENT,
  `reservationId` int DEFAULT NULL COMMENT '若来源于预约则指向 Reservation',
  `customerId` int NOT NULL,
  `roomId` int NOT NULL,
  `checkInTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `plannedCheckOutTime` datetime NOT NULL,
  `actualCheckOutTime` datetime DEFAULT NULL,
  `guestCount` int NOT NULL,
  `depositAmount` decimal(10,2) DEFAULT '0.00',
  `status` enum('在住','已退房') COLLATE utf8mb4_unicode_ci DEFAULT '在住',
  `staffId` int DEFAULT NULL COMMENT '办理入住的员工',
  PRIMARY KEY (`checkInId`),
  KEY `fk_stay_reservation` (`reservationId`),
  KEY `fk_stay_customer` (`customerId`),
  KEY `fk_stay_room` (`roomId`),
  KEY `fk_stay_staff` (`staffId`),
  CONSTRAINT `fk_stay_customer` FOREIGN KEY (`customerId`) REFERENCES `customer` (`customerId`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `fk_stay_reservation` FOREIGN KEY (`reservationId`) REFERENCES `reservation` (`reservationId`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `fk_stay_room` FOREIGN KEY (`roomId`) REFERENCES `room` (`roomId`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `fk_stay_staff` FOREIGN KEY (`staffId`) REFERENCES `employee` (`employeeId`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='入住表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `stay`
--

LOCK TABLES `stay` WRITE;
/*!40000 ALTER TABLE `stay` DISABLE KEYS */;
INSERT INTO `stay` VALUES (1,6,6,202,'2025-05-05 14:00:00','2025-05-06 12:00:00','2025-05-06 11:20:00',1,100.00,'已退房',1),(2,7,7,103,'2025-04-01 15:00:00','2025-04-03 12:00:00','2025-04-03 10:30:00',2,300.00,'已退房',2),(3,NULL,3,101,'2025-07-01 15:30:00','2025-07-02 12:00:00',NULL,1,50.00,'在住',1),(4,NULL,13,301,'2025-03-05 12:00:00','2025-03-07 12:00:00','2025-03-07 11:50:00',3,200.00,'已退房',1),(5,1,1,104,'2025-06-10 15:00:00','2025-06-12 12:00:00',NULL,1,200.00,'在住',2),(6,5,5,303,'2025-09-10 14:00:00','2025-09-12 12:00:00',NULL,2,1000.00,'在住',2),(7,11,11,403,'2025-07-20 15:00:00','2025-07-22 12:00:00',NULL,2,0.00,'在住',1),(8,NULL,14,302,'2025-02-01 11:00:00','2025-02-02 12:00:00','2025-02-02 11:00:00',1,50.00,'已退房',1),(9,3,3,102,'2025-07-01 14:30:00','2025-07-02 12:00:00',NULL,1,50.00,'在住',2),(10,4,4,301,'2025-08-20 15:00:00','2025-08-24 12:00:00',NULL,4,500.00,'在住',2),(11,8,8,204,'2025-03-10 15:00:00','2025-03-12 12:00:00',NULL,1,200.00,'在住',1),(12,9,9,201,'2025-02-15 13:00:00','2025-02-16 12:00:00','2025-02-16 10:00:00',1,150.00,'已退房',1);
/*!40000 ALTER TABLE `stay` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-01-01 20:55:02
