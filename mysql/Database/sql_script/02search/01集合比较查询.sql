-- @author 巷北  
-- @time 2026-01-01 20:21:25  

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
