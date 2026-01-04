-- @author 巷北  
-- @time 2026-01-01 20:23:09  

SELECT * FROM Customer
WHERE name LIKE '测试%';

SELECT * FROM Room
WHERE description LIKE '%靠窗%';

SELECT * FROM Customer
WHERE phone REGEXP '^138';

SELECT * FROM Reservation
WHERE remark LIKE '%家庭%' OR remark LIKE '%儿童%';
