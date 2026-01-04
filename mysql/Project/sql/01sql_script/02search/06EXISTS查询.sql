-- @author 巷北  
-- @time 2026-01-01 20:25:56  

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
