-- @author 巷北  
-- @time 2026-01-01 20:27:32  

SELECT DATE_FORMAT(paymentTime, '%Y-%m') AS ym, SUM(paidAmount) AS monthRevenue
FROM Payment
WHERE paymentTime BETWEEN '2025-01-01' AND '2025-12-31'
GROUP BY ym
ORDER BY ym;

SELECT c.customerId, c.name, COUNT(*) AS visits
FROM Customer c
JOIN Stay s ON s.customerId = c.customerId
WHERE s.checkInTime >= DATE_SUB(NOW(), INTERVAL 6 MONTH)
GROUP BY c.customerId, c.name
ORDER BY visits DESC
LIMIT 5;
