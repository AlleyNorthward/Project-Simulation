-- @author 巷北  
-- @time 2026-01-01 20:24:01  

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
  SELECT MAX(s2.checkInTime) 
  FROM Stay s2 
  WHERE s2.customerId = c.customerId
);
