-- @author 巷北  
-- @time 2026-01-01 20:25:04  

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
