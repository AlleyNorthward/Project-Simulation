-- @author 巷北  
-- @time 2026-01-01 20:22:19  

SELECT * FROM RoomType
WHERE area BETWEEN 20.00 AND 50.00;

SELECT * FROM Bill
WHERE totalAmount BETWEEN 500.00 AND 2000.00;

SELECT * FROM Reservation
WHERE NOT (checkOutDate <= '2025-07-01' OR checkInDate >= '2025-07-10');

SELECT DISTINCT c.* FROM Customer c
JOIN Stay s ON s.customerId = c.customerId
WHERE s.checkInTime >= DATE_SUB(NOW(), INTERVAL 30 DAY);
