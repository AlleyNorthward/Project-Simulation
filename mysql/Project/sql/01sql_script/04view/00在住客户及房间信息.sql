-- @author 巷北  
-- @time 2026-01-01 21:49:11  

CREATE OR REPLACE VIEW v_CurrentStay AS
SELECT 
    s.checkInId,
    c.customerId,
    c.name AS customerName,
    r.roomId,
    rt.name AS roomType,
    s.checkInTime,
    s.plannedCheckOutTime,
    s.status AS stayStatus,
    e.name AS handledBy
FROM Stay s
JOIN Customer c ON s.customerId = c.customerId
JOIN Room r ON s.roomId = r.roomId
JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
LEFT JOIN Employee e ON s.staffId = e.employeeId
WHERE s.status = '在住';
