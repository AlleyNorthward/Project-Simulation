-- @author 巷北  
-- @time 2026-01-01 21:59:30  

CREATE OR REPLACE VIEW v_RoomMaintenance AS
SELECT 
    m.maintenanceId,
    r.roomId,
    rt.name AS roomType,
    e.employeeId AS technicianId,
    e.name AS technicianName,
    m.startTime,
    m.endTime,
    m.problemDescription,
    m.status AS maintenanceStatus
FROM Maintenance m
JOIN Room r ON m.roomId = r.roomId
JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
JOIN Employee e ON m.employeeId = e.employeeId;
