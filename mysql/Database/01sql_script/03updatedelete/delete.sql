-- @author 巷北  
-- @time 2026-01-01 21:29:48  

-- 01
CREATE TABLE IF NOT EXISTS archive_Customer LIKE Customer;

INSERT INTO archive_Customer SELECT * FROM Customer
WHERE membershipLevelId = 10 AND registrationDate < DATE_SUB(NOW(), INTERVAL 1 YEAR);

DELETE FROM Customer
WHERE membershipLevelId = 10 AND registrationDate < DATE_SUB(NOW(), INTERVAL 1 YEAR);


-- 02 No
SELECT c.customerId FROM Customer c
LEFT JOIN Stay s ON s.customerId = c.customerId
LEFT JOIN (
  SELECT s2.customerId FROM Stay s2 JOIN ConsumptionRecord cr ON cr.checkInId = s2.checkInId
) t ON t.customerId = c.customerId
WHERE s.customerId IS NULL AND t.customerId IS NULL;

DELETE FROM Customer
WHERE customerId IN (
  SELECT c.customerId FROM (
    SELECT c.customerId FROM Customer c
    LEFT JOIN Stay s ON s.customerId = c.customerId
    LEFT JOIN (
      SELECT s2.customerId FROM Stay s2 JOIN ConsumptionRecord cr ON cr.checkInId = s2.checkInId
    ) t ON t.customerId = c.customerId
    WHERE s.customerId IS NULL AND t.customerId IS NULL
  ) AS to_delete
);

-- 03
DELETE FROM Maintenance
WHERE status = '已完成' AND endTime < DATE_SUB(NOW(), INTERVAL 3 YEAR);

--04
DELETE FROM Reservation
WHERE status = '已取消' AND checkInDate < DATE_SUB(NOW(), INTERVAL 60 DAY);

-- 05
SELECT s.serviceId FROM Service s
LEFT JOIN ConsumptionRecord cr ON cr.serviceId = s.serviceId
WHERE s.status = '停用' AND cr.recordId IS NOT NULL;

DELETE FROM Service
WHERE status = '停用' AND serviceId NOT IN (SELECT DISTINCT serviceId FROM ConsumptionRecord WHERE serviceId IS NOT NULL);

-- 06
CREATE TABLE IF NOT EXISTS archive_Product LIKE Product;

INSERT INTO archive_Product SELECT * FROM Product WHERE price = 0.00;
DELETE FROM Product WHERE price = 0.00;

-- 07
DELETE p FROM Payment p
LEFT JOIN Bill b ON p.billId = b.billId
WHERE b.billId IS NULL;

-- 08

DELETE FROM Bill
WHERE createdTime < DATE_SUB(NOW(), INTERVAL 1 YEAR) AND paymentStatus = '已支付';

-- 09 No
DELETE FROM Room
WHERE floor >= 5 AND roomId IN (
  SELECT roomId FROM (
    SELECT r.roomId FROM Room r
    LEFT JOIN Stay s ON s.roomId = r.roomId
    WHERE s.roomId IS NULL
  ) t
);

-- 10
START TRANSACTION;
-- 1) 删除支付
DELETE p FROM Payment p
JOIN Bill b ON p.billId = b.billId
JOIN Stay s ON b.checkInId = s.checkInId
WHERE s.customerId = 15;

-- 2) 删除账单
DELETE b FROM Bill b
JOIN Stay s2 ON b.checkInId = s2.checkInId
WHERE s2.customerId = 15;

-- 3) 删除消费记录
DELETE cr FROM ConsumptionRecord cr
JOIN Stay s3 ON cr.checkInId = s3.checkInId
WHERE s3.customerId = 15;

-- 4) 删除入住
DELETE FROM Stay WHERE customerId = 15;

-- 5) 删除预约
DELETE FROM Reservation WHERE customerId = 15;

-- 6) 删除客户
DELETE FROM Customer WHERE customerId = 15;

COMMIT;
