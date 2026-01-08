-- @author 巷北  
-- @time 2026-01-01 21:49:52  

CREATE OR REPLACE VIEW v_RoomAvailability AS
SELECT 
    rt.roomTypeId,
    rt.name AS roomTypeName,
    COUNT(r.roomId) AS totalRooms,
    SUM(CASE WHEN r.status = '空闲' THEN 1 ELSE 0 END) AS freeRooms,
    ROUND(SUM(CASE WHEN r.status = '空闲' THEN 1 ELSE 0 END)/COUNT(r.roomId)*100,2) AS freeRate
FROM Room r
JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
GROUP BY rt.roomTypeId, rt.name;
