-- @author 巷北  
-- @time 2026-01-01 21:12:46  

SET NAMES utf8mb4;
USE hotel_management;

START TRANSACTION;
UPDATE Employee
SET position = '技术负责人'
WHERE employeeId = 7;

UPDATE Room
SET status = '空闲', description = '维修完成，已复检'
WHERE roomId = 204;

UPDATE Customer
SET membershipLevelId = 3
WHERE customerId = 3;

UPDATE Employee
SET permissionLevel = '普通员工'
WHERE hireDate < '2018-01-01';

UPDATE Maintenance
SET status = '已完成'
WHERE endTime IS NOT NULL AND status <> '已完成';
COMMIT;

START TRANSACTION;
UPDATE Stay
SET depositAmount = 100.00
WHERE status = '在住' AND depositAmount < 100.00;
COMMIT;

UPDATE Room
SET discountEligible = TRUE
WHERE roomTypeId = 2;

UPDATE Customer c
JOIN (
  SELECT s.customerId, IFNULL(SUM(p.paidAmount),0) AS totalPaid
  FROM Stay s
  JOIN Bill b ON b.checkInId = s.checkInId
  JOIN Payment p ON b.billId = p.billId
  GROUP BY s.customerId
) t ON c.customerId = t.customerId
SET c.membershipLevelId = 3
WHERE t.totalPaid > 2000.00;

UPDATE Customer
SET membershipLevelId = 10
WHERE remarks LIKE '%试用%';

UPDATE Bill b
JOIN (
  SELECT billId, MIN(paymentTime) AS firstPaymentTime
  FROM Payment
  GROUP BY billId
) pmin ON b.billId = pmin.billId
SET b.settledTime = pmin.firstPaymentTime
WHERE b.paymentStatus = '已支付' AND b.settledTime IS NULL;
