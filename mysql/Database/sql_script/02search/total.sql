SELECT * FROM RoomType
WHERE basePrice > 1000.00;

SELECT * FROM Employee
WHERE hireDate < '2020-01-01';

SELECT * FROM Reservation
WHERE reservedRoomCount > 1;

SELECT * FROM Reservation
WHERE status = '已预订' AND depositAmount >= 500.00;

SELECT * FROM Reservation
WHERE roomTypeId IN (3,4,6);

SELECT * FROM Customer
WHERE membershipLevelId NOT IN (1,2);

SELECT r.* FROM Room r
WHERE r.roomTypeId IN (
  SELECT roomTypeId FROM RoomType WHERE basePrice > 1000.00
);

SELECT * FROM Bill
WHERE billId NOT IN (SELECT DISTINCT billId FROM Payment);

SELECT * FROM RoomType
WHERE area BETWEEN 20.00 AND 50.00;

SELECT * FROM Bill
WHERE totalAmount BETWEEN 500.00 AND 2000.00;

SELECT * FROM Reservation
WHERE NOT (checkOutDate <= '2025-07-01' OR checkInDate >= '2025-07-10');

SELECT DISTINCT c.* FROM Customer c
JOIN Stay s ON s.customerId = c.customerId
WHERE s.checkInTime >= DATE_SUB(NOW(), INTERVAL 30 DAY);

SELECT * FROM Customer
WHERE name LIKE '测试%';

SELECT * FROM Room
WHERE description LIKE '%靠窗%';

SELECT * FROM Customer
WHERE phone REGEXP '^138';

SELECT * FROM Reservation
WHERE remark LIKE '%家庭%' OR remark LIKE '%儿童%';

SELECT s.checkInId, c.name AS customerName, r.roomId AS roomNumber, e.name AS handledBy
FROM Stay s
JOIN Customer c ON s.customerId = c.customerId
JOIN Room r ON s.roomId = r.roomId
LEFT JOIN Employee e ON s.staffId = e.employeeId
WHERE s.status = '在住';

SELECT rt.roomTypeId, rt.name AS roomTypeName, COUNT(*) AS freeRoomCount
FROM Room r
JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
WHERE r.status = '空闲'
GROUP BY rt.roomTypeId, rt.name;

SELECT c.customerId, c.name,
       IFNULL(SUM(p.paidAmount), 0) AS totalPaid
FROM Customer c
LEFT JOIN Stay s ON s.customerId = c.customerId
LEFT JOIN Bill b ON b.checkInId = s.checkInId
LEFT JOIN Payment p ON p.billId = b.billId
GROUP BY c.customerId, c.name
ORDER BY totalPaid DESC;

SELECT c.customerId, c.name, s.checkInTime, b.totalAmount
FROM Customer c
LEFT JOIN Stay s ON s.customerId = c.customerId
LEFT JOIN Bill b ON b.checkInId = s.checkInId
WHERE s.checkInTime = (
  SELECT MAX(s2.checkInTime) FROM Stay s2 WHERE s2.customerId = c.customerId
);

SELECT * FROM Customer
WHERE customerId IN (
  SELECT s.customerId
  FROM Stay s
  JOIN Bill b ON b.checkInId = s.checkInId
  JOIN Payment p ON p.billId = b.billId
  GROUP BY s.customerId
  HAVING SUM(p.paidAmount) > 1000.00
);

SELECT * FROM RoomType
WHERE basePrice > (SELECT AVG(basePrice) FROM RoomType);

SELECT c.customerId, c.name,
       (SELECT MAX(cr.time) FROM Stay s JOIN ConsumptionRecord cr ON cr.checkInId = s.checkInId WHERE s.customerId = c.customerId) AS lastConsumptionTime
FROM Customer c;

SELECT DISTINCT c.* FROM Customer c
WHERE c.customerId IN (
  SELECT r.customerId FROM Reservation r
  JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
  WHERE rt.name = '总统套房'
)
AND c.customerId NOT IN (
  SELECT s.customerId FROM Stay s
);

SELECT DISTINCT c.* FROM Customer c
WHERE EXISTS (
  SELECT 1 FROM Stay s
  JOIN Bill b ON b.checkInId = s.checkInId
  WHERE s.customerId = c.customerId AND b.paymentStatus <> '已支付'
);

SELECT r.* FROM Room r
WHERE EXISTS (
  SELECT 1 FROM Maintenance m WHERE m.roomId = r.roomId
);

SELECT s.* FROM Service s
WHERE NOT EXISTS (
  SELECT 1 FROM ConsumptionRecord cr WHERE cr.serviceId = s.serviceId
);

SELECT p.* FROM Product p
WHERE EXISTS (
  SELECT 1 FROM ConsumptionRecord cr WHERE cr.productId = p.productId
);

SELECT c.customerId, c.name, COUNT(cr.recordId) AS consumeCount
FROM Customer c
JOIN Stay s ON s.customerId = c.customerId
JOIN ConsumptionRecord cr ON cr.checkInId = s.checkInId
GROUP BY c.customerId, c.name
HAVING COUNT(cr.recordId) > 5
ORDER BY consumeCount DESC;

SELECT rt.roomTypeId, rt.name, AVG(s.guestCount) AS avgGuest
FROM RoomType rt
JOIN Room r ON r.roomTypeId = rt.roomTypeId
JOIN Stay s ON s.roomId = r.roomId
GROUP BY rt.roomTypeId, rt.name
HAVING AVG(s.guestCount) > 2;

SELECT DATE_FORMAT(paymentTime, '%Y-%m') AS ym, SUM(paidAmount) AS monthRevenue
FROM Payment
WHERE paymentTime BETWEEN '2025-01-01' AND '2025-12-31'
GROUP BY ym
ORDER BY ym;

SELECT c.customerId, c.name, COUNT(*) AS visits
FROM Customer c
JOIN Stay s ON s.customerId = c.customerId
WHERE s.checkInTime >= DATE_SUB(NOW(), INTERVAL 6 MONTH)
GROUP BY c.customerId, c.name
ORDER BY visits DESC
LIMIT 5;

