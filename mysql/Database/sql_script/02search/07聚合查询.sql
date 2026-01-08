-- @author 巷北  
-- @time 2026-01-01 20:26:45  

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
