-- @author 巷北  
-- @time 2026-01-01 22:00:06  

CREATE OR REPLACE VIEW v_MonthlyRevenue AS
SELECT 
    DATE_FORMAT(p.paymentTime, '%Y-%m') AS month,
    SUM(p.paidAmount) AS totalRevenue
FROM Payment p
GROUP BY DATE_FORMAT(p.paymentTime, '%Y-%m')
ORDER BY month;
