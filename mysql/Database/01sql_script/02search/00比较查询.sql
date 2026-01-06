-- @author 巷北  
-- @time 2026-01-01 20:19:16  

-- 这里就是举几个例子, 不是执行脚本, 可以单独使用

-- 01 基础房价超过1000的房型
SELECT * FROM RoomType
WHERE basePrice > 1000.00;

-- 02 查找2020年之前入职的员工
SELECT * FROM Employee
WHERE hireDate < '2020-01-01';

-- 03 查找预定房数大于1的预定记录
SELECT * FROM Reservation
WHERE reservedRoomCount > 1;

-- 04查找depositAmount大于500的预定记录
SELECT * FROM Reservation
WHERE status = '已预订' AND depositAmount >= 500.00;
